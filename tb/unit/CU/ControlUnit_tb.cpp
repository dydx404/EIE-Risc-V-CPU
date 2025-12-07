#include <gtest/gtest.h>
#include <verilated.h>
#include "VControlUnit.h"

static VControlUnit *dut;

static void apply(uint8_t op, uint8_t funct3, uint8_t funct7, bool zero) {
    dut->op     = op;
    dut->funct3 = funct3;
    dut->funct7 = funct7;
    dut->zero   = zero;
    dut->eval();
}

// ---------------------- Tests --------------------------

TEST(ControlUnit, Rtype_ADD) {
    apply(0b0110011, 0b000, 0b0000000, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->ALUSrc,   0);
    EXPECT_EQ(dut->ALUControl, 0b0000); // ADD
    EXPECT_EQ(dut->MemWrite, 0);
    EXPECT_EQ(dut->PCSrc,    0b00);
    EXPECT_EQ(dut->ResultSrc, 0b00);
}

TEST(ControlUnit, Rtype_SUB) {
    apply(0b0110011, 0b000, 0b0100000, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->ALUSrc,   0);
    EXPECT_EQ(dut->ALUControl, 0b0001); // SUB
}

TEST(ControlUnit, Rtype_AND) {
    apply(0b0110011, 0b111, 0b0000000, 0);

    EXPECT_EQ(dut->ALUControl, 0b0010); // AND
}

TEST(ControlUnit, Itype_ADDI) {
    apply(0b0010011, 0b000, 0, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ImmSrc,   0b000);
    EXPECT_EQ(dut->ALUControl, 0b0000); // ADD
    EXPECT_EQ(dut->ResultSrc, 0b00);
    EXPECT_EQ(dut->PCSrc,    0b00);
}

TEST(ControlUnit, Load_LW) {
    apply(0b0000011, 0b010, 0, 0); // LW

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->MemWrite, 0);
    EXPECT_EQ(dut->ImmSrc,   0b000);
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ALUControl, 0b0000); // ADD
    EXPECT_EQ(dut->ResultSrc, 0b01);    // from memory
    EXPECT_EQ(dut->PCSrc,    0b00);
    EXPECT_EQ(dut->AddressingControl, 0b010);
}

TEST(ControlUnit, Store_SW) {
    apply(0b0100011, 0b010, 0, 0); // SW

    EXPECT_EQ(dut->RegWrite, 0);
    EXPECT_EQ(dut->MemWrite, 1);
    EXPECT_EQ(dut->ImmSrc,   0b001);
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ALUControl, 0b0000); // ADD
    EXPECT_EQ(dut->PCSrc,    0b00);
    EXPECT_EQ(dut->AddressingControl, 0b010);
}

TEST(ControlUnit, BEQ_Taken) {
    apply(0b1100011, 0b000, 0, /*zero=*/1);

    EXPECT_EQ(dut->RegWrite, 0);
    EXPECT_EQ(dut->PCSrc,    0b01); // branch taken
    EXPECT_EQ(dut->ImmSrc,   0b010);
}

TEST(ControlUnit, BEQ_NotTaken) {
    apply(0b1100011, 0b000, 0, /*zero=*/0);
    EXPECT_EQ(dut->PCSrc,    0b00);
}

TEST(ControlUnit, BNE_Taken) {
    apply(0b1100011, 0b001, 0, /*zero=*/0);
    EXPECT_EQ(dut->PCSrc,    0b01);
}

TEST(ControlUnit, BNE_NotTaken) {
    apply(0b1100011, 0b001, 0, /*zero=*/1);
    EXPECT_EQ(dut->PCSrc,    0b00);
}

TEST(ControlUnit, JAL) {
    apply(0b1101111, 0, 0, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->PCSrc,    0b01);  // PC+imm
    EXPECT_EQ(dut->ImmSrc,   0b011); // J-type
    EXPECT_EQ(dut->ResultSrc, 0b10); // PC+4
}

TEST(ControlUnit, JALR) {
    apply(0b1100111, 0, 0, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->PCSrc,    0b10);  // JALR path
    EXPECT_EQ(dut->ImmSrc,   0b000); // I-type
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ResultSrc, 0b10); // PC+4
}

TEST(ControlUnit, LUI) {
    apply(0b0110111, 0, 0, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->ImmSrc,   0b100); // U-type
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ALUControl, 0b1111); // LUI op
    EXPECT_EQ(dut->ResultSrc, 0b00);
    EXPECT_EQ(dut->PCSrc,    0b00);
}

TEST(ControlUnit, UnknownOpcode_Defaults) {
    apply(0b0000000, 0, 0, 0);

    EXPECT_EQ(dut->RegWrite, 0);
    EXPECT_EQ(dut->MemWrite, 0);
    EXPECT_EQ(dut->PCSrc,    0b00);
    EXPECT_EQ(dut->ResultSrc, 0b00);
}

// ---------------------- main ---------------------------

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    dut = new VControlUnit;
    int result = RUN_ALL_TESTS();
    delete dut;

    return result;
}
