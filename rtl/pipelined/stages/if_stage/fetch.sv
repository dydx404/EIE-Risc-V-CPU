module fetch #(
    DATA_WIDTH = 32
) (
    input   logic                     clk,
    input   logic                     rst,
    input   logic                     en,
    input   logic                     branchE,
    input   logic                     jalrinsE,
    input   logic [DATA_WIDTH-1:0]    pc_targetE,
    input   logic [DATA_WIDTH-1:0]    alu_outE,
    output  logic [DATA_WIDTH-1:0]    instrF,
    output  logic [DATA_WIDTH-1:0]    pcF,
    output  logic [DATA_WIDTH-1:0]    pc_plus4F
);

    //PC
    PCFlat pc (
        .clk(clk),
        .rst(rst),
        .en(en),
        .branchE(branchE),
        .jalrinsE(jalrinsE),
        .pc_targetE(pc_targetE),
        .alu_outE(alu_outE),
        .pcF(pcF),
        .pc_plus4F(pc_plus4F)
    );

    //Instruction memory
    InstrMem instrmem (
        .addr(pcF),
        .instrF(instrF)
    );

endmodule
