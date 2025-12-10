#include <gtest/gtest.h>
#include <verilated.h>
#include "VDecode.h"

static VDecode *dut;

static void tick() {
    dut->CLK = 0;
    dut->eval();
    dut->CLK = 1;
    dut->eval();
}

// ---------------------- Tests --------------------------

TEST(DecodeStage, WritebackAndRead) {
    dut->regwrite = 0;
    tick();

    // Write x5 = 0xDEADBEEF
    dut->regwrite = 1;
    dut->rdW      = 5;
    dut->resultW  = 0xDEADBEEF;
    tick();

    // add x1, x5, x0
    dut->instrd = 0b0000000'00000'00101'000'00001'0110011;
    dut->regwrite = 0;
    tick();

    EXPECT_EQ(dut->rd1, 0xDEADBEEF);
    EXPECT_EQ(dut->rd2, 0x00000000);
}

TEST(DecodeStage, X0IsImmutable) {
    dut->regwrite = 1;
    dut->rdW      = 0;
    dut->resultW  = 0xFFFFFFFF;
    tick();

    // add x1, x0, x0
    dut->instrd = 0b0000000'00000'00000'000'00001'0110011;
    tick();

    EXPECT_EQ(dut->rd1, 0x00000000);
}

TEST(DecodeStage, RTypeADDDecode) {
    // add x3, x1, x2
    dut->instrd = 0b0000000'00010'00001'000'00011'0110011;
    tick();

    EXPECT_EQ(dut->rs1D, 1);
    EXPECT_EQ(dut->rs2D, 2);
    EXPECT_EQ(dut->rdD,  3);

    EXPECT_EQ(dut->regwriteD, 1);
    EXPECT_EQ(dut->memwriteD, 0);
    EXPECT_EQ(dut->memReadD,  0);   // ✅ NEW
    EXPECT_EQ(dut->branchD,   0);
    EXPECT_EQ(dut->jumpD,     0);

    EXPECT_EQ(dut->alucontrolD, 0b0000); // ADD
}

TEST(DecodeStage, LoadLWDecode) {
    // lw x4, 12(x2)
    uint32_t imm  = 12;
    uint32_t rs1  = 2;
    uint32_t rd   = 4;
    uint32_t funct3 = 0b010;
    uint32_t opcode = 0b0000011;

    dut->instrd = (imm << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
    tick();

    EXPECT_EQ(dut->rs1D, 2);
    EXPECT_EQ(dut->rdD,  4);

    EXPECT_EQ(dut->regwriteD, 1);
    EXPECT_EQ(dut->memwriteD, 0);
    EXPECT_EQ(dut->memReadD,  1);   // ✅ NEW (CRITICAL)
    EXPECT_EQ(dut->resultsrcD, 1);
    EXPECT_EQ(dut->alusrcD, 1);
    EXPECT_EQ(dut->extimmD, 12);
}

TEST(DecodeStage, StoreSWDecode) {
    // sw x5, 8(x3)
    dut->instrd = 0b0000000'00101'00011'010'01000'0100011;
    tick();

    EXPECT_EQ(dut->rs1D, 3);
    EXPECT_EQ(dut->rs2D, 5);

    EXPECT_EQ(dut->regwriteD, 0);
    EXPECT_EQ(dut->memwriteD, 1);
    EXPECT_EQ(dut->memReadD,  0);   // ✅ NEW
    EXPECT_EQ(dut->alusrcD,   1);
    EXPECT_EQ(dut->extimmD,   8);
}

TEST(DecodeStage, BranchBEQDecode) {
    // beq x1, x2, +16
    dut->instrd = 0b0000000'00010'00001'000'00010'1100011;
    tick();

    EXPECT_EQ(dut->branchD, 1);
    EXPECT_EQ(dut->jumpD,   0);
    EXPECT_EQ(dut->jalrD,   0);
    EXPECT_EQ(dut->memReadD, 0);   // ✅ NEW
    EXPECT_EQ(dut->alucontrolD, 0b0001); // SUB
}

TEST(DecodeStage, JALDecode) {
    // jal x1, 32
    dut->instrd = 0b00000000001000000000'00001'1101111;
    tick();

    EXPECT_EQ(dut->jumpD, 1);
    EXPECT_EQ(dut->jalrD, 0);
    EXPECT_EQ(dut->resultsrcD, 2);
    EXPECT_EQ(dut->regwriteD, 1);
    EXPECT_EQ(dut->memReadD,  0);   // ✅ NEW
}

TEST(DecodeStage, JALRDecode) {
    // jalr x1, 4(x2)
    dut->instrd = 0b000000000100'00010'000'00001'1100111;
    tick();

    EXPECT_EQ(dut->jumpD, 1);
    EXPECT_EQ(dut->jalrD, 1);
    EXPECT_EQ(dut->regwriteD, 1);
    EXPECT_EQ(dut->alusrcD, 1);
    EXPECT_EQ(dut->memReadD, 0);   // ✅ NEW
}

TEST(DecodeStage, LUICheck) {
    // lui x3, 0x12345
    dut->instrd = 0b00010010001101000101'00011'0110111;
    tick();

    EXPECT_EQ(dut->regwriteD, 1);
    EXPECT_EQ(dut->alusrcD,   1);
    EXPECT_EQ(dut->alucontrolD, 0b1111); // LUI
    EXPECT_EQ(dut->memReadD,  0);        // ✅ NEW
}

// ---------------------- main ---------------------------

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    dut = new VDecode;
    int result = RUN_ALL_TESTS();
    delete dut;

    return result;
}
