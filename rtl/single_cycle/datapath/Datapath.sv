module Datapath (
    input  logic        clk,
    input  logic        rst,

    // Control signals from Control Unit
    input  logic        regwrite,       // write enable for RegFile
    input  logic        alusrc,         // 0: rs2, 1: immext
    input  logic [1:0]  immsrc,         // selects immediate format for Extend
    input  logic [2:0]  alucontrol,     // 3-bit ALU control from ControlUnit
    input  logic        pcsrc,          // branch/jump taken (from CU, already uses zero)
    input  logic [1:0]  resultsrc,      // 00: ALU, 01: DataMem, 10: PC+4
    input  logic        memwrite,       // store enable
    input  logic        memread,        // load enable
    input  logic        jalre,          // 1 = JALR, use ALU result as PC
    input  logic [2:0]  dmem_access_ctrl, // passed straight to DataMemory

    // Outputs back to ControlUnit
    output logic        zero,           // from ALU
    output logic [6:0]  opcode,
    output logic [2:0]  funct3,
    output logic [6:0]  funct7,

    // Optional "debug" outputs
    output logic [31:0] pc,             // current PC
    output logic [31:0] instr,          // current instruction
    output logic [31:0] alu_result,     // ALU result
    output logic [31:0] a0              // x10 from RegFile (exposed)
);

    // Internal wires

    // Register file wires
    logic [4:0]  rs1, rs2, rd;
    logic [31:0] rd1, rd2;

    // Immediate
    logic [31:0] immext;

    // ALU operand B after ALUSrc mux
    logic [31:0] srcB;

    // DataMemory
    logic [31:0] read_data;

    // Writeback data to RegFile
    logic [31:0] wb_data;

    // PC control (2-bit for PC module)
    logic [1:0]  pcsrc_sel;

    // For PC+4 used in writeback (for JAL)
    logic [31:0] pc_plus4;

    // PC + Instruction Memory

    // PC block
    PC #(
        .DATA_WIDTH(32)
    ) pc_inst (
        .clk    (clk),
        .rst    (rst),
        .pcsrc  (pcsrc_sel),   // 2-bit select we derive below
        .immop  (immext),      // from Extend
        .alu_out(alu_result),  // for JALR
        .pc     (pc)
    );

    // Instruction memory
    InstrMem #(
        .DATA_WIDTH(32),
        .ADDR_WIDTH(10)
    ) instr_mem_inst (
        .addr  (pc),
        .instr (instr)
    );

    // Instruction field decode

    assign opcode = instr[6:0];
    assign funct3 = instr[14:12];
    assign funct7 = instr[31:25];

    assign rd  = instr[11:7];
    assign rs1 = instr[19:15];
    assign rs2 = instr[24:20];

    // Register File

    RegFile regfile_inst (
        .clk (clk),
        .we  (regwrite),
        .rs1 (rs1),
        .rs2 (rs2),
        .rd  (rd),
        .wd  (wb_data),
        .rd1 (rd1),
        .rd2 (rd2),
        .a0  (a0)
    );

    // Immediate Extend

    Extend extend_inst (
        .imm    (instr[31:7]),
        .immsrc (immsrc),
        .immext (immext)
    );

    // ALU

    // ALUSrc mux: choose between register operand and immediate
    assign srcB = alusrc ? immext : rd2;

    ALU #(
        .LEN(32)
    ) alu_inst (
        .aluop1   (rd1),
        .aluop2   (srcB),
        .alu_ctrl ({1'b0, alucontrol}),  // (since rn, control unit only geneating 3 bit but alu needs 4 bit (0 start val once corrected))
        .aluout   (alu_result),
        .zero     (zero)
    );

    // Data Memory

    DataMemory #(
        .STORAGE_WIDTH(8),
        .ADDR_WIDTH(17)
    ) dmem_inst (
        .clk_i          (clk),
        .mem_read_i     (memread),
        .mem_write_i    (memwrite),
        .addr_i         (alu_result),
        .write_data_i   (rd2),
        .access_ctrl_i  (dmem_access_ctrl),
        .read_data_o    (read_data)
    );

    //PC+4 (for JAL writeback)

    assign pc_plus4 = pc + 32'd4;

    // Writeback mux (ResultSrc)

    always_comb begin
        unique case (resultsrc)
            2'b00: wb_data = alu_result; // normal ALU result
            2'b01: wb_data = read_data;  // load from DataMemory
            2'b10: wb_data = pc_plus4;   // JAL link value
            default: wb_data = alu_result;
        endcase
    end

    // PC source select (pcsrc_sel for PC)
    always_comb begin
        if (!pcsrc) begin
            pcsrc_sel = 2'b00;      // normal sequential PC+4
        end else if (jalre) begin
            pcsrc_sel = 2'b10;      // JALR: use ALU result
        end else begin
            pcsrc_sel = 2'b01;      // branch / JAL: PC + immediate
        end
    end

endmodule
