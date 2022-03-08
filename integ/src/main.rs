mod serial;
use serial::Serial;

mod scen_parser;
use scen_parser::{Command, Instruction, Scenario, SerialRecip};

use std::time::Instant;

use clap::{load_yaml, App};

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    let scenario_fname = matches.value_of("scenario").unwrap();
    let mut scenario = Scenario::new(scenario_fname);

    let (mut rapi, mut ocpp, rst_timeout) = scenario.get_settings();

    rapi.open();
    ocpp.open();

    println!("Ports was succesfully opened");
    println!("Please RESET board!");

    let mut rapi_booted = false;
    let mut ocpp_booted = false;

    let now = Instant::now();
    let mut elapsed = now.elapsed();

    while elapsed.as_millis() <= rst_timeout {
        let mut data = String::new();
        if !rapi_booted {
            rapi.read_str(&mut data);
            // println!("RAPI: {}", data);
            if data == "STARTING RAPI\n" {
                rapi_booted = true;
            }
            data = String::new();
        }
        if !ocpp_booted {
            ocpp.read_str(&mut data);
            // println!("OCPP: {}", data);
            if data == "STARTING OCPP\n" {
                ocpp_booted = true;
            }
        }
        if ocpp_booted && rapi_booted {
            break;
        }
        elapsed = now.elapsed();
    }
    if elapsed.as_millis() > rst_timeout {
        eprintln!("ERROR: failed to start Board, try to increase `RST_TIMEOUT` in settings");
        std::process::exit(1);
    }

    if rapi_booted {
        println!("RAPI succesfully started");
    } else {
        println!("ERROR: RAPI failed to start");
        std::process::exit(1)
    }
    if ocpp_booted {
        println!("OCPP succesfully started")
    } else {
        println!("ERROR: OCPP failed to start");
        std::process::exit(1)
    }

    println!("Starting testing");

    loop {
        let instr = scenario.next_instruction();
        if let None = instr {
            break;
        }
        let instr = instr.unwrap();
        match instr.serial {
            SerialRecip::Rapi => match instr.cmd {
                Command::Send => {
                    let res = rapi.write(instr.value.as_bytes());
                    if let Err(e) = res {
                        println!("ERROR RAPI WRITE: `{:?}`", e);
                        std::process::exit(1);
                    }
                },
                Command::Expect => {
                    let now = Instant::now();
                    let mut elapsed = now.elapsed();

                    let mut data = String::new();
                    while elapsed.as_millis() <= rapi.get_timeout() {
                        rapi.read_str(&mut data);
                        data.pop();
                        if compare_str(data.clone(), instr.value.clone()) {
                            break;
                        }
                        elapsed = now.elapsed();
                        data = String::new();
                    }
                    if elapsed.as_millis() > rapi.get_timeout() {
                        eprintln!("Failed rapi expect");
                        std::process::exit(1);
                    }
                }
            },
            SerialRecip::Ocpp => match instr.cmd {
                Command::Send => {
                    ocpp.write(instr.value.as_bytes());
                },
                Command::Expect => {
                    let now = Instant::now();
                    let mut elapsed = now.elapsed();

                    let mut data = String::new();
                    while elapsed.as_millis() <= ocpp.get_timeout() {
                        ocpp.read_str(&mut data);
                        data.pop();
                        if compare_str(data.clone(), instr.value.clone()) {
                            break;
                        }
                        elapsed = now.elapsed();
                        data = String::new();
                    }
                    if elapsed.as_millis() > rapi.get_timeout() {
                        eprintln!("Failed ocpp expect");
                        std::process::exit(1);
                    }
                }
            },
        };
    }
    println!("TEST FINISHED");
}


fn compare_str(str1: String, str2: String) -> bool {
    for i in 0..str1.len() {
        if str1.chars().nth(i).unwrap() != str2.chars().nth(i).unwrap() {
            return false;
        }
    }
    true
}