#include <gtest/gtest.h>
#include "VExtend.h"
#include "verilated.h"

// -------------------------------------------------------------
// Reference model for "packed" instruction: instr[31:7] only.
// DUT receives Immediate = instr[31:7], NOT the full instruction.
// -------------------------------------------------------------
uint32_t ref_extend_packed(uint32_t instr, uint32_t src) {
    uint32_t imm = instr >> 7;  // packed bits

    switch (src) {

        // ------------------ I-type ------------------
        // imm[11:0] = instr[31:20] = imm[24:13]
        case 0: {
            int32_t raw = (int32_t)(imm >> 13);
            return raw;
        }

        // ------------------ S-type ------------------
        // imm[11:5] = instr[31:25] = imm[24:18]
        // imm[4:0]  = instr[11:7]  = imm[4:0]
        case 1: {
            int32_t raw =
                ((imm >> 18) << 5) |
                (imm & 0x1F);

            if (raw & 0x800) raw |= 0xFFFFF000;
            return raw;
        }

        // ------------------ B-type ------------------
        // imm[12]   = instr[31]    = imm[24]
        // imm[11]   = instr[7]     = imm[0]
        // imm[10:5] = instr[30:25] = imm[23:18]
        // imm[4:1]  = instr[11:8]  = imm[4:1]
        // imm[0]    = 0
        case 2: {
            uint32_t bit31       = (imm >> 24) & 1;
            uint32_t bit7        = (imm >> 0)  & 1;
            uint32_t bits30_25   = (imm >> 18) & 0x3F;
            uint32_t bits11_8    = (imm >> 1)  & 0xF;

            int32_t raw =
                (bit31 << 12) |
                (bit7  << 11) |
                (bits30_25 << 5) |
                (bits11_8  << 1);

            if (raw & 0x1000)
                raw |= 0xFFFFE000;

            return raw;
        }

// ------------------ J-type (packed instr[31:7]) ------------------
case 3: {
    uint32_t imm20     = (imm >> 24) & 1;        // instr[31]
    uint32_t imm19_12  = (imm >> 5)  & 0xFF;     // instr[19:12]
    uint32_t imm11     = (imm >> 13) & 1;        // instr[20]
    uint32_t imm10_1   = (imm >> 14) & 0x3FF;    // instr[30:21]

    int32_t raw =
        (imm20     << 20) |
        (imm19_12  << 12) |
        (imm11     << 11) |
        (imm10_1   << 1);

    if (raw & 0x00100000)
        raw |= 0xFFE00000;

    return raw;
}

        // ------------------ U-type ------------------
        // imm[31:12] = instr[31:12] = imm[24:5]
        case 4: {
            uint32_t upper20 = (imm >> 5);
            return upper20 << 12;
        }

        default:
            return 0;
    }
}

// -------------------------------------------------------------
// GoogleTest Fixture
// -------------------------------------------------------------
class ExtendTest : public ::testing::Test {
protected:
    VExtend* dut = nullptr;

    void SetUp() override { dut = new VExtend; }
    void TearDown() override { delete dut; }
};

// -------------------------------------------------------------
// Random Tests
// -------------------------------------------------------------
TEST_F(ExtendTest, Randomized) {
    for (int i = 0; i < 500; i++) {
        uint32_t instr = (rand() & 0xFFFFF080);  // valid bits
        uint32_t src   = rand() % 5;

        dut->Immediate = instr >> 7;
        dut->ImmSrc    = src;
        dut->eval();

        uint32_t ref = ref_extend_packed(instr, src);

        ASSERT_EQ(dut->ImmExt, ref)
            << "instr=0x" << std::hex << instr
            << " src=" << std::dec << src;
    }
}

// -------------------------------------------------------------
// Edge Cases
// -------------------------------------------------------------
TEST_F(ExtendTest, EdgeCases) {
    uint32_t instr = 0xABCDE000;

    dut->Immediate = instr >> 7;
    dut->ImmSrc    = 4;
    dut->eval();

    ASSERT_EQ(dut->ImmExt, ref_extend_packed(instr, 4));
}
