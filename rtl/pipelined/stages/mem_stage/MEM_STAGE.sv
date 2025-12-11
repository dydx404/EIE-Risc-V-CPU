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

    // -------------------------
    // Output to MEM/WB Register
    // -------------------------
    output logic [31:0] readDataM        // Loaded data (if load)
);

    // =====================================
    //   Instantiate original DataMemory
    // =====================================

    DataMemory data_mem (
        .clk_i         (clk),

        .mem_read_i    (memReadM),
        .mem_write_i   (memWriteM),

        .addr_i        (aluResultM),
        .write_data_i  (writeDataM),

        .access_ctrl_i (addressingModeM),

        .read_data_o   (readDataM)
    );

endmodule
