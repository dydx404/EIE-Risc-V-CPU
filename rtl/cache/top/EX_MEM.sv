// Latches ALU (arithmetic logic unit) result, store data, destination register,
// PC+4 / branch target and control signals into Memory (MEM) stage.

module EX_MEM (
    input  logic        clk,
    input  logic        rst,

    // Hazard control
    input  logic        stall,
    input  logic        flush,

    // From EX stage: data
    input  logic [31:0] pcPlus4E,
    input  logic [31:0] aluResultE,
    input  logic [31:0] writeDataE,
    input  logic [4:0]  rdE,

    // From EX stage: control
    input  logic        regWriteE,
    input  logic        memWriteE,
    input  logic [1:0]  resultSrcE,
    input  logic [2:0]  addressingmodeE,

    // To MEM stage: data
    output logic [31:0] pcPlus4M,
    output logic [31:0] aluResultM,
    output logic [31:0] writeDataM,
    output logic [4:0]  rdM,

    // To MEM stage: control
    output logic        regWriteM,
    output logic        memWriteM,
    output logic [1:0]  resultSrcM,
    output logic [2:0]  addressingmodeM
);

    always_ff @(posedge clk or posedge rst) begin
        if (rst || flush) begin
            pcPlus4M     <= 32'b0;
            aluResultM   <= 32'b0;
            writeDataM   <= 32'b0;
            rdM          <= 5'b0;

            regWriteM    <= 1'b0;
            memWriteM    <= 1'b0;
            resultSrcM   <= 2'b00;
            addressingmodeM <= 3'b000;
        end else if (!stall) begin
            pcPlus4M     <= pcPlus4E;
            aluResultM   <= aluResultE;
            writeDataM   <= writeDataE;
            rdM          <= rdE;

            regWriteM    <= regWriteE;
            memWriteM    <= memWriteE;
            resultSrcM   <= resultSrcE;
            addressingmodeM <= addressingmodeE;
        end
    end

endmodule
