#include <gtest/gtest.h>
#include "VDataMemory.h"
#include "verilated.h"

class DataMemoryTest : public ::testing::Test {
protected:
    VDataMemory* mem;

    void SetUp() override {
        mem = new VDataMemory();

        // Initial stabilisation (prevents unintended writes at time 0)
        mem->clk = 0;
        mem->mem_write = 0;
        mem->addr = 0;
        mem->write_data = 0;
        mem->eval();
    }

    void TearDown() override {
        delete mem;
    }

    // single rising-edge cycle
    void tick() {
        mem->clk = 1;
        mem->eval();
        mem->clk = 0;
        mem->eval();
    }
};

// -----------------------------------------------------------------------------
// Test: synchronous write + asynchronous read
// -----------------------------------------------------------------------------
TEST_F(DataMemoryTest, WriteAndReadBack) {

    // Apply inputs BEFORE the write clock edge
    mem->addr = 5;
    mem->write_data = 0xCAFEBABE;
    mem->mem_write = 1;
    mem->eval();    // <-- stabilise BEFORE tick

    tick();         // Write happens here

    // Disable write and read back the value
    mem->mem_write = 0;
    mem->addr = 5;
    mem->eval();

    EXPECT_EQ(mem->read_data, 0xCAFEBABE);
}

// -----------------------------------------------------------------------------
// Test: write-enable must block writes when disabled
// -----------------------------------------------------------------------------
TEST_F(DataMemoryTest, WriteEnableBlocksWrites) {

    // First, write a known value
    mem->addr = 3;
    mem->write_data = 0x11111111;
    mem->mem_write = 1;
    mem->eval();
    tick();

    // Attempt to overwrite WITHOUT mem_write
    mem->addr = 3;
    mem->write_data = 0x22222222;
    mem->mem_write = 0;
    mem->eval();
    tick();     // No write should occur

    // Check contents remain unchanged
    mem->addr = 3;
    mem->eval();
    EXPECT_EQ(mem->read_data, 0x11111111);
}

// -----------------------------------------------------------------------------
// Test: multiple word writes and reads
// -----------------------------------------------------------------------------
TEST_F(DataMemoryTest, MultipleAddresses) {

    // Write four increasing values
    for (int i = 0; i < 4; i++) {
        mem->addr = i;
        mem->write_data = 0x12340000 + i;
        mem->mem_write = 1;
        mem->eval();
        tick();
    }

    mem->mem_write = 0;

    // Read and verify each location
    for (int i = 0; i < 4; i++) {
        mem->addr = i;
        mem->eval();
        EXPECT_EQ(mem->read_data, 0x12340000 + i);
    }
}
