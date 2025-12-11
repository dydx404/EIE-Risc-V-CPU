# regfile-zero test

    addi x1, x0, 5
    addi x2, x0, 7

    addi x0, x0, 99       # should be ignored

    add  x3, x1, x2       # x3 = 12

    add  x4, x0, x3       # x4 = 12 if x0 works

    addi a0, x4, 0        # a0 = 12 expected

    beq x0, x0, .         # infinite loop
