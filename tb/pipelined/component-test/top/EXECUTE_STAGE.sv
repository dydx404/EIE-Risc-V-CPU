// ========================================
//   EXECUTE_STAGE.sv
//   Pipeline EX stage for RV32I CPU
// ========================================

module EXECUTE_STAGE (
    input  logic        clk,
    input  logic        rst,

    // -------------------------
    // From ID/EX pipeline register
    // -------------------------
    input  logic [31:0] pcE,           // PC value for branch target
    input  logic [31:0] rd1E,          // operand A (from register file)
    input  logic [31:0] rd2E,          // operand B (from register file)
    input  logic [31:0] extImmE,       // sign-extended immediate
    input  logic [4:0]  rdE,           // destination register (unchanged)

    // Control signals from ID/EX
    input  logic [3:0]  aluControlE,   // ALU control (e.g. ADD/SUB/XOR/SLT)
    input  logic        aluSrcE,       // select immediate vs register
    input  logic        branchE,       // BEQ/BNE/etc.
    input  logic        regWriteE,
    input  logic        memReadE,
    input  logic        memWriteE,
    input  logic [1:0]  resultSrcE,
    input  logic [2:0]  addressingModeE,

    // -------------------------
    // Forwarding inputs
    // -------------------------
    input  logic [1:0]  forwardAE,     // forwarding select for operand A
    input  logic [1:0]  forwardBE,     // forwarding select for operand B
    input  logic [31:0] aluResultM,    // forwarded from MEM stage
    input  logic [31:0] resultW,       // forwarded from WB stage

    // -------------------------
    // Outputs to EX/MEM register
    // -------------------------
    output logic [31:0] aluResultE,
    output logic [31:0] writeDataE,    // store data (forwarded rs2)
    output logic [31:0] branchTargetE, // PC + immediate
    output logic        branchTakenE   // final branch decision
);

    // ======================================================
    // Forwarding MUX for operand A
    // ======================================================
    logic [31:0] srcAE;

    always_comb begin
        unique case (forwardAE)
            2'b00: srcAE = rd1E;        // normal path
            2'b10: srcAE = aluResultM;  // from MEM
            2'b01: srcAE = resultW;     // from WB
            default: srcAE = rd1E;
        endcase
    end

    // ======================================================
    // Forwarding MUX for operand B (before ALUSrc)
    // ======================================================
    logic [31:0] tempBE;

    always_comb begin
        unique case (forwardBE)
            2'b00: tempBE = rd2E;
            2'b10: tempBE = aluResultM;
            2'b01: tempBE = resultW;
            default: tempBE = rd2E;
        endcase
    end

    // Store data (forwarded)
    assign writeDataE = tempBE;

    // ======================================================
    // ALUSrc MUX (select immediate or register)
    // ======================================================
    logic [31:0] srcBE;
    assign srcBE = aluSrcE ? extImmE : tempBE;

    // ======================================================
    // ALU
    // ======================================================
    logic zeroE;

    ALU alu_core (
        .aluop1(srcAE),
        .aluop2(srcBE),
        .alu_ctrl(aluControlE),
        .aluout(aluResultE),
        .zero(zeroE)
    );

    // ======================================================
    // Branch target calculation
    // ======================================================
    assign branchTargetE = pcE + extImmE;

    // BEQ-type decision (can expand later)
    assign branchTakenE = branchE && zeroE;

endmodule

