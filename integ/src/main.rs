mod serial;
use serial::{open_port, is_opened_port, read_port, write_port};

mod scen_parser;
use scen_parser::get_settings;


use clap::{load_yaml, App};

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    let scenario_fname = matches.value_of("scenario").unwrap();
    let (rapi, ocpp) = get_settings(scenario_fname);


    let rapi_serial = open_port(rapi.port(), rapi.baudrate(), 100);
    
    if let Some(e) = is_opened_port(&port) {
        eprintln!("ERROR: failed to open port `{}` at {} baudrate cause {}", rapi.port(), rapi.baudrate(), e);
        std::process::exit(1);
    };

    // let mut _port = port.unwrap();


}
