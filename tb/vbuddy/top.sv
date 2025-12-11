module top (
    input  logic        clk,
    input  logic        rst,
    output logic [31:0] a0      // expose register x10 (a0)
);

    // -----------------------------------------
    // Program Counter / Instruction
    // -----------------------------------------
    logic [31:0] pc;
    logic [31:0] instr;

    // Immediate from extender
    logic [31:0] immext;

    // -----------------------------------------
    // Instruction Memory
    // -----------------------------------------
    InstrMem imem (
        .addr   (pc),     // word-aligned
        .instr  (instr)
    );

    // -----------------------------------------
    // Decode fields
    // -----------------------------------------
    logic [6:0] opcode;
    logic [4:0] rd;
    logic [2:0] funct3;
    logic [4:0] rs1;
    logic [4:0] rs2;
    logic [6:0] funct7;

    assign opcode = instr[6:0];
    assign rd     = instr[11:7];
    assign funct3 = instr[14:12];
    assign rs1    = instr[19:15];
    assign rs2    = instr[24:20];
    assign funct7 = instr[31:25];

    // -----------------------------------------
    // Control Unit Outputs
    // -----------------------------------------
    logic        RegWrite;
    logic [3:0]  ALUControl;
    logic        ALUSrc;
    logic        MemWrite;
    logic [1:0]  PCSrc;
    logic [1:0]  ResultSrc;
    logic [2:0]  ImmSrc;
    logic [2:0]  AddressingControl;

    logic        zero;

    ControlUnit control (
        .zero              (zero),
        .op                (opcode),
        .funct3            (funct3),
        .funct7            (funct7),

        .RegWrite          (RegWrite),
        .ALUControl        (ALUControl),
        .ALUSrc            (ALUSrc),
        .MemWrite          (MemWrite),
        .PCSrc             (PCSrc),
        .ResultSrc         (ResultSrc),
        .ImmSrc            (ImmSrc),
        .AddressingControl (AddressingControl)
    );

    // -----------------------------------------
    // Register File
    // -----------------------------------------
    logic [31:0] rd1, rd2;
    logic [31:0] result;
    logic [31:0] wd_safe;

    // optional safety gate; could also just use `result`
    assign wd_safe = RegWrite ? result : 32'b0;

    RegFile regfile (
        .clk            (clk),
        .we3            (RegWrite),   // write-enable from CU
        .ad1            (rs1),        // rs1 index
        .ad2            (rs2),        // rs2 index
        .ad3            (rd),         // rd index
        .wd3            (wd_safe),    // ALU or Mem or PC+4 result
        .rd1            (rd1),        // operand 1
        .rd2            (rd2),        // operand 2

        .testRegAddress (5'd10),      // x10 = a0
        .testRegData    (a0)
    );

    // -----------------------------------------
    // Immediate Generator
    // -----------------------------------------
    Extend immgen (
        .Immediate (instr[31:7]),
        .ImmSrc    (ImmSrc),
        .ImmExt    (immext)
    );

    // -----------------------------------------
    // ALU
    // -----------------------------------------
    logic [31:0] srca, srcb, alu_out;

    assign srca = rd1;
    assign srcb = (ALUSrc == 1'b1) ? immext : rd2;

    ALU alu (
        .aluop1   (srca),
        .aluop2   (srcb),
        .alu_ctrl (ALUControl),
        .aluout   (alu_out),
        .zero     (zero)
    );

    // -----------------------------------------
    // Data Memory
    // -----------------------------------------
    logic [31:0] read_data;

    DataMemory datamem (
        .clk_i          (clk),
        .mem_read_i     (ResultSrc == 2'b01),  // only when selecting memory
        .mem_write_i    (MemWrite),
        .addr_i         (alu_out),
        .write_data_i   (rd2),
        .access_ctrl_i  (AddressingControl),
        .read_data_o    (read_data)
    );

    // -----------------------------------------
    // Result Selection (ALU, Memory, PC+4)
    // -----------------------------------------
    logic [31:0] pc_plus4;
    assign pc_plus4 = pc + 32'd4;

    logic [31:0] read_data_safe;
    assign read_data_safe = (ResultSrc == 2'b01) ? read_data : 32'b0;

    always_comb begin
        unique case (ResultSrc)
            2'b00: result = alu_out;        // ALU result
            2'b01: result = read_data_safe; // Memory load
            2'b10: result = pc_plus4;       // JAL/JALR link address
            default: result = 32'b0;
        endcase
    end

    // -----------------------------------------
    // PC Logic
    // -----------------------------------------
    PCFlat pc_unit (
        .clk     (clk),
        .rst     (rst),
        .pcsrc   (PCSrc),
        .immop   (immext),
        .alu_out (alu_out),
        .pc      (pc)
    );

 integer cycle = 0;
   always_ff @(posedge clk) begin
    cycle <= cycle + 1;  
    $display("Cycle %0d", cycle);
    $display("PC           = %h", pc);
    $display("instr        = %h", instr);

    // a0 monitor
    $display("a0 (x10)     = %h", a0);

end

endmodule
