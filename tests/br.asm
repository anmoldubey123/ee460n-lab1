        .ORIG x3000
START   BRn START
        BRz DONE
        BRp START
        BRnz DONE
        BRnp START
        BRzp DONE
        BRnzp START
        BR DONE
DONE    HALT
        .END
