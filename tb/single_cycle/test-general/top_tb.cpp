#include <iostream>
#include <iomanip>
#include "Vtop.h"
#include "verilated.h"

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);
    Vtop *dut = new Vtop;

    dut->clk = 0;
    dut->rst = 1;

    // reset for a few cycles
    for (int i = 0; i < 5; i++)
    {
        dut->clk ^= 1;
        dut->eval();
    }

    dut->rst = 0;

    // run CPU for N cycles
    for (int cycle = 0; cycle < 500; cycle++)
    {
        for (int i = 0; i < 2; i++)
        {
            dut->clk ^= 1;
            dut->eval();
        }
        std::cout << "Cycle " << cycle
                  << " | a0 = " << dut->a0
                  << std::endl;
    }

    delete dut;
    return 0;
}
