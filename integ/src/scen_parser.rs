use crate::serial::Serial;


pub fn get_settings(fname: &str) -> (Serial, Serial) {
    (Serial::new("/dev/ttyACM0", 9600), Serial::new("/dev/ttyUSB0", 9600))
}