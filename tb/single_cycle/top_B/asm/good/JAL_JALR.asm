test_jal:
    jal ra, target      # Jump to target, save return address
    addi a0, zero, 0x2
    j end_test          # Skip over target code
    
target:
    addi a0, zero, 0x1      # Load test value
    jalr zero, ra, 0   # Return to caller
    
end_test:
	addi a0, zero, 0x3