.text
.globl main

main:
    JAL ra, init
    j   main

init:
    li s2, 0x0   
    li s3, 0xff  # load s3 with 0xff
    li a0, 0x0   # result reg init at 0
    li s4, 0xf  

loopi:
    slli s2, s2, 1    # shift left by 1
    addi s2, s2, 1    # add 1
    and  a0, s3, s2   # and with 0x11111111 

wait:
    addi s4, s4, -1
    BNE s4, zero, wait
    addi s4, zero, 0xf
    bne s3, s2, loopi
    ADDI a0, zero, 0
    RET