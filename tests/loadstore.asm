        .ORIG x3000
        LDB R1, R2, #5
        LDW R3, R4, #-1
        STB R5, R6, #0
        STW R7, R0, #31
        LDW R1, R0, x-2
        .END
