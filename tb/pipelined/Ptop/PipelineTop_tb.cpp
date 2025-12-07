#include <iostream>
#include <iomanip>
#include "VPipelineTop.h"
#include "verilated.h"

#ifdef TRACE
#include "verilated_vcd_c.h"
#endif

// ---------------------------------------------------------
// Step one cycle (posedge + negedge)
// ---------------------------------------------------------
void step_cycle(VPipelineTop *dut)
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
    VPipelineTop *dut = new VPipelineTop;

#ifdef TRACE
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    dut->trace(tfp, 99);
    tfp->open("top.vcd");
#endif

    // -----------------------------------------------------
    // Initialize
    // -----------------------------------------------------
    dut->clk = 0;
    dut->regaddr = 10; // x10 = a0
    dut->rst = 1;

    // Pulse reset for exactly ONE cycle (correct behavior)
    step_cycle(dut);

    dut->rst = 0;

    // -----------------------------------------------------
    // Run CPU
    // -----------------------------------------------------
    const int NUM_CYCLES = 800000;

    for (int cycle = 0; cycle < NUM_CYCLES; cycle++)
    {
        step_cycle(dut);

        // Print CPU observable register (x10 = a0)
        std::cout << "Cycle " << std::setw(3) << cycle
                  << " | TestReg = 0x" << std::hex << std::setw(8)
                  << std::setfill('0') << dut->regdata
                  << std::dec << std::endl;

#ifdef TRACE
        tfp->dump(cycle);
#endif
    }

#ifdef TRACE
    tfp->close();
#endif

    delete dut;
    return 0;
}
