// Latches memory read data, ALU result, PC+4 and destination register, and
// control signals for the Write-back (WB) stage.

module MEM_WB (
    input  logic        clk,
    input  logic        rst,

    // Hazard control
    input  logic        stall,
    input  logic        flush,

    // From MEM stage: data
    input  logic [31:0] readDataM,
    input  logic [31:0] aluResultM,
    input  logic [31:0] pcPlus4M,
    input  logic [4:0]  rdM,

    // From MEM stage: control
    input  logic        regWriteM,
    input  logic [1:0]  resultSrcM,

    // To WB stage: data
    output logic [31:0] readDataW,
    output logic [31:0] aluResultW,
    output logic [31:0] pcPlus4W,
    output logic [4:0]  rdW,

    // To WB stage: control
    output logic        regWriteW,
    output logic [1:0]  resultSrcW
);

    always_ff @(posedge clk or posedge rst) begin
        if (rst || flush) begin
            readDataW  <= 32'b0;
            aluResultW <= 32'b0;
            pcPlus4W   <= 32'b0;
            rdW        <= 5'b0;

            regWriteW  <= 1'b0;
            resultSrcW <= 2'b00;
        end else if (!stall) begin
            readDataW  <= readDataM;
            aluResultW <= aluResultM;
            pcPlus4W   <= pcPlus4M;
            rdW        <= rdM;

            regWriteW  <= regWriteM;
            resultSrcW <= resultSrcM;
        end
    end

endmodule
