# Zain Asif Statement

- [**Overview**](#overview)
- [**Contribution**](#contribution)
- [**Design Decisions**](#special-design-decisions)
- [**Mistakes Made**](#mistakes-made)
- [**What I Could Do Differently**](#what-i-could-do-differently)
- [**Reflection**](#reflection)

## Overview
This file describes my (Zain Asif) contribution to the design, implementation and verification of our RISC-V processor project. I worked across both single cycle and pipeline extension, implementing units such as the ALU and RegFile, datapath wiring and pipeline registers. Also tested small part of verification using VBuddy.

## Contribution
### Single Cycle
- [ALU](#alu)
- [Regfile](#regfile)
- [Datapath](#datapath)
### Pipelined
- [Pipeline Registers](#pipeline-registers)
- [Forwarding Unit](#forwarding-unit)
- [A0 VBuddy Testing](#a0-vbuddy-testing)

#### **[ALU](https://github.com/dydx404/EIE-Risc-V-CPU/tree/feature/alu)** 
Implemented the single cycle ALU (Arithmetic Logic Unit) with a 4-bit control signal, supporting ten RV32I operations: ADD, SUB, AND, OR, XOR, SLT, SLTU, SLL, SRL, and SRA/SRAI. This included handling signed versus unsigned comparisons and shift behaviour correctly.
#### **[Regfile](https://github.com/dydx404/EIE-Risc-V-CPU/tree/feature/regfile)**
Single cycle register file with asynchronous reads for rs1/rs2 and synchronous writes on the rising clock edge, while hard wiring x0 to zero. I also added a dedicated debug output to continuously expose x10 (a0) so it could be monitored by the testbench and Vbuddy debugging tool.
#### **[Datapath](https://github.com/dydx404/EIE-Risc-V-CPU/tree/feature/datapath)**
Wired up the single cycle datapath by instantiating and connecting the Program Counter, Instruction Memory, Register File, Immediate Extension unit, ALU (Arithmetic Logic Unit), Data Memory, PC+4 logic, branch/jump target logic, and the writeback multiplexer. I also exposed internal signals such as pc, instr, alu_result, and a0 to simplify debugging and verification.
#### **[Pipeline Registers](https://github.com/dydx404/EIE-Risc-V-CPU/tree/feature/pipeline_regs)**
Implemented all four pipeline register stages (IF/ID, ID/EX, EX/MEM, MEM/WB), capturing the relevant data and control signals between stages. Each register supports stall and flush control, allowing the hazard unit to insert bubbles or hold values when needed.
##### **[Forwarding Unit](https://github.com/dydx404/EIE-Risc-V-CPU/tree/feature/forwarding-unit/)**
Designed the forwarding unit for the pipelined core to resolve read after write data hazards without stalling. It compares source registers in the Execute stage with destination registers in later stages and drives 2 bit select signals to choose between the original operand, the EX/MEM result, or the MEM/WB writeback value.
##### **[A0 VBuddy Testing](https://github.com/dydx404/EIE-Risc-V-CPU/tree/feature/a0-test)**
Wrote a C++ testbench that links the single cycle core to VBuddy, driving clock and reset, running the test program, and continuously displaying the lower 16 bits of a0 on the display. This was used to verify that the core executes the instruction memory correctly and produces the expected final value in a0.


## Special Design Decisions
**1. Expanding ALU control from 3 bits to 4 bits** 


Originally, the Control Unit generated a 3 bit ALU control signal, but the ALU needed to support more than 8 operations. Instead of having overlapping functions, I switched to a 4-bit control signal.
This gave each ALU operation its own unique encoding, reduced potential crossovers, and made the ALU easier to understand, extend and debug (especially for instructions like SRA/SRAI that behave differently from SRL/SRLI).
([expansion commit](https://github.com/dydx404/EIE-Risc-V-CPU/commit/b8f2bcad2b74b382b4946b17dedfd27459b54c67))

**2. Exposing a0**

I added a direct output for register x10 (a0) inside the Register File. Since a0 is commonly used in test programs, exposing it simplified verification. This made it possible for both the Verilator testbench and VBuddy to monitor the processor's output live without modifying internal CPU behaviour.

**3. Less Logic in Datapath.sv**

When building the single-cycle datapath, I avoided adding too much logic inside the Datapath module. Instead, the datapath is a simple connection of modules (PC, InstrMem, RegFile, ALU, etc.). This choice made the design easier to reason about and kept responsibility for control flow inside the Control Unit.

**4. Bundling signals in pipeline registers**

For the pipelined version, I grouped control and data signals cleanly within each pipeline register stage. This avoided long lists of individual wires and made it much easier to expand or debug the pipeline during integration.

## Mistakes Made
When initially configuring the datapath, the control unit was setup to use a 3 bit ALU control signal, whereas the ALU expected 4 bit. Thus, as a temporary solution for initial testing, I set the leftmost bit fixed at 0, but this inhibited the instructions that could be run. Communicated with team member in charge of datapath and eventually we came to a solution of him configuring control unit to take a 4 bit signal, and then some reconfig of the ALU instructions as to how they are executed.
([datapath modifcation commit](https://github.com/dydx404/EIE-Risc-V-CPU/commit/b8f2bcad2b74b382b4946b17dedfd27459b54c67))

Another mistake began when I first began wiring up the datapath. I had some misunderstanding as to how each module implemented by other team members truly behaved, leading to confusion as to which signals were handled by the individual component and which needed to be handled by the datapath. To resolve this, I went through each individual component to fully understand as to how they were implemented, before integrating them. For example, the PC module already had a top level wrapper for easier instantiation, which I had initially overlooked.

## What I Could Do Differently
One improvement would be to establish clear and consistent signal names across the team before starting the project. This would have reduced confusion when integrating different modules, and personally made the datapath wiring stage smoother.

I would also document assumptions and create a small readme for each individual component during development. This would help the entire team better understand how each module worked, prevented misunderstandings about responsibilities and made integration more cohesive.

## Reflection
Working on this project helped me develop a much deeper understanding of the RISC-V CPU's inner workings, consolidating the content we studied in lectures. Implementing individual components and top modules gave me a clearer image of how each part interacts.

This also taught me how to work more effectively in a larger codebase with a team, and how to use Git and GitHub collaboratively.

Overall, this project has improved my confidence in working with SystemVerilog, and showed me the importance of clear design decisions and effective communication when working on hardware development within a team.