#include <gtest/gtest.h>
#include "VControlUnit.h"
#include "verilated.h"

class TestControlUnit : public ::testing::Test {
protected:
    VControlUnit* dut;

    void SetUp() override {
        dut = new VControlUnit();
    }

    void TearDown() override {
        delete dut;
    }

    void eval(uint32_t opcode, uint32_t funct3, uint32_t funct7, uint32_t zero) {
        dut->opcode = opcode;
        dut->funct3 = funct3;
        dut->funct7 = funct7;
        dut->zero   = zero;
        dut->eval();
    }
};


//addi
TEST_F(TestControlUnit, ADDI) {
    eval(0b0010011, 0b000, 0, 0);

    EXPECT_EQ(dut->regwrite, 1);
    EXPECT_EQ(dut->alusrc,   1);
    EXPECT_EQ(dut->immsrc,   0b00);
    EXPECT_EQ(dut->alucontrol, 0b0000);
    EXPECT_EQ(dut->pcsrc,    0b00);
    EXPECT_EQ(dut->resultsrc, 0b00);
    EXPECT_EQ(dut->memwrite, 0);
    EXPECT_EQ(dut->addressingcontrol, 0);
}

//bne, with branch
TEST_F(TestControlUnit, BNE_Taken) {
    eval(0b1100011, 0b001, 0, 0);

    EXPECT_EQ(dut->regwrite, 0);
    EXPECT_EQ(dut->alusrc,   0);
    EXPECT_EQ(dut->immsrc,   0b010);
    EXPECT_EQ(dut->alucontrol, 0b0001);
    EXPECT_EQ(dut->pcsrc,    0b01);  //when zero = 0
    EXPECT_EQ(dut->resultsrc, 0b00);
    EXPECT_EQ(dut->memwrite, 0);
    EXPECT_EQ(dut->addressingcontrol, 0b000);
}
 //bne, no branch
TEST_F(TestControlUnit, BNE_NotTaken) {
    eval(0b1100011, 0b001, 0, 1);

    EXPECT_EQ(dut->pcsrc, 0); //when zero = 1
}


//add
TEST_F(TestControlUnit, ADD_RType) {
    eval(0b0110011, 0b000, 0b0000000, 0);

    EXPECT_EQ(dut->regwrite, 1);
    EXPECT_EQ(dut->alusrc,   0);
    EXPECT_EQ(dut->immsrc,   0b000);
    EXPECT_EQ(dut->alucontrol, 0b0000);
    EXPECT_EQ(dut->pcsrc,    0b00);
    EXPECT_EQ(dut->resultsrc, 0b00);
    EXPECT_EQ(dut->memwrite, 0);
    EXPECT_EQ(dut->addressingcontrol,0b000);
}


//lbu
TEST_F(TestControlUnit, LBU) {
    eval(0b0000011, 0b100, 0, 0);

    EXPECT_EQ(dut->regwrite, 1);
    EXPECT_EQ(dut->alusrc,   1);
    EXPECT_EQ(dut->immsrc,   0b000);
    EXPECT_EQ(dut->alucontrol, 0b0000);
    EXPECT_EQ(dut->pcsrc,    0b00);
    EXPECT_EQ(dut->resultsrc, 0b01);
    EXPECT_EQ(dut->memwrite, 0);
    EXPECT_EQ(dut->addressingcontrol, 0b000);
}

//lb
TEST_F(TestControlUnit, LB) {
    eval(0b0000011, 0b000, 0, 0);

    EXPECT_EQ(dut->regwrite, 1);
    EXPECT_EQ(dut->alusrc,   1);
    EXPECT_EQ(dut->immsrc,   0b000);
    EXPECT_EQ(dut->alucontrol, 0b0000);
    EXPECT_EQ(dut->pcsrc,    0b00);
    EXPECT_EQ(dut->resultsrc, 0b01);
    EXPECT_EQ(dut->memwrite, 0);
    EXPECT_EQ(dut->addressingcontrol, 0b000);
}

//sb
TEST_F(TestControlUnit, SB) {
    eval(0b0100011, 0b000, 0, 0);

    EXPECT_EQ(dut->regwrite, 0);
    EXPECT_EQ(dut->alusrc,   1);
    EXPECT_EQ(dut->immsrc,   0b001);
    EXPECT_EQ(dut->alucontrol, 0b0000);
    EXPECT_EQ(dut->pcsrc,    0b00);
    EXPECT_EQ(dut->resultsrc, 0b00);
    EXPECT_EQ(dut->memwrite, 1);
    EXPECT_EQ(dut->addressingcontrol,0b000);
}


//jal
TEST_F(TestControlUnit, JAL) {
    eval(0b1101111, 0, 0, 0);

    EXPECT_EQ(dut->regwrite, 1);
    EXPECT_EQ(dut->alusrc,   0);
    EXPECT_EQ(dut->immsrc,   0b100);
    EXPECT_EQ(dut->alucontrol, 0b0000);
    EXPECT_EQ(dut->pcsrc,    0b01);
    EXPECT_EQ(dut->resultsrc, 0b10);
    EXPECT_EQ(dut->memwrite, 0);
    EXPECT_EQ(dut->addressingcontrol,0b000);
}


//jalr
TEST_F(TestControlUnit, JALR) {
    eval(0b1100111, 0, 0, 0);

    EXPECT_EQ(dut->regwrite, 0);
    EXPECT_EQ(dut->alusrc,   0);
    EXPECT_EQ(dut->immsrc,   0b000);
    EXPECT_EQ(dut->alucontrol, 0b0000);
    EXPECT_EQ(dut->pcsrc,    0b10);
    EXPECT_EQ(dut->resultsrc, 0b00);
    EXPECT_EQ(dut->memwrite, 0);
    EXPECT_EQ(dut->addressingcontrol,0b000);
}

//lui
TEST_F(TestControlUnit, LUI) {
    eval(0b0110111, 0, 0, 0);

    EXPECT_EQ(dut->regwrite, 0);
    EXPECT_EQ(dut->alusrc,   0);
    EXPECT_EQ(dut->immsrc,   0b011);
    EXPECT_EQ(dut->alucontrol, 0b0000);
    EXPECT_EQ(dut->pcsrc,    0b00);
    EXPECT_EQ(dut->resultsrc, 0b00);
    EXPECT_EQ(dut->memwrite, 0);
    EXPECT_EQ(dut->addressingcontrol, 0b000);
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}