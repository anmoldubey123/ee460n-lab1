        .ORIG x3000
        NOP
        RTI
        RET
        HALT
        TRAP x25
        TRAP x23
        JMP R3
        JSRR R5
        NOT R1, R2
        .END
