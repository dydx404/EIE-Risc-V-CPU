module Decode(
    input   logic [31:0]instrd,
    input   logic       regwrite,
    input   logic       CLK,
    input   logic [4:0] rdW,
    input   logic [31:0]resultW,

    output  logic [2:0] addressingmodeD,
    output  logic [1:0] resultsrcD,
    output  logic       regwriteD,
    output  logic       memwriteD,
    //output  logic [1:0] pcsrcD,
    output  logic       branchD,
    output  logic       jumpD,
    output  logic       jalrD,
    output  logic [3:0] alucontrolD,
    output  logic       alusrcD,
    output  logic [31:0]rd1,
    output  logic [31:0]rd2,
    output  logic [4:0] rs1D,
    output  logic [4:0] rs2D,
    output  logic [4:0] rdD,
    output  logic [31:0]extimmD
);

    logic [2:0] immsrc;
    assign rdD = instrd[11:7];
    assign rs1D = instrd[19:15];
    assign rs2D = instrd[24:20];

    ControlUnit Control(
        .op(instrd[6:0]),
        .funct3(instrd[14:12]),
        .funct7(instrd[30]),
        
        .RegWrite(regwriteD),
        .ALUControl(alucontrolD),
        .ALUSrc(alusrcD),
        .MemWrite(memwriteD),
        //.PCSrc(pcsrcD),
        .branchD(branchD),
        .jumpD(jumpD),
        .jalrD(jalrD),
        .ResultSrc(resultsrcD),
        .ImmSrc(immsrc),
        .AddressingControl(addressingmodeD)
    );

    RegisterFile Reg(
        .CLK(CLK),
        .A1(instrd[19:15]),
        .A2(instrd[24:20]),
        .A3(rdW),
        .WD3(resultW),
        .WE3(regwrite),
        .RD1(rd1),
        .RD2(rd2)
    );

    Extend ImmGen(
        .Instr(instrd[31:7]),
        .ImmSrc(immsrc),
        .Imm(extimmD)
    );

    
endmodule
