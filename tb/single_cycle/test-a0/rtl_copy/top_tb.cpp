#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"   // Vbuddy (Virtual Buddy) interface

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    Vtop *top = new Vtop;

    // Waveform setup
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("single.vcd");

    // Vbuddy setup
    if (vbdOpen() != 1) return -1;
    vbdHeader("Single-cycle a0");

    // Reset sequence
    top->clk = 0;
    top->rst = 1;
    top->eval();
    tfp->dump(0);

    // Hold reset for a few cycles
    for (int i = 0; i < 5; i++) {
        top->clk = 1;
        top->eval();
        tfp->dump(2*i + 1);

        top->clk = 0;
        top->eval();
        tfp->dump(2*i + 2);
    }

    top->rst = 0;   // release reset

    const int MAX_CYCLES = 5000;
    int sim_time = 20;   // start time after reset section

    for (int i = 0; i < MAX_CYCLES; i++) {
        // full clock cycle = rising + falling edge
        top->clk = 1;
        top->eval();
        tfp->dump(sim_time++);

        top->clk = 0;
        top->eval();
        tfp->dump(sim_time++);

        int a0 = static_cast<int>(top->a0);

        // Show a0 on the hex displays (lower 16 bits)
        vbdHex(4, (a0 >> 12) & 0xF);
        vbdHex(3, (a0 >>  8) & 0xF);
        vbdHex(2, (a0 >>  4) & 0xF);
        vbdHex(1,  a0        & 0xF);

        // Advance Vbuddy’s internal cycle counter so it redraws
        vbdCycle(i + 1);


        // stop if button pressed
        if (vbdFlag()) break;
    }

    vbdClose();
    tfp->close();
    delete top;
    return 0;
}
