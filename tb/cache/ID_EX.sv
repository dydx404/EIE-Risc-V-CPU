module ID_EX (
    input  logic        clk,
    input  logic        rst,
    input  logic        stall,
    input  logic        flush,
    // data + control inputs...
    input  logic [31:0] pcD,
    input  logic [31:0] pcPlus4D,
    input  logic [31:0] rd1D,
    input  logic [31:0] rd2D,
    input  logic [31:0] immExtD,
    input  logic [4:0]  rs1D,
    input  logic [4:0]  rs2D,
    input  logic [4:0]  rdD,
    input  logic        regWriteD,
    input  logic        memWriteD,
    input  logic        memReadD,
    input  logic [1:0]  resultSrcD,
    input  logic        aluSrcD,
    input  logic [3:0]  aluControlD,
    input  logic        branchD,
    input  logic        jumpD,
    input  logic        jalrD,
    input  logic [2:0]  addressingmodeD,

    // outputs ...
    output logic [31:0] pcE,
    output logic [31:0] pcPlus4E,
    output logic [31:0] rd1E,
    output logic [31:0] rd2E,
    output logic [31:0] immExtE,
    output logic [4:0]  rs1E,
    output logic [4:0]  rs2E,
    output logic [4:0]  rdE,
    output logic        regWriteE,
    output logic        memWriteE,
    output logic        memReadE,
    output logic [1:0]  resultSrcE,
    output logic        aluSrcE,
    output logic [3:0]  aluControlE,
    output logic        branchE,
    output logic        jumpE,
    output logic        jalrE,
    output logic [2:0]  addressingmodeE
);

    always_ff @(posedge clk or posedge rst) begin
        if (rst) begin
            pcE            <= '0;
            pcPlus4E       <= '0;
            rd1E           <= '0;
            rd2E           <= '0;
            immExtE        <= '0;
            rs1E           <= '0;
            rs2E           <= '0;
            rdE            <= '0;
            regWriteE      <= 1'b0;
            memWriteE      <= 1'b0;
            memReadE       <= 1'b0;
            resultSrcE     <= 2'b00;
            aluSrcE        <= 1'b0;
            aluControlE    <= 4'b0000;
            branchE        <= 1'b0;
            jumpE          <= 1'b0;
            jalrE          <= 1'b0;
            addressingmodeE<= 3'b000;
        end else if (flush) begin           // ✅ flush wins
            pcE            <= '0;
            pcPlus4E       <= '0;
            rd1E           <= '0;
            rd2E           <= '0;
            immExtE        <= '0;
            rs1E           <= '0;
            rs2E           <= '0;
            rdE            <= '0;
            regWriteE      <= 1'b0;
            memWriteE      <= 1'b0;
            memReadE       <= 1'b0;
            resultSrcE     <= 2'b00;
            aluSrcE        <= 1'b0;
            aluControlE    <= 4'b0000;
            branchE        <= 1'b0;
            jumpE          <= 1'b0;
            jalrE          <= 1'b0;
            addressingmodeE<= 3'b000;
        end else if (!stall) begin          // ✅ normal update only when not stalled
            pcE            <= pcD;
            pcPlus4E       <= pcPlus4D;
            rd1E           <= rd1D;
            rd2E           <= rd2D;
            immExtE        <= immExtD;
            rs1E           <= rs1D;
            rs2E           <= rs2D;
            rdE            <= rdD;
            regWriteE      <= regWriteD;
            memWriteE      <= memWriteD;
            memReadE       <= memReadD;
            resultSrcE     <= resultSrcD;
            aluSrcE        <= aluSrcD;
            aluControlE    <= aluControlD;
            branchE        <= branchD;
            jumpE          <= jumpD;
            jalrE          <= jalrD;
            addressingmodeE<= addressingmodeD;
        end
        // else: hold values on stall
    end

endmodule
