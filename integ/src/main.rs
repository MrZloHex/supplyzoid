mod serial;
use serial::Serial;

mod scen_parser;
use scen_parser::{Scenario, Instruction, SerialRecip, Command};

use std::time::Instant;

use clap::{load_yaml, App};

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    let scenario_fname = matches.value_of("scenario").unwrap();
    let mut scenario = Scenario::new(scenario_fname);
    let mut rapi = Serial::init();
    let mut ocpp = Serial::init();

    if matches.is_present("data_in_scen") {
        let settings = scenario.get_settings();
        rapi.setup(settings.0);
        ocpp.setup(settings.1);
    } else {
        let mut setuped = true;
        if matches.is_present("rapi_port") {
            rapi.set_port(matches.value_of("rapi_port").unwrap());
        } else {
            eprintln!("ERROR: doesn't provided port for RAPI"); setuped = false;
        }
        if matches.is_present("rapi_baud") {
            rapi.set_baudrate(matches.value_of("rapi_baud").unwrap().parse::<u32>().unwrap());
        } else {
            eprintln!("ERROR: doesn't provided baudrate for RAPI"); setuped = false;
        }
        if matches.is_present("ocpp_port") {
            ocpp.set_port(matches.value_of("ocpp_port").unwrap());
        } else {
            eprintln!("ERROR: doesn't provided port for OCPP"); setuped = false;
        }
        if matches.is_present("ocpp_baud") {
            ocpp.set_baudrate(matches.value_of("ocpp_baud").unwrap().parse::<u32>().unwrap());
        } else {
            eprintln!("ERROR: doesn't provided baudrate for OCPP"); setuped = false;
        }
 
        if !setuped { std::process::exit(1); }
    }

    ocpp.open();
    rapi.open();

    println!("Ports was succesfully opened");
    println!("Please RESET board!");

    let reset_timeout= 10_000;
    
    let mut rapi_booted = false;
    let mut ocpp_booted = false;

    let now = Instant::now();
    let mut elapsed = now.elapsed();
    
    while elapsed.as_millis() < reset_timeout {
        let mut data = String::new();
        if !rapi_booted {
            rapi.read(&mut data);
            // println!("RAPI: {}", data);
            if data == "STARTING RAPI\n" {
                rapi_booted = true;
            }
            data = String::new();
        }
        if !ocpp_booted {
            ocpp.read(&mut data);
            // println!("OCPP: {}", data);
            if data == "STARTING OCPP\n" {
                ocpp_booted = true;
                break;
            }
        }
        elapsed = now.elapsed();
    }

    if rapi_booted { println!("RAPI succesfully started") }
    else { println!("ERROR: RAPI failed to start"); std::process::exit(1) }
    if ocpp_booted { println!("OCPP succesfully started") }
    else { println!("ERROR: OCPP failed to start"); std::process::exit(1) }

    println!("Starting testing");

    loop {
        let instr = scenario.next_instruction();
        if let None = instr { break; }
        let instr = instr.unwrap();
        match instr.serial {
            SerialRecip::Rapi => {
                match instr.cmd {
                    Command::Send => {
                        rapi.write(instr.value.as_bytes());
                    },
                    Command::Expect => {

                    }
                }
            },
            SerialRecip::Ocpp => {
                match instr.cmd {
                    Command::Send => {
                        ocpp.write(instr.value.as_bytes());
                    },
                    Command::Expect => {
                        
                    }
                }
            }
        }
    }
    println!("TEST FINISHED");
   
}
