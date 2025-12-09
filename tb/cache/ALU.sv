// ========================================
//   ALU.sv  (clean zero flag, clear opcodes)
// ========================================
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
        // default
        aluout = '0;

        unique case (alu_ctrl)
            4'b0000: begin
                // ADD / ADDI
                aluout = aluop1 + aluop2;
            end

            4'b0001: begin
                // SUB  (used for SUB / BEQ base)
                aluout = aluop1 - aluop2;
            end

            4'b0010: begin
                // AND / ANDI
                aluout = aluop1 & aluop2;
            end

            4'b0011: begin
                // OR / ORI
                aluout = aluop1 | aluop2;
            end

            4'b0100: begin
                // XOR / XORI
                aluout = aluop1 ^ aluop2;
            end

            4'b0101: begin
                // SLT (signed) / BLT
                aluout = ($signed(aluop1) < $signed(aluop2)) ? 32'd1 : 32'd0;
            end

            4'b0110: begin
                // SLTU (unsigned) / BLTU
                aluout = (aluop1 < aluop2) ? 32'd1 : 32'd0;
            end

            4'b0111: begin
                // SLL / SLLI
                aluout = aluop1 << aluop2[4:0];
            end

            4'b1000: begin
                // SRL / SRLI
                aluout = aluop1 >> aluop2[4:0];
            end

            4'b1001: begin
                // BGE (signed)
                aluout = ($signed(aluop1) >= $signed(aluop2)) ? 32'd1 : 32'd0;
            end

            4'b1010: begin
                // BGEU (unsigned)
                aluout = ($unsigned(aluop1) >= $unsigned(aluop2)) ? 32'd1 : 32'd0;
            end

            4'b1011: begin
                // SRA / SRAI
                aluout = $signed(aluop1) >>> aluop2[4:0];
            end

            4'b1100: begin
                // SUB (again) – used for BNE difference if you like
                aluout = aluop1 - aluop2;
            end

            4'b1111: begin
                // LUI: rd = ImmExt (aluop2)
                aluout = aluop2;
            end

            default: begin
                aluout = '0;
            end
        endcase
    end

    // Zero flag is **always** consistent: result == 0
    assign zero = (aluout == {LEN{1'b0}});

endmodule
