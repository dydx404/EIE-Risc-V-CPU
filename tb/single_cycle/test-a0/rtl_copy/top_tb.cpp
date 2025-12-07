#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    Vtop *top = new Vtop;

    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("single.vcd");

    // Vbuddy setup
    if (vbdOpen() != 1) return -1;
    vbdHeader("Single-cycle a0");

    // reset
    top->clk = 0;
    top->rst = 1;
    top->eval();

    for (int i = 0; i < 5; i++) {
        top->clk = !top->clk;
        top->eval();
        tfp->dump(2 * i);
    }
    top->rst = 0;

    const int MAX_CYCLES = 5000;

    for (int i = 0; i < MAX_CYCLES; i++) {
        // full clock cycle
        for (int tick = 0; tick < 2; tick++) {
            top->clk = !top->clk;
            top->eval();
            tfp->dump(10 * i + tick);
        }

        int a0 = (int)top->a0;

        // old: hex display
        // vbdHex(4, (a0 >> 12) & 0xF);
        // vbdHex(3, (a0 >>  8) & 0xF);
        // vbdHex(2, (a0 >>  4) & 0xF);
        // vbdHex(1,  a0        & 0xF);

        // new: waveform plot of a0
        // clamp to 0–255 so it fits on the graph
        vbdPlot(a0 & 0xFF, 0, 255);

        vbdCycle(i + 1);

        if ((i % 1000) == 0) {
            printf("cycle %5d  a0 = 0x%08x\n", i, a0);
        }

        // stop if button pressed
        if (vbdFlag()) break;
    }

    vbdClose();
    tfp->close();
    delete top;
    return 0;
}
