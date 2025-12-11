module PCTarget #(
    DATA_WIDTH = 32
) (
    input   logic [DATA_WIDTH-1:0]  pc,
    input   logic [DATA_WIDTH-1:0]  imm,
    output  logic [DATA_WIDTH-1:0]  pc_target
);
    // PC target address calculation
    assign pc_target = pc + imm;
endmodule
