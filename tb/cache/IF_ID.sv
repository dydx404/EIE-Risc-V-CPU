module IF_ID (
    input  logic        clk,
    input  logic        rst,
    input  logic        stall,
    input  logic        flush,
    input  logic [31:0] instrF,
    input  logic [31:0] pcF,
    input  logic [31:0] pcPlus4F,

    output logic [31:0] instrD,
    output logic [31:0] pcD,
    output logic [31:0] pcPlus4D
);

    always_ff @(posedge clk or posedge rst) begin
        if (rst) begin
            instrD   <= 32'b0;
            pcD      <= 32'b0;
            pcPlus4D <= 32'b0;
        end else if (flush) begin         // ✅ flush wins
            instrD   <= 32'b0;
            pcD      <= 32'b0;
            pcPlus4D <= 32'b0;
        end else if (!stall) begin        // ✅ only update when not stalled
            instrD   <= instrF;
            pcD      <= pcF;
            pcPlus4D <= pcPlus4F;
        end
        // else: keep previous values on stall
    end

endmodule
