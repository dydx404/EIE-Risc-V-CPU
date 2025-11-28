# basic arithmetic program test

    addi x1, x0, 10         # x1 = 10
    addi x2, x0, 4          # x2 = 4
    add  x3, x1, x2         # x3 = 14

    sub  x4, x3, x1         # x4 = 4
    xor  x5, x4, x2         # x5 = 4 ^ 4 = 0
    or   x6, x5, x1         # x6 = 10
    and  x7, x6, x3         # x7 = 10 & 14 = 10

    slt  x8, x7, x6         # x8 = (10 < 10) ? 1 : 0 = 0
    sltu x9, x4, x3         # x9 = (4 < 14) ? 1 : 0 = 1

    # accumulate into a0
    add  a0, x7, x9         # a0 = 10 + 1 = 11

    beq x0, x0, .           # infinite loop
