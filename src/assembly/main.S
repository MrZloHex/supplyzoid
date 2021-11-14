.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

#include "../../include/RCC.h"
#include "../../include/GPIO.h"
@ #include "../../include/delay.h"

.global delay

.section .text

.word	_estack
.word	_start 		  + 1
.word	_nmi_handler  + 1 
.word	_hard_fault   + 1 
.word	_memory_fault + 1 
.word	_bus_fault 	  + 1 
.word	_usage_fault  + 1 



_start:
	@ enabling clock for PORTA
	LDR R6, =RCC_APB2ENR
	LDR R5, [R6]
	ORR R5, 0x000000004 @ R E D C  B |A| R O    p.146 RM0008
				 		@ 0 0 0 0  0 |0| 0 0
	STR R5, [R6]


	@ setting mode to PA_4
	LDR R6, =GPIOA_CRL
	LDR R5, =0x00020000
	STR R5, [R6]

loop:
	@ set PA_4 to high
	LDR R6, =GPIOA_ODR
	LDR R5, =0x00000010
	STR R5, [R6]

	BL delay

	@ set PA_4 to high
	LDR R6, =GPIOA_ODR
	LDR R5, =0x00000000
	STR R5, [R6]

	BL delay

	B 	loop

exit:
	NOP
	B 	exit






_dummy:
_nmi_handler:
_hard_fault:
_memory_fault:
_bus_fault:
_usage_fault:
    ADD R0, 1
    ADD R1, 1
    B _dummy