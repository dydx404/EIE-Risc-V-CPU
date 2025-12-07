module InstrMem (
    input   logic [31:0] addr,
    output  logic [31:0] instrF
);

    // 256 words of instruction memory (1 KB)
    logic [31:0] memory [0:255];

    initial begin
        $readmemh("program.mem", memory);

        // Debug dump
        $display("[InstrMem] word[0] = %h", memory[0]);
        $display("[InstrMem] word[1] = %h", memory[1]);
        $display("[InstrMem] word[2] = %h", memory[2]);
        $display("[InstrMem] word[3] = %h", memory[3]);
    end

    // ✅ Word-aligned access: PC / 4
    assign instrF = memory[addr[31:2]];

endmodule
