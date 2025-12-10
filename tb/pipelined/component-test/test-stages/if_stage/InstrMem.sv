module InstrMem(
    input   logic [31:0]        addr,
    output  logic [31:0]        instrF
);
    // Memory Declaration
    logic [7:0] memory [32'hbfc00fff:32'hbfc00000];

    // Initialize Instruction Memory (replace with $readmemh if needed)
    initial begin
        // Add more instructions as needed
    end

    // Read Instruction
    assign instrF = {memory[addr[11:0]], memory[addr[11:0]+1], memory[addr[11:0]+2], memory[addr[11:0]+3]}; // Word-aligned address, able to load byte
endmodule
