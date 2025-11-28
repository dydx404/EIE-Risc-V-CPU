# 0x00000000
main:
    jal   ra, L1          # 00c000ef  → ra = 0x00000004, PC = 0x0000000c

# 0x00000004  (return target of the jal)
    addi  a0, zero, 2     # 00200513  → a0 = 2

# 0x00000008
    jal   zero, L2        # 00c0006f  → unconditional jump to L2 (no link)

# 0x0000000c
L1:
    addi  a0, zero, 1     # 00100513  → a0 = 1

# 0x00000010
    jalr  zero, 0(ra)     # 00008067  → ret; jump back to 0x00000004

# 0x00000014
L2:
    addi  a0, zero, 3     # 00300513  → a0 = 3
