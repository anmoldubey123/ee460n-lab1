        .ORIG x0200
A       LEA R0, B
        BRnzp A
B       .FILL x1234
        .END
