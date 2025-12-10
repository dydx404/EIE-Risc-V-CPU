module WB_STAGE (
    // From MEM/WB pipeline register
    input  logic [31:0] aluResultW,
    input  logic [31:0] readDataW,
    input  logic [31:0] pcPlus4W,
    input  logic [1:0]  resultSrcW,

    // Output to register file
    output logic [31:0] resultW
);

    always_comb begin
        unique case (resultSrcW)
            2'b00: resultW = aluResultW;   // ALU instruction
            2'b01: resultW = readDataW;    // Load instruction
            2'b10: resultW = pcPlus4W;     // JAL / JALR
            default: resultW = aluResultW;
        endcase
    end

endmodule

