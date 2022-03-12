use crate::serial::Serial;

use std::fs::File;
use std::io::{BufRead, BufReader};

#[derive(Debug)]
pub struct Error {
    pub error_msg: String,
    pub error_val: String,
}

pub enum SerialRecip {
    Rapi,
    Ocpp,
}

pub enum Command {
    Send,
    Expect,
}

pub struct Instruction {
    pub serial: SerialRecip,
    pub cmd: Command,
    pub value: String,
}

impl Instruction {
    pub fn new(ser: String, command: String, val: String) -> Result<Instruction, Error> {
        let serial = if ser == "RAPI" {
            SerialRecip::Rapi
        } else if ser == "OCPP" {
            SerialRecip::Ocpp
        } else {
            return Err(Error {
                error_msg: "unknown Serial Recipient".to_string(),
                error_val: ser,
            });
        };

        let cmd = if command == "SEND" {
            Command::Send
        } else if command == "EXPT" {
            Command::Expect
        } else {
            return Err(Error {
                error_msg: "unknown command".to_string(),
                error_val: command,
            });
        };

        let mut value = val;
        match serial {
            SerialRecip::Rapi => value.push('\r'),
            SerialRecip::Ocpp => value.push('\n'),
        }

        Ok(Instruction { serial, cmd, value })
    }
}


pub struct IntegrationTest {
    data: Vec<String>,
    scenarios: Vec<Scenario>,
    scen_quantity: usize
}

impl IntegrationTest {
    pub fn new(fname: &str) -> IntegrationTest {
        let file = File::open(fname).unwrap();
        let reader = BufReader::new(file);
        let mut test = IntegrationTest {
            data: Vec::new(),
            scenarios: Vec::new(),
            scen_quantity: 0
        };

        let mut scenario_name = String::new();
        let mut scenario = Vec::<String>::new();
        let mut is_scen = false;

        for line in reader.lines() {
            let line = line.unwrap();
            if line.is_empty() || line.starts_with('#') {
                continue;
            }
            if test.scen_quantity == 0 {
                if line.starts_with("SCENARIO") {
                    test.scen_quantity += 1;
                    is_scen = true;
                    scenario_name = line.strip_prefix("SCENARIO ").unwrap().to_string();
                    continue;
                }
                test.data.push(line.clone());
            }
            if test.scen_quantity > 0 {
                if is_scen {
                    if line.starts_with("END") {
                        is_scen = false;
                        test.scenarios.push(Scenario::new(scenario_name, scenario));
                        scenario_name = String::new();
                        scenario = Vec::new();
                        continue;
                    }
                    scenario.push(line);
                } else if line.starts_with("SCENARIO") {
                    test.scen_quantity += 1;
                    is_scen = true;
                    scenario_name = line.strip_prefix("SCENARIO ").unwrap().to_string();
                }
            }
        }

        test
    }

    pub fn get_settings(&self) -> (Serial, Serial, u128) {
        let mut rapi_port = String::new();
        let mut ocpp_port = String::new();
        let mut rapi_baud: u32 = 9600;
        let mut ocpp_baud: u32 = 9600;

        // Default settings
        let mut rapi_port_timeout = 100;
        let mut ocpp_port_timeout = 100;
        let mut rapi_expt_timeout = 100;
        let mut ocpp_expt_timeout = 100;
        let mut rst_timeout = 10_000;

        for line in self.data.clone() {
            let tokens: Vec<String> = line.split_whitespace().map(|s| s.to_string()).collect();
            match tokens[0].as_str() {
                "RAPI_PORT" => rapi_port = tokens[1].clone(),
                "RAPI_BAUD" => rapi_baud = tokens[1].parse::<u32>().unwrap(),
                "RAPI_PORT_TIMEOUT" => rapi_port_timeout = tokens[1].parse::<u64>().unwrap(),
                "RAPI_MSG_TIMEOUT" => rapi_expt_timeout = tokens[1].parse::<u128>().unwrap(),
                "OCPP_PORT" => ocpp_port = tokens[1].clone(),
                "OCPP_BAUD" => ocpp_baud = tokens[1].parse::<u32>().unwrap(),
                "OCPP_PORT_TIMEOUT" => ocpp_port_timeout = tokens[1].parse::<u64>().unwrap(),
                "OCPP_MSG_TIMEOUT" => ocpp_expt_timeout = tokens[1].parse::<u128>().unwrap(),
                "RST_TIMEOUT" => rst_timeout = tokens[1].parse::<u128>().unwrap(),
                _ => {
                    eprintln!("ERROR: unknown token in data segment: `{}`", line);
                    std::process::exit(1);
                }
            }
        }

        (
            Serial::new(rapi_port, rapi_baud, rapi_port_timeout, rapi_expt_timeout),
            Serial::new(ocpp_port, ocpp_baud, ocpp_port_timeout, ocpp_expt_timeout),
            rst_timeout,
        )
    }

    pub fn get_scenarios(&self) -> Vec<Scenario> {
        self.scenarios.clone()
    }
}


#[derive(Clone)]
pub struct Scenario {
    scenario: Vec<String>,
    name: String,
    line: usize,
}

impl Scenario {
    pub fn new(name: String, scenario: Vec<String>) -> Scenario {
        Scenario {
            scenario,
            name,
            line: 0
        }
    }

    pub fn next_instruction(&mut self) -> Option<Instruction> {
        if self.line == self.scenario.len() {
            return None;
        }

        let line = self.scenario[self.line].clone();
        let tokens: Vec<String> = line.splitn(3, ' ').map(|s| s.to_string()).collect();
        let instr = Instruction::new(tokens[0].clone(), tokens[1].clone(), tokens[2].clone());
        if let Err(e) = instr {
            eprintln!("ERROR: {}: {} on {} instruction", e.error_msg, e.error_val, self.line);
            std::process::exit(1);
        };

        let instr = instr.unwrap();
        self.line += 1;

        Some(instr)
    }

    pub fn name(&self) -> String {
        self.name.clone()
    }
}
