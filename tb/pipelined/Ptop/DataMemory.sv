// byte-addressable 128 KB Data Memory for RV32I single-cycle CPU.

module DataMemory #(
    parameter STORAGE_WIDTH = 8,
    parameter ADDR_WIDTH    = 17
)(

    input  logic                 clk_i,
    input  logic                 mem_read_i,
    input  logic                 mem_write_i,

    input  logic [31:0]          addr_i,
    input  logic [31:0]          write_data_i,

    input  logic [2:0]           access_ctrl_i,

    output logic [31:0]          read_data_o
);

    // ------------------ CONTROL DECODE ------------------
    logic [1:0] size;
    logic       zero_extend;

    assign size        = access_ctrl_i[1:0];
    assign zero_extend = access_ctrl_i[2];

    // ------------------ DATA MEMORY ----------------------
    logic [STORAGE_WIDTH-1:0] ram_array [0:(1<<ADDR_WIDTH)-1];

    logic [ADDR_WIDTH-1:0] addr;
    assign addr = addr_i[ADDR_WIDTH-1:0];

    // ======================================================
    // Natural preload (NO `SIMULATION` define needed)
    // Reads file path from datamem_path.txt in working dir
    // ======================================================
    initial begin
        string path;
        int file;

        $display("[DataMemory] Trying to open datamem_path.txt...");

        file = $fopen("datamem_path.txt", "r");

        if (!file) begin
            $display("[DataMemory] ERROR: Could not open datamem_path.txt.");
            $display("[DataMemory] No data preloaded into memory.");
        end
        else begin
            $display("[DataMemory] datamem_path.txt opened successfully.");

            void'($fgets(path, file));
            $fclose(file);

            $display("[DataMemory] Loading '%s' into data memory @ 0x10000...", path);

            // Load bytes into DMEM starting at base_data = 0x10000
            $readmemh(path, ram_array, 17'h10000);

            $display("[DataMemory] Data memory populated from '%s'.", path);
        end
    end


    // ======================================================
    // WRITE logic (SB, SH, SW)
    // ======================================================
    always_ff @(posedge clk_i) begin
        if (mem_write_i) begin
            unique case (size)
                2'b00:  ram_array[addr] <= write_data_i[7:0];     // SB
                2'b01: begin                                       // SH
                    ram_array[addr]   <= write_data_i[7:0];
                    ram_array[addr+1] <= write_data_i[15:8];
                end
                2'b10: begin                                       // SW
                    ram_array[addr]   <= write_data_i[7:0];
                    ram_array[addr+1] <= write_data_i[15:8];
                    ram_array[addr+2] <= write_data_i[23:16];
                    ram_array[addr+3] <= write_data_i[31:24];
                end
                default: ; // do nothing
            endcase
        end
    end


    // ======================================================
    // READ logic (LBU, LB, LHU, LH, LW)
    // ======================================================
    always_comb begin
        if (!mem_read_i) begin
            read_data_o = 32'b0;
        end else begin
            unique case (size)

                // Byte (LB / LBU)
                2'b00: begin
                    logic [7:0] b = ram_array[addr];
                    read_data_o = zero_extend ?
                        {24'b0, b} :
                        {{24{b[7]}}, b};
                end

                // Halfword (LH / LHU)
                2'b01: begin
                    logic [7:0] lo  = ram_array[addr];
                    logic [7:0] hi  = ram_array[addr+1];
                    logic [15:0] half = {hi, lo};
                    read_data_o = zero_extend ?
                        {16'b0, half} :
                        {{16{half[15]}}, half};
                end

                // Word (LW)
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
            //debug
              
        end
    end
    
      always_ff @(posedge clk_i) begin
                     if (mem_write_i)
                      $display("[DMEM-WRITE] addr=%h data=%h", addr, write_data_i[7:0]);
                end

endmodule
