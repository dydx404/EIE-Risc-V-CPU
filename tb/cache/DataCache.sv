module DataCache (
    input  logic        clk,
    input  logic        reset,

    // From pipeline (MEM stage)
    input  logic        memReadM,
    input  logic        memWriteM,
    input  logic [31:0] addrM,
    input  logic [31:0] writeDataM,
    input  logic [2:0]  addressingModeM,   // LB/LBU/LH/LHU/LW, SB/SH/SW

    // Back to pipeline
    output logic [31:0] readDataM,
    output logic        stallM,

    // To main memory
    output logic        mem_read,
    output logic        mem_write,
    output logic [31:0] mem_addr,
    output logic [31:0] mem_wdata,
    input  logic [31:0] mem_rdata,
    input  logic        mem_ready
);

    // ============================================================
    // Cache geometry: 4KB, 2-way, 16B line
    // ============================================================
    localparam int LINE_WORDS = 4;     // 4 words × 4 B = 16 B/line
    localparam int NUM_SETS   = 128;
    localparam int NUM_WAYS   = 2;
    localparam int TAG_BITS   = 21;

    // Storage arrays
    logic [31:0]         data  [NUM_WAYS-1:0][NUM_SETS-1:0][LINE_WORDS-1:0];
    logic [TAG_BITS-1:0] tag   [NUM_WAYS-1:0][NUM_SETS-1:0];
    logic                valid [NUM_WAYS-1:0][NUM_SETS-1:0];
    logic                lru   [NUM_SETS-1:0];

    // ============================================================
    // Address breakdown
    // ============================================================
    wire [1:0]          word_offset = addrM[3:2];    // which word in line
    wire [1:0]          byte_offset = addrM[1:0];    // which byte in word
    wire [6:0]          set_index   = addrM[10:4];
    wire [TAG_BITS-1:0] addr_tag    = addrM[31:11];

    // ============================================================
    // Hit detection
    // ============================================================
    wire hit0 = valid[0][set_index] && (tag[0][set_index] == addr_tag);
    wire hit1 = valid[1][set_index] && (tag[1][set_index] == addr_tag);
    wire hit  = hit0 || hit1;

    // If way1 hits, use way1; else way0 (including the "hit0 only" case)
    wire hit_way  = hit1;
    wire repl_way = lru[set_index];    // victim chosen by LRU

    // ============================================================
    // FSM
    // ============================================================
    typedef enum logic [1:0] { IDLE, REFILL } cache_state_t;
    cache_state_t state, next_state;

    logic [TAG_BITS-1:0] miss_tag;
    logic [6:0]          miss_set;
    logic                miss_way;
    logic [1:0]          refill_cnt;
    logic [31:0]         refill_base_addr;

    // ============================================================
    // STORE MERGED WORD (for write-through + cache update on hits)
    // ============================================================
    logic [31:0] store_merged_word;

    always_comb begin
        // Default: just take the incoming word
        store_merged_word = writeDataM;

        // Only meaningful for write hits (oldword available)
        if (memWriteM && hit) begin
            logic [31:0] oldword;
            oldword = data[hit_way][set_index][word_offset];

            unique case (addressingModeM[1:0])
                2'b00: begin
                    // SB: write one byte
                    store_merged_word = oldword;
                    store_merged_word[8*byte_offset +: 8] = writeDataM[7:0];
                end

                2'b01: begin
                    // SH: write halfword
                    store_merged_word = oldword;
                    store_merged_word[16*byte_offset[1] +: 16] = writeDataM[15:0];
                end

                default: begin
                    // SW: full 32-bit store
                    store_merged_word = writeDataM;
                end
            endcase
        end
    end

    // ============================================================
    // Stall logic
    // ============================================================
    // Block on any miss (load or store) and during refill
    assign stallM = (state == IDLE)
                  ? ((memReadM || memWriteM) && !hit)
                  : 1'b1;

    // ============================================================
    // Combinational control FSM
    // ============================================================
    always_comb begin
        mem_read   = 1'b0;
        mem_write  = 1'b0;
        mem_addr   = 32'h0;
        mem_wdata  = 32'h0;
        next_state = state;

        unique case (state)
            IDLE: begin
                if (memReadM || memWriteM) begin
                    if (hit) begin
                        // Hit: loads use cache; stores are write-through
                        if (memWriteM) begin
                            mem_write = 1'b1;
                            mem_addr  = addrM;
                            mem_wdata = store_merged_word; // SB/SH/SW merged
                        end
                    end else begin
                        // Miss (load or store) -> write-allocate
                        next_state = REFILL;
                        // REFILL state will drive mem_read/mem_addr
                    end
                end
            end

            REFILL: begin
                // Refilling one line from backing memory
                mem_read = 1'b1;
                mem_addr = refill_base_addr + {refill_cnt, 2'b00};

                if (mem_ready && (refill_cnt == LINE_WORDS-1))
                    next_state = IDLE;
            end

            default: begin
                // Safety net for illegal state encodings
                next_state = IDLE;
            end
        endcase
    end

    // ============================================================
    // Sequential state and storage update
    // ============================================================
    integer w, s;
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            state            <= IDLE;
            refill_cnt       <= 2'b00;
            miss_tag         <= '0;
            miss_set         <= '0;
            miss_way         <= 1'b0;
            refill_base_addr <= 32'h0;

            // Arrays use blocking assignment in reset loops (Verilator-safe)
            for (w = 0; w < NUM_WAYS; w = w + 1) begin
                for (s = 0; s < NUM_SETS; s = s + 1) begin
                    valid[w][s] = 1'b0;
                    tag[w][s]   = '0;
                end
            end

            for (s = 0; s < NUM_SETS; s = s + 1)
                lru[s] = 1'b0;

        end else begin
            state <= next_state;

            unique case (state)
                IDLE: begin
                    // Write hit: update cached word + LRU
                    if (memWriteM && hit) begin
                        data[hit_way][set_index][word_offset] <= store_merged_word;
                        lru[set_index] <= ~hit_way;
                    end

                    // Capture miss info when we are about to enter REFILL
                    if (next_state == REFILL && (memReadM || memWriteM) && !hit) begin
                        miss_tag <= addr_tag;
                        miss_set <= set_index;

                        if (!valid[0][set_index])       miss_way <= 1'b0;
                        else if (!valid[1][set_index])  miss_way <= 1'b1;
                        else                            miss_way <= repl_way;

                        refill_cnt       <= 2'b00;
                        // Base address aligned to start of 16B line
                        refill_base_addr <= {addr_tag, set_index, 4'b0000};
                    end
                end

                REFILL: begin
                    if (mem_ready) begin
                        // One 32-bit word per beat
                        data[miss_way][miss_set][refill_cnt] <= mem_rdata;

                        if (refill_cnt == LINE_WORDS-1) begin
                            tag[miss_way][miss_set]   <= miss_tag;
                            valid[miss_way][miss_set] <= 1'b1;
                            lru[miss_set]             <= ~miss_way;
                            refill_cnt                <= 2'b00;
                        end else begin
                            refill_cnt <= refill_cnt + 2'b01;
                        end
                    end
                end

                default: begin
                    // Safety net — recover to clean state
                    state <= IDLE;
                end
            endcase
        end
    end

    // ============================================================
    // LOAD EXTRACT (LB/LBU/LH/LHU/LW) – from cached word
    // ============================================================
    always_comb begin
        readDataM = 32'h0;

        if (state == IDLE && memReadM && hit) begin
            logic [31:0] raw;
            raw = data[hit_way][set_index][word_offset];

            unique case (addressingModeM)
                // LB
                3'b000: begin
                    logic [7:0] b;
                    b = raw[8*byte_offset +: 8];
                    readDataM = {{24{b[7]}}, b};
                end

                // LBU
                3'b100: begin
                    logic [7:0] b;
                    b = raw[8*byte_offset +: 8];
                    readDataM = {24'b0, b};
                end

                // LH
                3'b001: begin
                    logic [15:0] h;
                    h = raw[16*byte_offset[1] +: 16];
                    readDataM = {{16{h[15]}}, h};
                end

                // LHU
                3'b101: begin
                    logic [15:0] h;
                    h = raw[16*byte_offset[1] +: 16];
                    readDataM = {16'b0, h};
                end

                // LW (and anything else)
                default: begin
                    readDataM = raw;
                end
            endcase
        end
    end

endmodule
