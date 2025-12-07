#include <verilated.h>
#include "VPipelineTop.h"
#include <iostream>

static VPipelineTop* dut;

static void tick() {
    dut->clk = 0;
    dut->eval();

    dut->clk = 1;
    dut->eval();
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    dut = new VPipelineTop;

    // ----------------------------
    // Apply reset
    // ----------------------------
    dut->rst = 1;
    tick();
    tick();
    dut->rst = 0;

    std::cout << "[TB] Reset released." << std::endl;

    // ----------------------------
    // Run a few dummy cycles
    // ----------------------------
    for (int i = 0; i < 20; i++) {
        tick();
        std::cout << "[TB] Cycle " << i
                  << " | ResultW = 0x"
                  << std::hex << dut->ResultW
                  << std::dec << std::endl;
    }

    std::cout << "[TB] Simulation finished cleanly." << std::endl;

    delete dut;
    return 0;
}
