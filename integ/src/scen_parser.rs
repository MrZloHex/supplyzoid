use crate::serial::Serial;

use std::fs::File;
use std::io::{BufRead, BufReader};

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

        println!("SCEN: {:?}\nMETADATA: {:?}", scen.sceanario, scen.data);

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
                ";" => (),
                "" => (),
                _ => {
                    eprintln!("ERROR: unknown token in data segment: `{}`", line);
                    std::process::exit(1);
                }
            }
        }

        (Serial::new(rapi_port, rapi_baud), Serial::new(ocpp_port, ocpp_baud))
    }
}
