        .ORIG x3000
SUB     ADD R1, R1, #1
        RET
MAIN    JSR SUB
        JSRR R2
        JSR MAIN
        HALT
        .END
