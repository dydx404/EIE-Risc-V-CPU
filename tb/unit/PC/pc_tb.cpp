#include "gtest/gtest.h"
#include "VPC.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

VPC *top;
VerilatedVcdC *tfp;
unsigned long tickcount = 0;

// One full clock cycle
void tick()
{
    // Falling edge
    top->clk = 0;
    top->eval();
    tfp->dump(tickcount++);

    // Rising edge (active PC update)
    top->clk = 1;
    top->eval();
    tfp->dump(tickcount++);

    // Back to falling
    top->clk = 0;
    top->eval();
}

class PCTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        top->clk = 0;
        top->pcsrc = 0;
        top->immop = 0;

        // Apply reset ONCE
        top->rst = 1;
        tick();       // PC becomes 0
        top->rst = 0; // DO NOT tick again
    }

    void TearDown() override {}
};

// -----------------------------------------------------
// TEST 1: Reset sets PC to 0
// -----------------------------------------------------
TEST_F(PCTest, ResetPC)
{
    EXPECT_EQ(top->pc, 0u);
}

// -----------------------------------------------------
// TEST 2: Normal increment (pcsrc=0 → PC+4)
// -----------------------------------------------------
TEST_F(PCTest, PCIncrements)
{
    tick();
    EXPECT_EQ(top->pc, 4u);

    tick();
    EXPECT_EQ(top->pc, 8u);

    tick();
    EXPECT_EQ(top->pc, 12u);
}

// -----------------------------------------------------
// TEST 3: Branch forward
// -----------------------------------------------------
TEST_F(PCTest, BranchForward)
{
    top->immop = 16; // jump by +16 bytes
    top->pcsrc = 1;

    tick();
    EXPECT_EQ(top->pc, 16u);

    top->pcsrc = 0;
    tick();
    EXPECT_EQ(top->pc, 20u);
}

// -----------------------------------------------------
// TEST 4: Branch backward (negative immop)
// -----------------------------------------------------
TEST_F(PCTest, BranchBackward)
{
    // Move PC to 12 first
    tick(); // 4
    tick(); // 8
    tick(); // 12

    top->immop = -8; // expect 12 - 8 = 4
    top->pcsrc = 1;

    tick();
    EXPECT_EQ(top->pc, 4u);
}

// -----------------------------------------------------
// TEST 5: Branch overrides +4 increment
// -----------------------------------------------------
TEST_F(PCTest, BranchOverridesIncrement)
{
    top->alu_out = 100;
    top->pcsrc = 2;

    tick();
    EXPECT_EQ(top->pc, 100u);
}

TEST_F(PCTest, MuxGround)
{
    top->alu_out = 100;
    top->pcsrc = 3;

    tick();
    EXPECT_EQ(top->pc, 0u);
}

// -----------------------------------------------------
// MAIN
// -----------------------------------------------------
int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);

    top = new VPC;
    tfp = new VerilatedVcdC;

    Verilated::traceEverOn(true);
    top->trace(tfp, 99);
    tfp->open("pc_wave.vcd");

    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    tfp->close();
    delete top;
    delete tfp;

    return result;
}