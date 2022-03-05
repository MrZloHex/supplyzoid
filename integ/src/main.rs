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
            }
        }
        if ocpp_booted && rapi_booted { break; }
        elapsed = now.elapsed();
    }
    if elapsed.as_millis() > rst_timeout {
        eprintln!("ERROR: failed to start Board, try to increase `RST_TIMEOUT` in settings");
        std::process::exit(1);
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
