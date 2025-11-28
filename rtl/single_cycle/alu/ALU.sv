module ALU #(
    parameter LEN = 32
) (
    input  logic [LEN-1:0] aluop1,      // first operand
    input  logic [LEN-1:0] aluop2,      // second operand
    input  logic [3:0]     alu_ctrl,    // operation selector
    output logic [LEN-1:0] aluout,      // ALU output
    output logic           zero         // 1 if result == 0
);

    always_comb begin
        unique case (alu_ctrl)
            4'b0000: aluout = aluop1 + aluop2;                 // ADD
            4'b0001: aluout = aluop1 - aluop2;                 // SUB
            4'b0010: aluout = aluop1 & aluop2;                 // AND
            4'b0011: aluout = aluop1 | aluop2;                 // OR
            4'b0100: aluout = aluop1 ^ aluop2;                 // XOR

            4'b0101: begin                                     // SLT (signed)
                if ($signed(aluop1) < $signed(aluop2))
                    aluout = {{(LEN-1){1'b0}}, 1'b1};
                else
                    aluout = {LEN{1'b0}};
            end

            4'b0110: begin                                      // SLTU (unsigned)
                if (aluop1 < aluop2)
                    aluout = {{(LEN-1){1'b0}}, 1'b1};
                else
                    aluout = {LEN{1'b0}};
            end

            4'b0111: aluout = aluop1 << aluop2[4:0];            // SLL
            4'b1000: aluout = aluop1 >> aluop2[4:0];            // SRL

            4'b1011: aluout = $signed(aluop1) >>> aluop2[4:0];  // SRA / SRAI

            // LUI:
            rd = imm (upper 20 bits, low 12 = 0) - ImmExt
            // For LUI, ControlUnit sets ALUSrc=1 so aluop2 = ImmExt.

            4'b1111: aluout = aluop2;


            default: aluout = {LEN{1'b0}};                      // default case
        endcase
    end

    // zero flag
    assign zero = (aluout == {LEN{1'b0}});

endmodule
