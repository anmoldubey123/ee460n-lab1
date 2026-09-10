; comment before ORIG
; another one
        .ORIG x3000
        HALT ; comment after no-operand instruction
        NOP ; another
        RET ; and here
; standalone comment line
        ADD R1, R2, R3 ; normal case
        .END ; comment after END
; stuff after END should be ignored
        ADD R7, R7, R7
