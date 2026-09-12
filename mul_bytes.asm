; Name: Anmol Dubey
; UTEID: ad56328

        .ORIG x3000
        LEA R0, PTR
        LDW R0, R0, #0 ; R0 = x3100

        AND R3, R3, #0 
        AND R4, R4, #0

        ;load operands
        LDB R1, R0, #0  ; r2 <- m[x3100] loading first operand
        LDB R2, R0, #1 ; r1 <- m[x3101] loading second operand
        BRz DONE


;multiply loop. r3 = r1 * r2
LOOP    ADD R3, R3, R1 ; accumulating contents of r1 into r3
        ADD R2, R2, #-1 ; decrement r2 serving as counter
        BRp LOOP

        STB R3, R0, #2 ; r3 -> m[x3102]


        ;load overflow check bitmask into R5
        LEA R5, FLAG
        LDW R5, R5, #0

        AND R5, R5, R3 ; AND product with xFF00
        BRz DONE
        ADD R4, R4, #1


DONE    STB R4, R0, #3 
        STB R3, R0, #2

        HALT



PTR     .FILL x3100
FLAG    .FILL xFF00

        .END


