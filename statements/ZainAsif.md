# Zain Asif Statement

- [**Overview**](#overview)
- [**Contribution**](#contribution)
- [**Design Decisions**](#special-design-decisions)
- [**Mistakes Made**](#mistakes-made)
- [**What I Could Do Differently**](#what-i-could-do-differently)
- [**Reflection**](#reflection)

## Overview
This file describes my (Zain Asif) contribution to the design, implementation and verification of our RISC-V processer project. I worked across both single cycle and pipeline extension, implementing units such as the ALU and RegFile, datapath wiring and pipeline registers. Also tested small part of verification using VBuddy.

## Contribution
### Single Cycle
- [ALU](#alu)
- [Regfile](#regfile)
- [Datapath](#datapath)
### Pipelined
- [Pipeline Registers](#pipeline-registers)
- [Forwarding Unit](#forwarding-unit)
- [A0 VBuddy Testing](#a0-vbuddy-testing)

#### ALU 
Implemented the single cycle ALU (Arithmetic Logic Unit) with a 4-bit control signal, supporting ten RV32I operations: ADD, SUB, AND, OR, XOR, SLT, SLTU, SLL, SRL, and SRA/SRAI. This included handling signed versus unsigned comparisons and shift behaviour correctly.
#### Regfile
Single cycle register file with asynchronous reads for rs1/rs2 and synchronous writes on the rising clock edge, while hard wiring x0 to zero. I also added a dedicated debug output to continuously expose x10 (a0) so it could be monitored by the testbench and Vbuddy debugging tool.
#### Datapath
Wired up the single cycle datapath by instantiating and connecting the Program Counter, Instruction Memory, Register File, Immediate Extension unit, ALU (Arithmetic Logic Unit), Data Memory, PC+4 logic, branch/jump target logic, and the writeback multiplexer. I also exposed internal signals such as pc, instr, alu_result, and a0 to simplify debugging and verification.
#### Pipeline Registers
Implemented all four pipeline register stages (IF/ID, ID/EX, EX/MEM, MEM/WB), capturing the relevant data and control signals between stages. Each register supports stall and flush control, allowing the hazard unit to insert bubbles or hold values when needed.
##### Forwarding Unit
Designed the forwarding unit for the pipelined core to resolve read after write data hazards without stalling. It compares source registers in the Execute stage with destination registers in later stages and drives 2 bit select signals to choose between the original operand, the EX/MEM result, or the MEM/WB writeback value.
##### A0 VBuddy Testing
Wrote a C++ testbench that links the single cycle core to VBuddy, driving clock and reset, running the test program, and continuously displaying the lower 16 bits of a0 on the display. This was used to verify that the core executes the instruction memory correctly and produces the expected final value in a0.

## Special Design Decisions
Instead of using 3 bit control signal for ALU, decided to use 4 bit signal. This allowed ALU to be implemented in a cleaner manner, by giving each operation unique encoding.

Exposed a0 as a debug port in RegFile.sv. This allowed for easier debugging and display on the VBuddy.

//ELABORATE

## Mistakes Made
When initially configuring the datapath, the control unit was setup to use a 3 bit ALU control signal, whereas the ALU expected 4 bit. Thus, as a temporary solution for intial testing, I set the leftmost bit fixed at 0, but this inhibited the instructions that could be run. Communicated with team member in charge of datapath and eventaully we came to a solution of him configuring control unit to take a 4 bit signal, and then some reconfig of the ALU instructions as to how they are executed.

//NEED SOME MORE
## What I Could Do Differently

## Reflection
