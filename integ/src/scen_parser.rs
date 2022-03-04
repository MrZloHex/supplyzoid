use crate::serial::Serial;

use std::fs::File;
use std::io::{BufRead, BufReader};

pub fn get_settings(fname: &str) -> (Serial, Serial) {
    let file = File::open(fname).unwrap();
    let reader = BufReader::new(file);

    (Serial::new("/dev/ttyACM0", 9600), Serial::new("/dev/ttyUSB0", 9600))
}