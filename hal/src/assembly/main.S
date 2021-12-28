
.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb

#include "RCC.h"
#include "GPIO.h"


.section .text




_start:
	ldr   R0, =_estack
 	mov   SP, R0 

	
	@ enabling clock for PORTA
	LDR R6, =RCC_IOPENR
	LDR R5, [R6]
	LDR R4, =0x00000001
	ORRS R5, R5, R4   @ R . R F E  D C B |A|    5.4.12 RM0454
			          @ 0 . 0 0 0  0 0 0 |1|
	STR R5, [R6]


	@ setting mode to PA_0
	LDR R6, =GPIOA_MODER
	LDR R5, [R6]
	LDR R4, =0x00000001
	ORRS R5, R5, R4
	STR R5, [R6]

	@ set HIGH to PA_0
	LDR R6, =GPIOA_ODR
	LDR R5, [R6]
	LDR R4, =0x00000001
	ORRS R5, R5, R4
	STR R5, [R6]


exit:
	NOP
	B 	exit


@ delay_asm:
@     PUSH {FP, LR}
@     MOV R0, 0
@ 	LDR R1, =0xFFFFF
@ _delay:
@ 	ADD R0, R0, 1
@ 	CMP R0, R1
@ 	BNE _delay
@     POP {FP, LR}
@     BX  LR







_dummy:
_nmi_handler:
_hard_fault:
_memory_fault:
_bus_fault:
_usage_fault:
    @ ADD R0, R0, 1
    @ ADD R1, R1, 1
    B _dummy