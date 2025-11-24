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
        uint32_t    a;
        uint32_t    b;
        uint8_t     ctrl;      // 4-bit ALU control
        uint32_t    exp;       // expected aluout
        bool        exp_zero;  // expected zero flag
        const char* name;
    };

    Test tests[] = {
        // --- ADD / SUB ---
        {10,          5,         0b0000, 15,         false, "ADD"},
        {0xFFFFFFFFu, 1,         0b0000, 0,          true,  "ADD_wrap_zero"},
        {10,          5,         0b0001, 5,          false, "SUB"},
        {5,           5,         0b0001, 0,          true,  "SUB_zero"},

        // --- AND / OR / XOR ---
        {0xF0F0F0F0,  0x0FF00FF0, 0b0010, 0x00F000F0, false, "AND"},
        {0xF0F0F0F0,  0x0FF00FF0, 0b0011, 0xFFF0FFF0, false, "OR"},
        {0xAAAA5555,  0xFFFF0000, 0b0100, 0x55555555, false, "XOR"},

        // --- SLT (signed) ---
        {(uint32_t)-3,           5,        0b0101, 1, false, "SLT_signed_lt"},
        {7,            (uint32_t)-2,       0b0101, 0, true,  "SLT_signed_ge"},

        // --- SLTU (unsigned) ---
        {3,            5,         0b0110, 1, false, "SLTU_lt"},
        {0xFFFFFFFFu,  0,         0b0110, 0, true,  "SLTU_ge"},

        // --- SLL ---
        {1,            4,         0b0111, 16,        false, "SLL"},
        // shift amount > 31, should be masked by [4:0] (36 -> 4)
        {1,            36,        0b0111, 16,        false, "SLL_shamt_mask"},

        // --- SRL ---
        {0x00000080,   3,         0b1000, 0x00000010, false, "SRL"},
        {0xFFFFFFFFu,  4,         0b1000, 0x0FFFFFFF, false, "SRL_all_ones"},

        // --- SRA ---
        {0xF0000000u,  4,         0b1001, 0xFF000000, false, "SRA_neg_shift"},
        {0x80000000u,  1,         0b1001, 0xC0000000, false, "SRA_sign_extend"},
        {0x00000000u,  5,         0b1001, 0x00000000, true,  "SRA_zero"},
    };

    int NUM_TESTS = sizeof(tests) / sizeof(tests[0]);
    bool all_ok = true;

    for (int i = 0; i < NUM_TESTS; i++) {
        // set inputs
        top->aluop1   = tests[i].a;
        top->aluop2   = tests[i].b;
        top->alu_ctrl = tests[i].ctrl;

        // evaluate ALU
        top->eval();

        // simple time = i for waveform
        tfp->dump(i);

        // check result
        uint32_t got_res  = top->aluout;
        bool     got_zero = top->zero;

        bool pass_res  = (got_res == tests[i].exp);
        bool pass_zero = (got_zero == tests[i].exp_zero);

        if (!pass_res || !pass_zero) {
            all_ok = false;
            printf(
                "FAIL %-16s ctrl=0x%X  a=0x%08X b=0x%08X  "
                "got=0x%08X zero=%d  exp=0x%08X zero=%d\n",
                tests[i].name,
                tests[i].ctrl, tests[i].a, tests[i].b,
                got_res, got_zero,
                tests[i].exp, tests[i].exp_zero
            );
        } else {
            printf("PASS %-16s\n", tests[i].name);
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
