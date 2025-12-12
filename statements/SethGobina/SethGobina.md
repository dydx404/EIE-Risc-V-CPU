# CPU Control Unit & Immediate Generator Reflection

## Contents
- [Introduction](#introduction)
- [Beginning the Control Unit](#beginning-the-control-unit)
- [Challenges in Control Signal Design](#challenges-in-control-signal-design)
  - [Overreliance on the Provided Datapath](#overreliance-on-the-provided-datapath)
- [Immediate Generator](#immediate-generator)
- [Testing](#testing)
- [Pipeline CPU – Decoder](#pipeline-cpu--decoder)
- [Diagrams](#diagrams)
- [Conclusion](#conclusion)

---

## Introduction

I worked on the Control Unit and Immediate Generator for the Single-Cycle CPU, and the Decode section for the Pipeline CPU. When approaching their design, I initially underestimated how many subtle details and fine‑tuning would be involved. Both modules seemed conceptually straightforward on paper, but implementing the Control Unit in hardware quickly revealed several hidden complexities. My process ended up being far more iterative than expected, and most of my challenges were a result of lapses in judgement and misapplying the provided datapath.

---

## Beginning the Control Unit

To begin working on the Control Unit, I first needed to determine what commands we would be required to execute, all of which were outlined on the Project Brief page. After examining the `pdf.s` file and cross‑referencing it with the RISC‑V specification table, it became clear that some commands, such as **RET** or **J**, were not provided in the specification. I figured that we would need to approach them as pseudo‑instructions, implementing them using the commands available to us. Through my research, I realised that:

* **LI** could be implemented as `ADDI rd, x0, imm`
* **J** as `JAL x0, imm`
* **RET** as `JALR x0, x1, 0`

Beginning was fairly straightforward – the architecture provided to us outlined which inputs and outputs were relevant for the Control Unit, so I started by extracting the opcode, funct3 and funct7 signals from the instruction and setting outputs accordingly. I later realised that blindly following that architecture was unreliable.

---

## Challenges in Control Signal Design

Understanding which control signals were in certain states for certain instructions required a deeper understanding of the function of each of them. Once I had successfully mapped each section of the initial instruction to its opcode, funct3 and funct7, it became a process of tracing the data along the architecture and analysing what signals would be necessary for the specified outcome.

It was when I reached the **JAL** and **JALR** instructions that I realised the structure I had been using was incomplete.

### Overreliance on the Provided Datapath
![](https://github.com/EIE2-IAC-Labs/Project_Brief/raw/main/images/single-cycle.jpg)

Relying on the provided datapath too much became one of the biggest hurdles and led to several setbacks. For example:

* The **WD3 MUX** lacked output options needed for Jump and Return.
* The control signal **ResultSrc** had too few bits.
* There was no way to access **PCPlus4** to use it for JAL.

With some essential functions practically impossible to program, we discussed how to modify the structure to fulfil the task. Our next iteration saw an increase in control signals and MUXes, giving us the necessary tools to execute all of our programs. It was then a straightforward task of using the case() function to detect each instruction.

<img width="6050" height="3970" alt="single cycle" src="https://github.com/user-attachments/assets/61653d9a-83be-4130-b9fc-9189d635cf18" />

---

## Immediate Generator

The Immediate Extension module was not too difficult, aside from many small mistakes on my part. It was mainly a task of deciphering the immediate formats in the RISC‑V specification and accurately connecting them. Human error was the largest hurdle.

One major point of confusion was distinguishing between **LB** and **LBU**, since I was unsure how to differentiate signed vs. unsigned processing. The provided architecture did not include a control signal for this, so I considered implementing our own. However, further research showed this was non‑standard, and group discussion prompted us to handle it outside of the Control Unit or Immediate Extension.

---

## Testing

Once I felt I had completed my sections, I tested them by creating C++ test benches for the Control Unit and Immediate Extension separately, and also created a Top Module with its own test bench. These tests were successful and prompted the submission of my next iteration.

The final iteration of the single‑cycle CPU, however, included heavy alterations resulting from collaboration within the group, primarily to make debugging easier. Instead of distinct blocks for each instruction, the new structure featured a **nested layout**, grouping similar instructions together, such as those with the same Immediate Type or those classified as Branch Instructions. 

```cpp
#include <gtest/gtest.h>
#include <verilated.h>
#include "VControlUnit.h"

static VControlUnit *dut;

static void apply(uint8_t op, uint8_t funct3, uint8_t funct7, bool zero) {
    dut->op     = op;
    dut->funct3 = funct3;
    dut->funct7 = funct7;
    dut->zero   = zero;
    dut->eval();
}

// ---------------------- Tests --------------------------

TEST(ControlUnit, Rtype_ADD) {
    apply(0b0110011, 0b000, 0b0000000, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->ALUSrc,   0);
    EXPECT_EQ(dut->ALUControl, 0b0000); // ADD
    EXPECT_EQ(dut->MemWrite, 0);
    EXPECT_EQ(dut->PCSrc,    0b00);
    EXPECT_EQ(dut->ResultSrc, 0b00);
}

TEST(ControlUnit, Rtype_SUB) {
    apply(0b0110011, 0b000, 0b0100000, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->ALUSrc,   0);
    EXPECT_EQ(dut->ALUControl, 0b0001); // SUB
}

TEST(ControlUnit, Rtype_AND) {
    apply(0b0110011, 0b111, 0b0000000, 0);

    EXPECT_EQ(dut->ALUControl, 0b0010); // AND
}

TEST(ControlUnit, Itype_ADDI) {
    apply(0b0010011, 0b000, 0, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ImmSrc,   0b000);
    EXPECT_EQ(dut->ALUControl, 0b0000); // ADD
    EXPECT_EQ(dut->ResultSrc, 0b00);
    EXPECT_EQ(dut->PCSrc,    0b00);
}

TEST(ControlUnit, Load_LW) {
    apply(0b0000011, 0b010, 0, 0); // LW

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->MemWrite, 0);
    EXPECT_EQ(dut->ImmSrc,   0b000);
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ALUControl, 0b0000); // ADD
    EXPECT_EQ(dut->ResultSrc, 0b01);    // from memory
    EXPECT_EQ(dut->PCSrc,    0b00);
    EXPECT_EQ(dut->AddressingControl, 0b010);
}

TEST(ControlUnit, Store_SW) {
    apply(0b0100011, 0b010, 0, 0); // SW

    EXPECT_EQ(dut->RegWrite, 0);
    EXPECT_EQ(dut->MemWrite, 1);
    EXPECT_EQ(dut->ImmSrc,   0b001);
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ALUControl, 0b0000); // ADD
    EXPECT_EQ(dut->PCSrc,    0b00);
    EXPECT_EQ(dut->AddressingControl, 0b010);
}

TEST(ControlUnit, BEQ_Taken) {
    apply(0b1100011, 0b000, 0, /*zero=*/1);

    EXPECT_EQ(dut->RegWrite, 0);
    EXPECT_EQ(dut->PCSrc,    0b01); // branch taken
    EXPECT_EQ(dut->ImmSrc,   0b010);
}

TEST(ControlUnit, BEQ_NotTaken) {
    apply(0b1100011, 0b000, 0, /*zero=*/0);
    EXPECT_EQ(dut->PCSrc,    0b00);
}

TEST(ControlUnit, BNE_Taken) {
    apply(0b1100011, 0b001, 0, /*zero=*/0);
    EXPECT_EQ(dut->PCSrc,    0b01);
}

TEST(ControlUnit, BNE_NotTaken) {
    apply(0b1100011, 0b001, 0, /*zero=*/1);
    EXPECT_EQ(dut->PCSrc,    0b00);
}

TEST(ControlUnit, JAL) {
    apply(0b1101111, 0, 0, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->PCSrc,    0b01);  // PC+imm
    EXPECT_EQ(dut->ImmSrc,   0b011); // J-type
    EXPECT_EQ(dut->ResultSrc, 0b10); // PC+4
}

TEST(ControlUnit, JALR) {
    apply(0b1100111, 0, 0, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->PCSrc,    0b10);  // JALR path
    EXPECT_EQ(dut->ImmSrc,   0b000); // I-type
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ResultSrc, 0b10); // PC+4
}

TEST(ControlUnit, LUI) {
    apply(0b0110111, 0, 0, 0);

    EXPECT_EQ(dut->RegWrite, 1);
    EXPECT_EQ(dut->ImmSrc,   0b100); // U-type
    EXPECT_EQ(dut->ALUSrc,   1);
    EXPECT_EQ(dut->ALUControl, 0b1111); // LUI op
    EXPECT_EQ(dut->ResultSrc, 0b00);
    EXPECT_EQ(dut->PCSrc,    0b00);
}

TEST(ControlUnit, UnknownOpcode_Defaults) {
    apply(0b0000000, 0, 0, 0);

    EXPECT_EQ(dut->RegWrite, 0);
    EXPECT_EQ(dut->MemWrite, 0);
    EXPECT_EQ(dut->PCSrc,    0b00);
    EXPECT_EQ(dut->ResultSrc, 0b00);
}

// ---------------------- main ---------------------------

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    dut = new VControlUnit;
    int result = RUN_ALL_TESTS();
    delete dut;

    return result;
}

```
---

## Pipeline CPU – Decoder

For the Pipeline CPU, my task was to create the Decoder, which was essentially a top module for the Control Unit, Register File, and Extend module. It was straightforward, but we did need to resolve a few incompatibilities between components. For example, some modules used **PCSrc** as a control signal, while others used **Jump**, **Branch**, and **JALR** separately. This was simply a communication issue.

---

## Diagrams

I also worked on the diagrams for the single-cycle and pipeline CPUs. Besides the challenge of finding a suitable program proving futile, leading me to hand-draw it entirely, it proved difficult to create a consistent diagram that reconciled all of the components into one coherent module, as different components used different names for the same signal. In the end, though, it was simply a matter of settling on one of the names, as the diagrams simply serve to provide a visual representation of the architecture we are based on.

---

## Conclusion

Overall, I learned a lot about CPU architecture and the implementation of the Control Unit. I also learned how instructions such as **JALR** interact with different parts of the CPU and how they execute their commands. If I were to do it again, I would pace myself a lot better at the start, taking time to understand the architecture before blindly referencing the one provided to us. A bit of research on industry standards in architecture may have helped with developing an intuition. In general, the experience showed me how iterative the design process can be and how important it is to understand the datapath in depth before relying on it.
