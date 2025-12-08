module DataCache (
    input  logic        clk,
    input  logic        reset,

    input  logic        memReadM,
    input  logic        memWriteM,
    input  logic [31:0] addrM,
    input  logic [31:0] writeDataM,

    output logic [31:0] readDataM,
    output logic        stallM,

    output logic        mem_read,
    output logic        mem_write,
    output logic [31:0] mem_addr,
    output logic [31:0] mem_wdata,
    input  logic [31:0] mem_rdata,
    input  logic        mem_ready
);

    localparam int LINE_WORDS = 4;
    localparam int NUM_SETS   = 128;
    localparam int NUM_WAYS   = 2;
    localparam int TAG_BITS  = 21;

    logic [31:0] data [NUM_WAYS-1:0][NUM_SETS-1:0][LINE_WORDS-1:0];
    logic [TAG_BITS-1:0] tag   [NUM_WAYS-1:0][NUM_SETS-1:0];
    logic                valid [NUM_WAYS-1:0][NUM_SETS-1:0];
    logic                lru   [NUM_SETS-1:0];

    wire [1:0] word_offset = addrM[3:2];
    wire [6:0] set_index   = addrM[10:4];
    wire [TAG_BITS-1:0] addr_tag = addrM[31:11];

    wire hit0 = valid[0][set_index] && (tag[0][set_index] == addr_tag);
    wire hit1 = valid[1][set_index] && (tag[1][set_index] == addr_tag);
    wire hit  = hit0 || hit1;
    wire hit_way  = hit1;
    wire repl_way = lru[set_index];

    typedef enum logic [1:0] { IDLE, REFILL } cache_state_t;
    cache_state_t state, next_state;

    logic [TAG_BITS-1:0] miss_tag;
    logic [6:0]          miss_set;
    logic                miss_way;
    logic [1:0]          refill_cnt;
    logic [31:0]         refill_base_addr;

    assign stallM = (state == IDLE) ? (memReadM && !hit) : 1'b1;

    always_comb begin
        mem_read   = 1'b0;
        mem_write  = 1'b0;
        mem_addr   = 32'h0;
        mem_wdata  = 32'h0;
        next_state = state;

        case (state)
            IDLE: begin
                if (memReadM || memWriteM) begin
                    if (hit) begin
                        if (memWriteM) begin
                            mem_write = 1'b1;
                            mem_addr  = addrM;
                            mem_wdata = writeDataM;
                        end
                    end else begin
                        if (memReadM)
                            next_state = REFILL;
                        else begin
                            mem_write = 1'b1;
                            mem_addr  = addrM;
                            mem_wdata = writeDataM;
                        end
                    end
                end
            end

            REFILL: begin
                mem_read = 1'b1;
                mem_addr = refill_base_addr + {refill_cnt, 2'b00};
                if (mem_ready) begin
                    if (refill_cnt == LINE_WORDS-1)
                        next_state = IDLE;
                end
            end
        endcase
    end

    integer w, s;
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            state <= IDLE;
            for (w = 0; w < NUM_WAYS; w = w + 1)
                for (s = 0; s < NUM_SETS; s = s + 1) begin
                    valid[w][s] = 1'b0;
                    tag[w][s]   = '0;
                end
            for (s = 0; s < NUM_SETS; s = s + 1)
                lru[s] = 1'b0;

            refill_cnt       <= 2'b00;
            miss_tag         <= '0;
            miss_set         <= '0;
            miss_way         <= 1'b0;
            refill_base_addr <= 32'h0;

        end else begin
            state <= next_state;

            case (state)
                IDLE: begin
                    if (memWriteM && hit) begin
                        data[hit_way][set_index][word_offset] <= writeDataM;
                        lru[set_index] <= ~hit_way;
                    end

                    if ((next_state == REFILL) && memReadM && !hit) begin
                        miss_tag <= addr_tag;
                        miss_set <= set_index;
                        if (!valid[0][set_index]) miss_way <= 1'b0;
                        else if (!valid[1][set_index]) miss_way <= 1'b1;
                        else miss_way <= repl_way;

                        refill_cnt       <= 2'b00;
                        refill_base_addr <= {addr_tag, set_index, 2'b00, 2'b00};
                    end
                end

                REFILL: begin
                    if (mem_ready) begin
                        data[miss_way][miss_set][refill_cnt] <= mem_rdata;

                        if (refill_cnt == LINE_WORDS-1) begin
                            tag[miss_way][miss_set]   <= miss_tag;
                            valid[miss_way][miss_set] <= 1'b1;
                            lru[miss_set] <= ~miss_way;
                            refill_cnt <= 2'b00;
                        end else
                            refill_cnt <= refill_cnt + 2'b01;
                    end
                end
            endcase
        end
    end

    always_comb begin
        readDataM = 32'h0;
        if (state == IDLE && memReadM && hit)
            readDataM = data[hit_way][set_index][word_offset];
    end

endmodule
