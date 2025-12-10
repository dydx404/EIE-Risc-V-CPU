// Holds fetched program counter (PC), PC+4 and instruction between 
// Instruction Fetch (IF) and Instruction Decode (ID) stages.

module IF_ID (
    input  logic        clk,
    input  logic        rst,

    // Hazard control
    input  logic        stall,    // 1 = hold current contents (StallDecode / StallFetch)
    input  logic        flush,    // 1 = insert NOP into decode (FlushDecode)

    // From IF stage
    input  logic [31:0] pcF,
    input  logic [31:0] pcPlus4F,
    input  logic [31:0] instrF,

    // To ID stage
    output logic [31:0] pcD,
    output logic [31:0] pcPlus4D,
    output logic [31:0] instrD
);

    always_ff @(posedge clk or posedge rst) begin
        if (rst || flush) begin
            // On reset or flush, turn decode stage into a NOP
            pcD       <= 32'b0;
            pcPlus4D  <= 32'b0;
            instrD    <= 32'b0;     // treat all-zero as NOP (addi x0,x0,0)
        end else if (!stall) begin
            pcD       <= pcF;
            pcPlus4D  <= pcPlus4F;
            instrD    <= instrF;
        end
    end

endmodule

