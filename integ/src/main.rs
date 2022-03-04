mod serial;
use serial::{open_port, is_opened_port, read_port, write_port, Serial};

mod scen_parser;
use scen_parser::Scenario;


use clap::{load_yaml, App};

fn main() {
    let yaml = load_yaml!("cli.yaml");
    let matches = App::from(yaml).get_matches();

    let scenario_fname = matches.value_of("scenario").unwrap();
    let mut scenario = Scenario::new(scenario_fname);
    let mut rapi = Serial::new("".to_string(), 0);
    let mut ocpp = Serial::new("".to_string(), 0);

    if matches.is_present("data_in_scen") {
        let settings = scenario.get_settings();
        rapi.set_port(settings.0.get_port());
        rapi.set_baudrate(settings.0.get_baudrate());
        ocpp.set_port(settings.1.get_port());
        ocpp.set_baudrate(settings.0.get_baudrate());
    }


    let rapi_serial = open_port(rapi.get_port(), rapi.get_baudrate(), 100);
    let ocpp_serial = open_port(ocpp.get_port(), ocpp.get_baudrate(), 100);
    
    is_opened_port(&rapi_serial, &rapi);
    is_opened_port(&ocpp_serial, &ocpp);

    // let mut _port = port.unwrap();


}
