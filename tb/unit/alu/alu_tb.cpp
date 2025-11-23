#include "verilated.h"
#include "verilated_vcd_c.h"
#include "VALU.h"

int main(int argc, char **argv, char **env) {

    Verilated::commandArgs(argc, argv);

    VALU* top = new VALU;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("alu.vcd");

    struct Test {
        uint32_t a;
        uint32_t b;
        uint8_t  ctrl;
        uint32_t exp;
        bool     exp_zero;
        const char* name;
    };

    Test tests[] = {
        {10,  5, 0b000, 15, false, "ADD"},
        {10,  5, 0b001, 5,  false, "SUB"},
        {5,   5, 0b001, 0,  true,  "SUB_zero"},
        {0xF0F0F0F0, 0x0FF00FF0, 0b010, 0x00F000F0, false, "AND"},
        {0xF0F0F0F0, 0x0FF00FF0, 0b011, 0xFFF0FFF0, false, "OR"},
        {0xAAAA5555, 0xFFFF0000, 0b100, 0x55555555, false, "XOR"},
        {(uint32_t)-3, 5, 0b101, 1, false, "SLT_lt"},
        {7, (uint32_t)-2, 0b101, 0, true,  "SLT_ge"},
        {1, 4, 0b110, 16, false, "SLL"},
        {0x80, 3, 0b111, 0x10, false, "SRL"},
    };

    int NUM_TESTS = sizeof(tests) / sizeof(tests[0]);
    bool all_ok = true;

    // testss
    for (int i = 0; i < NUM_TESTS; i++) {

        // inputs
        top->aluop1   = tests[i].a;
        top->aluop2   = tests[i].b;
        top->alu_ctrl = tests[i].ctrl;

        // evaluate ALU
        top->eval();

        tfp->dump(i);

        // check result
        uint32_t got_res  = top->aluout;
        bool     got_zero = top->zero;

        bool pass_res  = (got_res == tests[i].exp);
        bool pass_zero = (got_zero == tests[i].exp_zero);

        if (!pass_res || !pass_zero) {
            all_ok = false;
            printf("FAIL %-12s ctrl=%u  a=0x%08X b=0x%08X  got=0x%08X zero=%d  exp=0x%08X zero=%d\n",
                tests[i].name,
                tests[i].ctrl, tests[i].a, tests[i].b,
                got_res, got_zero,
                tests[i].exp, tests[i].exp_zero
            );
        } else {
            printf("PASS %-12s\n", tests[i].name);
        }
    }


    // close
    tfp->close();
    delete tfp;
    delete top;

    if (!all_ok) {
        printf("ALU unit test FAILED\n");
        return 1;
    }

    printf("ALU unit test PASSED\n");
    return 0;
}
