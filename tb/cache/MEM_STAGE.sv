module MEM_STAGE (
    input  logic        clk,
    input  logic        rst,

    // -------------------------
    // From EX/MEM Register
    // -------------------------
    input  logic [31:0] aluResultM,      // Address for load/store
    input  logic [31:0] writeDataM,      // Store data (rs2 forwarded)
    input  logic        memReadM,        // Load instruction?
    input  logic        memWriteM,       // Store instruction?
    input  logic [2:0]  addressingModeM, // Byte/Half/Word + signed/unsigned
                                         // (not used by cache yet, but kept for compatibility)

    // -------------------------
    // Output to MEM/WB Register
    // -------------------------
    output logic [31:0] readDataM,       // Loaded data (if load)

    // -------------------------
    // Cache stall back to pipeline
    // -------------------------
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
        .clk        (clk),
        .reset      (rst),

        // From pipeline
        .memReadM   (memReadM),
        .memWriteM  (memWriteM),
        .addrM      (aluResultM),
        .writeDataM (writeDataM),

        // Back to pipeline
        .readDataM  (readDataM),
        .stallM     (cacheStallM),

        // To main memory
        .mem_read   (mem_read),
        .mem_write  (mem_write),
        .mem_addr   (mem_addr),
        .mem_wdata  (mem_wdata),
        .mem_rdata  (mem_rdata),
        .mem_ready  (mem_ready)
    );

    // ============================
    // Main Backing Memory
    // ============================
    MainMemory main_mem (
        .clk      (clk),
        .mem_read (mem_read),
        .mem_write(mem_write),
        .addr     (mem_addr),
        .wdata    (mem_wdata),
        .rdata    (mem_rdata),
        .ready    (mem_ready)
    );

endmodule
