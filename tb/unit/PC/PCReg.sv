module PCReg #(
    DATA_WIDTH = 32
) (
    input   logic                     clk,
    input   logic                     rst,
    input   logic [DATA_WIDTH-1:0]    pc_next,
    output  logic [DATA_WIDTH-1:0]    pc
);
    // PC register
    always_ff @(posedge clk) begin
        if (rst) begin
            pc <= '0;
        end else begin
            pc <= pc_next;
        end
    end
endmodule
