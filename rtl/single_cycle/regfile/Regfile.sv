module RegFile (
    input  logic        clk,      // clock
    input  logic        we,       // write enable
    input  logic [4:0]  rs1,      // read address 1
    input  logic [4:0]  rs2,      // read address 2
    input  logic [4:0]  rd,       // write address
    input  logic [31:0] wd,       // write data
    output logic [31:0] rd1,      // read data 1
    output logic [31:0] rd2,      // read data 2
    output logic [31:0] a0        // exposed
);

    // 32 general-purpose registers x0..x31
    logic [31:0] regs [0:31];

    // ---------- Combinational reads with write-first bypass ----------
    always_comb begin
        // rs1
        if (rs1 == 5'd0) begin
            rd1 = 32'b0;                          // x0 is always 0
        end else if (we && (rd == rs1) && (rd != 5'd0)) begin
            rd1 = wd;                             // same-cycle write -> see new value
        end else begin
            rd1 = regs[rs1];
        end

        // rs2
        if (rs2 == 5'd0) begin
            rd2 = 32'b0;
        end else if (we && (rd == rs2) && (rd != 5'd0)) begin
            rd2 = wd;
        end else begin
            rd2 = regs[rs2];
        end
    end

    // synchronous write
    always_ff @(posedge clk) begin
        // keep x0 hard-wired
        regs[0] <= 32'b0;

        if (we && (rd != 5'd0)) begin
            regs[rd] <= wd;
        end
    end

    // expose a0
    assign a0 = regs[5'd10];

endmodule
