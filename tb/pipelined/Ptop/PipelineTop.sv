module PipelineTop #(
    DATA_WIDTH = 32,
    REG_ADDR_WIDTH = 5
)(
    input   logic                       clk,
    input   logic                       rst,
    input   logic [REG_ADDR_WIDTH-1:0]  regaddr,  // For testing purposes
    output  logic [DATA_WIDTH-1:0]      regdata,  // For testing purposes
    output  logic [DATA_WIDTH-1:0]      ResultW
);

    // IF Stage
    //////////////////////////////////////
    logic [DATA_WIDTH-1:0] instrF;
    logic [DATA_WIDTH-1:0] pcF;
    logic [DATA_WIDTH-1:0] pc_plus4F;

    fetch fetch(
        .clk(clk),
        .rst(rst),
        .en(!stallF),
        .branchE(jumpE||branchTakenE),
        .jalrinsE(jalrE),
        .pc_targetE(branchTargetE),
        .alu_outE(aluResultE),
    
        .instrF(instrF),
        .pcF(pcF),
        .pc_plus4F(pc_plus4F)
    );
    //////////////////////////////////////

    // IF tp ID
    //////////////////////////////////////
    logic [DATA_WIDTH-1:0] instrD;
    logic [DATA_WIDTH-1:0] pcD;
    logic [DATA_WIDTH-1:0] pc_plus4D;

    IF_ID if_id(
        .clk(clk),
        .rst(rst),
        .stall(stallD),
        .flush(flushD),
        .instrF(instrF),
        .pcF(pcF),
        .pcPlus4F(pc_plus4F),

        .instrD(instrD),
        .pcD(pcD),
        .pcPlus4D(pc_plus4D)
    );
    //////////////////////////////////////

    // ID Stage
    //////////////////////////////////////
    logic                  regwriteD;
    logic [1:0]            resultsrcD;
    logic                  memwriteD;
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

    Decode decode(
        .instrd(instrD),
        .regwrite(regWriteW),
        .CLK(clk),
        .rdW(rdW),
        .resultW(resultW),

        .addressingmodeD(addressingcontrolD),
        .resultsrcD(resultsrcD),
        .regwriteD(regwriteD),
        .memwriteD(memwriteD),
        .branchD(branchD),
        .jumpD(jumpD),
        .jalrD(jalrinstrD),
        .alucontrolD(alucontrolD),
        .alusrcD(alusrcD),
        .rd1(rd1D),
        .rd2(rd2D),
        .rs1D(rs1D),
        .rs2D(rs2D),
        .rdD(rdD),
        .extimmD(extimmD)
    );
    //////////////////////////////////////

    // ID to EX
    //////////////////////////////////////
    // To EX stage: data
    logic [31:0] pcE;
    logic [31:0] pcPlus4E;
    logic [31:0] rd1E;
    logic [31:0] rd2E;
    logic [31:0] immExtE;
    logic [4:0]  rs1E;
    logic [4:0]  rs2E;
    logic [4:0]  rdE;

    // To EX stage: control
    logic        regWriteE;
    logic        memWriteE;
    logic [1:0]  resultSrcE;
    logic        aluSrcE;
    logic [3:0]  aluControlE;
    logic        branchE;
    logic        jumpE;
    logic        jalrE;
    logic [2:0]  addressingmodeE;

    ID_EX id_ex(
        .clk(clk),
        .rst(rst),
        .stall(1'b0),
        .flush(flushE),
        // Data
        .pcD(pcD),
        .pcPlus4D(pc_plus4D),
        .rd1D(rd1D),
        .rd2D(rd2D),
        .immExtD(extimmD),
        .rs1D(rs1D),
        .rs2D(rs2D),
        .rdD(rdD),
        // Control
        .regWriteD(regwriteD),
        .memWriteD(memwriteD),
        .resultSrcD(resultsrcD),
        .aluSrcD(alusrcD),
        .aluControlD(alucontrolD),
        .branchD(branchD),
        .jumpD(jumpD),
        .jalrD(jalrinstrD),
        .addressingmodeD(addressingcontrolD),

        // Outputs to EX stage
        .pcE(pcE),
        .pcPlus4E(pcPlus4E),
        .rd1E(rd1E),
        .rd2E(rd2E),
        .immExtE(immExtE),
        .rs1E(rs1E),
        .rs2E(rs2E),
        .rdE(rdE),
        .regWriteE(regWriteE),
        .memWriteE(memWriteE),
        .resultSrcE(resultSrcE),
        .aluSrcE(aluSrcE),
        .aluControlE(aluControlE),
        .branchE(branchE),
        .jumpE(jumpE),
        .jalrE(jalrE),
        .addressingmodeE(addressingmodeE)
    );
    //////////////////////////////////////

    // EX Stage
    //////////////////////////////////////
    logic [31:0] aluResultE;
    logic [31:0] writeDataE;    // store data (forwarded rs2)
    logic [31:0] branchTargetE; // PC + immediate
    logic        branchTakenE;   // final branch decision

    EXECUTE_STAGE execute_stage(
        .pcE(pcE),
        .rd1E(rd1E),
        .rd2E(rd2E),
        .extImmE(immExtE),

        .aluSrcE(aluSrcE),
        .aluControlE(aluControlE),
        .branchE(branchE),

        .forwardAE(forwardAE),
        .forwardBE(forwardBE),
        .aluResultM(aluResultM),
        .resultW(resultW),

        .aluResultE(aluResultE),
        .writeDataE(writeDataE),
        .branchTargetE(branchTargetE),
        .branchTakenE(branchTakenE)
    );
    //////////////////////////////////////

    // EX to MEM
    //////////////////////////////////////
    logic [31:0] pcPlus4M;
    logic [31:0] aluResultM;
    logic [31:0] writeDataM;
    logic [4:0]  rdM;

    logic        regWriteM;
    logic        memWriteM;
    logic [1:0]  resultSrcM;
    logic        branchTakenM;
    logic [2:0]  addressingmodeM;

    EX_MEM ex_mem(
        .clk(clk),
        .rst(rst),
        .stall(1'b0),
        .flush(1'b0),
        // Data
        .pcPlus4E(pcPlus4E),
        .aluResultE(aluResultE),
        .writeDataE(writeDataE),
        .rdE(rdE),
        // Control
        .regWriteE(regWriteE),
        .memWriteE(memWriteE),
        .resultSrcE(resultSrcE),
        .addressingmodeE(addressingmodeE),

        // Outputs to MEM stage
        .pcPlus4M(pcPlus4M),
        .aluResultM(aluResultM),
        .writeDataM(writeDataM),
        .rdM(rdM),
        .regWriteM(regWriteM),
        .memWriteM(memWriteM),
        .resultSrcM(resultSrcM),
        .addressingmodeM(addressingmodeM)
    );
    //////////////////////////////////////

    // MEM Stage
    //////////////////////////////////////
    logic [31:0] readDataM;

    MEM_STAGE mem_stage(
        .clk(clk),
        .rst(rst),
        .aluResultM(aluResultM),
        .writeDataM(writeDataM),
        .memWriteM(memWriteM),
        .memReadM(resultSrcM==2'b01),
        .addressingModeM(addressingmodeM),

        .readDataM(readDataM)
    );
    //////////////////////////////////////

    // MEM to WB
    //////////////////////////////////////
    // To WB stage: data
    logic [31:0] readDataW;
    logic [31:0] aluResultW;
    logic [31:0] pcPlus4W;
    logic [4:0]  rdW;

    // To WB stage: control
    logic        regWriteW;
    logic [1:0]  resultSrcW;

    MEM_WB mem_wb(
        .clk(clk),
        .rst(rst),
        .stall(1'b0),
        .flush(1'b0),
        // Data
        .readDataM(readDataM),
        .aluResultM(aluResultM),
        .pcPlus4M(pcPlus4M),
        .rdM(rdM),
        // Control
        .regWriteM(regWriteM),
        .resultSrcM(resultSrcM),

        // Outputs to WB stage
        .readDataW(readDataW),
        .aluResultW(aluResultW),
        .pcPlus4W(pcPlus4W),
        .rdW(rdW),
        .regWriteW(regWriteW),
        .resultSrcW(resultSrcW)
    );
    //////////////////////////////////////

    // WB Stage
    //////////////////////////////////////
    logic [31:0] resultW;

    WB_STAGE wb_stage(
        .readDataW(readDataW),
        .aluResultW(aluResultW),
        .pcPlus4W(pcPlus4W),
        .resultSrcW(resultSrcW),

        .resultW(resultW)
    );
    //////////////////////////////////////

    // Hazard Unit
    //////////////////////////////////////
    logic [1:0]  forwardAE;
    logic [1:0]  forwardBE;
    logic        stallF;
    logic        stallD;
    logic        flushD;
    logic        flushE;

    HazardUnit hazard_unit(
        .rs1D(rs1D),
        .rs2D(rs2D),
        .rs1E(rs1E),
        .rs2E(rs2E),
        .rdE(rdE),
        .rdM(rdM),
        .rdW(rdW),
        .regWriteM(regWriteM),
        .regWriteW(regWriteW),
        .regWriteE(regWriteE),
        .pcsrcE(branchTakenE||jumpE),

        .forwardAE(forwardAE),
        .forwardBE(forwardBE),
        .stallF(stallF),
        .stallD(stallD),
        .flushD(flushD),
        .flushE(flushE)
    );
    //////////////////////////////////////

endmodule
