#!/bin/bash

if [ $1 -eq 1 ]
then
    cd ocpp && make
elif [ $1 -eq 0 ]
then
    cd evse && make
fi
