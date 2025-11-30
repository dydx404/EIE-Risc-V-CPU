module fetch #(
    DATA_WIDTH = 32
) (
    input   logic                     clk,
    input   logic                     rst,
    input   logic                     en,
    input   logic                     jumpE,
    input   logic                     branchE,
    input   logic                     zeroE,
    input   logic                     jalrins,
    input   logic [DATA_WIDTH-1:0]    immopE,
    input   logic [DATA_WIDTH-1:0]    alu_outE,
    input   logic [DATA_WIDTH-1:0]    pcE,
    output  logic [DATA_WIDTH-1:0]    instrF,
    output  logic [DATA_WIDTH-1:0]    pcF,
    output  logic [DATA_WIDTH-1:0]    pc_plus4F
);

    //PC
    PCFlat pc (
        .clk(clk),
        .rst(rst),
        .en(en),
        .jumpE(jumpE),
        .branchE(branchE),
        .zeroE(zeroE),
        .jalrins(jalrins),
        .immopE(immopE),
        .alu_outE(alu_outE),
        .pcE(pcE),
        .pcF(pcF),
        .pc_plus4F(pc_plus4F)
    );

    //Instruction memory
    InstrMem instrmem (
        .addr(pcF),
        .instrF(instrF)
    );

endmodule
