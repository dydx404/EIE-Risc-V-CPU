module MEM_STAGE (
    input  logic        clk,
    input  logic        rst,

    // From EX/MEM
    input  logic [31:0] aluResultM,
    input  logic [31:0] writeDataM,
    input  logic        memWriteM,
    input  logic        memReadM,
    input  logic [2:0]  addressingModeM,

    // To MEM/WB
    output logic [31:0] readDataM,

    // To Hazard Unit
    output logic        cacheStallM
);

    // ============================
    // Wires between cache & memory
    // ============================
    logic        mem_read;
    logic        mem_write;
    logic [31:0] mem_addr;
    logic [31:0] mem_wdata;
    logic [31:0] mem_rdata;
    logic        mem_ready;

    // ============================
    // Data Cache
    // ============================
    DataCache dcache (
        .clk(clk),
        .reset(rst),

        // From pipeline
        .memReadM(memReadM),
        .memWriteM(memWriteM),
        .addrM(aluResultM),
        .writeDataM(writeDataM),

        // Back to pipeline
        .readDataM(readDataM),
        .stallM(cacheStallM),

        // To main memory
        .mem_read(mem_read),
        .mem_write(mem_write),
        .mem_addr(mem_addr),
        .mem_wdata(mem_wdata),
        .mem_rdata(mem_rdata),
        .mem_ready(mem_ready)
    );

    // ============================
    // Main Backing Memory
    // ============================
    MainMemory main_mem (
        .clk(clk),
        .mem_read(mem_read),
        .mem_write(mem_write),
        .addr(mem_addr),
        .wdata(mem_wdata),
        .rdata(mem_rdata),
        .ready(mem_ready)
    );

endmodule
