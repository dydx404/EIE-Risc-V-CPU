#include <gtest/gtest.h>
#include <verilated.h>
#include "VWB_STAGE.h"

static VWB_STAGE *dut;

// ---------------------- ALU RESULT WRITEBACK ----------------------

TEST(WBStage, WritebackFromALU) {
    dut->aluResultW = 0x11111111;
    dut->readDataW  = 0x22222222;
    dut->pcPlus4W   = 0x33333333;

    dut->resultSrcW = 0b00;  // ALU
    dut->eval();

    EXPECT_EQ(dut->resultW, 0x11111111);
}

// ---------------------- LOAD RESULT WRITEBACK ----------------------

TEST(WBStage, WritebackFromMemory) {
    dut->aluResultW = 0xAAAAAAAA;
    dut->readDataW  = 0xDEADBEEF;
    dut->pcPlus4W   = 0xBBBBBBBB;

    dut->resultSrcW = 0b01;  // Load
    dut->eval();

    EXPECT_EQ(dut->resultW, 0xDEADBEEF);
}

// ---------------------- PC+4 WRITEBACK (JAL/JALR) ----------------------

TEST(WBStage, WritebackFromPCPlus4) {
    dut->aluResultW = 0xAAAAAAAA;
    dut->readDataW  = 0xBBBBBBBB;
    dut->pcPlus4W   = 0x00001004;

    dut->resultSrcW = 0b10;  // PC+4
    dut->eval();

    EXPECT_EQ(dut->resultW, 0x00001004);
}

// ---------------------- DEFAULT FALLBACK ----------------------

TEST(WBStage, DefaultCaseFallsBackToALU) {
    dut->aluResultW = 0xCAFEBABE;
    dut->readDataW  = 0x00000000;
    dut->pcPlus4W   = 0x00000000;

    dut->resultSrcW = 0b11;  // undefined
    dut->eval();

    EXPECT_EQ(dut->resultW, 0xCAFEBABE);
}

// ---------------------- main ---------------------------

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    dut = new VWB_STAGE;
    int result = RUN_ALL_TESTS();
    delete dut;

    return result;
}
