#include <iostream>
#include <iomanip>
#include "Vtop.h"
#include "verilated.h"

#ifdef TRACE
#include "verilated_vcd_c.h"
#endif

#include "vbuddy.cpp"

// ---------------------------------------------------------
// Step one cycle (posedge + negedge)
// ---------------------------------------------------------
void step_cycle(Vtop *dut)
{
    // rising edge
    dut->clk = 1;
    dut->eval();

    // falling edge
    dut->clk = 0;
    dut->eval();
}

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);
    Vtop *dut = new Vtop;

#ifdef TRACE
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    dut->trace(tfp, 99);
    tfp->open("top.vcd");
#endif

    // -----------------------------------------------------
    // VBuddy init
    // -----------------------------------------------------
    if (vbdOpen() != 1) return -1;
    vbdHeader("PDF: a0 waveform");
    // 0 = normal toggle mode; fine for plotting
    vbdSetMode(0);

    // -----------------------------------------------------
    // Reset
    // -----------------------------------------------------
    dut->clk = 0;
    dut->rst = 1;

    // one reset cycle
    step_cycle(dut);

    dut->rst = 0;

    // -----------------------------------------------------
    // Run CPU + plot a0 every cycle
    // -----------------------------------------------------
    const int NUM_CYCLES = 200000;

    for (int cycle = 0; cycle < NUM_CYCLES; cycle++)
    {
        step_cycle(dut);

        // current a0 value (pdf bin count; should be 0–255)
        int a0 = static_cast<int>(dut->a0 & 0xFF);

        // send a0 to VBuddy plot (y-range 0..255)
        vbdPlot(a0, 0, 255);
        vbdCycle(cycle + 1);

        // debug print to console every 64 cycles
        if ((cycle % 64) == 0)
        {
            std::cout << "Cycle " << std::setw(6) << cycle
                      << " | a0 = " << std::setw(3) << a0
                      << std::endl;
        }

#ifdef TRACE
        tfp->dump(cycle);
#endif

        // allow early stop from VBuddy button
        if (vbdFlag()) break;
        if (Verilated::gotFinish()) break;
    }

#ifdef TRACE
    tfp->close();
#endif

    vbdClose();
    delete dut;
    return 0;
}
