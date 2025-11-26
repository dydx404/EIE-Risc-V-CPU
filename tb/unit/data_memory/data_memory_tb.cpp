#include <gtest/gtest.h>
#include "VDataMemory.h"
#include "verilated.h"

// Handle Verilator time
static vluint64_t main_time = 0;
double sc_time_stamp() { return main_time; }

// Helper to tick the clock
void tick(VDataMemory* mem) {
    mem->clk_i = 0;
    mem->eval();
    main_time++;

    mem->clk_i = 1;
    mem->eval();
    main_time++;
}

class DataMemoryTest : public ::testing::Test {
protected:
    VDataMemory* mem;

    void SetUp() override {
        mem = new VDataMemory;
        main_time = 0;

        // default inputs
        mem->clk_i = 0;
        mem->mem_read_i = 0;
        mem->mem_write_i = 0;
        mem->addr_i = 0;
        mem->write_data_i = 0;
        mem->access_ctrl_i = 0;
        mem->eval();
    }

    void TearDown() override {
        delete mem;
    }

    // Write one byte (SB)
    void write_byte(uint32_t addr, int8_t value) {
        mem->addr_i = addr;
        mem->write_data_i = value;
        mem->access_ctrl_i = 0b000;    // size=byte, signed/zero doesn't matter
        mem->mem_write_i = 1;
        tick(mem);
        mem->mem_write_i = 0;
        mem->eval();
    }

    // Read byte using LB
    int32_t read_byte_signed(uint32_t addr) {
        mem->addr_i = addr;
        mem->access_ctrl_i = 0b000;   // LB
        mem->mem_read_i = 1;
        mem->eval();
        return mem->read_data_o;
    }

    // Read byte using LBU
    uint32_t read_byte_unsigned(uint32_t addr) {
        mem->addr_i = addr;
        mem->access_ctrl_i = 0b100;    // LBU: zero_extend=1, size=00
        mem->mem_read_i = 1;
        mem->eval();
        return mem->read_data_o;
    }
};

// ================================================================
// BASIC TEST: SB + LB/LBU verify sign/zero extension
// ================================================================
TEST_F(DataMemoryTest, ByteLoadStore) {
    // Write -1 = 0xFF
    write_byte(0, -1);

    // Read LB -> should be -1 (0xFFFFFFFF)
    EXPECT_EQ(read_byte_signed(0), 0xFFFFFFFF);

    // Read LBU -> should be 255 (0x000000FF)
    EXPECT_EQ(read_byte_unsigned(0), 0x000000FF);
}

// ================================================================
// Halfword store / load
// ================================================================
TEST_F(DataMemoryTest, HalfwordLoadStore) {
    // SH: write 0xABCD at address 4
    mem->addr_i = 4;
    mem->write_data_i = 0x0000ABCD;
    mem->access_ctrl_i = 0b001;     // SH: size=01
    mem->mem_write_i = 1;
    tick(mem);
    mem->mem_write_i = 0;

    // LH (signed load)
    mem->addr_i = 4;
    mem->access_ctrl_i = 0b001;  // LH: size=01, zero_extend=0
    mem->mem_read_i = 1;
    mem->eval();
    EXPECT_EQ(mem->read_data_o, (int32_t)0xFFFFABCD);   // negative

    // LHU (zero-extended)
    mem->access_ctrl_i = 0b101;  // LHU: zero_extend=1, size=01
    mem->eval();
    EXPECT_EQ(mem->read_data_o, 0x0000ABCDu);
}

// ================================================================
// Full word (32-bit store/load)
// ================================================================
TEST_F(DataMemoryTest, WordLoadStore) {
    uint32_t value = 0x1234ABCD;

    // SW
    mem->addr_i = 100;
    mem->write_data_i = value;
    mem->access_ctrl_i = 0b010;  // SW size=10
    mem->mem_write_i = 1;
    tick(mem);
    mem->mem_write_i = 0;

    // LW
    mem->addr_i = 100;
    mem->access_ctrl_i = 0b010;  // LW
    mem->mem_read_i = 1;
    mem->eval();
    EXPECT_EQ(mem->read_data_o, value);
}

// ================================================================
// Byte addressing offsets within a word
// ================================================================
TEST_F(DataMemoryTest, ByteOffsets) {
    // Create 4-byte word: [11 22 33 44]
    write_byte(200, 0x11);
    write_byte(201, 0x22);
    write_byte(202, 0x33);
    write_byte(203, 0x44);

    EXPECT_EQ(read_byte_unsigned(200), 0x11);
    EXPECT_EQ(read_byte_unsigned(201), 0x22);
    EXPECT_EQ(read_byte_unsigned(202), 0x33);
    EXPECT_EQ(read_byte_unsigned(203), 0x44);
}

// ================================================================
// SIMULATION Preload Test (only valid when using sample.mem test)
// ================================================================
TEST_F(DataMemoryTest, PreloadMemory) {
#ifdef SIMULATION
    // If sample.mem contains:
    // 55
    // AA
    // FF
    // ...
    // then after preload at 0x10000:
    // ram_array[0x10000] = 0x55, etc.

    mem->addr_i = 0x10000;
    mem->access_ctrl_i = 0;   // LB (signed ok)
    mem->mem_read_i = 1;
    mem->eval();

    // Example expected value (adjust based on sample.mem content)
    EXPECT_EQ(mem->read_data_o & 0xFF, 0x55);
#endif
}
