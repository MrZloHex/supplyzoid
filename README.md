# Supplyzoid

## What is supplyzoid

Add-on for [open evse](https://github.com/lincomatic/open_evse) which provides functionality of [OCPP](https://www.openchargealliance.org/) protocol.

## Description

This project is an add-on for [epen evse](). _Open Evse_ doesn't support OCPP protocol so _supplyzoid_ makes it.</br>
On hardware level, it is a STM32 with 2 [_serial connectors_](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter): one for RAPI communications which goes into _Open Evse_ board,
and  another for OCPP communication which goes into ESP bridge for recieving messages to OCPP Server.</br>
On firmware level, it supports full Core Profile of OCPP (for more details see [it](armm0plus/README.md)

## Project directories

 - amd64: Directory for testing some code and firmware architecture with target for `AMD64`
 - armm0plus: Main directory with firmware for `ARM M0+` processor, e.g. [STM32G070RB](https://www.st.com/en/microcontrollers-microprocessors/stm32g070rb.html)
 - chksum: Sources for mini-app for calculating checksum for [`RAPI`](https://github.com/lincomatic/open_evse/blob/development/firmware/open_evse/rapi_proc.h) messages
 - refs: Some imgaes, documents and datasheets used as guide for developing

