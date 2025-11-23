#include "VExtend.h"
#include "verilated.h"
#include <iostream>
#include <iomanip>

void test(VExtend* dut, uint32_t imm, uint8_t immsrc) {
    dut->imm = imm >> 7;
    dut->immsrc = immsrc;
    dut->eval();

    std::cout << "immsrc = " << int(immsrc)
              << "  imm = 0x" << std::hex << imm
              << "  -> immext = 0x" << std::setw(8) << std::setfill('0') 
              << dut->immext << std::dec << std::endl;
}

int main() {
    const char* argv[] = {""};
    Verilated::commandArgs(1, argv);
    VExtend* dut = new VExtend;

    //I-type
    test(dut, 0xFFF00000, 0b00);
    test(dut, 0x00100000, 0b00);

    //S-type
    test(dut, 0x00F00F80, 0b01);

    //B-type
    test(dut, 0x80000000, 0b10);

    //J-type
    test(dut, 0x40000000, 0b11);

    delete dut;
    return 0;
}