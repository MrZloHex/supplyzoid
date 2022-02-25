# Supplyzoid

## Description

See it [here](../README.md)

## Supported Messages

### OCPP

 - Authorise
 - Boot Notification
 - Data Transfer
 - Heartbeat
 - Meter Values
 - Remote Start Transaction
 - Remote Stop Transaction
 - Reset
 - Start Transaction
 - Stop Transaction
 - Status Transaction

### RAPI

 - Boot Notification
 - Disable
 - Enable
 - State Transition
 - External Button
 - Get Ammeter Settings
 - Get Auth lock
 - Get Current Voltage
 - Get Fault Counter
 - Get Energy Usage
 - Get Fault Counter
 - Get Version
 - Get Voltmeter Settings
 - Print Text On LCD Disply
 - Reset
 - Set Ammeter Settings
 - Set Auth Lock
 - Set Heartbeat
 - Set Voltmeter Settings
 - Sleep

## Usage

### Target Arch

Now is available only STM32G070RB processor or __maybe__ any ARM M0+ 32 bit

### Compiling

__Dependencies__: 
 - Platformio
 - arm-none-eabi-gcc

To compile run:

```cmd
$ pio run
```

### Uploading

__Upload protocol__: st-link

Connect _STM32G070RB_ to st-link and to PC and run:

```cmd
$ pio run -t upload
```

### Setup

|        |  RX | TX  | Serial |
|--------|-----|-----|--------|
| USART1 | PC5 | PC4 | OCPP   |
| USART2 | PA3 | PA2 | RAPI   |


### Testing

Coming up soon...

## Architecture

