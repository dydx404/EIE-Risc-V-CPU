module InstrMem #(
    DATA_WIDTH = 32,
    ADDR_WIDTH = 10 // Sets how big the memory is. 2^10 = 1024 words
) (
    input   logic [DATA_WIDTH-1:0]        addr,
    output  logic [DATA_WIDTH-1:0]        instr
);
    // Memory Declaration
    logic [DATA_WIDTH-1:0] memory [0:(1<<ADDR_WIDTH)-1];

    // Initialize Instruction Memory (replace with $readmemh if needed)
    initial begin
        $readmemh("program.mem", memory);
        // Add more instructions as needed
    end

    // Read Instruction
    assign instr = memory[addr[ADDR_WIDTH+1:2]]; // Word-aligned address
    //PC is byte address but instuctions are 32bits=4bytes=1 word
    //so we need to turn a byte address into a word index
    //byte address divided by 4= Instruction right shift by 2 bits
endmodule
