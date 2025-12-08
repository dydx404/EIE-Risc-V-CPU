module MainMemory (
    input  logic        clk,
    input  logic        mem_read,
    input  logic        mem_write,
    input  logic [31:0] addr,
    input  logic [31:0] wdata,
    output logic [31:0] rdata,
    output logic        ready
);

    logic [31:0] mem [0:2047];  // 8KB backing store (safe size)

    initial $readmemh("program.mem", mem);

    always_ff @(posedge clk) begin
        ready <= 1'b0;

        if (mem_write) begin
            mem[addr[12:2]] <= wdata;
            ready <= 1'b1;
        end

        if (mem_read) begin
            rdata <= mem[addr[12:2]];
            ready <= 1'b1;
        end
    end

endmodule
