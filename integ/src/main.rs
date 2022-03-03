extern crate serialport;

use std::time::Duration;
use std::io::{self, Write};

fn main() {
    let port_name = "/dev/ttyACM0";
    let baud_rate: u32 = 9600;
    
    let port = serialport::new(port_name, baud_rate)
    .timeout(Duration::from_millis(100))
    .open();
    
    match port {
        Ok(mut port) => {
            let mut serial_buf: Vec<u8> = vec![0; 1000];
            println!("Receiving data on {} at {} baud:", &port_name, &baud_rate);
            loop {
                match port.read(serial_buf.as_mut_slice()) {
                    Ok(t) => io::stdout().write_all(&serial_buf[..t]).unwrap(),
                    Err(ref e) if e.kind() == io::ErrorKind::TimedOut => (),
                    Err(e) => eprintln!("{:?}", e),
                }
            }
        }
        Err(e) => {
            eprintln!("Failed to open \"{}\". Error: {}", port_name, e);
            std::process::exit(1);
        }
    }


    println!("Hello, world!");
}
