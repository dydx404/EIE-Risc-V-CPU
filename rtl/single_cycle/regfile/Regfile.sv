// RegFile.sv

module RegFile (
    input  logic        clk,      // clock
    input  logic        we,       // write enable
    input  logic [4:0]  rs1,      // read address 1
    input  logic [4:0]  rs2,      // read address 2
    input  logic [4:0]  rd,       // write address
    input  logic [31:0] wd,       // write data
    output logic [31:0] rd1,      // read data 1
    output logic [31:0] rd2       // read data 2
);

    // 32 general-purpose registers
    logic [31:0] regs [0:31];

    // Combinational reads
    always_comb begin
        rd1 = (rs1 == 5'd0) ? 32'b0 : regs[rs1];
        rd2 = (rs2 == 5'd0) ? 32'b0 : regs[rs2];
    end

    // Synchronous write
    always_ff @(posedge clk) begin
        if (we && (rd != 5'd0)) begin
            regs[rd] <= wd;
        end
    // not touch regs[0] here so x0 stays 0

    end

endmodule
