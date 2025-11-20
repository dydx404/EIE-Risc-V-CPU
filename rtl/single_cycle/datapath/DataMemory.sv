module DataMemory #(
    parameter DATA_WIDTH = 32,
    parameter ADDR_WIDTH = 10   // 1024 words = 4 KB
)(
    input  logic                      clk,
    input  logic                      mem_write,     // store enable
    input  logic [ADDR_WIDTH-1:0]     addr,          // word address
    input  logic [DATA_WIDTH-1:0]     write_data,    // data for store
    output logic [DATA_WIDTH-1:0]     read_data      // data for load
);

    // Memory array
    logic [DATA_WIDTH-1:0] mem_array [0:(1<<ADDR_WIDTH)-1];

    // ------------------------------------------------------------
    // Combinational read
    // ------------------------------------------------------------
    always_comb begin
        read_data = mem_array[addr];
    end

    // ------------------------------------------------------------
    // Synchronous write
    // ------------------------------------------------------------
    always_ff @(posedge clk) begin
        if (mem_write) begin
            mem_array[addr] <= write_data;
        end
    end

endmodule

