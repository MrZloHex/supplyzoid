#![allow(unused_must_use)]

mod serial;

mod scen_parser;
use scen_parser::{Command, SerialRecip, IntegrationTest};

extern crate colored;
use colored::*;

use std::time::Instant;

use clap::{load_yaml, App};

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    let scenario_fname = matches.value_of("scenario").unwrap();
    let test = IntegrationTest::new(scenario_fname);

    let verbose = matches.is_present("verbose");

    let (mut rapi, mut ocpp, rst_timeout) = test.get_settings();
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

    let mut ti = 0;
    for mut scenario in test.get_scenarios() {
        if verbose { if ti == 0 { println!(); } else { println!("\n"); } ti += 1; }
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
                            println!("{}: Sending    {}   -> {} - {}", "INFO".bright_cyan(), "RAPI".bold(), "CLIENT".bold(), instr.value.italic());
                        }
                        rapi.write(instr.value.as_bytes());

                    },
                    Command::Expect => {
                        let now = Instant::now();
                        let mut elapsed = now.elapsed();

                        if verbose {
                            println!("{}: Expecting  {} -> {}   - {}", "INFO".bright_cyan(), "CLIENT".bold(), "RAPI".bold(), instr.value.italic());
                        }

                        let mut data = String::new();
                        while elapsed.as_millis() <= rapi.get_msg_timeout() {
                            rapi.read_str(&mut data);
                            if data.is_empty() {
                                elapsed = now.elapsed();
                                continue;
                            } else {
                                let test_info = if compare_str(instr.value.clone(), data.clone()) {
                                    "OKAY".green().bold()
                                } else {
                                    is_test_passed = false;
                                    "FAIL".red().bold()
                                };

                                if verbose {
                                    println!(" {} Got                         - {}", test_info, data.italic());
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
                            print!("{}: Sending    {} -> {} - {}", "INFO".bright_cyan(), "SERVER".bold(), "CLIENT".bold(), instr.value.italic());
                        }
                        ocpp.write(instr.value.as_bytes());
                    },
                    Command::Expect => {
                        let now = Instant::now();
                        let mut elapsed = now.elapsed();

                        if verbose {
                            print!("{}: Expecting  {} -> {} - {}", "INFO".bright_cyan(), "CLIENT".bold(), "SERVER".bold(), instr.value.italic());
                        }

                        let mut data = String::new();
                        while elapsed.as_millis() <= ocpp.get_msg_timeout() {
                            ocpp.read_str(&mut data);
                            if data.is_empty() {
                                elapsed = now.elapsed();
                                continue;
                            } else {
                                let test_info = if compare_str(instr.value.clone(), data.clone()) {
                                    "OKAY".green().bold()
                                } else {
                                    is_test_passed = false;
                                    "FAIL".red().bold()
                                };

                                if verbose {
                                    print!(" {} Got                         - {}", test_info, data.italic());
                                }
                                break;
                            }
                        }
                        if elapsed.as_millis() > ocpp.get_msg_timeout() {
                            is_test_passed = false;
                            break;
                        }
                    }
                }
            };
            if !is_test_passed {
                break;
            }
        }
        if is_test_passed {
            print!("\n {} {}\t{}", "Test".bold(), scenario.name().italic(), "PASSED".bold().green());
        } else {
            print!("\n {} {}\t{}", "Test".bold(), scenario.name().italic(), "FAILED".bold().red());
        }
    }   
    
    println!("\n\n---- {} ----", "Finishing testing".bold().blue());
}


fn compare_str(expt: String, data: String) -> bool {
    // println!("E: {}\nD: {}", expt, data);
    if expt.len() != data.len() {
        return false;
    }
    for (ch_e, ch_d) in expt.chars().zip(data.chars()) {
        if ch_e == '@' {
            continue;
        }
        if ch_d != ch_e {
            return false;
        }
    }
    true
}