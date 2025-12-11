
//byte-addressable 128 KB Data Memory for RV32I single-cycle CPU.

// Supports the following:
//   - LB  / LBU    (byte load, signed/zero-extended)
//   - LH  / LHU    (halfword load, signed/zero-extended)
//   - LW            (word load)
//   - SB            (byte store)
//   - SH            (halfword store)
//   - SW            (word store)

module DataMemory #(
    parameter STORAGE_WIDTH = 8,          // each entry = 1 byte
    parameter ADDR_WIDTH    = 17          // 2^17 = 131072 bytes = 128 KB
)(

    // CPU interface

    input  logic                 clk_i,
    input  logic                 mem_read_i,
    input  logic                 mem_write_i,

    input  logic [31:0]          addr_i,          // byte address (from ALU)
    input  logic [31:0]          write_data_i,    // data to store (rs2)

    // access_ctrl_i format:
    //   [1:0] = size    (00=byte, 01=halfword, 10=word)
    //   [2]   = zero_extend (1=zero-extend, 0=sign-extend)
    input  logic [2:0]           access_ctrl_i,

    output logic [31:0]          read_data_o      // always 32-bit output
);

    // Decode control signals

    logic [1:0] size;
    logic       zero_extend;

    assign size        = access_ctrl_i[1:0];
    assign zero_extend = access_ctrl_i[2];

 
    // 128 KB Byte-addressable RAM

    logic [STORAGE_WIDTH-1:0] ram_array [0:(1<<ADDR_WIDTH)-1];

    // Use lower 17 bits as direct byte index
    logic [ADDR_WIDTH-1:0] addr;
    assign addr = addr_i[ADDR_WIDTH-1:0];


    // SIMULATION MEMORY PRELOAD SOCKET
    // Loads external .mem test data at address 0x10000
    // This block does not synthesize and is removed in hardware builds.
    `ifdef SIMULATION
    initial begin
        string path;
        int file;

        file = $fopen("../../rtl/datamem_path.txt", "r");
        if (file)
            $display("[DataMemory] Preload file opened.");
        else
            $display("[DataMemory] Failed to open preload file.");

        $fgets(path, file);
        $fclose(file);

        $display("[DataMemory] Loading external data from '%s' at 0x10000...", path);
        $readmemh(path, ram_array, 17'h10000);
        $display("[DataMemory] Memory load complete.");
    end
    `endif


    // WRITE Logic (SB, SH, SW)
    // Writes occur on the rising edge of clk.
    always_ff @(posedge clk_i) begin
        if (mem_write_i) begin
            unique case (size)

                // SB (Store Byte)
                2'b00: begin
                    ram_array[addr] <= write_data_i[7:0];
                end

                // SH (Store Halfword)
                2'b01: begin
                    ram_array[addr]     <= write_data_i[7:0];
                    ram_array[addr+1]   <= write_data_i[15:8];
                end

                // SW (Store Word)
                2'b10: begin
                    ram_array[addr]     <= write_data_i[7:0];
                    ram_array[addr+1]   <= write_data_i[15:8];
                    ram_array[addr+2]   <= write_data_i[23:16];
                    ram_array[addr+3]   <= write_data_i[31:24];
                end

                default: ;
            endcase
        end
    end

    // READ Logic (LB/LBU, LH/LHU, LW)
    // Combinational: output updates immediately for loads.

    always_comb begin
        if (!mem_read_i) begin
            read_data_o = 32'b0;
        end else begin
            unique case (size)


                // LB / LBU

                2'b00: begin
                    logic [7:0] b = ram_array[addr];

                    if (zero_extend)
                        read_data_o = {24'b0, b};          // LBU
                    else
                        read_data_o = {{24{b[7]}}, b};      // LB
                end


                // LH / LHU

                2'b01: begin
                    logic [7:0] lo = ram_array[addr];
                    logic [7:0] hi = ram_array[addr+1];
                    logic [15:0] half = {hi, lo};

                    if (zero_extend)
                        read_data_o = {16'b0, half};              // LHU
                    else
                        read_data_o = {{16{half[15]}}, half};     // LH
                end


                // LW (32-bit load)

                2'b10: begin
                    read_data_o = {
                        ram_array[addr+3],
                        ram_array[addr+2],
                        ram_array[addr+1],
                        ram_array[addr]
                    };
                end

                default: read_data_o = 32'b0;
            endcase
        end
    end

endmodule
