.thumb
.syntax unified

#include "RCC.h"

.global rcc_apb2enr_clck_port



@ p.146 RM0008
rcc_apb2enr_clck_port:
    PUSH {FP, LR}

_if_porta_rcc_apb2enr_clck_port:
    CMP R0, PORTA
    BNE _elif_portb_rcc_apb2enr_clck_port
    MOV R2, 0x000000004
    B   _end_if_rcc_apb2enr_clck_port
_elif_portb_rcc_apb2enr_clck_port:
    CMP R0, PORTB
    BNE _elif_portc_rcc_apb2enr_clck_port
    MOV R2, 0x000000008
    B   _end_if_rcc_apb2enr_clck_port
_elif_portc_rcc_apb2enr_clck_port:
    CMP R0, PORTC
    BNE _elif_portd_rcc_apb2enr_clck_port
    MOV R2, 0x000000010
    B   _end_if_rcc_apb2enr_clck_port
_elif_portd_rcc_apb2enr_clck_port:
    CMP R0, PORTD
    BNE _elif_porte_rcc_apb2enr_clck_port
    MOV R2, 0x000000020
    B   _end_if_rcc_apb2enr_clck_port
_elif_porte_rcc_apb2enr_clck_port:
    CMP R0, PORTE
    BNE _elif_portf_rcc_apb2enr_clck_port
    MOV R2, 0x000000040
    B   _end_if_rcc_apb2enr_clck_port
_elif_portf_rcc_apb2enr_clck_port:
    CMP R0, PORTF
    BNE _elif_portg_rcc_apb2enr_clck_port
    MOV R2, 0x000000080
    B   _end_if_rcc_apb2enr_clck_port
_elif_portg_rcc_apb2enr_clck_port:
    CMP R0, PORTG
    BNE _else_rcc_apb2enr_clck_port
    MOV R2, 0x000000100
    B   _end_if_rcc_apb2enr_clck_port
_else_rcc_apb2enr_clck_port:
    MOV R0, 1
    B   _ret_rcc_apb2enr_clck_port


_end_if_rcc_apb2enr_clck_port:
    LDR R6, =RCC_APB2ENR
	LDR R5, [R6]
	ORR R5, R2
	STR R5, [R6]

    MOV R0, 0

_ret_rcc_apb2enr_clck_port:
    POP {FP, LR}
    BX  LR


