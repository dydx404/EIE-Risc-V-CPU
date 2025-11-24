module Extend(
    input  wire [31:7] imm,
    input  wire [2:0]  immsrc,
    output reg  [31:0] immext
);

    always @* begin
        case(immsrc)
            2'b000: immext = {{20{imm[31]}}, imm[31:20]}; //I-type
            2'b001: immext = {{20{imm[31]}},imm[31:25], imm[11:7]}; //S-Type
            2'b010: immext = {{19{imm[31]}}, imm[31], imm[7], imm[30:25], imm[11:8], 1'b0}; //B-Type
            2'b011: immext = {{12{imm{31}}}, imm[31:12]} //U-Type
            2'b100: immext = {{11{imm[31]}}, imm[31], imm[19:12], imm[20], imm[30:21], 1'b0}; //J-Type
            default: immext = {{32{1'b0}}};
        endcase
    end

endmodule
