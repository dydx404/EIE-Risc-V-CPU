module Decode(
    input   logic [31:0] instrd,
    input   logic        regwrite,   // WB-stage write enable
    input   logic        CLK,
    input   logic [4:0]  rdW,        // WB destination reg
    input   logic [31:0] resultW,    // WB data

    output  logic [2:0]  addressingmodeD,
    output  logic [1:0]  resultsrcD,
    output  logic        regwriteD,
    output  logic        memwriteD,
    output  logic        memReadD,
    output  logic        branchD,
    output  logic        jumpD,
    output  logic        jalrD,
    output  logic [3:0]  alucontrolD,
    output  logic        alusrcD,

    output  logic [31:0] rd1,
    output  logic [31:0] rd2,
    output  logic [4:0]  rs1D,
    output  logic [4:0]  rs2D,
    output  logic [4:0]  rdD,
    output  logic [31:0] extimmD,

    input  logic [4:0]  testRegAddress,
    output logic [31:0] testRegData
);

    logic [2:0]  immsrc;
    logic [31:0] rd1_raw, rd2_raw;

    // ----------------------------
    // Register specifiers
    // ----------------------------
    assign rdD  = instrd[11:7];
    assign rs1D = instrd[19:15];
    assign rs2D = instrd[24:20];

    // Load detection helper
    assign memReadD = (resultsrcD == 2'b01);

    // ----------------------------
    // Control Unit
    // ----------------------------
    ControlUnit Control(
        .op(instrd[6:0]),
        .funct3(instrd[14:12]),
        .funct7(instrd[31:25]),

        .RegWrite(regwriteD),
        .ALUControl(alucontrolD),
        .ALUSrc(alusrcD),
        .MemWrite(memwriteD),
        .branchD(branchD),
        .jumpD(jumpD),
        .jalrD(jalrD),
        .ResultSrc(resultsrcD),
        .ImmSrc(immsrc),
        .AddressingControl(addressingmodeD)
    );

    // ----------------------------
    // Register File (RAW OUTPUTS)
    // ----------------------------
    RegFile Reg(
        .clk(CLK),
        .we3(regwrite),
        .ad1(rs1D),
        .ad2(rs2D),
        .ad3(rdW),
        .wd3(resultW),

        .rd1(rd1_raw),
        .rd2(rd2_raw),

        .testRegAddress(testRegAddress),
        .testRegData(testRegData)
    );

    // ----------------------------
    // ✅ WB → ID BYPASS (CRITICAL FIX)
    // ----------------------------
    assign rd1 = (regwrite && (rdW != 5'd0) && (rdW == rs1D)) ? resultW : rd1_raw;
    assign rd2 = (regwrite && (rdW != 5'd0) && (rdW == rs2D)) ? resultW : rd2_raw;

    // ----------------------------
    // Immediate Generator
    // ----------------------------
    Extend ImmGen(
        .Immediate(instrd[31:7]),
        .ImmSrc(immsrc),
        .ImmExt(extimmD)
    );

endmodule
