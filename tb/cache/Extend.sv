// ========================================
//   Extend.sv  (FIXED for Immediate[31:7])
// ========================================
// ImmSrc encoding (from your ControlUnit):
//   3'b000 : I-type
//   3'b001 : S-type
//   3'b010 : B-type
//   3'b011 : J-type
//   3'b100 : U-type

module Extend (
    input  logic [31:7] Immediate,   // instr[31:7]
    input  logic [2:0]  ImmSrc,
    output logic [31:0] ImmExt
);

    always_comb begin
        unique case (ImmSrc)

            // -----------------------------
            // I-type: instr[31:20]
            // -----------------------------
            3'b000: begin
                ImmExt = {{20{Immediate[31]}}, Immediate[31:20]};
            end

            // -----------------------------
            // S-type: instr[31:25] | instr[11:7]
            // -----------------------------
            3'b001: begin
                ImmExt = {{20{Immediate[31]}},
                          Immediate[31:25],
                          Immediate[11:7]};
            end

            // -----------------------------
            // B-type (branch):
            // imm[12]   = instr[31]
            // imm[11]   = instr[7]
            // imm[10:5] = instr[30:25]
            // imm[4:1]  = instr[11:8]
            // imm[0]    = 0
            // -----------------------------
            3'b010: begin
                ImmExt = {{19{Immediate[31]}},
                          Immediate[31],        // bit 12
                          Immediate[7],         // bit 11
                          Immediate[30:25],     // bits 10:5
                          Immediate[11:8],      // bits 4:1
                          1'b0};                // bit 0
            end

            // -----------------------------
            // J-type (JAL):
            // imm[20]   = instr[31]
            // imm[19:12]= instr[19:12]
            // imm[11]   = instr[20]
            // imm[10:1] = instr[30:21]
            // imm[0]    = 0
            // -----------------------------
            3'b011: begin
                ImmExt = {{11{Immediate[31]}},
                          Immediate[31],        // bit 20
                          Immediate[19:12],     // bits 19:12
                          Immediate[20],        // bit 11
                          Immediate[30:21],     // bits 10:1
                          1'b0};                // bit 0
            end

            // -----------------------------
            // U-type (LUI / AUIPC):
            // imm[31:12] = instr[31:12]
            // -----------------------------
            3'b100: begin
                ImmExt = {Immediate[31:12], 12'b0};
            end

            default: begin
                ImmExt = 32'b0;
            end
        endcase
    end

endmodule
