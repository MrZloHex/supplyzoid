.thumb
.syntax unified

#include "GPIO.h"

.global cfg_prt_p_mode

@ p.171 RM0008
cfg_prt_p_mode:
    PUSH {FP, LR}

    @ ARGS: R0 - PORT
    @       R1 - PIN 
    @       R2 - MODE

    @ LOC VARS: R3 - ADDR GPIO PORT
    @           R4 - VALUE 

    @ obtainiing PORT
_if_porta_cfg_prt_p_mode:
    CMP R0, PORTA
    BNE _elif_portb_cfg_prt_p_mode
    LDR R3, =GPIOA_CRL
    B   _endif_port_cfg_prt_p_mode
_elif_portb_cfg_prt_p_mode:
    CMP R0, PORTB
    BNE _elif_portc_cfg_prt_p_mode
    LDR R3, =GPIOB_CRL
    B   _endif_port_cfg_prt_p_mode
_elif_portc_cfg_prt_p_mode:
    CMP R0, PORTC
    BNE _elif_portd_cfg_prt_p_mode
    LDR R3, =GPIOC_CRL
    B   _endif_port_cfg_prt_p_mode
_elif_portd_cfg_prt_p_mode:
    CMP R0, PORTD
    BNE _elif_porte_cfg_prt_p_mode
    LDR R3, =GPIOD_CRL
    B   _endif_port_cfg_prt_p_mode
_elif_porte_cfg_prt_p_mode:
    CMP R0, PORTE
    BNE _elif_portf_cfg_prt_p_mode
    LDR R3, =GPIOE_CRL
    B   _endif_port_cfg_prt_p_mode
_elif_portf_cfg_prt_p_mode:
    CMP R0, PORTF
    BNE _elif_portg_cfg_prt_p_mode
    LDR R3, =GPIOF_CRL
    B   _endif_port_cfg_prt_p_mode
_elif_portg_cfg_prt_p_mode:
    CMP R0, PORTG
    BNE _else_cfg_prt_p_mode
    LDR R3, =GPIOG_CRL
    B   _endif_port_cfg_prt_p_mode
_else_cfg_prt_p_mode:
    MOV R0, 1
    B   _ret_cfg_prt_p_mode
_endif_port_cfg_prt_p_mode:


    @OBTAIN PIN

_ret_cfg_prt_p_mode:
    POP {FP, LR}
    BX  LR