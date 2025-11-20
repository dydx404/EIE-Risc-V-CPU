// -----------------------------------------------------------------------------
// Local verification copy of DataMemory
// Canonical version lives in rtl/single_cycle/datapath/
// -----------------------------------------------------------------------------
module DataMemory #(
    parameter DATA_WIDTH = 32,
    parameter ADDR_WIDTH = 10
)(
    input  logic                      clk,
    input  logic                      mem_write,
    input  logic [ADDR_WIDTH-1:0]     addr,
    input  logic [DATA_WIDTH-1:0]     write_data,
    output logic [DATA_WIDTH-1:0]     read_data
);

    logic [DATA_WIDTH-1:0] mem_array [0:(1<<ADDR_WIDTH)-1];

    // asynchronous read
    always_comb begin
        read_data = mem_array[addr];
    end

    // synchronous write
    always_ff @(posedge clk) begin
        if (mem_write) begin
            mem_array[addr] <= write_data;
        end
    end

endmodule
