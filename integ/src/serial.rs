extern crate serialport;
use serialport::{Error, SerialPort};

use std::io::{self, Write};
use std::time::Duration;

pub struct Serial {
    port: String,
    baudrate: u32,
    serial: Option<Box<dyn SerialPort>>,
    timeout: u64,
    expect_timeout: u128
}

impl Serial {
    pub fn init() -> Serial {
        Serial {
            port: String::new(),
            baudrate: 0,
            serial: None,
            timeout: 100,
            expect_timeout: 100
        }
    }
    pub fn new(port: String, baudrate: u32, timeout: u64, expect_timeout: u128) -> Serial {
        Serial {
            port,
            baudrate,
            serial: None,
            timeout,
            expect_timeout
        }
    }

    pub fn setup(&mut self, ex: Serial) {
        self.port = ex.port;
        self.baudrate = ex.baudrate;
        self.timeout = ex.timeout;
    }

    pub fn set_port(&mut self, port: &str) {
        self.port = port.to_string();
    }

    pub fn set_baudrate(&mut self, baud: u32) {
        self.baudrate = baud;
    }

    pub fn get_port(&self) -> &str {
        self.port.as_str()
    }

    pub fn get_baudrate(&self) -> u32 {
        self.baudrate.clone()
    }

    pub fn get_timeout(&self) -> u128 {
        self.expect_timeout.clone()
    }

    pub fn open(&mut self) {
        let port_r = serialport::new(self.port.as_str(), self.baudrate)
            .timeout(Duration::from_millis(self.timeout))
            .open();

        match port_r {
            Ok(port) => self.serial = Some(port),
            Err(e) => {
                eprintln!(
                    "ERROR: failed to open port `{}` at {} baudrate cause {}",
                    self.port, self.baudrate, e
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

    pub fn read(&mut self, buf: &mut Vec<u8>) -> Result<usize, std::io::Error> {
        self.serial.as_mut().unwrap().read(buf.as_mut_slice())
    }
}
