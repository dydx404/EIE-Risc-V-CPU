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

#### **[ALU](https://github.com/dydx404/EIE-Risc-V-CPU/blob/d5ce1ae76d9da69ed8229eed5307071deed41e6e/rtl/single_cycle/alu/ALU.sv)** 
Implemented the single-cycle ALU (Arithmetic Logic Unit) using a 4-bit control signal to give each operation a unique encoding. The ALU supports ten RV32I operations: ADD, SUB, AND, OR, XOR, SLT, SLTU, SLL, SRL, and SRA/SRAI. Signed and unsigned comparisons are handled explicitly using $signed, and shift operations correctly limit the shift amount to the lower 5 bits. A zero flag is generated combinationally from the ALU output and is used by the control logic for branch decisions. A simple unit testbench was written to test all supported operations and ensure correctness ([Link](https://github.com/dydx404/EIE-Risc-V-CPU/blob/d5ce1ae76d9da69ed8229eed5307071deed41e6e/tb/unit/alu/alu_tb.cpp)).
#### **[Regfile](https://github.com/dydx404/EIE-Risc-V-CPU/blob/2135f95f9d38997522e26b9bb768f5e764bd3ba3/rtl/single_cycle/regfile/RegFile.sv)**
Implemented a 32×32-bit single-cycle register file with two asynchronous read ports (rs1, rs2) and one synchronous write port (rd) on the rising clock edge. Register x0 is hardwired to zero at all times. The design includes same-cycle write-through behaviour so that reads return the newly written value when a read and write target the same register in a single cycle. For debugging and verification, register x10 (a0) is continuously exposed as a dedicated output, allowing easy observation in both Verilator and VBuddy without modifying the datapath. A lightweight unit testbench was used to confirm read/write timing ([Link](https://github.com/dydx404/EIE-Risc-V-CPU/blob/2135f95f9d38997522e26b9bb768f5e764bd3ba3/tb/unit/regfile/regfile_tb.cpp)).
#### **[Datapath](https://github.com/dydx404/EIE-Risc-V-CPU/blob/99d167b922d8a23e048dbb67bdb551956c5d6015/rtl/single_cycle/datapath/Datapath.sv)**
Wired up the complete single-cycle datapath by instantiating and connecting the Program Counter, Instruction Memory, Register File, Immediate Extend unit, ALU, Data Memory, and writeback logic. Implemented the ALUSrc multiplexer to select between register and immediate operands, and the ResultSrc writeback multiplexer to select between ALU results, memory loads, and PC+4 for JAL instructions. PC update logic supports normal sequential execution, branches, JAL, and JALR through a derived PC source select. The datapath exposes internal signals such as pc, instr, alu_result, and a0 to simplify debugging and integration testing, while keeping most control logic outside the datapath itself.
#### **[Pipeline Registers](https://github.com/dydx404/EIE-Risc-V-CPU/tree/f139b92ce6561f556f9ac6fca4d93487a976cf4c/rtl/pipelined/pipeline_registers)**
Implemented all four pipeline register stages (IF/ID, ID/EX, EX/MEM, MEM/WB) to latch both data and control signals between pipeline stages on the rising clock edge. Each pipeline register supports stall (hold current state) and flush (insert a bubble by zeroing outputs) to allow the hazard logic to control instruction flow. IF/ID flush inserts a NOP by forcing instrD = 0 (treated as addi x0, x0, 0). ID/EX and later stages zero relevant control signals on flush so incorrect writes/memory operations cannot occur after a hazard or control redirect. This kept the pipeline behaviour predictable during integration and made hazards easier to reason about.
#### **[Forwarding Unit](https://github.com/dydx404/EIE-Risc-V-CPU/blob/681d1bc8de60df46cd86e70782b85d395a75a23e/rtl/pipelined/forwarding_unit/forwardingUnit.sv)**
Implemented all four pipeline register stages (IF/ID, ID/EX, EX/MEM, MEM/WB) to latch both data and control signals between pipeline stages on the rising clock edge. Each pipeline register supports stall (hold current state) and flush (insert a bubble by zeroing outputs) to allow the hazard logic to control instruction flow. IF/ID flush inserts a NOP by forcing instrD = 0 (treated as addi x0, x0, 0). ID/EX and later stages zero relevant control signals on flush so incorrect writes/memory operations cannot occur after a hazard or control redirect. This kept the pipeline behaviour predictable during integration and made hazards easier to reason about.
#### **[A0 VBuddy Testing](https://github.com/dydx404/EIE-Risc-V-CPU/tree/f9be767ed07dc9b53c244fa9b713af2346330519/tb/single_cycle/test-a0/rtl_copy)**
Wrote a C++ Verilator testbench to run the single-cycle CPU and display register x10 (a0) on VBuddy in real time. The testbench initialises VBuddy, applies reset, steps the clock, and continuously reads top->a0 from the exposed debug port in the register file. The lower 16 bits were displayed using vbdHex to provide a simple sanity check that instruction memory execution and writeback were behaving as expected (tested using a small program such as 10 + 20 = 30).

https://github.com/user-attachments/assets/1fd66059-7e83-4343-bd51-0d5247b3e494


#### **Group Readme Skeleton**
Assisted in developing the group readme by creating the skeleton, which was then built on by other group members.


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
