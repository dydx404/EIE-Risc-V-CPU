module ControlUnit #(
    //parameter DATA_WIDTH = 32
) (
    input  logic        zero,    // used for BEQ/BNE
    input  logic [6:0]  op,      // opcode
    input  logic [2:0]  funct3,  // funct3
    input  logic [6:0]  funct7,  // funct7

    output logic        RegWrite,
    output logic [3:0]  ALUControl,
    output logic        ALUSrc,
    output logic        MemWrite,
    output logic [1:0]  PCSrc,
    output logic [1:0]  ResultSrc,
    output logic [2:0]  ImmSrc,
    output logic [2:0]  AddressingControl
);

    // ALU op encoding
    localparam [3:0] ALU_ADD  = 4'b0000;
    localparam [3:0] ALU_SUB  = 4'b0001;
    localparam [3:0] ALU_AND  = 4'b0010;
    localparam [3:0] ALU_OR   = 4'b0011;
    localparam [3:0] ALU_XOR  = 4'b0100;
    localparam [3:0] ALU_SLT  = 4'b0101;
    localparam [3:0] ALU_SLTU = 4'b0110;
    localparam [3:0] ALU_SLL  = 4'b0111;
    localparam [3:0] ALU_SRL  = 4'b1000;
    localparam [3:0] ALU_SRA  = 4'b1011;
    localparam [3:0] ALU_LUI  = 4'b1111;

    always_comb begin
        // ---------------------- DEFAULT VALUES ----------------------
        RegWrite          = 1'b0;
        ALUControl        = ALU_ADD;
        ALUSrc            = 1'b0;
        MemWrite          = 1'b0;
        PCSrc             = 2'b00; // PC+4
        ResultSrc         = 2'b00; // ALU result
        ImmSrc            = 3'b000; // default I-type
        AddressingControl = 3'b000;

        // ---------------------- OPCODE DECODER ----------------------
        case (op)

        // ====================== R-TYPE ==============================
        7'b0110011: begin
            RegWrite  = 1'b1;
            ALUSrc    = 1'b0;
            ResultSrc = 2'b00;
            PCSrc     = 2'b00;

            case (funct3)
                3'b000: begin
                    if (funct7 == 7'b0100000)
                        ALUControl = ALU_SUB;
                    else
                        ALUControl = ALU_ADD;
                end
                3'b001: ALUControl = ALU_SLL;
                3'b010: ALUControl = ALU_SLT;
                3'b011: ALUControl = ALU_SLTU;
                3'b100: ALUControl = ALU_XOR;
                3'b110: ALUControl = ALU_OR;
                3'b111: ALUControl = ALU_AND;
                3'b101: begin
                    if (funct7 == 7'b0000000)
                        ALUControl = ALU_SRL;
                    else
                        ALUControl = ALU_SRA;
                end
                default: begin end
            endcase
        end

        // ====================== BRANCHES ============================
        7'b1100011: begin
            RegWrite   = 1'b0;
            ImmSrc     = 3'b010; // B-type
            ALUSrc     = 1'b0;
            ALUControl = ALU_SUB;

            case (funct3)
                3'b000: PCSrc = zero ? 2'b01 : 2'b00; // BEQ
                3'b001: PCSrc = zero ? 2'b00 : 2'b01; // BNE
                default: PCSrc = 2'b00;
            endcase
        end

        // ====================== I-TYPE ALU ==========================
        7'b0010011: begin
            RegWrite  = 1'b1;
            ImmSrc    = 3'b000; // I-type
            ALUSrc    = 1'b1;
            ResultSrc = 2'b00;
            PCSrc     = 2'b00;

            case (funct3)
                3'b000: ALUControl = ALU_ADD;   // ADDI
                3'b010: ALUControl = ALU_SLT;   // SLTI
                3'b011: ALUControl = ALU_SLTU;  // SLTIU
                3'b001: ALUControl = ALU_SLL;   // SLLI
                3'b100: ALUControl = ALU_XOR;   // XORI
                3'b110: ALUControl = ALU_OR;    // ORI
                3'b111: ALUControl = ALU_AND;   // ANDI

                3'b101: begin
                    if (funct7 == 7'b0000000)
                        ALUControl = ALU_SRL;  // SRLI
                    else
                        ALUControl = ALU_SRA;  // SRAI
                end
                default: begin end
            endcase
        end

        // ====================== JAL ================================
        7'b1101111: begin
            RegWrite  = 1'b1;
            ImmSrc    = 3'b011;  // J-type
            ResultSrc = 2'b10;   // PC+4
            PCSrc     = 2'b01;   // PC + imm
            ALUControl = ALU_ADD;
            ALUSrc    = 1'b0;
        end

        // ====================== LOAD ===============================
        7'b0000011: begin
            RegWrite          = 1'b1;
            ImmSrc            = 3'b000; // I-type
            ALUSrc            = 1'b1;
            ALUControl        = ALU_ADD;
            ResultSrc         = 2'b01; // memory
            PCSrc             = 2'b00;
            MemWrite          = 1'b0;
            AddressingControl = funct3;
        end

        // ====================== STORE ==============================
        7'b0100011: begin
            RegWrite          = 1'b0;
            ImmSrc            = 3'b001; // S-type
            ALUSrc            = 1'b1;
            ALUControl        = ALU_ADD;
            PCSrc             = 2'b00;
            MemWrite          = 1'b1;
            AddressingControl = funct3;
        end

        // ====================== JALR ===============================
        7'b1100111: begin
            RegWrite  = 1'b1;
            ImmSrc    = 3'b000; // I-type
            ResultSrc = 2'b10;  // PC+4
            PCSrc     = 2'b10;  // JALR path
            ALUControl = ALU_ADD;
            ALUSrc    = 1'b1;
        end

        // ====================== LUI ================================
        7'b0110111: begin
            RegWrite  = 1'b1;
            ImmSrc    = 3'b100; // U-type
            ResultSrc = 2'b00;  // ALU
            PCSrc     = 2'b00;
            ALUSrc    = 1'b1;
            ALUControl = ALU_LUI;
        end

        // ====================== DEFAULT ============================
        default: begin end

        endcase
    end

endmodule
