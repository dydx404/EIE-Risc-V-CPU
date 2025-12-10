// ========================================
//   EXECUTE_STAGE.sv
//   Pipeline EX stage for RV32I CPU
// ========================================
module EXECUTE_STAGE (
    // -------------------------
    // From ID/EX pipeline register
    // -------------------------
    input  logic [31:0] pcE,           // PC value for branch target
    input  logic [31:0] rd1E,          // operand A (from register file)
    input  logic [31:0] rd2E,          // operand B (from register file)
    input  logic [31:0] extImmE,       // sign-extended immediate

    // Control signals from ID/EX
    input  logic [3:0]  aluControlE,   // ALU control
    input  logic        aluSrcE,       // select immediate vs register
    input  logic        branchE,       // this instruction is a branch

    // -------------------------
    // Forwarding inputs
    // -------------------------
    input  logic [1:0]  forwardAE,     // forwarding select for operand A
    input  logic [1:0]  forwardBE,     // forwarding select for operand B
    input  logic [31:0] aluResultM,    // EX/MEM ALU result for forwarding
    input  logic [31:0] resultW,       // MEM/WB writeback value for forwarding

    // -------------------------
    // Outputs to later stages
    // -------------------------
    output logic [31:0] aluResultE,    // main ALU result
    output logic [31:0] writeDataE,    // data to store in MEM
    output logic [31:0] branchTargetE, // branch target address
    output logic        branchTakenE   // asserted when branch should be taken
);

    // =====================================
    // Forwarding MUX: srcAE
    // =====================================
    logic [31:0] srcAE;

    always_comb begin
        unique case (forwardAE)
            2'b00: srcAE = rd1E;        // from ID/EX
            2'b10: srcAE = aluResultM;  // from EX/MEM
            2'b01: srcAE = resultW;     // from MEM/WB
            default: srcAE = rd1E;
        endcase
    end

    // =====================================
    // Forwarding MUX: tempBE (pre-ALUSrc)
    // =====================================
    logic [31:0] tempBE;

    always_comb begin
        unique case (forwardBE)
            2'b00: tempBE = rd2E;        // from ID/EX
            2'b10: tempBE = aluResultM;  // from EX/MEM
            2'b01: tempBE = resultW;     // from MEM/WB
            default: tempBE = rd2E;
        endcase
    end

    // This is the value that will go to the data memory on stores
    assign writeDataE = tempBE;

    // =====================================
    // ALUSrc mux
    // =====================================
    logic [31:0] srcBE;

    // If aluSrcE==1 -> use immediate; otherwise use register/forwarded
    assign srcBE = aluSrcE ? extImmE : tempBE;

    // =====================================
    // ALU
    // =====================================
    logic zeroE;   // currently unused for branches, but available

    ALU alu_core (
        .aluop1   (srcAE),
        .aluop2   (srcBE),
        .alu_ctrl (aluControlE),
        .aluout   (aluResultE),
        .zero     (zeroE)
    );

    // =====================================
    // Branch target = PC + ImmExt
    // =====================================
    assign branchTargetE = pcE + extImmE;

    // =====================================
    // Branch decision logic
    //
    // We compare the *register* values (after forwarding):
    //   cmpA = srcAE        (rs1)
    //   cmpB = tempBE       (rs2)
    //
    // Branch type is encoded in aluControlE.
    //
    // Expect the ControlUnit to use:
    //   BEQ  -> aluControl = 4'b0001
    //   BNE  -> aluControl = 4'b1100
    //   BLT  -> aluControl = 4'b0101
    //   BGE  -> aluControl = 4'b1001
    //   BLTU -> aluControl = 4'b0110
    //   BGEU -> aluControl = 4'b1010
    // =====================================
    logic        takeBranch;
    logic [31:0] cmpA, cmpB;

    assign cmpA = srcAE;
    assign cmpB = tempBE;

    always_comb begin
        takeBranch = 1'b0;

        if (branchE) begin
            unique case (aluControlE)
                4'b0001: takeBranch = (cmpA == cmpB);                         // BEQ
                4'b1100: takeBranch = (cmpA != cmpB);                         // BNE
                4'b0101: takeBranch = ($signed(cmpA) <  $signed(cmpB));       // BLT
                4'b1001: takeBranch = ($signed(cmpA) >= $signed(cmpB));       // BGE
                4'b0110: takeBranch = (cmpA < cmpB);                          // BLTU
                4'b1010: takeBranch = ($unsigned(cmpA) >= $unsigned(cmpB));   // BGEU
                default: takeBranch = 1'b0;
            endcase
        end
    end

    assign branchTakenE = takeBranch;

endmodule
