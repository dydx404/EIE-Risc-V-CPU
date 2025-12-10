#include <gtest/gtest.h>
#include <verilated.h>
#include "VHazardUnit.h"

static VHazardUnit *dut;

// ---------------- Utility ----------------

static void eval() {
    dut->eval();
}

// ---------------- Tests ------------------

TEST(HazardUnit, NoHazard_Defaults) {
    dut->rs1E = 1;
    dut->rs2E = 2;
    dut->rs1D = 3;
    dut->rs2D = 4;
    dut->rdE  = 5;
    dut->rdM  = 6;
    dut->rdW  = 7;

    dut->regWriteM = 0;
    dut->regWriteW = 0;
    dut->memReadE  = 0;
    dut->pcsrcE    = 0;

    eval();

    EXPECT_EQ(dut->forwardAE, 0);
    EXPECT_EQ(dut->forwardBE, 0);
    EXPECT_EQ(dut->stallF,    0);
    EXPECT_EQ(dut->stallD,    0);
    EXPECT_EQ(dut->flushD,    0);
    EXPECT_EQ(dut->flushE,    0);
}

TEST(HazardUnit, LoadUseHazard_StallAndFlush) {
    // lw  x5, ...
    // add x6, x5, x7   ← uses rdE immediately

    dut->memReadE = 1;
    dut->rdE      = 5;
    dut->rs1D     = 5;   // dependency
    dut->rs2D     = 0;

    dut->pcsrcE   = 0;

    eval();

    EXPECT_EQ(dut->stallF, 1);
    EXPECT_EQ(dut->stallD, 1);
    EXPECT_EQ(dut->flushE, 1);
    EXPECT_EQ(dut->flushD, 0);
}

TEST(HazardUnit, ForwardFromMEM) {
    // EX needs rs1E = x3
    // MEM is writing rdM = x3

    dut->regWriteM = 1;
    dut->rdM       = 3;
    dut->rs1E      = 3;
    dut->rs2E      = 1;

    dut->regWriteW = 0;
    dut->memReadE  = 0;
    dut->pcsrcE    = 0;

    eval();

    EXPECT_EQ(dut->forwardAE, 2);  // 2'b10
    EXPECT_EQ(dut->forwardBE, 0);
}

TEST(HazardUnit, ForwardFromWB) {
    // EX needs rs2E = x4
    // WB is writing rdW = x4

    dut->regWriteM = 0;
    dut->regWriteW = 1;
    dut->rdW       = 4;
    dut->rs1E      = 1;
    dut->rs2E      = 4;

    dut->memReadE  = 0;
    dut->pcsrcE    = 0;

    eval();

    EXPECT_EQ(dut->forwardAE, 0);
    EXPECT_EQ(dut->forwardBE, 1);  // 2'b01
}

TEST(HazardUnit, BranchFlush) {
    dut->pcsrcE = 1;
    dut->memReadE = 0;

    eval();

    EXPECT_EQ(dut->flushD, 1);
    EXPECT_EQ(dut->flushE, 1);
    EXPECT_EQ(dut->stallF, 0);
    EXPECT_EQ(dut->stallD, 0);
}

// ---------------- main -------------------

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    dut = new VHazardUnit;
    int result = RUN_ALL_TESTS();
    delete dut;

    return result;
}
