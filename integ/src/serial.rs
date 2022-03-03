extern crate serialport;
use serialport::{SerialPort, Error};

use std::time::Duration;
use std::io::{self, Write};

pub fn open_port(port_name: &str, baud_rate: u32, timeout: u64) -> Result<Box<dyn SerialPort>, Error> {
    serialport::new(port_name, baud_rate)
        .timeout(Duration::from_millis(timeout))
        .open()
}

pub fn is_opened_port(port: &Result<Box<dyn SerialPort>, Error>) -> Option<&Error> {
    match port {
        Ok(_) => None,
        Err(e) => Some(e)
    }
}

pub fn read_port(port: &mut Box<dyn SerialPort>, buf: &mut [u8]) -> Result<usize, std::io::Error> {
    port.read(buf)
}



