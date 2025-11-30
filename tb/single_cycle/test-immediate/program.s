    .text
    .globl main

main:
    # --- I-type immediates ---
    addi x1, x0, -5          # x1 = -5
    addi x2, x0, 10          # x2 = 10
    andi x3, x2, 0xF          # x3 = 10 & 15 = 10
    ori  x4, x1, 0xF          # x4 = (-5) | 15 = 0xFFFFFFFB | 0xF = 0xFFFFFFFF = -1
    xori x5, x2, 3            # x5 = 10 ^ 3 = 9

    # slti / sltiu tests (signed vs unsigned)
    slti  x6, x1, 0           # -5 < 0 → x6 = 1
    sltiu x7, x1, 1           # unsigned(-5) = large → >1 → x7 = 0

    # --- Shift immediates (very important) ---
    addi x8, x0, 16           # x8 = 16
    slli x8, x8, 1            # 16 << 1 = 32
    srli x9, x8, 2            # 32 >> 2 = 8
    srai x10, x1, 1           # -5 >> 1 = -3   (arithmetic shift)

    # --- U-type (LUI) ---
    # LUI loads upper 20 bits, lower 12 bits = 0
    lui x11, 0x12345          # x11 = 0x12345000

    # --- S-type + LW test ---
    # Base at 0x100
    lui  x12, 0x0
    addi x12, x12, 0x100      # x12 = 0x100
    addi x13, x0, 42
    sw   x13, 0(x12)          # MEM[0x100] = 42
    lw   x14, 0(x12)          # x14 = 42

    # --- B-type immediate ---
    # Simple forward branch that skips one instruction
    addi x15, x0, 1
    beq  x15, x15, skip       # always taken
    addi x15, x15, 99         # skipped
skip:
    addi x15, x15, 2          # x15 = 3

    # --- J-type immediate ---
    jal x0, done_j
    addi x2, x2, 99           # skipped
done_j:

    # --- Combine all results into a0 ---
    # Expected final value (compute below)
    add  a0, x0, x3           # a0 = 10
    add  a0, a0, x5           # +9  = 19
    add  a0, a0, x6           # +1  = 20
    add  a0, a0, x8           # +32 = 52
    add  a0, a0, x9           # +8  = 60
    add  a0, a0, x10          # +(-3) = 57
    add  a0, a0, x11          # +0x12345000
    add  a0, a0, x14          # +42
    add  a0, a0, x15          # +3

loop:
    jal x0, loop              # freeze a0 forever
