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
Try run tests from file [`integ_tests`](test_scenarios/integ_tests):
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
Example [scenario](test_scenarios/example_tests).

#### Settings

Settings can be written in form of key-value pairs separated with _space_. \
Available settings:

| Name              | Value Type | Def value | Description                                                        |
|------------------:|:----------:|:---------:|:-------------------------------------------------------------------|
| RST_TIMEOUT       | u128       |   10000   | Timeout in _ms_ for push _reset_ button on supplyzoid              |
| RAPI_PORT         | String     |     -     | Port to which is connected rapi_port on board                      |
| OCPP_PORT         | String     |     -     | Port to which is connected ocpp_port on board                      |
| RAPI_BAUD         | u32        |    9600   | Baudrate of rapi_port                                              |
| OCPP_BAUD         | u32        |    9600   | Baudrate of ocpp_port                                              |
| RAPI_PORT_TIMEOUT | u64        |    100    | Timeout in _ms_ for one try to read/write data from/into rapi_port |
| OCPP_PORT_TIMEOUT | u64        |    100    | Timeout in _ms_ for one try to read/write data from/into ocpp_port |
| RAPI_MSG_TIMEOUT  | u128       |    100    | Timeout in _ms_ for expecting message from rapi_port               |
| OCPP_MSG_TIMEOUT  | u128       |    100    | Timeout in _ms_ for expecting message from ocpp_port               |

#### Scenarios

Scenarios should start with _keyword_ `SCENARIO` and finish with `END`. \
One test file can include multiple scenarios. \
Also scenarios could be named by writting _name_ after keyword `SCENARIO`.

Inside scenario block you can write commands which will be execute at test time.

#### Command

Command contains 3 segments which is separeted with _space_.

Firstly goes _PORT_ with will communicate command. It can either `RAPI` either `OCPP`. \ 
Secondly goes type of command:
 - SEND - Send value for defined port
 - EXPT - Expect value from defined port up to timeout.

And thirdly goes value of command.

Example of command: `RAPI SEND $GS^30`

#### Notes

To make a comment at start of line put `#` and after a space write your comment.

In value of _EXPT_ command you can put `@` to indicate that this could be any character at real data. \
This maight be useful in situation when the expecting value contains timestamp with milliseconds and you can't be 100% sure that value would everytime as you define up to 1 ms so you can just put `@@@` and forget about ms.

### Passig the test

The test is considered __passed__ only if all send commands were executed without any trouble and all expected values were received. 

## Flags & Options

| Long     | Short | Type | Description                  |
|:--------:|:-----:|:----:|:----------------------------:|
| scenario | s     | opt  | Path to file with tests      |
| verbose  | v     | flag | Verbose output while testing |
| help     | h     | flag | Help output                  |
| version  | V     | flag | Displays version of app      |