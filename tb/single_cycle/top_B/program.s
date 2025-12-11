# regfile-zero test

    addi x1, x0, 5        # x1 = 5
    addi x2, x0, 7        # x2 = 7

    addi x0, x0, 99       # attempt write to x0 (must be ignored)

    add  x3, x1, x2       # x3 = 12

    add  x4, x0, x3       # x4 = x0 + 12 = 12 (if x0 broken → incorrect)

    addi a0, x4, 0        # expected a0 = 12

    beq x0, x0, .         # infinite loop to stabilize output
