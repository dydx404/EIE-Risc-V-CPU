main:
	addi	t1, zero, 0xf
    addi	a0, t1, 0x0
    addi 	a1, t1, 0x0
iloop:
	addi	a0, a1, 0
    addi	a1, a1, -1
    bne		a1, zero, iloop