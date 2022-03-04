extern crate serialport;
use serialport::{SerialPort, Error};

use std::time::Duration;
use std::io::{self, Write};


pub struct Serial {
    port: String,
    baudrate: u32
}

impl Serial {
    pub fn new(port: String, baudrate: u32) -> Serial {
        Serial {
            port,
            baudrate
        }
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
}

pub fn open_port(port_name: &str, baud_rate: u32, timeout: u64) -> Result<Box<dyn SerialPort>, Error> {
    serialport::new(port_name, baud_rate)
        .timeout(Duration::from_millis(timeout))
        .open()
}

pub fn is_opened_port(port: &Result<Box<dyn SerialPort>, Error>, serial: &Serial) {
    match port {
        Ok(_) => (),
        Err(e) => {
            eprintln!("ERROR: failed to open port `{}` at {} baudrate cause {}", serial.get_port(), serial.get_baudrate(), e);
            std::process::exit(1);
        }
    }
}

pub fn read_port(port: &mut Box<dyn SerialPort>, buf: &mut Vec<u8>) -> Result<usize, std::io::Error> {
    port.read(buf.as_mut_slice())
}

pub fn write_port(port: &mut Box<dyn SerialPort>, data: &[u8]) -> Result<usize, std::io::Error> {
    port.write(data)
}



