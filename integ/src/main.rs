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
    }

    ocpp.open();
    rapi.open();
    // let mut _port = port.unwrap();


}
