        .ORIG x3000
        LSHF R2, R3, #3
        RSHFL R2, R3, #7
        RSHFA R2, R3, #7
        LSHF R0, R1, #0
        RSHFA R7, R7, #15
        .END
