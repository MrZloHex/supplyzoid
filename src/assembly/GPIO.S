.thumb
.syntax unified

#include "GPIO.h"

.global cfg_prt_p_mode

@ p.171 RM0008
cfg_prt_p_mode:
    PUSH {FP, LR}

    NOP

_ret:
    POP {FP, LR}
    BX  LR