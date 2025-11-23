#include "verilated.h"
#include "verilated_vcd_c.h"
#include "VRegFile.h"

// Simple helper for pass/fail messages
bool check(bool cond, const char* msg) {
    if (!cond) {
        printf("FAIL: %s\n", msg);
        return false;
    } else {
        printf("PASS: %s\n", msg);
        return true;
    }
}

int main(int argc, char **argv, char **env) {
    Verilated::commandArgs(argc, argv);

    VRegFile* top = new VRegFile;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("regfile.vcd");

    vluint64_t time = 0;
    auto tick = [&]() {
        top->clk = 0;
        top->eval();
        tfp->dump(time++);

        top->clk = 1;
        top->eval();
        tfp->dump(time++);
    };

    bool all_ok = true;

    // inital val
    top->clk = 0;
    top->we  = 0;
    top->rs1 = 0;
    top->rs2 = 0;
    top->rd  = 0;
    top->wd  = 0;
    top->eval();

    // x0 should always read 0
    all_ok &= check(top->rd1 == 0, "x0 read as 0 (initial)");

    // Test 1: normal write to x5, then read back
    top->we  = 1;
    top->rd  = 5;
    top->wd  = 42;
    top->rs1 = 5;
    top->rs2 = 0;
    top->eval();   // combinational read should already see bypassed value

    all_ok &= check(top->rd1 == 42, "write-first bypass on rs1 (x5)");
    all_ok &= check(top->rd2 == 0,  "x0 still 0 on rs2");

    // commit write on rising edge
    tick();

    // now disable write and read back x5
    top->we  = 0;
    top->rs1 = 5;
    top->rs2 = 0;
    top->eval();

    all_ok &= check(top->rd1 == 42, "x5 holds value after clock");

    // Test 2: write to x10 (a0) and check a0 output
    top->we  = 1;
    top->rd  = 10;        // x10 / a0
    top->wd  = 99;
    top->rs1 = 10;        // read same reg to see bypass
    top->eval();

    all_ok &= check(top->rd1 == 99, "write-first bypass on rs1 (x10)");
    tick();                  // commit write

    top->we  = 0;
    top->rs1 = 10;
    top->eval();

    all_ok &= check(top->rd1 == 99, "x10 holds value after clock");
    all_ok &= check(top->a0  == 99, "a0 output reflects x10");

    // Test 3: attempt to write x0 (should be ignored)
    top->we  = 1;
    top->rd  = 0;         // x0
    top->wd  = 123;
    top->rs1 = 0;
    top->eval();

    all_ok &= check(top->rd1 == 0, "x0 read remains 0 during write attempt");
    tick();

    top->we  = 0;
    top->rs1 = 0;
    top->eval();

    all_ok &= check(top->rd1 == 0, "x0 stays 0 after write attempt");

    // cleanup
    tfp->close();
    delete tfp;
    delete top;

    if (!all_ok) {
        printf("RegFile unit test FAILED\n");
        return 1;
    }

    printf("RegFile unit test PASSED\n");
    return 0;
}
