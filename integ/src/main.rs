mod serial;
use serial::Serial;

mod scen_parser;
use scen_parser::Scenario;


use clap::{load_yaml, App};

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    let scenario_fname = matches.value_of("scenario").unwrap();
    let mut scenario = Scenario::new(scenario_fname);
    let mut rapi = Serial::init();
    let mut ocpp = Serial::init();

    if matches.is_present("data_in_scen") {
        let settings = scenario.get_settings();
        rapi.setup(settings.0);
        ocpp.setup(settings.1);
    } else {
        let mut setuped = true;
        if matches.is_present("rapi_port") {
            rapi.set_port(matches.value_of("rapi_port").unwrap());
        } else {
            eprintln!("ERROR: doesn't provided port for RAPI"); setuped = false;
        }
        if matches.is_present("rapi_baud") {
            rapi.set_baudrate(matches.value_of("rapi_baud").unwrap().parse::<u32>().unwrap());
        } else {
            eprintln!("ERROR: doesn't provided baudrate for RAPI"); setuped = false;
        }
        if matches.is_present("ocpp_port") {
            ocpp.set_port(matches.value_of("ocpp_port").unwrap());
        } else {
            eprintln!("ERROR: doesn't provided port for OCPP"); setuped = false;
        }
        if matches.is_present("ocpp_baud") {
            ocpp.set_baudrate(matches.value_of("ocpp_baud").unwrap().parse::<u32>().unwrap());
        } else {
            eprintln!("ERROR: doesn't provided baudrate for OCPP"); setuped = false;
        }
 
        if !setuped { std::process::exit(1); }
    }

    ocpp.open();
    rapi.open();

}
