mod serial;
use serial::{open_port, is_opened_port, read_port};

use std::io::{self, Write};

extern crate serialport;

fn main() {
    let port_name_rapi = "/dev/ttyACM0";
    let baud_rate_rapi: u32 = 9600;
    
    let port = open_port(port_name_rapi, baud_rate_rapi, 100);
    
    if let Some(e) = is_opened_port(&port) {
        eprintln!("ERROR: failed to open port `{}` at {} baudrate cause {}", port_name_rapi, baud_rate_rapi, e);
        std::process::exit(1);
    };

    let mut port = port.unwrap();

    loop {
        let mut serial_buf: Vec<u8> = vec![0; 100];
        match read_port(&mut port, serial_buf.as_mut_slice()) {
            Ok(t) => io::stdout().write_all(&serial_buf[..t]).unwrap(),
            Err(ref e) if e.kind() == std::io::ErrorKind::TimedOut => (),
            Err(e) => eprintln!("{:?}", e),
        }
    }
}
