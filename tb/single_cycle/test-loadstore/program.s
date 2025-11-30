    .text
    .globl main

main:
    # x1 = base address (0x00000100)
    lui   x1, 0x0
    addi  x1, x1, 0x100        # x1 = 0x100

    # x2 = 42
    addi  x2, x0, 42

    # store x2 at [x1 + 0]
    sw    x2, 0(x1)

    # x3 = 99
    addi  x3, x0, 99

    # store x3 at [x1 + 4]
    sw    x3, 4(x1)

    # load into x4
    lw    x4, 0(x1)

    # load into x5
    lw    x5, 4(x1)

    # a0 = x4 + x5  (expected: 42 + 99 = 141)
    add   a0, x4, x5

    # stay here forever (stable a0 output)
loop:
    jal   x0, loop
