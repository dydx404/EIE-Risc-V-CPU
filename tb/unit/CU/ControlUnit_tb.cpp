#include <gtest/gtest.h>
#include "VControlUnit.h"
#include "verilated.h"
#include <cstdint>

struct CtrlRef {
    bool     regwrite;
    bool     alusrc;
    uint8_t  immsrc;      // 2 bits
    uint8_t  alucontrol;  // 3 bits
    bool     pcsrc;
    uint8_t  resultsrc;   // 2 bits
    bool     memwrite;
    bool     jalre;
};

static CtrlRef ref_control(uint8_t opcode, uint8_t funct3,
                           uint8_t funct7, bool zero) {
    CtrlRef c{};

    // defaults (match default: block in CU)
    c.regwrite   = 0;
    c.alusrc     = 0;
    c.immsrc     = 0b00;
    c.alucontrol = 0b000;
    c.pcsrc      = 0;
    c.resultsrc  = 0b00;
    c.memwrite   = 0;
    c.jalre      = 0;

    switch (opcode) {
        // ADDI
        case 0b0010011: { // 0x13
            if (funct3 == 0b000) {
                c.regwrite   = 1;
                c.alusrc     = 1;
                c.immsrc     = 0b00;   // I-type
                c.alucontrol = 0b000;  // ADD
                c.pcsrc      = 0;
                c.resultsrc  = 0b00;   // ALU result
                c.memwrite   = 0;
                c.jalre      = 0;
            }
            break;
        }

        // BNE
        case 0b1100011: { // 0x63
            if (funct3 == 0b001) {
                c.regwrite   = 0;
                c.alusrc     = 0;
                c.immsrc     = 0b10;   // B-type
                c.alucontrol = 0b001;  // SUB / compare
                c.pcsrc      = !zero;  // BNE
                c.resultsrc  = 0b00;
                c.memwrite   = 0;
                c.jalre      = 0;
            }
            break;
        }

        // ADD (R-type)
        case 0b0110011: { // 0x33
            if (funct3 == 0b000 && funct7 == 0b0000000) {
                c.regwrite   = 1;
                c.alusrc     = 0;
                c.immsrc     = 0b00;   // don't care but 00 is fine
                c.alucontrol = 0b000;  // ADD
                c.pcsrc      = 0;
                c.resultsrc  = 0b00;
                c.memwrite   = 0;
                c.jalre      = 0;
            }
            break;
        }

        // LBU
        case 0b0000011: { // 0x03
            if (funct3 == 0b100) {
                c.regwrite   = 1;
                c.alusrc     = 1;
                c.immsrc     = 0b00;   // I-type
                c.alucontrol = 0b000;  // ADD address
                c.pcsrc      = 0;
                c.resultsrc  = 0b01;   // Data memory
                c.memwrite   = 0;
                c.jalre      = 0;
            }
            break;
        }

        // SB
        case 0b0100011: { // 0x23
            if (funct3 == 0b000) {
                c.regwrite   = 0;
                c.alusrc     = 1;
                c.immsrc     = 0b01;   // S-type
                c.alucontrol = 0b000;  // ADD address
                c.pcsrc      = 0;
                c.resultsrc  = 0b00;
                c.memwrite   = 1;
                c.jalre      = 0;
            }
            break;
        }

        // JAL
        case 0b1101111: { // 0x6F
            c.regwrite   = 1;
            c.alusrc     = 0;
            c.immsrc     = 0b11;   // J-type
            c.alucontrol = 0b000;  // don't-care
            c.pcsrc      = 1;      // jump
            c.resultsrc  = 0b10;   // PC+4
            c.memwrite   = 0;
            c.jalre      = 0;
            break;
        }

        // JALR (RET)
        case 0b1100111: { // 0x67
            c.regwrite   = 0;      // RET via rd=x0
            c.alusrc     = 0;
            c.immsrc     = 0b00;   // I-type
            c.alucontrol = 0b000;
            c.pcsrc      = 1;      // take jump
            c.resultsrc  = 0b00;
            c.memwrite   = 0;
            c.jalre      = 1;      // select rs1+imm path
            break;
        }

        default:
            // keep defaults: NOP / safe
            break;
    }

    return c;
}

// ---------------- Test Fixture ----------------

class ControlUnitTest : public ::testing::Test {
protected:
    VControlUnit* dut = nullptr;

    void SetUp() override {
        dut = new VControlUnit;
    }

    void TearDown() override {
        delete dut;
    }

    void drive(uint8_t opcode, uint8_t funct3,
               uint8_t funct7, bool zero) {
        dut->opcode = opcode;
        dut->funct3 = funct3;
        dut->funct7 = funct7;
        dut->zero   = zero;
        dut->eval();
    }

    void checkAgainstRef(uint8_t opcode, uint8_t funct3,
                         uint8_t funct7, bool zero) {
        drive(opcode, funct3, funct7, zero);
        CtrlRef r = ref_control(opcode, funct3, funct7, zero);

        EXPECT_EQ(dut->regwrite,   r.regwrite);
        EXPECT_EQ(dut->alusrc,     r.alusrc);
        EXPECT_EQ(dut->immsrc,     r.immsrc);
        EXPECT_EQ(dut->alucontrol, r.alucontrol);
        EXPECT_EQ(dut->pcsrc,      r.pcsrc);
        EXPECT_EQ(dut->resultsrc,  r.resultsrc);
        EXPECT_EQ(dut->memwrite,   r.memwrite);
        EXPECT_EQ(dut->jalre,      r.jalre);
    }
};

// ------------- Directed tests -------------

TEST_F(ControlUnitTest, ADDI) {
    // opcode=0x13, funct3=000
    checkAgainstRef(0b0010011, 0b000, 0, false);
    checkAgainstRef(0b0010011, 0b000, 0, true);
}

TEST_F(ControlUnitTest, BNE) {
    // BNE taken when zero==0
    checkAgainstRef(0b1100011, 0b001, 0, false);
    // BNE not taken when zero==1
    checkAgainstRef(0b1100011, 0b001, 0, true);
}

TEST_F(ControlUnitTest, ADD_Rtype) {
    checkAgainstRef(0b0110011, 0b000, 0b0000000, false);
}

TEST_F(ControlUnitTest, LBU) {
    checkAgainstRef(0b0000011, 0b100, 0, false);
}

TEST_F(ControlUnitTest, SB) {
    checkAgainstRef(0b0100011, 0b000, 0, false);
}

TEST_F(ControlUnitTest, JAL) {
    checkAgainstRef(0b1101111, 0, 0, false);
}

TEST_F(ControlUnitTest, JALR) {
    checkAgainstRef(0b1100111, 0, 0, false);
}

// ------------- Randomized tests -------------

TEST_F(ControlUnitTest, RandomSupportedInstructions) {
    for (int i = 0; i < 500; ++i) {
        int kind = rand() % 8;
        bool zero = rand() & 1;

        uint8_t opcode = 0, funct3 = 0, funct7 = 0;

        switch (kind) {
            case 0: // ADDI
                opcode = 0b0010011;
                funct3 = 0b000;
                break;
            case 1: // BNE
                opcode = 0b1100011;
                funct3 = 0b001;
                break;
            case 2: // ADD
                opcode = 0b0110011;
                funct3 = 0b000;
                funct7 = 0b0000000;
                break;
            case 3: // LBU
                opcode = 0b0000011;
                funct3 = 0b100;
                break;
            case 4: // SB
                opcode = 0b0100011;
                funct3 = 0b000;
                break;
            case 5: // JAL
                opcode = 0b1101111;
                break;
            case 6: // JALR
                opcode = 0b1100111;
                break;
            case 7: // random unsupported → should be default NOP
            default:
                opcode = rand() & 0x7F;
                funct3 = rand() & 0x7;
                funct7 = rand() & 0x7F;
                break;
        }

        checkAgainstRef(opcode, funct3, funct7, zero);
    }
}
