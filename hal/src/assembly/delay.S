.thumb
.syntax unified

.global delay_asm

delay_asm:
    PUSH {FP, LR}
	
    MOV R0, 0
	LDR R1, =0xFFFFF
_delay:
	ADD R0, R0, 1
	CMP R0, R1
	BNE _delay

    POP {FP, LR}
    BX  LR
