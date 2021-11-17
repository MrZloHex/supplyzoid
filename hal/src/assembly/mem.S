.thumb
.syntax unified

.global store
.global load

store:
    PUSH {FP, LR}

    @ args: 
    @       R0 - address
    @       R1 - value

    STR R1, [R0]

    POP {FP, LR}
    BX  LR

load:
    PUSH {FP, LR}

    @ args: 
    @       R0 - address
    
    @ ret:
    @       R0 - value

    LDR R1, [R0]
    MOV R0, R1

    POP {FP, LR}
    BX  LR
