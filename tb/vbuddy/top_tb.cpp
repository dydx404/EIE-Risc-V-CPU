#include <iostream>
#include <iomanip>
#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

#define MAX_SIM_CYC 2000000

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

int main(int argc, char **argv, char **env)
{
    int simcyc;
    int tick;

    char prog = argv[argc - 1][0];

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vtop *top = new Vtop;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("CPU.vcd");

    // init Vbuddy
    if (vbdOpen() != 1)
    {
        return (-1);
    }
    vbdHeader("Single Cycle");
    vbdSetMode(1);

    // intialise
    top->clk = 1;
    top->rst = 1;
    step_cycle(top);
    top->rst = 0;

    // run simulation for MAX_SIM_CYC clock cycles
    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
    {
        // dump variables into VCD file and toggle clock
        for (tick = 0; tick < 2; tick++)
        {
            tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        // Test data
        if (simcyc > 123705)
        {
            vbdPlot(top->a0, 0, 255);
            vbdBar(top->a0 & 0xFF);
            vbdCycle(simcyc);
        }

        // either simulation finished, or 'q' is pressed
        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}