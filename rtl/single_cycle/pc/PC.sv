module PC #(
    DATA_WIDTH = 32
) (
    input   logic                     clk,
    input   logic                     rst,
    input   logic [1:0]               pcsrc,
    input   logic [DATA_WIDTH-1:0]    immop,
    input   logic [DATA_WIDTH-1:0]    alu_out,
    output  logic [DATA_WIDTH-1:0]    pc
);
    // Wires
    logic [DATA_WIDTH-1:0] pc_plus4;
    logic [DATA_WIDTH-1:0] pc_target;
    logic [DATA_WIDTH-1:0] pc_next;

    // Instantiate PCPlus4
    PCPlus4 #(
        .DATA_WIDTH(DATA_WIDTH)
    ) pc_plus4_inst (
        .pc(pc),
        .pc_next(pc_plus4)
    );

    // Instantiate PCTarget
    PCTarget #(
        .DATA_WIDTH(DATA_WIDTH)
    ) pc_target_inst (
        .pc(pc),
        .imm(immop),
        .pc_target(pc_target)
    );

    // Instantiate mux
    mux4 #(
        .DATA_WIDTH(DATA_WIDTH)
    ) pc_mux_inst (
        .in0(pc_plus4),
        .in1(pc_target),
        .in2(alu_out),
        .sel(pcsrc),
        .out(pc_next)
    );

    // Instantiate PCReg
    PCReg #(
        .DATA_WIDTH(DATA_WIDTH)
    ) pc_reg_inst (
        .clk(clk),
        .rst(rst),
        .pc_next(pc_next),
        .pc(pc)
    );
endmodule
