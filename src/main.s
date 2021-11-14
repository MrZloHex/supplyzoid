.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

.global vtable
.global reset_handler

.equ RCC_APB2ENR, 0x40021018
.equ GPIOA_CRL,   0x40010800
.equ GPIOA_ODR,   0x4001080C

// Vector table
.type vtable, %object
vtable:
	.word _estack
	.word reset_handler
.size vtable, .-vtable


.type reset_handler, %function
reset_handler:
	start:
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


		@ set PA_4 to high
		LDR R6, =GPIOA_ODR
		LDR R5, =0x00000010
		STR R5, [R6]



	exit:
		NOP
		B exit
		

.size reset_handler, .-reset_handler
