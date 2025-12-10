Overview

As the Repository Master and Lead Developer, I spearheaded the  architectural design, implementation, and the verification of the RISC-V CPU, transitioningfrom a single-cycle to pipelined, cached architecture.

1. Workflow Development

1.Single-cycle
 
  Develop
  Implemented the DataMemory Module
  Implemented the Top for single cycle
  Contribution in the Implementation of Control Unit

  Verifications
  Data_Memory Unit Verification
  Control_Unit Unit Verification(Help)
  Immegen Unit Verification(Help)

  CPU Verifications
      ALU & Arithmetic ADD, SUB, SRA, SRAI, edge cases
      Immediate Instructions
      Load / Store Instructions
      Branches & Control Flow
      Jumps
      Reference Program Execution

2.Pipeline 

  Develop

  Implemented the  mem-stage in Pipeline
  Implemented the  execute-stage in Pipeline
  Implemented the  writeback-stage in Pipeline
  Contribution in the Pipeline Top

  Verifications

  Execut-Stage Unit Verification
  Hazard-Unit Unit Verification
  ID-Stage Unit Verification
  IF-Stage Unit Verification
  Mem-Stage Unit Verification
  Pipeline CPU Verifications: Reference Program

3.cache

  Development
  Replace Direct DataMemory access with two-level memory 
  Implemented the  Cache-Main Memory interface
  Redesigned the Top-Level 

  Verifications
  Reference Test Programs


4.Mistake I made

5.Key Design Decisions

6.Other Contributions

6.Reflections
What I learned
Mistakes
Improvement
Takeaways

7.Commit Records



Workflow Development

I designed and enforced a structured Git workflow to support parallel development, verification traceability, and safe system integration across the team. The repository was organized with clear separation between RTL (rtl/), testbenches (tb/), software (sw/), documentation (docs/), and tooling, ensuring scalability and clean navigation. Development followed a feature-branch model (feature/*) with main reserved strictly for stable, reviewed code. All non-trivial changes were integrated via Pull Requests, supported by PR/Issue templates to standardize documentation, encourage peer review(though scarce), and enforce verification reporting before merge. Branch protection rules prevented direct writes to main, reducing regression risk and preserving a clean project history.

Task allocation was initially managed through a GitHub Project dashboard to visualize progress and ownership; however, as dependencies between stages and individual capabilities makes intergration more complicated, we 
have to work largely asymmetrically and ended up talking on whatsapp and direct allocated tasks.Overall, the workflow emphasized isolation of experimental work, controlled integration, and verifiable development—mirroring professional hardware–software co-design practice.




Single-Cycle 

Data Memory Design (Single-Cycle CPU)

Initially, both instruction and data memory were modeled as 32-bit word-addressable memory, following the approach from Lab 4. This worked well for basic LW and SW operations, but became inconvenient once byte and halfword load/store instructions (LB, LBU, LH, LHU, SB, SH) were required. Supporting these instructions with a word-addressable memory would have required additional masking and shifting logic.

To simplify the design, I switched to a byte-addressable memory model, where each memory location stores 8 bits. With this structure:

Byte accesses map directly to one memory entry

Halfword accesses span two consecutive entries

Word accesses span four consecutive entries

This naturally matches the little-endian format used by RISC-V, where the least significant byte is stored at the lowest address.

The data memory is implemented as a 128 KB RAM, using only the lower 17 bits of the address. This keeps memory bounded for simulation while remaining consistent with the project memory map. Stores are implemented as synchronous writes, while loads use combinational reads, which fits the timing assumptions of a single-cycle CPU.

Signed and unsigned loads are handled using a dedicated zero-extend control signal, ensuring correct behavior for all RV32I load instructions. A simulation-only preload mechanism using $readmemh is included to load external test programs, enabling direct testing with the F1 program, the reference pdf.asm program, and all verification tests in tb/asm.

Overall, switching to a byte-addressable memory greatly simplified the implementation of RV32I load/store instructions and improved correctness and flexibility of the single-cycle CPU.


Unit-Verification

Data Memory Unit Testing

The Data Memory was verified using a Verilator-driven GoogleTest testbench. Tests directly exercise all RV32I load/store widths—byte, halfword, and word—checking correct write behavior and validating signed vs. unsigned load semantics. Little-endian ordering is confirmed through targeted byte-offset reads, and a simulation-only test verifies correct $readmemh preload at the data-memory base address. These tests provide cycle-accurate validation of the module’s functional correctness before pipeline integration.



Top Design
Single-Cycle Top Datapath Overview

This diagram shows the complete single-cycle RV32I datapath integrated in the top module. Each instruction flows through fetch → decode → execute → memory → write-back in one clock cycle.

The PC and Instruction Memory handle instruction fetch, with PC+4, branch targets, and jump targets selected through PCSrc.

The Control Unit decodes opcode, funct3, funct7, and the ALU zero flag to generate all control signals, including RegWrite, ALUSrc, ALUControl, ResultSrc, PCSrc, and AddressingControl.

The Register File provides two read operands (rs1, rs2) and one synchronous write-back port (rd).

The Immediate Generator (Extend) produces the correct immediate format for each instruction type.

The ALU performs arithmetic, logic, and branch comparisons.

The Data Memory supports byte, halfword, and word load/store operations using the unified AddressingControl.

The Result MUX selects between ALU result, memory read data, and PC+4 for write-back.

From an integration perspective, this top-level design is structurally straightforward, but the main source of bugs came from control signal alignment and port naming mismatches across independently developed modules 

Top Verifications:
General CPU Test


ALU Test
system.

Immediate Test

Load/Store Test


Reference Program Test (Official)



Pipeline Verifications:
unit test
 executestage
 hazard unit
 id stage
 mem stage 
 pipeline registers
 test stage
 wb stage
 top



I didn't use gtkwave because I find opening the graph each time very annoying, instead I create a debug sections in the cpu that essentially let me observe key value.






cache








Pipeline Stage Implementation

To transition the design from a single-cycle CPU to a pipelined architecture, I implemented the Execute (EX), Memory (MEM), and Writeback (WB) stages and integrated them into the 5-stage pipeline structure. This work primarily involved creating stage-specific wrapper modules that isolate each functional unit (ALU, branch logic, data memory, result selection) and connecting them through well-defined pipeline registers (ID/EX, EX/MEM, MEM/WB). The Execute stage handles operand forwarding, ALU operations, and branch decision signals; the Memory stage wraps the existing Data Memory module with properly pipelined control signals; and the Writeback stage produces the final result to be written into the register file. Much of the effort involved aligning naming conventions, standardising control signal propagation across pipeline boundaries, and ensuring hazard-unit outputs (stalls, flushes, forwarding controls) were correctly wired into the stage wrappers. The result is a cleanly modularised pipeline where each stage has a clear interface and state, making debugging and verification far easier than in the monolithic single-cycle design.


Execute (EX) Stage

The Execute stage is responsible for operand selection, forwarding resolution, ALU execution, and branch decision generation. Two dedicated forwarding multiplexers are used to resolve data hazards on both ALU operands. For operand A (srcAE), the stage selects between the original register value (rd1E), the forwarded ALU result from the MEM stage (aluResultM), or the forwarded writeback result (resultW) based on forwardAE. Operand B follows the same structure through forwardBE, producing tempBE, which is also reused as the forwarded store data path (writeDataE).

The ALUSrcE control signal then selects between the forwarded register operand and the sign-extended immediate to form the final ALU input. The ALU generates both the arithmetic result (aluResultE) and the zero flag (zeroE). In parallel, the branch target address is computed using pcE + extImmE, and the final branch decision is produced as branchTakenE = branchE && zeroE. This makes the EX stage fully responsible for branch resolution and data hazard correction before entering the MEM stage.


Memory (MEM) Stage

The Memory stage serves as a thin wrapper around the Data Memory module, receiving all required data and control signals through the EX/MEM pipeline register. The ALU result provides the memory address, while the forwarded store data (writeDataE) is passed as the write operand for store instructions. Memory write is controlled solely by the pipelined MemWriteM signal, while load data is returned as readDataM.

This stage is intentionally kept simple: it does not perform computation, but instead isolates memory access timing and side effects from the execute logic. This separation allows memory behavior to be verified independently and later replaced cleanly by the cache subsystem without modifying the EX or WB stages.


Writeback (WB) Stage

The Writeback stage is implemented as a pure combinational result-selection unit. It chooses the final value to be written back into the register file based on resultSrcW. The selection logic supports three cases:

00: ALU result (aluResultW)

01: Data memory read value (readDataW)

10: PC + 4 (pcPlus4W) for JAL and JALR

The selected output (resultW) is forwarded both to the register file for architectural writeback and back into the forwarding network for hazard resolution. By isolating all writeback selection into this single module, the design ensures that the register file sees a clean, fully-resolved data source regardless of instruction type.


2.
