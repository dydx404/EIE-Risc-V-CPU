#include "VControlUnit.h"
#include "verilated.h"
#include <iostream>

void run_test(VControlUnit* dut,
              uint32_t opcode,
              uint32_t funct3,
              uint32_t funct7,
              uint32_t zero)
{
    dut->opcode = opcode;
    dut->funct3 = funct3;
    dut->funct7 = funct7;
    dut->zero   = zero;

    dut->eval();

    std::cout << "inputs:"
              << " opcode = "  << std::hex << opcode
              << " funct3 = "  << funct3
              << " funct7 = "  << funct7
              << " zero = "    << zero
              << "\n outputs:\n"
              << "  regwrite  = " << (int)dut->regwrite  << "\n"
              << "  alusrc    = " << (int)dut->alusrc    << "\n"
              << "  immsrc    = " << (int)dut->immsrc    << "\n"
              << "  alucontrol= " << (int)dut->alucontrol<< "\n"
              << "  pcsrc     = " << (int)dut->pcsrc     << "\n"
              << "  resultsrc = " << (int)dut->resultsrc << "\n"
              << "  memwrite  = " << (int)dut->memwrite  << "\n"
              << "  jalre     = " << (int)dut->jalre     << "\n"
              << "----------------------------------------\n";
}

int main(int argc, char** argv)
{
    Verilated::commandArgs(argc, argv);
    VControlUnit* dut = new VControlUnit;

    // ADDI
    run_test(dut, 0b0010011, 0b000, 0, 0);

    // BNE (funct3 = 001)
    run_test(dut, 0b1100011, 0b001, 0, 0);   // zero=0 (should branch)
    run_test(dut, 0b1100011, 0b001, 0, 1);   // zero=1 (shouldn't branch)

    // ADD
    run_test(dut, 0b0110011, 0b000, 0b0000000, 0);

    // LBU
    run_test(dut, 0b0000011, 0b100, 0, 0);

    // SB
    run_test(dut, 0b0100011, 0b000, 0, 0);

    // JAL
    run_test(dut, 0b1101111, 0b110, 0, 0);

    // JALR
    run_test(dut, 0b1100111, 0b000, 0, 0);

    delete dut;
    return 0;
}