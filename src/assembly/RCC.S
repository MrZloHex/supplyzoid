.thumb
.syntax unified

#include "RCC.h"

.global rcc_apb2enr_clck_port



@ p.146 RM0008
rcc_apb2enr_clck_port:
    PUSH {FP, LR}

_if_porta:
    CMP R0, PORTA
    BNE _elif_portb
    MOV R2, 0x000000004
    B   _after_if
_elif_portb:
    CMP R0, PORTB
    BNE _elif_portc
    MOV R2, 0x000000008
    B   _after_if
_elif_portc:
    CMP R0, PORTC
    BNE _elif_portd
    MOV R2, 0x000000010
    B   _after_if
_elif_portd:
    CMP R0, PORTD
    BNE _elif_porte
    MOV R2, 0x000000020
    B   _after_if
_elif_porte:
    CMP R0, PORTE
    BNE _elif_portf
    MOV R2, 0x000000040
    B   _after_if
_elif_portf:
    CMP R0, PORTF
    BNE _elif_portg
    MOV R2, 0x000000080
    B   _after_if
_elif_portg:
    CMP R0, PORTG
    BNE _else
    MOV R2, 0x000000100
    B   _after_if
_else:
    MOV R0, 1
    B   _ret


_after_if:
    LDR R6, =RCC_APB2ENR
	LDR R5, [R6]
	ORR R5, R2
	STR R5, [R6]

    MOV R0, 0

_ret:
    POP {FP, LR}
    BX  LR


