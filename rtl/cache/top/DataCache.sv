module DataCache (
    input  logic        clk, reset,

    // From pipeline MEM stage
    input  logic        memReadM,
    input  logic        memWriteM,
    input  logic [31:0] addrM,
    input  logic [31:0] writeDataM,

    output logic [31:0] readDataM,
    output logic        stallM,

    // To backing memory
    output logic        mem_read,
    output logic        mem_write,
    output logic [31:0] mem_addr,
    output logic [31:0] mem_wdata,
    input  logic [31:0] mem_rdata,
    input  logic        mem_ready
);

    // -----------------------------
    // Cache geometry:
    // 4096 B = 1024 words
    // 4 words/line → 256 lines
    // 2-way → 128 sets
    // -----------------------------

    logic [31:0] data [1:0][0:127][0:3];
    logic [20:0] tag  [1:0][0:127];
    logic        valid[1:0][0:127];
    logic        lru  [0:127];

    // FSM
    typedef enum logic [1:0] {
        IDLE,
        MISS_READ,
        REFILL
    } cache_state_t;

    cache_state_t state;

    // Address decode
    wire [1:0]  word_offset = addrM[3:2];
    wire [6:0]  set_index   = addrM[10:4];
    wire [20:0] addr_tag    = addrM[31:11];

    // Tag compare
    wire hit0 = valid[0][set_index] && (tag[0][set_index] == addr_tag);
    wire hit1 = valid[1][set_index] && (tag[1][set_index] == addr_tag);
    wire hit  = hit0 || hit1;
    wire hit_way = hit1;  // 0 if way0 hit, 1 if way1 hit

    wire repl_way = lru[set_index];

    // FSM (fill later)
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            state <= IDLE;
        end else begin
            case (state)
                IDLE: begin
                    if ((memReadM || memWriteM) && !hit)
                        state <= MISS_READ;
                end

                MISS_READ: begin
                    if (mem_ready)
                        state <= REFILL;
                end

                REFILL: begin
                    state <= IDLE;
                end
            endcase
        end
    end

    // Stall logic
    assign stallM = (state != IDLE);

endmodule
