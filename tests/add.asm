        .ORIG x3000
        ADD R1, R2, R3
        ADD R1, R1, #-1
        AND R4, R5, R6
        AND R0, R0, #0
        XOR R7, R1, R2
        XOR R3, R3, #15
        .END
