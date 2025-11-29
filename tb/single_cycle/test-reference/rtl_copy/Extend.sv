// Sign-extend / immediate generator for RV32I
// Immediate = instr[31:7]  (upper 25 bits of instruction)
module Extend #(
    parameter DATA_WIDTH = 32
) (
    input  logic [31:7] Immediate,   // instr[31:7]
    input  logic [2:0]  ImmSrc,      // 000=I, 001=S, 010=B, 011=J, 100=U
    output logic [31:0] ImmExt       // sign-extended immediate
);

    always_comb begin
        unique case (ImmSrc)

            // I-type: imm[11:0] = instr[31:20]
            // 32 = 20 sign bits + 12 payload
            3'b000: ImmExt = {{20{Immediate[31]}}, Immediate[31:20]};

            // S-type: imm[11:5] = instr[31:25], imm[4:0] = instr[11:7]
            3'b001: ImmExt = {{20{Immediate[31]}}, Immediate[31:25], Immediate[11:7]};

            // B-type:
            // imm[12]   = instr[31]
            // imm[11]   = instr[7]
            // imm[10:5] = instr[30:25]
            // imm[4:1]  = instr[11:8]
            // imm[0]    = 0
            // total payload = 13 bits → 19 sign bits
            3'b010: ImmExt = {{19{Immediate[31]}},
                               Immediate[31],        // imm[12]
                               Immediate[7],         // imm[11]
                               Immediate[30:25],     // imm[10:5]
                               Immediate[11:8],      // imm[4:1]
                               1'b0};                // imm[0]

            // J-type (JAL):
            // imm[20]     = instr[31]
            // imm[19:12]  = instr[19:12]
            // imm[11]     = instr[20]
            // imm[10:1]   = instr[30:21]
            // imm[0]      = 0
            // payload = 21 bits → 11 sign bits
            3'b011: ImmExt = {{11{Immediate[31]}},
                               Immediate[31],        // imm[20]
                               Immediate[19:12],     // imm[19:12]
                               Immediate[20],        // imm[11]
                               Immediate[30:21],     // imm[10:1]
                               1'b0};                // imm[0]

            // U-type (LUI/AUIPC):
            // imm[31:12] placed in upper bits, low 12 bits are zero
            3'b100: ImmExt = {Immediate[31:12], 12'b0};

            default: ImmExt = 32'b0;
        endcase
    end

endmodule
