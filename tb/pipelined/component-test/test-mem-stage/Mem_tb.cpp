#include <gtest/gtest.h>
#include <verilated.h>
#include "VMEM_STAGE.h"

static VMEM_STAGE *dut;

static void tick() {
    dut->clk = 0; dut->eval();
    dut->clk = 1; dut->eval();
}

// ---------------------- SW → LW ----------------------

TEST(MEMStage, StoreWordLoadWord) {
    uint32_t addr = 0x100;   // safe test address
    uint32_t data = 0xDEADBEEF;

    // STORE WORD
    dut->aluResultM = addr;
    dut->writeDataM = data;
    dut->memWriteM  = 1;
    dut->memReadM   = 0;
    dut->addressingModeM = 0b010; // word

    tick();

    // LOAD WORD
    dut->memWriteM = 0;
    dut->memReadM  = 1;

    dut->eval();

    EXPECT_EQ(dut->readDataM, data);
}

// ---------------------- SB → LB ----------------------

TEST(MEMStage, StoreByteLoadByteSigned) {
    uint32_t addr = 0x120;
    uint32_t data = 0x00000080;  // -128 signed

    // SB
    dut->aluResultM = addr;
    dut->writeDataM = data;
    dut->memWriteM  = 1;
    dut->memReadM   = 0;
    dut->addressingModeM = 0b000; // byte, signed

    tick();

    // LB
    dut->memWriteM = 0;
    dut->memReadM  = 1;

    dut->eval();

    EXPECT_EQ((int32_t)dut->readDataM, -128);
}

// ---------------------- SB → LBU ----------------------

TEST(MEMStage, StoreByteLoadByteUnsigned) {
    uint32_t addr = 0x140;
    uint32_t data = 0x000000FF;  // 255

    // SB
    dut->aluResultM = addr;
    dut->writeDataM = data;
    dut->memWriteM  = 1;
    dut->memReadM   = 0;
    dut->addressingModeM = 0b100; // byte, unsigned

    tick();

    // LBU
    dut->memWriteM = 0;
    dut->memReadM  = 1;

    dut->eval();

    EXPECT_EQ(dut->readDataM, 255);
}

// ---------------------- SH → LH ----------------------

TEST(MEMStage, StoreHalfLoadHalfSigned) {
    uint32_t addr = 0x160;
    uint32_t data = 0x00008000; // -32768 signed

    // SH
    dut->aluResultM = addr;
    dut->writeDataM = data;
    dut->memWriteM  = 1;
    dut->memReadM   = 0;
    dut->addressingModeM = 0b001; // half, signed

    tick();

    // LH
    dut->memWriteM = 0;
    dut->memReadM  = 1;

    dut->eval();

    EXPECT_EQ((int32_t)dut->readDataM, -32768);
}

// ---------------------- SH → LHU ----------------------

TEST(MEMStage, StoreHalfLoadHalfUnsigned) {
    uint32_t addr = 0x180;
    uint32_t data = 0x0000FFFF; // 65535

    // SH
    dut->aluResultM = addr;
    dut->writeDataM = data;
    dut->memWriteM  = 1;
    dut->memReadM   = 0;
    dut->addressingModeM = 0b101; // half, unsigned

    tick();

    // LHU
    dut->memWriteM = 0;
    dut->memReadM  = 1;

    dut->eval();

    EXPECT_EQ(dut->readDataM, 65535);
}

// ---------------------- READ DISABLED ----------------------

TEST(MEMStage, MemReadDisabled) {
    dut->memReadM = 0;
    dut->memWriteM = 0;
    dut->eval();

    EXPECT_EQ(dut->readDataM, 0);
}

// ---------------------- main ---------------------------

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    dut = new VMEM_STAGE;
    int result = RUN_ALL_TESTS();
    delete dut;

    return result;
}
