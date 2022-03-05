use crate::serial::Serial;

use std::fs::File;
use std::io::{BufRead, BufReader};

#[derive(Debug)]
pub struct Error {
    pub error_msg: String,
    pub error_val: String
}

pub enum SerialRecip {
    Rapi,
    Ocpp
}

pub enum Command {
    Send,
    Expect
}

pub struct Instruction {
    pub serial: SerialRecip,
    pub cmd: Command,
    pub value: String
}

impl Instruction {
    pub fn new(ser: String, command: String, value: String) -> Result<Instruction, Error> {
        let serial = if ser == "RAPI" {
            SerialRecip::Rapi
        } else if ser == "OCPP" {
            SerialRecip::Ocpp
        } else {
            return Err(Error{ error_msg: "unknown Serial Recipient".to_string(), error_val: ser})
        };

        let cmd = if command == "SEND" {
            Command::Send
        } else if command == "EXPECT" {
            Command::Expect
        } else {
            return Err(Error{ error_msg: "unknown command".to_string(), error_val: command})
        };

        Ok(
            Instruction{
                serial,
                cmd,
                value
            }
        )

        
    }
}

pub struct Scenario {
    data: Vec<String>,
    sceanario: Vec<String>,
    line: usize,
    fcap: usize
}

impl Scenario {
    pub fn new(fname: &str) -> Scenario {
        let file = File::open(fname).unwrap();
        let reader = BufReader::new(file);
        let mut scen = Scenario {
            data: Vec::new(),
            sceanario: Vec::new(),
            line: 0,
            fcap: 0
        };

        let mut is_sc = false;
        for line in reader.lines() {
            let line = line.unwrap();
            if line == "" || line.starts_with('#') { continue; }
            if is_sc {
                scen.sceanario.push(line);
                scen.fcap += 1;
            } else {
                if line == "SCENARIO".to_string() {
                    is_sc = true;
                    continue;
                }
                scen.data.push(line);
            }
        }

        // println!("SCEN: {:?}\nMETADATA: {:?}", scen.sceanario, scen.data);

        scen
    }

    pub fn get_settings(&mut self) -> (Serial, Serial) {
        let mut rapi_port = String::new();
        let mut ocpp_port = String::new();
        let mut rapi_baud: u32 = 0;
        let mut ocpp_baud: u32 = 0;

        for line in self.data.clone() {
            let tokens: Vec<String> = line.split_whitespace().map(|s| s.to_string()).collect();
            match tokens[0].as_str() {
                "RAPI_PORT" => rapi_port = tokens[1].clone(),
                "RAPI_BAUD" => rapi_baud = tokens[1].parse::<u32>().unwrap(),
                "OCPP_PORT" => ocpp_port = tokens[1].clone(),
                "OCPP_BAUD" => ocpp_baud = tokens[1].parse::<u32>().unwrap(),
                _ => {
                    eprintln!("ERROR: unknown token in data segment: `{}`", line);
                    std::process::exit(1);
                }
            }
        }

        (Serial::new(rapi_port, rapi_baud), Serial::new(ocpp_port, ocpp_baud))
    }

    pub fn next_instruction(&mut self) -> Option<Instruction> {
        if self.line == self.fcap { return None; }

        let line = self.sceanario[self.line].clone();
        let tokens: Vec<String> = line.split_whitespace().map(|s| s.to_string()).collect();
        let instr = Instruction::new(tokens[0].clone(), tokens[1].clone(), tokens[2].clone());
        if let Err(e) = instr {
            eprintln!("{}: {}", e.error_msg, e.error_val);
            std::process::exit(1);
        };

        let instr = instr.unwrap();
        self.line += 1;

        Some(instr)
    }
}