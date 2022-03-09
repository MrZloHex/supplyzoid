mod serial;

mod scen_parser;
use scen_parser::{Command, Scenario, SerialRecip};

extern crate colored;
use colored::*;

use std::time::Instant;

use clap::{load_yaml, App};

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    let scenario_fname = matches.value_of("scenario").unwrap();
    let mut scenario = Scenario::new(scenario_fname);

    let verbose = matches.is_present("verbose");

    let (mut rapi, mut ocpp, rst_timeout) = scenario.get_settings();
    if verbose {
        println!("{}:", "Settings INFO".bright_cyan());
        println!
        (
            "{}:\t{}\t\t- {}\n\t{}\t- {}\n\t{}\t\t- {} ms\n\t{}\t- {} ms",
            "RAPI".bright_blue(),
            "Port".bold(), rapi.get_port(),
            "Baud Rate".bold(), rapi.get_baudrate(),
            "Timeout".bold(), rapi.get_timeout(),
            "Msg Timeout".bold(), rapi.get_msg_timeout()
        );
        println!
        (
            "{}:\t{}\t\t- {}\n\t{}\t- {}\n\t{}\t\t- {} ms\n\t{}\t- {} ms",
            "OCPP".bright_blue(),
            "Port".bold(), ocpp.get_port(),
            "Baud Rate".bold(), ocpp.get_baudrate(),
            "Timeout".bold(), ocpp.get_timeout(),
            "Msg Timeout".bold(), ocpp.get_msg_timeout()
        );
        println!("\n\t{}\t- {} ms\n", "Reset timout".bold(), rst_timeout);
    }

    rapi.open();
    ocpp.open();

    if verbose {
        println!("{}: ports was succesfully opened", "INFO".bright_cyan())
    }

    println!("{} the board", "RESET".bright_yellow().blink());

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
                if verbose {
                    println!("{}: {} booted succesfully", "INFO".bright_cyan(), "RAPI".bold())
                }
            }
            data = String::new();
        }
        if !ocpp_booted {
            ocpp.read_str(&mut data);
            // println!("OCPP: {}", data);
            if data == "STARTING OCPP\n" {
                ocpp_booted = true;
                if verbose {
                    println!("{}: {} booted succesfully", "INFO".bright_cyan(), "OCPP".bold())
                }
            }
        }
        if ocpp_booted && rapi_booted {
            break;
        }
        elapsed = now.elapsed();
    }

    if elapsed.as_millis() > rst_timeout || !rapi_booted || !ocpp_booted {
        eprintln!("{}: failed to boot board for testing, try to increase {} in settings", "ERROR".bright_red(), "RST_TIMEOUT".bold());
        std::process::exit(1);
    }

    println!("\n----- {} ----", "Starting testing".bold().blue());

    let mut is_test_passed = true;

    loop {
        let instr = scenario.next_instruction();
        if instr.is_none() {
            break;
        }
        let instr = instr.unwrap();
        match instr.serial {
            SerialRecip::Rapi => match instr.cmd {
                Command::Send => {
                    if verbose {
                        println!("{}: Sending   to   {} - {}", "INFO".bright_cyan(), "RAPI".bold(), instr.value.italic());
                    }
                    rapi.write(instr.value.as_bytes());

                },
                Command::Expect => {
                    let now = Instant::now();
                    let mut elapsed = now.elapsed();

                    if verbose {
                        println!("{}: Expecting from {} - {}", "INFO".bright_cyan(), "RAPI".bold(), instr.value.italic());
                    }

                    let mut data = String::new();
                    while elapsed.as_millis() <= rapi.get_msg_timeout() {
                        rapi.read_str(&mut data);
                        if data.is_empty() {
                            elapsed = now.elapsed();
                            continue;
                        } else {
                            let test_info = if compare_str(data.clone(), instr.value.clone()) {
                                "OKAY".green().bold()
                            } else {
                                is_test_passed = false;
                                "FAIL".red().bold()
                            };

                            if verbose {
                                println!(" {} Got                 - {}", test_info, data.italic());
                            }
                            break;
                        }
                    }
                    if elapsed.as_millis() > rapi.get_msg_timeout() {
                        is_test_passed = false;
                        break;
                    }
                    
                }
            },
            SerialRecip::Ocpp => match instr.cmd {
                Command::Send => {
                    if verbose {
                        print!("{}: Sending   to   {} - {}", "INFO".bright_cyan(), "OCPP".bold(), instr.value.italic());
                    }
                    ocpp.write(instr.value.as_bytes());
                },
                Command::Expect => {
                    let now = Instant::now();
                    let mut elapsed = now.elapsed();

                    if verbose {
                        print!("{}: Expecting from {} - {}", "INFO".bright_cyan(), "OCPP".bold(), instr.value.italic());
                    }

                    let mut data = String::new();
                    while elapsed.as_millis() <= ocpp.get_msg_timeout() {
                        ocpp.read_str(&mut data);
                        if data.is_empty() {
                            elapsed = now.elapsed();
                            continue;
                        } else {
                            let test_info = if compare_str(data.clone(), instr.value.clone()) {
                                "OKAY".green().bold()
                            } else {
                                is_test_passed = false;
                                "FAIL".red().bold()
                            };

                            if verbose {
                                println!(" {} Got                 - {}", test_info, data.italic());
                            }
                            break;
                        }
                    }
                    if elapsed.as_millis() > ocpp.get_msg_timeout() {
                        is_test_passed = false;
                        break;
                    }
                }
            },
        };
        if !is_test_passed {
            break;
        }
    }
    if is_test_passed {
        println!("\n {} {}\t{}", "Test".bold(), scenario.name().italic(), "PASSED".bold().green());
    } else {
        println!("\n {} {}\t{}", "Test".bold(), scenario.name().italic(), "FAILED".bold().red());
    }

    println!("\n---- {} ----", "Finishing testing".bold().blue());
}


fn compare_str(str1: String, str2: String) -> bool {
    if str1.len() != str2.len() {
        return false;
    }
    for i in 0..str1.len() {
        if str1.chars().nth(i).unwrap() != str2.chars().nth(i).unwrap() {
            return false;
        }
    }
    true
}