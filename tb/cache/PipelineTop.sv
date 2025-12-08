module PipelineTop #(
    parameter DATA_WIDTH     = 32,
    parameter REG_ADDR_WIDTH = 5
)(
    input  logic                      clk,
    input  logic                      rst,
    input  logic [REG_ADDR_WIDTH-1:0] regaddr,   // test port
    output logic [DATA_WIDTH-1:0]     regdata,   // test port
    output logic [DATA_WIDTH-1:0]     ResultW,   // WB result
    output logic [31:0]               pc_out,    // current PC
    output logic [4:0]                rdW_out,   // WB dest reg
    output logic                      regWriteW_out // WB write enable
);

    // ============================================================
    // Global hazard / stall signals
    // ============================================================
    logic [1:0] forwardAE, forwardBE;
    logic       stallF, stallD, flushD, flushE;
    logic       cacheStallM;   // from MEM stage (data cache)

    // ============================================================
    // IF STAGE
    // ============================================================
    logic [31:0] instrF, pcF, pc_plus4F;

    fetch fetch_i (
        .clk        (clk),
        .rst        (rst),
        .en         (!stallF && !cacheStallM),  // freeze on hazards OR cache stall
        .branchE    (jumpE || branchTakenE),
        .jalrinsE   (jalrE),
        .pc_targetE (branchTargetE),
        .alu_outE   (aluResultE),

        .instrF     (instrF),
        .pcF        (pcF),
        .pc_plus4F  (pc_plus4F)
    );

    // ============================================================
    // IF → ID
    // ============================================================
    logic [31:0] instrD, pcD, pc_plus4D;

    IF_ID if_id_i (
        .clk       (clk),
        .rst       (rst),
        .stall     (stallD || cacheStallM),   // freeze on decode stall OR cache stall
        .flush     (flushD),
        .instrF    (instrF),
        .pcF       (pcF),
        .pcPlus4F  (pc_plus4F),

        .instrD    (instrD),
        .pcD       (pcD),
        .pcPlus4D  (pc_plus4D)
    );

    // ============================================================
    // ID STAGE
    // ============================================================
    logic        regwriteD, memwriteD, alusrcD;
    logic        branchD, jumpD, jalrinstrD;
    logic [1:0]  resultsrcD;
    logic [3:0]  alucontrolD;
    logic [2:0]  addressingcontrolD;
    logic [31:0] rd1D, rd2D, extimmD;
    logic [4:0]  rs1D, rs2D, rdD;
    logic        memReadD;

    Decode decode_i (
        .instrd          (instrD),
        .regwrite        (regWriteW),      // WB-stage write enable
        .CLK             (clk),
        .rdW             (rdW),
        .resultW         (ResultW),

        .addressingmodeD (addressingcontrolD),
        .resultsrcD      (resultsrcD),
        .regwriteD       (regwriteD),
        .memwriteD       (memwriteD),
        .memReadD        (memReadD),
        .branchD         (branchD),
        .jumpD           (jumpD),
        .jalrD           (jalrinstrD),
        .alucontrolD     (alucontrolD),
        .alusrcD         (alusrcD),

        .rd1             (rd1D),
        .rd2             (rd2D),
        .rs1D            (rs1D),
        .rs2D            (rs2D),
        .rdD             (rdD),
        .extimmD         (extimmD),

        .testRegAddress  (regaddr),
        .testRegData     (regdata)
    );

    // ============================================================
    // ID → EX
    // ============================================================
    logic [31:0] pcE, pcPlus4E, rd1E, rd2E, immExtE;
    logic [4:0]  rs1E, rs2E, rdE;
    logic        regWriteE, memWriteE, aluSrcE;
    logic        branchE, jumpE, jalrE;
    logic [1:0]  resultSrcE;
    logic [3:0]  aluControlE;
    logic [2:0]  addressingmodeE;
    logic        memReadE;

    ID_EX id_ex_i (
        .clk            (clk),
        .rst            (rst),
        .stall          (cacheStallM),   // freeze EX inputs while cache is busy
        .flush          (flushE),

        // Data in
        .pcD            (pcD),
        .pcPlus4D       (pc_plus4D),
        .rd1D           (rd1D),
        .rd2D           (rd2D),
        .immExtD        (extimmD),
        .rs1D           (rs1D),
        .rs2D           (rs2D),
        .rdD            (rdD),

        // Control in
        .regWriteD      (regwriteD),
        .memWriteD      (memwriteD),
        .memReadD       (memReadD),
        .resultSrcD     (resultsrcD),
        .aluSrcD        (alusrcD),
        .aluControlD    (alucontrolD),
        .branchD        (branchD),
        .jumpD          (jumpD),
        .jalrD          (jalrinstrD),
        .addressingmodeD(addressingcontrolD),

        // Data out
        .pcE            (pcE),
        .pcPlus4E       (pcPlus4E),
        .rd1E           (rd1E),
        .rd2E           (rd2E),
        .immExtE        (immExtE),
        .rs1E           (rs1E),
        .rs2E           (rs2E),
        .rdE            (rdE),

        // Control out
        .regWriteE      (regWriteE),
        .memWriteE      (memWriteE),
        .memReadE       (memReadE),
        .resultSrcE     (resultSrcE),
        .aluSrcE        (aluSrcE),
        .aluControlE    (aluControlE),
        .branchE        (branchE),
        .jumpE          (jumpE),
        .jalrE          (jalrE),
        .addressingmodeE(addressingmodeE)
    );

    // ============================================================
    // EX STAGE
    // ============================================================
    logic [31:0] aluResultE, writeDataE, branchTargetE;
    logic        branchTakenE;

    EXECUTE_STAGE execute_stage_i (
        .pcE          (pcE),
        .rd1E         (rd1E),
        .rd2E         (rd2E),
        .extImmE      (immExtE),

        .aluSrcE      (aluSrcE),
        .aluControlE  (aluControlE),
        .branchE      (branchE),

        .forwardAE    (forwardAE),
        .forwardBE    (forwardBE),
        .aluResultM   (aluResultM),
        .resultW      (ResultW),

        .aluResultE   (aluResultE),
        .writeDataE   (writeDataE),
        .branchTargetE(branchTargetE),
        .branchTakenE (branchTakenE)
    );

    // ============================================================
    // EX → MEM
    // ============================================================
    logic [31:0] pcPlus4M, aluResultM, writeDataM;
    logic [4:0]  rdM;
    logic        regWriteM, memWriteM;
    logic [1:0]  resultSrcM;
    logic [2:0]  addressingmodeM;

    EX_MEM ex_mem_i (
        .clk            (clk),
        .rst            (rst),
        .stall          (cacheStallM),   // hold EX/MEM on cache stall
        .flush          (1'b0),

        .pcPlus4E       (pcPlus4E),
        .aluResultE     (aluResultE),
        .writeDataE     (writeDataE),
        .rdE            (rdE),

        .regWriteE      (regWriteE),
        .memWriteE      (memWriteE),
        .resultSrcE     (resultSrcE),
        .addressingmodeE(addressingmodeE),

        .pcPlus4M       (pcPlus4M),
        .aluResultM     (aluResultM),
        .writeDataM     (writeDataM),
        .rdM            (rdM),
        .regWriteM      (regWriteM),
        .memWriteM      (memWriteM),
        .resultSrcM     (resultSrcM),
        .addressingmodeM(addressingmodeM)
    );

    // ============================================================
    // MEM STAGE  (DataCache + MainMemory live inside here)
    // ============================================================
    logic [31:0] readDataM;

    MEM_STAGE mem_stage_i (
        .clk            (clk),
        .rst            (rst),

        .aluResultM     (aluResultM),
        .writeDataM     (writeDataM),
        .memReadM       (resultSrcM == 2'b01),  // load when ResultSrc selects memory
        .memWriteM      (memWriteM),
        .addressingModeM(addressingmodeM),

        .readDataM      (readDataM),
        .cacheStallM    (cacheStallM)          // drives global stalls
    );

    // ============================================================
    // MEM → WB
    // ============================================================
    logic [31:0] readDataW, aluResultW, pcPlus4W;
    logic [4:0]  rdW;
    logic        regWriteW;
    logic [1:0]  resultSrcW;

    MEM_WB mem_wb_i (
        .clk        (clk),
        .rst        (rst),
        .stall      (1'b0),
        .flush      (1'b0),

        .readDataM  (readDataM),
        .aluResultM (aluResultM),
        .pcPlus4M   (pcPlus4M),
        .rdM        (rdM),
        .regWriteM  (regWriteM),
        .resultSrcM (resultSrcM),

        .readDataW  (readDataW),
        .aluResultW (aluResultW),
        .pcPlus4W   (pcPlus4W),
        .rdW        (rdW),
        .regWriteW  (regWriteW),
        .resultSrcW (resultSrcW)
    );

    // ============================================================
    // WB STAGE
    // ============================================================
    WB_STAGE wb_stage_i (
        .readDataW  (readDataW),
        .aluResultW (aluResultW),
        .pcPlus4W   (pcPlus4W),
        .resultSrcW (resultSrcW),

        .resultW    (ResultW)
    );

    assign rdW_out       = rdW;
    assign regWriteW_out = regWriteW;

    // ============================================================
    // HAZARD UNIT
    // ============================================================
    HazardUnit hazard_unit_i (
        .rs1D      (rs1D),
        .rs2D      (rs2D),
        .rs1E      (rs1E),
        .rs2E      (rs2E),
        .rdE       (rdE),
        .rdM       (rdM),
        .rdW       (rdW),
        .regWriteM (regWriteM),
        .regWriteW (regWriteW),
        .memReadE  (memReadE),
        .pcsrcE    (branchTakenE || jumpE),

        .forwardAE (forwardAE),
        .forwardBE (forwardBE),
        .stallF    (stallF),
        .stallD    (stallD),
        .flushD    (flushD),
        .flushE    (flushE)
    );

    assign pc_out = pcF;

// ============================================================
// ================= PIPELINE DEBUG OBSERVERS =================
// ============================================================
/*
always_ff @(posedge clk or posedge rst) begin
    if (rst) begin
        // no printing during reset
    end else begin
        // ---------------- IF ----------------
        $display("[IF ] PC=%h | instrF=%h",
                 pcF, instrF);

        // ---------------- ID ----------------
        $display("[ID ] rs1=%0d rs2=%0d rd=%0d | regW=%b memW=%b aluSrc=%b aluCtrl=%b",
                 rs1D, rs2D, rdD,
                 regwriteD, memwriteD,
                 alusrcD, alucontrolD);

        // ---------------- EX ----------------
        $display("[EX ] A=%h B=%h | ALU=%h | branchTaken=%b",
                 rd1E, rd2E,
                 aluResultE,
                 branchTakenE);

        // ---------------- MEM ----------------
        $display("[MEM] addr=%h writeData=%h memW=%b memR=%b readData=%h stall=%b",
                 aluResultM,
                 writeDataM,
                 memWriteM,
                 (resultSrcM == 2'b01),
                 readDataM,
                 cacheStallM);

        // ---------------- WB ----------------
        if (regWriteW) begin
            $display("[WB ] rd=%0d data=%h src=%b",
                     rdW, ResultW, resultSrcW);
        end

        // ---------------- HAZARD ----------------
        $display("[HZ ] stallF=%b stallD=%b flushD=%b flushE=%b | rs1D=%0d rs2D=%0d rdE=%0d memReadE=%b",
                 stallF, stallD, flushD, flushE,
                 rs1D, rs2D, rdE, memReadE);

        // ---------------- CYCLE SEPARATOR ----------------
        $display("------------------------------------------------------");
    end
end
*/
// ============================================================
// ============================================================



endmodule
