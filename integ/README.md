# Integ

## Description

Application created for performing integration tests on _Supplyzoid_. Tests can be written using secial syntax, which is described.

## Testing

### Compiling

For start testing you need to compile our just use precompiled version of _integ_. \
To compile run:
```cmd
$ cargo build --release
```

At `./integ/target/release/` will be produced `integ` executable which you can already use.

### Run tests

For run test you need to firstly _test scenarios_, but there are some for default testing. They are located at [`./integ/test_scenarios`](integ/test_scenarios).
Try run tests from file [`integ_tests`](integ/test_scenarios/integ_tests):
```cmd
$ ./integ --scenario="./test_scenarios/integ_tests"
```

If you see output like this:
```
RESET the board

----- Starting testing ----

 Test BOOTING	PASSED
 Test START CHARGING	PASSED
 Test STOP CHARGING	PASSED

---- Finishing testing ----
```

All is __OK__!

## Test syntax

### Segments

File with tests can be divided into 2 segments: _Settings_ and _Scenarios_.

#### Settings

Settings can be written in form of key-value pairs separated with _space_. \
Available settings:
 - RST_TIMEOUT \<u128\> - Timeout in _ms_ for push _reset_ button on supplyzoid
 - RAPI_PORT \<String\> - Port to which is connected rapi_port on board
 - OCPP_PORT \<String\> - Port to which is connected ocpp_port on board
 - RAPI_BAUD \<u32\> - Baudrate of rapi_port
 - OCPP_BAUD \<u32\> - Baudrate of ocpp_port
 - RAPI_PORT_TIMEOUT \<u64\> - Timeout in _ms_ for one try to read/write data from/into rapi_port
 - OCPP_PORT_TIMEOUT \<u64\> - Timeout in _ms_ for one try to read/write data from/into ocpp_port
 - RAPI_MSG_TIMEOUT \<u128\> - Timeout in _ms_ for expecting message from rapi_port
 - OCPP_MSG_TIMEOUT \<u128\> - Timeout in _ms_ for expecting message from ocpp_port

## Flags