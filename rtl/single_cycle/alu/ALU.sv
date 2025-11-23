// rtl/single_cycle/alu/ALU.sv
module ALU #(
    parameter LEN = 32
) (
    input  logic [LEN-1:0] aluop1,      // first operand
    input  logic [LEN-1:0] aluop2,      // second operand
    input  logic [2:0]     alu_ctrl,    // operation selector
    output logic [LEN-1:0] aluout,  // ALU output
    output logic           zero         // 1 if result == 0
);

    always_comb begin
        unique case (alu_ctrl)
            3'b000: aluout = aluop1 + aluop2;                 // ADD
            3'b001: aluout = aluop1 - aluop2;                 // SUB
            3'b010: aluout = aluop1 & aluop2;                 // AND
            3'b011: aluout = aluop1 | aluop2;                 // OR
            3'b100: aluout = aluop1 ^ aluop2;                 // XOR

            3'b101: begin                                     // SLT (signed)
                if ($signed(aluop1) < $signed(aluop2))
                    aluout = {{(LEN-1){1'b0}}, 1'b1};
                else
                    aluout = {LEN{1'b0}};
            end

            3'b110: aluout = aluop1 << aluop2[4:0];           // SLL
            3'b111: aluout = aluop1 >> aluop2[4:0];           // SRL

            default: aluout = {LEN{1'b0}};                    // default case
        endcase
    end

    // zero flag
    assign zero = (aluout == {LEN{1'b0}});

endmodule
