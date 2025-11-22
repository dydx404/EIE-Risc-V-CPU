module PCPlus4 #(
    DATA_WIDTH = 32
) (
    input   logic [DATA_WIDTH-1:0]  pc,
    output  logic [DATA_WIDTH-1:0]  pc_next
);
    // PC + 4
    assign pc_next = pc + 4;
endmodule
