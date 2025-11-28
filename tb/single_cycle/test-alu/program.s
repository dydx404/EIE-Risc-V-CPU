# --- Setup test values ---
addi x1, x0, -16          # x1 = 0xFFFFFFF0  (signed -16)
addi x2, x0, 3            # x2 = 3 (shift amount)

# --- Test SRA x1 >> x2 ---
sra  x3, x1, x2           # expected x3 = -16 >> 3 = -2 (0xFFFFFFFE)

# --- Now test SRAI ---
srai x4, x1, 3            # expected x4 = -2 (0xFFFFFFFE)

# --- Mix arithmetic with shifts ---
addi x5, x0, 32           # x5 = 32
sra  x6, x5, x2           # x6 = 32 >> 3 = 4
srai x7, x5, 3            # x7 = 4

# --- Combine everything into a0 ---
# a0 = x3 + x4 + x6 + x7 = (-2) + (-2) + 4 + 4 = 4

add  a0, x3, x4           # a0 = -2 + -2 = -4
add  a0, a0, x6           # a0 = -4 + 4 = 0
add  a0, a0, x7           # a0 = 0 + 4 = 4

# Expected final output:
# a0 = 4

beq x0, x0, .
