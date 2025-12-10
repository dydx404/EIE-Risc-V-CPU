#include <gtest/gtest.h>
#include <verilated.h>
#include "VEXECUTE_STAGE.h"

static VEXECUTE_STAGE *dut;

static void eval_only() {
    dut->eval();
}

// ---------------------- BASELINE (NO FORWARDING) ----------------------

TEST(EXStage, NoForwardingAdd) {
    dut->forwardAE = 0;
    dut->forwardBE = 0;

    dut->rd1E = 10;
    dut->rd2E = 20;
    dut->extImmE = 0;

    dut->aluSrcE = 0;     // use register
    dut->aluControlE = 0b0000; // ADD

    eval_only();

    EXPECT_EQ(dut->aluResultE, 30);
    EXPECT_EQ(dut->writeDataE, 20);
}

// ---------------------- FORWARD FROM MEM ----------------------

TEST(EXStage, ForwardFromMEM) {
    dut->forwardAE = 2;  // 10
    dut->forwardBE = 2;

    dut->rd1E = 1;
    dut->rd2E = 2;
    dut->aluResultM = 100;

    dut->aluSrcE = 0;
    dut->aluControlE = 0b0000;

    eval_only();

    EXPECT_EQ(dut->aluResultE, 200);
}

// ---------------------- FORWARD FROM WB ----------------------

TEST(EXStage, ForwardFromWB) {
    dut->forwardAE = 1;
    dut->forwardBE = 1;

    dut->rd1E = 1;
    dut->rd2E = 2;
    dut->resultW = 7;

    dut->aluSrcE = 0;
    dut->aluControlE = 0b0000;

    eval_only();

    EXPECT_EQ(dut->aluResultE, 14);
}

// ---------------------- ALUSrc IMMEDIATE ----------------------

TEST(EXStage, ALUSrcImmediate) {
    dut->forwardAE = 0;
    dut->forwardBE = 0;

    dut->rd1E = 10;
    dut->rd2E = 999;      // should be ignored
    dut->extImmE = 5;

    dut->aluSrcE = 1;     // immediate
    dut->aluControlE = 0b0000;

    eval_only();

    EXPECT_EQ(dut->aluResultE, 15);
}

// ---------------------- STORE DATA FORWARDING ----------------------

TEST(EXStage, WriteDataForwarding) {
    dut->forwardBE = 2;  // from MEM
    dut->rd2E = 5;
    dut->aluResultM = 77;

    eval_only();

    EXPECT_EQ(dut->writeDataE, 77);
}

// ---------------------- BRANCH TAKEN (BEQ) ----------------------

TEST(EXStage, BranchTaken) {
    dut->rd1E = 42;
    dut->rd2E = 42;
    dut->extImmE = 16;
    dut->pcE = 100;

    dut->forwardAE = 0;
    dut->forwardBE = 0;

    dut->aluSrcE = 0;
    dut->aluControlE = 0b0001;  // SUB (BEQ compare)
    dut->branchE = 1;

    eval_only();

    EXPECT_EQ(dut->branchTakenE, 1);
    EXPECT_EQ(dut->branchTargetE, 116);
}

// ---------------------- BRANCH NOT TAKEN ----------------------

TEST(EXStage, BranchNotTaken) {
    dut->rd1E = 10;
    dut->rd2E = 20;
    dut->extImmE = 16;
    dut->pcE = 100;

    dut->forwardAE = 0;
    dut->forwardBE = 0;

    dut->aluSrcE = 0;
    dut->aluControlE = 0b0001;  // SUB
    dut->branchE = 1;

    eval_only();

    EXPECT_EQ(dut->branchTakenE, 0);
}

// ---------------------- main ---------------------------

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    dut = new VEXECUTE_STAGE;
    int result = RUN_ALL_TESTS();
    delete dut;

    return result;
}
