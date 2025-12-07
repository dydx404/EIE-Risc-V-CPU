module PipelineTop #(
    parameter DATA_WIDTH     = 32,
    parameter REG_ADDR_WIDTH = 5
)(
    input  logic                       clk,
    input  logic                       rst,
    input  logic [REG_ADDR_WIDTH-1:0]  regaddr,  // (currently unused)
    output logic [DATA_WIDTH-1:0]      regdata,  // (currently unused)
    output logic [DATA_WIDTH-1:0]      ResultW
);

    // =========================================================
    // IF STAGE
    // =========================================================
    logic [DATA_WIDTH-1:0] instrF;
    logic [DATA_WIDTH-1:0] pcF;
    logic [DATA_WIDTH-1:0] pc_plus4F;

    // Hazard control
    logic        stallF;
    logic        stallD;
    logic        flushD;
    logic        flushE;

    // From EX / control flow
    logic [31:0] branchTargetE;
    logic [31:0] aluResultE;
    logic        branchTakenE;
    logic        jumpE;
    logic        jalrE;

    fetch fetch_i (
        .clk        (clk),
        .rst        (rst),
        .en         (!stallF),

        .branchE    (jumpE || branchTakenE),
        .jalrins    (jalrE),
        .pc_targetE (branchTargetE),
        .alu_outE   (aluResultE),

        .instrF     (instrF),
        .pcF        (pcF),
        .pc_plus4F  (pc_plus4F)
    );

    // =========================================================
    // IF → ID
    // =========================================================
    logic [DATA_WIDTH-1:0] instrD;
    logic [DATA_WIDTH-1:0] pcD;
    logic [DATA_WIDTH-1:0] pc_plus4D;

    IF_ID if_id_i (
        .clk      (clk),
        .rst      (rst),
        .stall    (stallD),
        .flush    (flushD),

        .pcF      (pcF),
        .pcPlus4F (pc_plus4F),
        .instrF   (instrF),

        .pcD      (pcD),
        .pcPlus4D (pc_plus4D),
        .instrD   (instrD)
    );

    // =========================================================
    // ID STAGE
    // =========================================================
    logic                  regwriteD;
    logic [1:0]            resultsrcD;
    logic                  memwriteD;
    logic                  memReadD;           // NEW: load flag at D
    logic [3:0]            alucontrolD;
    logic                  alusrcD;
    logic [DATA_WIDTH-1:0] rd1D;
    logic [DATA_WIDTH-1:0] rd2D;
    logic [4:0]            rs1D;
    logic [4:0]            rs2D;
    logic [4:0]            rdD;
    logic [DATA_WIDTH-1:0] extimmD;
    logic [2:0]            addressingcontrolD;
    logic                  jumpD;
    logic                  branchD;
    logic                  jalrinstrD;

    // WB stage outputs (defined later but used as inputs here)
    logic [DATA_WIDTH-1:0] resultW;
    logic                  regWriteW;
    logic [4:0]            rdW;

    Decode decode_i (
        .instrd         (instrD),
        .regwrite       (regWriteW),
        .CLK            (clk),
        .rdW            (rdW),
        .resultW        (resultW),

        .addressingmodeD(addressingcontrolD),
        .resultsrcD     (resultsrcD),
        .regwriteD      (regwriteD),
        .memwriteD      (memwriteD),
        .memReadD       (memReadD),        // NEW
        .branchD        (branchD),
        .jumpD          (jumpD),
        .jalrD          (jalrinstrD),
        .alucontrolD    (alucontrolD),
        .alusrcD        (alusrcD),
        .rd1            (rd1D),
        .rd2            (rd2D),
        .rs1D           (rs1D),
        .rs2D           (rs2D),
        .rdD            (rdD),
        .extimmD        (extimmD)
    );

    // =========================================================
    // ID → EX
    // =========================================================
    logic [31:0] pcE;
    logic [31:0] pcPlus4E;
    logic [31:0] rd1E;
    logic [31:0] rd2E;
    logic [31:0] immExtE;
    logic [4:0]  rs1E;
    logic [4:0]  rs2E;
    logic [4:0]  rdE;

    logic        regWriteE;
    logic        memReadE;          // NEW: in EX
    logic        memWriteE;
    logic [1:0]  resultSrcE;
    logic        aluSrcE;
    logic [3:0]  aluControlE;
    logic        branchE;
    logic        jumpE_int;
    logic        jalrE_int;
    logic [2:0]  addressingmodeE;

    ID_EX id_ex_i (
        .clk            (clk),
        .rst            (rst),
        .stall          (1'b0),     // stall via IF/ID + flushE (hazard unit)
        .flush          (flushE),

        // Data
        .pcD            (pcD),
        .pcPlus4D       (pc_plus4D),
        .rd1D           (rd1D),
        .rd2D           (rd2D),
        .immExtD        (extimmD),
        .rs1D           (rs1D),
        .rs2D           (rs2D),
        .rdD            (rdD),

        // Control
        .regWriteD      (regwriteD),
        .memReadD       (memReadD),
        .memWriteD      (memwriteD),
        .resultSrcD     (resultsrcD),
        .aluSrcD        (alusrcD),
        .aluControlD    (alucontrolD),
        .branchD        (branchD),
        .jumpD          (jumpD),
        .jalrD          (jalrinstrD),
        .addressingmodeD(addressingcontrolD),

        // To EX
        .pcE            (pcE),
        .pcPlus4E       (pcPlus4E),
        .rd1E           (rd1E),
        .rd2E           (rd2E),
        .immExtE        (immExtE),
        .rs1E           (rs1E),
        .rs2E           (rs2E),
        .rdE            (rdE),

        .regWriteE      (regWriteE),
        .memReadE       (memReadE),     // NEW: now valid at EX
        .memWriteE      (memWriteE),
        .resultSrcE     (resultSrcE),
        .aluSrcE        (aluSrcE),
        .aluControlE    (aluControlE),
        .branchE        (branchE),
        .jumpE          (jumpE_int),
        .jalrE          (jalrE_int),
        .addressingmodeE(addressingmodeE)
    );

    assign jumpE = jumpE_int;
    assign jalrE = jalrE_int;

    // =========================================================
    // EX STAGE
    // =========================================================
    logic [31:0] writeDataE;

    logic [1:0] forwardAE;
    logic [1:0] forwardBE;

    logic [31:0] aluResultM;   // from MEM (for forwarding)
    // resultW already defined

    EXECUTE_STAGE execute_stage_i (
        .clk             (clk),
        .rst             (rst),

        .pcE             (pcE),
        .rd1E            (rd1E),
        .rd2E            (rd2E),
        .extImmE         (immExtE),
        .rdE             (rdE),

        .aluControlE     (aluControlE),
        .aluSrcE         (aluSrcE),
        .branchE         (branchE),
        .regWriteE       (regWriteE),
        .memReadE        (memReadE),
        .memWriteE       (memWriteE),
        .resultSrcE      (resultSrcE),
        .addressingModeE (addressingmodeE),

        .forwardAE       (forwardAE),
        .forwardBE       (forwardBE),
        .aluResultM      (aluResultM),
        .resultW         (resultW),

        .aluResultE      (aluResultE),
        .writeDataE      (writeDataE),
        .branchTargetE   (branchTargetE),
        .branchTakenE    (branchTakenE)
    );

    // =========================================================
    // EX → MEM
    // =========================================================
    logic [31:0] pcPlus4M;
    logic [31:0] writeDataM;
    logic [4:0]  rdM;

    logic        regWriteM;
    logic        memReadM;
    logic        memWriteM;
    logic [1:0]  resultSrcM;
    logic [2:0]  addressingmodeM;

    EX_MEM ex_mem_i (
        .clk            (clk),
        .rst            (rst),
        .stall          (1'b0),
        .flush          (1'b0),

        // Data
        .pcPlus4E       (pcPlus4E),
        .branchTargetE  (branchTargetE),
        .aluResultE     (aluResultE),
        .writeDataE     (writeDataE),
        .rdE            (rdE),

        // Control
        .regWriteE      (regWriteE),
        .memReadE       (memReadE),
        .memWriteE      (memWriteE),
        .resultSrcE     (resultSrcE),
        .branchTakenE   (branchTakenE),
        .addressingmodeE(addressingmodeE),

        // To MEM
        .pcPlus4M       (pcPlus4M),
        .branchTargetM  (),            // optional
        .aluResultM     (aluResultM),
        .writeDataM     (writeDataM),
        .rdM            (rdM),

        .regWriteM      (regWriteM),
        .memReadM       (memReadM),
        .memWriteM      (memWriteM),
        .resultSrcM     (resultSrcM),
        .branchTakenM   (),            // optional
        .addressingmodeM(addressingmodeM)
    );

    // =========================================================
    // MEM STAGE
    // =========================================================
    logic [31:0] readDataM;

    MEM_STAGE mem_stage_i (
        .clk             (clk),
        .rst             (rst),
        .aluResultM      (aluResultM),
        .writeDataM      (writeDataM),
        .memReadM        (memReadM),
        .memWriteM       (memWriteM),
        .addressingModeM (addressingmodeM),

        .readDataM       (readDataM)
    );

    // =========================================================
    // MEM → WB
    // =========================================================
    logic [31:0] readDataW;
    logic [31:0] aluResultW;
    logic [31:0] pcPlus4W;

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

    // =========================================================
    // WB STAGE
    // =========================================================
    WB_STAGE wb_stage_i (
        .readDataW  (readDataW),
        .aluResultW (aluResultW),
        .pcPlus4W   (pcPlus4W),
        .resultSrcW (resultSrcW),

        .resultW    (resultW)
    );

    // =========================================================
    // HAZARD UNIT
    // =========================================================
    HazardUnit hazard_unit_i (
        .rs1E      (rs1E),
        .rs2E      (rs2E),
        .rs1D      (rs1D),
        .rs2D      (rs2D),
        .rdE       (rdE),
        .rdM       (rdM),
        .rdW       (rdW),

        .regWriteM (regWriteM),
        .regWriteW (regWriteW),

        .memReadE  (memReadE),                     // true load-use info
        .pcsrcE    (branchTakenE || jumpE || jalrE),

        .forwardAE (forwardAE),
        .forwardBE (forwardBE),
        .stallF    (stallF),
        .stallD    (stallD),
        .flushD    (flushD),
        .flushE    (flushE)
    );

    // =========================================================
    // Top-level outputs
    // =========================================================
    assign ResultW = resultW;
    assign regdata = '0;   // hook to RegFile debug later if needed

endmodule


