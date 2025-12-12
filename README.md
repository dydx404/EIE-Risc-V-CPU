# Team04-RISCV-CPU

[Jump](#single-cycle-rv32i-design) to Single Cycle CPU Documentation 

[Jump](#pipelined-rv32i-design) to Pipelined CPU Documentation

[Jump](#cache) to Cached CPU Documentation


## Repo Structure & Logic

As a team we decided to manage our repo in the following manner:
- Maintain a main branch containing the final and tested implementations of all CPU versions in their respective folders.
- The develop branch are used to store code and tb when they are verified
- we develop on seperate feature branches and combine them later for development

This approach provides a clear overview of our current progress, ensuring a clean and easily interpretable repository during examination.

## Team Member Statements
- **Yi Dong** (yi.dong23@imperial.ac.uk)  
  - [Yi Dong's Statement](statements/YiDong.md)
- **Seth Gobina** (seth.gobina24@imperial.ac.uk)  
  - [Seth Gobina's Statement](statements/SethGobina.md)
- **Zain Asif** (zain.asif23@imperial.ac.uk)  
  - [Zain Asif's Statement](statements/ZainAsif.md)
- **Mingze Chen** (mingze.chen24@imperial.ac.uk)  
  - [Mingze Chen's Statement](statements/MingzeChen/MingzeChen.md)

## Overall CPU Schematic

![CPU with Pipelining](/imgs/)

## Testing the CPU

- There is a script located in the root of the directory called start.sh
- This links to a master script contained within /testing/Master_test
- Executing the script displays a menu that enables the execution of various programs on the specified CPU.

In order to view values in a particular register of the CPU, we added a signal `testRegAddress` which is controlled at the top level module, and outputs data from a given register at the signal `testRegData`. This allows use to use register data to view outputs on vbuddy, which is useful for pdf plots and f1 program.

### When testing F1 and pdf:

- Move into the `testing/Master_test` directory
- Choose the `cpu_tb.cpp` test bench using single cycle, and `pipe_cpu_tb.cpp` if testing pipelined cpu
- Both test benches write to CPU.vcd

Below is a code snippet of the test bench. 

```C++

```
- The master script written in /testing/Master_test automatically configures the outputs depending on the program being run by passing arguments on execution

### Testing Write Up
The tests for both single cycle and the pipelined CPU were written up [here](/testing/Test%20results/Testing%20Write%20up.md) and [here](/testing/Test%20results/Pipelining%20tests.md) respectively using programs specified in the testing folder.

### Testing videos
The following videos demonstrate the F1 program's functionality on a pipelined CPU with both data memory cache and instruction memory cache.

#### F1 Program:

#### PDF for Noisy:

#### PDF for gaussian:

#### PDF for sine:

#### PDF for Triangle:

 

# Single Cycle RV32I Design

Legend: `L` = Lead `C` = Contributor

## Planning



## Implementation

### F1 ASM


### ALU



### PC

---



### Control Unit



### Memory

Early versions of the project (and Lab 4) used word-addressable memories, where each location stored a 32-bit word. That worked for LW/SW, but became awkward once we needed to support all byte and halfword load/store instructions (LB, LBU, LH, LHU, SB, SH). Word-addressable memory would have required extra masking and shifting to reconstruct sub-word accesses.

To avoid this, I implemented a **byte-addressable 128 KB data memory**, where each location stores 8 bits. In this model:

- Byte accesses map directly to a single entry.  
- Halfword accesses span two consecutive entries.  
- Word accesses span four consecutive entries.  

This layout matches RISC-V little-endian ordering: the least-significant byte lives at the lowest address. The design uses the lower 17 bits of the ALU address as the memory index, giving a bounded 2¹⁷-byte space that matches the project’s memory map and remains simulation-friendly.

Stores (SB/SH/SW) are synchronous, occurring on the rising clock edge, and write 1, 2, or 4 consecutive bytes. Loads (LB/LBU/LH/LHU/LW) are combinational, reconstructing data from the byte array and applying sign or zero extension based on a unified 3-bit `access_ctrl` bus: `[1:0]` encode the size (byte/halfword/word) and bit `[2]` indicates signed vs unsigned behaviour. That means the memory returns ISA-correct values directly, keeping the rest of the datapath simple.

For simulation, I added a preload mechanism using `$readmemh`, which loads external `.mem` images at the base address defined by the memory map. This allows the same data memory to be reused across:

- the team’s F1 starting light program  
- the official `pdf.asm` reference program  
- all five `tb/asm` verification programs  
- general top level tests 

and makes it easy to swap test programs without recompiling RTL.

## Design Decisions

### Control Decoder Table
| Instruction Type | op | RegWrite | ALUSrc | MemWrite | PCSrc | ImmSrc | ResultSrc
| -------- | :--------: | :--------: | :--------: | :--------: | :--------: | :--------: | :--------: 
| R-Type (51) | 0110011 | 1 | 0 | 0 | 00 | xxx | xx
| B-Type (99) | 1100011 | 0 | 0 | 0 | 00/01 | 010 | xx
| I-Type (19) | 0010011 | 1 | 1 | 0 | 00 | 000 | 00
| I-Type (3) | 0000011 | 1 | 1 | 0 | 00 | 000 | 01
| I-Type (103) | 1100111 | 1 | 1 | 0 | 10 | 000 | 10 
| J-Type (111) | 1101111 | 1 | x | 1 | 01 | 011 | 10
| S-Type (35) | 0100011 | 0 | 1 | 1 | 00 | 001 | xx

`AdressingControl` and `ALUControl` Not included as they usually are used to choose case for the Instruction Type being performed 


### Sign Extension
 Immsrc is a control signal produced by the control unit given to the `sextend.sv` module. Its purpose is to reconstruct immediate from the instruction word as each type have their own way of mapping the immediate onto the machine code. This control signal basically tells the `sextend.sv` which type of instruction is currently performed so that the immediate can be extracted accordingly.
 
| ImmSrc| ImmExt | Instruction Type 
| -------- | :--------: | :--------: | 
| 3'b000| {{20{Immediate[31]}}, Immediate[31:20]} | I-type |
| 3'b001| {{20{Immediate[31]}}, Immediate[31:25], Immediate[11:7]}| S-type|
| 3'b010| {{20{Immediate[31]}}, Immediate[7], Immediate[30:25], Immediate[11:8], 1'b0}|  B-type|
| 3'b011| {{12{Immediate[31]}},  Immediate[19:12], Immediate[20], Immediate[30:21], 1'b0} | J-type|
| 3'b100| {Immediate[31:12], 12'b0}| U-type|

### Addressing Control

This control signal is produced by the control unit and is used to choose how we want to construct the bytes onto word in data memory. This is especially useful for instructions such as `lb`, `lh`, `sh`, `sb` where we only want to extract/store a byte or half of the word instead of the entire word.

The addressing control is 3 bits wide, the MSB is to choose between signed or unsigned extension and the remaining bits are used for choosing the different modes and they are allocated for each cases as follows:

| AdddressingControl [1:0] | AddressingControl [2] | Load Instruction type | Store Instruction type |
| -------- | :--------: | :--------: | :--------: |
| 2'b00  | 1'b0 | `lb` | `sb` |
| 2'b00 | 1'b1 |  `lbu` | xx |
| 2'b01 | 1'b0 | `lh` | `sh` |
| 2'b01 | 1'b1 | `lhu` | xx |
| 2'b10 | xx | `lw` | `sw` |


### Implemented Instructions 

#### R-Type
`add` `sub` `sll` `slt` `sltu` `xor` `srl` `sra` `or` `and`
#### B-Type
`beq` `bne` 
#### I-Type
`addi` `slli` `slti` `sltiu` `xori` `srli` `srai` `ori` `andi` `lb` `lh` `lw` `lbu` `lhu` `jalr`
#### J-Type
`jal`
#### S-Type
`sb` `sh` `sw`
#### U-Type
`lui`
  
*  `blt` `bge` `bgeu` `bltu` have only been implemented in the pipelined version. Single cycle only implements `beq` and `bne`.

## Final Schematic for Single Cycle CPU

![Single Cycle CPU Schematic](imgs/SingleCycleCpu.jpeg)

# Pipelined RV32I Design


### Contributions


`mux.sv` is a 4 input multiplexer used as 3 instances in the architecture.

Legend: `L` = Lead `C` = Contributor

## Planning

We met as a team and began by examining the schematic for the pipelined cpu in lectures, modifying it for our own design.

Once this was done each team member was assigned a stage to work on and we began our implementation of the pipelined cpu, keeping frequent communication between team members to ensure each module interfaced correctly with one another.

## Implementation

### Changes to Existing Modules

#### ALU Pipelined

Additions were made to the [alu block](rtl_pipelined/alu.sv). It implements the rest of the branch type instructions, by setting the `Zero` flag high whenever a branch test is passed for example, for `blt`, if `SrcA < SrcB`, `Zero` is high. This means that if the current instruction in execute stage happens to be a branch type instruction and the test is passed, `BranchE` will be high from the control unit, and so will `Zero` from the ALU. As such, `ALUSrc` will be high, and a branch will be effected. 

#### Control Unit Pipelined

The improvements in the control unit module improve efficiency, readability, and robustness of control logic. This includes better handling of instruction types, streamlined input processing, and the introduction of new control signals for enhanced functionality. 
key differences:

- Rename signals like `RegWrite`, `ALUControl`, `MemWrite`, etc. to `RegWriteD`, `ALUControlD`, `MemWriteD`, to prepare for pipeline integration

- Remove the `zero` input and reduce `funct7` to a single logic bit, streamlining inputs

- Introduce new output signals like `JumpD`, `BranchD`, and `JALRInstrD`. These signals add more control functionality, offering finer control over jump, branch, and JALR (Jump and Link Register) instructions.

- Enhances the control logic for B-type instructions by including additional operations (`blt`, `bge`, `bltu`, `bgeu`). case structure simplified for better readability and maintenance.

- default case added in the main `case` statement, ensuring that all control signals are explicitly set to a known state when an unrecognized opcode is encountered.

- more cleanly structured with consistent indentation and improved commenting, improving code readability and maintainability.

### Pipelining

The pipeline of each stage is the one to its left.

We decided on this convention since we thought it would be easier to reason between stalling and flushing. Stalling implies that the inputs to the stage should remain unchanged, whereas flushing entails zeroing the inputs to the stage. As such, it made sense to define each pipeline of a stage to to the left of its corresponding stage. 

The hazard unit produces `StallFetch`, `StallDecode`, `FlushExecute`, `FlushDecode`. These are inputs to the relevant pipelines for those stages that need to be flushed or stalled. Inside the pipelined, when stall signal is high, the signals at the pipeline's inputs are not passed to the outputs, while when flush signal is high, the outputs are low. 

Each pipeline is in its own module, and those that are flushed / stalled at some point have internal signals to control that. 
Each stage is in its own module; the inputs to the module are those that are actually used for computing some value in that stage, while those that aren't used are connected directly to the next pipeline in the [top level module](rtl_pipelined/pipelined_cpu.sv). 

### Hazard Unit

The [Hazard unit](./rtl_pipelined/hazard_unit.sv) allows for the pipelined CPU to be able to perform instructions correctly without incurring delays for some special cases to ensure that it is as efficient as possible.

There are 3 different cases that we encountered that poses a challenge to pipelining and may result in an error if not taken care of which are the following:
1. When we use a register as an operand that was written to in the previous cycle. (RAW hazard)
2. When we have a branch instruction where we only know if we jump or not two cycles later in the execute stage.
3. Load instructions where it takes an extra cycle to load data. 

Thus, to solve these possible issues that the processor might encounter with pipelining we implement the following in our design:
1. Forwarding: allows the value of a register to be used in an operation right after it was written without having to wait for it to go through all the pipelining stages. 
2. Stalling: Stalling a stage means to maintain its state, so the inputs to the stage should not change on the next clock cycle. This allows for load instruction to have its values from memory to be loaded into the writeback stage so that it can be forwarded onto the execute stage.
3. Flushing: This resets the output of the pipeline flip-flops; This is very useful, for example in the case of branch, we do not know whether to jump or not until the branch instruction is in the execution stage. That means the next instruction in the instruction memory would be loaded onto the decode stage. This would create an error if the jump actually occurs, therefore we need to flush the decode stage when jump happens as if the instruction had never been loaded to the decode stage.

All three solutions/operations mentioned above are implemented in our pipelined CPU. Each operation may be used individually or simultaneously for specific cases/instruction. The control signal for forwarding, flush and stall are all produced/controlled by the hazard unit. RAW hazards are mitigated by forwarding from the Writeback or memory stages into the execute stage. If the current instruction in execute stage has a source register that's the same as the destination register as an instruction currently in writeback or memory stage, we forward data from writeback or memory stages respectively. We also only forward data from instructions that were going to write to a register. The zero register is never forwarded because it never has meaningful data being written to it (as it is hardwired to zero).

Lw issue is solved by stalling the decode and fetch stages. As such, we must flush the execute stage to prevent incorrect data from propagating forward.

If a control hazard is detected, the execute and decode stages are flushed (2 instructions after branch instruction are flushed) before moving to correct instruction. 



## Finalised Pipelined CPU Schematic



# Cache

## Cache specifications

word = 32

| Version | Capacity(words) | Words written at once | Blocks in cache | Blocks / set | Sets
| -------- | :--------: | :--------: | :--------: | :--------: | :--------: |
| Direct mapped | 8 | 1 | 8 | 1 | 8 |
| 2 way assosiative | 8 | 1 | 8 | 2 | 4 |



Legend: L = Lead C = Contributor

We introduced caching mechanisms for both instructions and data. Our initial focus was on configuring the instruction cache due to its relative simplicity; it only required adjustments in one pipelined stage. After successfully implementing and testing the instruction cache, we proceeded to tackle the data cache. We created two SystemVerilog blocks for direct-mapped and two-way associative configurations, seamlessly integrating them into the fetch and execute stages for our instruction and data caches, respectively.


## Proof that cache is being used


**If the following videos do not work, they are contained within the /imgs folder**

https://github.com/Arc-Cloud/Team04-RISCV-Proj/assets/33992400/e28dd883-036e-4780-bffe-a70179a75b80



The video above is a screen capture of waveforms produced when CPU with data cache is tested on [the assembly program from lectures](testing/Data%20cache%20test/dcache.asm).

`PCE` shows memory address of the instruction currently in execute stage. We see that `useCacheM` is high when `PCE = 0x10, 0x14, 0x18`. At these points, the instruction in memory stage is each of the lw instructions, and `useCacheM` is high only for lw instructions, so this works as expected. We also see that we get no hits, as expected.


Next we get cache hits for instructions from `0x08 and 0x14`, but `useCacheM` is high only for `PCE = 0x10, 0x14, 0x18`; only the lw instructions. So the hit we get by the branch instruction is meaningless, we only consider hits by the load instructions. 

Running through the loop, we see that this pattern continues until the loop ends.


## Cache Schematic

![Cache Integrated with Pipelined CPU](/imgs/Integrated%20Cache.jpeg)