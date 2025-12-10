module EX_MEM (
    input  logic        clk,
    input  logic        rst,
    input  logic        stall,
    input  logic        flush,

    // From EX stage
    input  logic [31:0] pcPlus4E,
    input  logic [31:0] aluResultE,
    input  logic [31:0] writeDataE,
    input  logic [4:0]  rdE,

    input  logic        regWriteE,
    input  logic        memWriteE,
    input  logic [1:0]  resultSrcE,
    input  logic [2:0]  addressingmodeE,

    // To MEM stage
    output logic [31:0] pcPlus4M,
    output logic [31:0] aluResultM,
    output logic [31:0] writeDataM,
    output logic [4:0]  rdM,

    output logic        regWriteM,
    output logic        memWriteM,
    output logic [1:0]  resultSrcM,
    output logic [2:0]  addressingmodeM
);

    always_ff @(posedge clk or posedge rst) begin
        if (rst) begin
            pcPlus4M        <= 32'b0;
            aluResultM      <= 32'b0;
            writeDataM      <= 32'b0;
            rdM             <= 5'b0;
            regWriteM       <= 1'b0;
            memWriteM       <= 1'b0;
            resultSrcM      <= 2'b0;
            addressingmodeM <= 3'b0;
        end else if (flush) begin
            // Insert a bubble into EX/MEM
            pcPlus4M        <= 32'b0;
            aluResultM      <= 32'b0;
            writeDataM      <= 32'b0;
            rdM             <= 5'b0;
            regWriteM       <= 1'b0;
            memWriteM       <= 1'b0;
            resultSrcM      <= 2'b0;
            addressingmodeM <= 3'b0;
        end else if (!stall) begin
            // Normal pipeline advance
            pcPlus4M        <= pcPlus4E;
            aluResultM      <= aluResultE;
            writeDataM      <= writeDataE;
            rdM             <= rdE;
            regWriteM       <= regWriteE;
            memWriteM       <= memWriteE;
            resultSrcM      <= resultSrcE;
            addressingmodeM <= addressingmodeE;
        end
        // else: stall => hold previous MEM-stage values
    end

endmodule
