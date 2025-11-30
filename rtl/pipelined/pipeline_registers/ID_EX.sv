// Latches register ops, immediate, destination register, PC values,
// and control signals from Instruction Decode (ID) into Execute (EX).

module ID_EX (
    input  logic        clk,
    input  logic        rst,

    // Hazard control
    input  logic        stall,
    input  logic        flush,

    // From ID stage: data
    input  logic [31:0] pcD,
    input  logic [31:0] pcPlus4D,
    input  logic [31:0] rd1D,
    input  logic [31:0] rd2D,
    input  logic [31:0] immExtD,
    input  logic [4:0]  rs1D,
    input  logic [4:0]  rs2D,
    input  logic [4:0]  rdD,

    // From ID stage: control
    input  logic        regWriteD,
    input  logic        memReadD,
    input  logic        memWriteD,
    input  logic [1:0]  resultSrcD,
    input  logic        aluSrcD,
    input  logic [3:0]  aluControlD,
    input  logic        branchD,
    input  logic        jumpD,
    input  logic        jalrD,

    // To EX stage: data
    output logic [31:0] pcE,
    output logic [31:0] pcPlus4E,
    output logic [31:0] rd1E,
    output logic [31:0] rd2E,
    output logic [31:0] immExtE,
    output logic [4:0]  rs1E,
    output logic [4:0]  rs2E,
    output logic [4:0]  rdE,

    // To EX stage: control
    output logic        regWriteE,
    output logic        memReadE,
    output logic        memWriteE,
    output logic [1:0]  resultSrcE,
    output logic        aluSrcE,
    output logic [3:0]  aluControlE,
    output logic        branchE,
    output logic        jumpE,
    output logic        jalrE
);

    always_ff @(posedge clk or posedge rst) begin
        if (rst || flush) begin
            // all control signals 0, data don't matter
            pcE         <= 32'b0;
            pcPlus4E    <= 32'b0;
            rd1E        <= 32'b0;
            rd2E        <= 32'b0;
            immExtE     <= 32'b0;
            rs1E        <= 5'b0;
            rs2E        <= 5'b0;
            rdE         <= 5'b0;

            regWriteE   <= 1'b0;
            memReadE    <= 1'b0;
            memWriteE   <= 1'b0;
            resultSrcE  <= 2'b00;
            aluSrcE     <= 1'b0;
            aluControlE <= 4'b0000;
            branchE     <= 1'b0;
            jumpE       <= 1'b0;
            jalrE       <= 1'b0;
        end else if (!stall) begin
            pcE         <= pcD;
            pcPlus4E    <= pcPlus4D;
            rd1E        <= rd1D;
            rd2E        <= rd2D;
            immExtE     <= immExtD;
            rs1E        <= rs1D;
            rs2E        <= rs2D;
            rdE         <= rdD;

            regWriteE   <= regWriteD;
            memReadE    <= memReadD;
            memWriteE   <= memWriteD;
            resultSrcE  <= resultSrcD;
            aluSrcE     <= aluSrcD;
            aluControlE <= aluControlD;
            branchE     <= branchD;
            jumpE       <= jumpD;
            jalrE       <= jalrD;
        end
    end

endmodule
