.thumb
.syntax unified

#include "RCC.h"

rcc_clck_port:
    PUSH {FP, LR}

    LDR R6, =RCC_APB2ENR
	LDR R5, [R6]
	ORR R5, 0x000000004 @ R E D C  B |A| R O    p.146 RM0008
				 		@ 0 0 0 0  0 |0| 0 0
	STR R5, [R6]

    POP {FP, LR}
    BX  LR