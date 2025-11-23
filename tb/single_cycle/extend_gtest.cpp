#include "gtest/gtest.h"
#include "VExtend.h"
#include "verilated.h"

class TestExtend : public ::testing::Test
{
protected:
    VExtend* dut;

    void SetUp() override
    {
        dut = new VExtend();
    }

    void TearDown() override
    {
        delete dut;
    }
};

TEST_F(TestExtend, I-Typep)
{
    uint32_t imm = 0x00100000;
    uint8_t  immsrc = 0b00;

    dut->imm = imm >> 7;
    dut->immsrc = immsrc;
    dut->eval();

    int32_t expected = int32_t(imm) >> 20;
    EXPECT_EQ(dut->immext, uint32_t(expected));
}

TEST_F(TestExtend, I-Typen)
{
    uint32_t imm = 0xFFF00000;
    uint8_t  immsrc = 0b00;

    dut->imm = imm >> 7;
    dut->immsrc = immsrc;
    dut->eval();

    int32_t s = int32_t(imm) >> 20;
    uint32_t expected = uint32_t(s);

    EXPECT_EQ(dut->immext, expected);
}

TEST_F(TestExtend, S-Type)
{
    uint32_t imm = 0x00F00F80;
    uint8_t  immsrc = 0b01;

    dut->imm = imm >> 7;
    dut->immsrc = immsrc;
    dut->eval();

    uint32_t upper = (imm >> 25) & 0x7F;
    uint32_t lower = (imm >> 7)  & 0x1F;
    int32_t combined = int32_t((upper << 5) | lower);
    uint32_t expected = combined;

    EXPECT_EQ(dut->immext, expected);
}

TEST_F(TestExtend, B-Type)
{
    uint32_t imm = 0x80000000;
    uint8_t  immsrc = 0b10;

    dut->imm = imm >> 7;
    dut->immsrc = immsrc;
    dut->eval();

    uint32_t imm31 = (imm >> 31) & 0x1;
    uint32_t imm7 = (imm >> 7)  & 0x1;
    uint32_t imm30_25 = (imm >> 25) & 0x3F;
    uint32_t imm11_8 = (imm >> 8) & 0xF;
    int32_t expected = (imm31 << 12) | (imm7  << 11) | (imm30_25 << 5) | (imm11_8  << 1);

    EXPECT_EQ(dut->immext, expected);
}

TEST_F(TestExtend, J-Type){

    uint32_t imm = 0x0FF12345;
    uint8_t immsrc = 0b11;

    dut->imm = imm >> 7;
    dut->immsrc = immsrc;
    dut->eval();

    uint32_t imm31 = (imm >> 31) & 0x1;
    uint32_t imm19_12 = (imm >> 12) & 0xFF;
    uint32_t imm20 = (imm >> 20) & 0x1; 
    uint32_t imm30_21 = (imm >> 21) & 0x3FF;

    int32_t expected = (imm31 << 20) | (imm19_12 << 12) |(imm20 << 11) | (imm30_21 << 1);

    EXPECT_EQ(dut->immext, uint32_t(expected));
}

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
