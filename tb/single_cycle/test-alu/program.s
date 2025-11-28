    .text
    .globl main

main:
    # x1 = 15
    addi x1, x0, 15

    # x2 = 7
    addi x2, x0, 7

    # --- ALU operations ---
    # x3 = x1 + x2 = 22
    add  x3, x1, x2

    # x4 = x1 - x2 = 8
    sub  x4, x1, x2

    # x5 = x1 XOR x2 = 15 ^ 7 = 8
    xor  x5, x1, x2

    # x6 = x1 OR x2 = 15 | 7 = 15
    or   x6, x1, x2

    # x7 = x1 AND x2 = 15 & 7 = 7
    and  x7, x1, x2

    # x8 = x1 << 1 = 30
    sll  x8, x1, x2   # 7 & 0x1F = 7 → 15 << 7 = 1920
                      # better use immediate form instead below

    # replace with immediate form (cleaner):
    addi x8, x1, 0    # x8 = 15
    slli x8, x8, 1    # x8 = 30

    # x9 = arithmetic right shift
    # 15 >> 1 = 7
    srai x9, x1, 1

    # x10 = slt(x1, x2) = 15 < 7 ? 0 : 0
    slt  x10, x1, x2  # = 0

    # x11 = sltu(x1, x2) = 15 < 7 ? 0 : 0
    sltu x11, x1, x2  # = 0

    # --- Combine results so the TB can check correctness ---
    # a0 = x3 + x4 + x5 + x6 + x7 + x8 + x9 + x10 + x11
    #     = 22 + 8 + 8 + 15 + 7 + 30 + 7 + 0 + 0
    #     = 97

    add  a0, x3, x4     # 22 + 8 = 30
    add  a0, a0, x5     # 30 + 8 = 38
    add  a0, a0, x6     # 38 + 15 = 53
    add  a0, a0, x7     # 53 + 7 = 60
    add  a0, a0, x8     # 60 + 30 = 90
    add  a0, a0, x9     # 90 + 7 = 97
    add  a0, a0, x10    # still 97
    add  a0, a0, x11    # still 97

loop:
    jal  x0, loop       # keep a0 stable forever
