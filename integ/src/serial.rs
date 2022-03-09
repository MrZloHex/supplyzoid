extern crate serialport;
use serialport::SerialPort;

extern crate colored;
use colored::*;

use std::io::Write;
use std::time::Duration;

pub struct Serial {
    port: String,
    baudrate: u32,
    serial: Option<Box<dyn SerialPort>>,
    timeout: u64,
    expect_timeout: u128
}

impl Serial {
    pub fn new(port: String, baudrate: u32, timeout: u64, expect_timeout: u128) -> Serial {
        Serial {
            port,
            baudrate,
            serial: None,
            timeout,
            expect_timeout
        }
    }

    pub fn get_port(&self) -> &str {
        self.port.as_str()
    }

    pub fn get_baudrate(&self) -> u32 {
        self.baudrate
    }

    pub fn get_timeout(&self) -> u64 {
        self.timeout
    }

    pub fn get_msg_timeout(&self) -> u128 {
        self.expect_timeout
    }

    pub fn open(&mut self) {
        let port_r = serialport::new(self.port.as_str(), self.baudrate)
            .timeout(Duration::from_millis(self.timeout))
            .open();

        match port_r {
            Ok(port) => self.serial = Some(port),
            Err(e) => {
                eprintln!(
                    "{}: failed to open port {} at {} baudrate cause {}",
                    "ERROR".bright_red(), self.port.bold(), self.baudrate.to_string().bold(), e.description.to_lowercase().italic()
                );
                std::process::exit(1);
            }
        }
    }

    pub fn write(&mut self, data: &[u8]) -> Result<usize, std::io::Error> {
        self.serial.as_mut().unwrap().write(data)
    }

    pub fn read_str(&mut self, buf: &mut String) -> Result<usize, std::io::Error> {
        self.serial.as_mut().unwrap().read_to_string(buf)
    }
}
