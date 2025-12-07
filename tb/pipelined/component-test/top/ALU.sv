module ALU #(
    parameter LEN = 32
) (
    input  logic [LEN-1:0] aluop1,      // first operand (rs1)
    input  logic [LEN-1:0] aluop2,      // second operand (rs2 or ImmExt)
    input  logic [3:0]     alu_ctrl,    // operation selector
    output logic [LEN-1:0] aluout,      // ALU output
    output logic           zero         // 1 if result == 0
);

    always_comb begin
        unique case (alu_ctrl)
            4'b0000: aluout = aluop1 + aluop2;                 // ADD / ADDI
            4'b0001: begin
                     aluout = aluop1 - aluop2;                 // SUB // BEQ
                     zero = (aluout == {LEN{1'b0}});
            end

            4'b0010: aluout = aluop1 & aluop2;                 // AND / ANDI
            4'b0011: aluout = aluop1 | aluop2;                 // OR / ORI
            4'b0100: aluout = aluop1 ^ aluop2;                 // XOR / XORI

            // SLT (signed) // BLT
            4'b0101: begin
                if ($signed(aluop1) < $signed(aluop2))
                    aluout = {{(LEN-1){1'b0}}, 1'b1};
                else
                    aluout = {LEN{1'b0}};
            zero = (aluout == {LEN{1'b1}});
            end

            // SLTU (unsigned) // BLTU
            4'b0110: begin
                if (aluop1 < aluop2)
                    aluout = {{(LEN-1){1'b0}}, 1'b1};
                else
                    aluout = {LEN{1'b0}};
            zero = (aluout == {LEN{1'b1}});
            end

            4'b0111: aluout = aluop1 <<  aluop2[4:0];           // SLL / SLLI
            4'b1000: aluout = aluop1 >>  aluop2[4:0];           // SRL / SRLI
            4'b1001: begin
                     aluout = (aluop1 >= aluop2) ? 1 : 0;       // BGE
                     zero = (aluout == {LEN{1'b1}});
            end

            4'b1010: begin
                     aluout = ($unsigned(aluop1) >= $unsigned(aluop2)) ? 1 : 0;
                     zero = (aluout == {LEN{1'b1}});              // BGEU
            end

            4'b1011: aluout = $signed(aluop1) >>> aluop2[4:0];  // SRA / SRAI
            4'b1100: begin
                     aluout = aluop1 - aluop2;
                     zero = (aluout != {LEN{1'b0}});               // BNE
            end     

            // LUI: rd = imm (upper 20 bits, low 12 = 0) – ImmExt
            // For LUI, ControlUnit sets ALUSrc=1 so aluop2 = ImmExt.
            4'b1111: aluout = aluop2;

            default: aluout = {LEN{1'b0}};
        endcase
    end

    // zero flag 
    //zero = (aluout == {LEN{1'b0}});

endmodule

