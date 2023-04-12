/**
  ******************************************************************************
  * @file    controller_memory.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_MEMORY_H__
#define __CONTROLLER_MEMORY_H__

#include "fram.h"
#include "controller_ocpp.h"

typedef struct Controller_Memory_S
{
    FRAM fram;

    bool in_transaction;
    OCPP_ChargePointStatus status;
	bool available;

    uint16_t address_intrans;
    uint16_t address_status;
    uint16_t address_available;
} Controller_Memory;

void
_controller_memory_init
(
    Controller_Memory *mem,
    I2C_HandleTypeDef *hi2c
);

void
_controller_memory_reset(Controller_Memory *mem);

void
_controller_memory_store(Controller_Memory *mem);

void
_controller_memory_load(Controller_Memory *mem);


#endif /* __CONTROLLLER_MEMORY_H__ */