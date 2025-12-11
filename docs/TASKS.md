# Project Tasks Overview — EIE RISC-V CPU

This document defines WHAT we are building and WHERE the work lives in the repo.

---

## Phase 1 — Single-Cycle CPU (baseline)

Goal: Fully functional RV32I single-cycle CPU that passes reference programs.

### Task SC1 — ALU
- Path: rtl/single_cycle/alu/
- Branch name: feature/alu
- Responsibilities:
  - Implement ALU supporting RV32I ops (ADD, SUB, AND, OR, XOR, SLT, SLTU, SLL, SRL, SRA, etc.)
  - ALU control (mapping funct3/funct7 to ALU opcode)
  - Clear module interface and parameterisation (e.g. XLEN)
- Tests:
  - Unit tests in tb/single_cycle/alu_tb.cpp (GoogleTest / Verilator)
  - Directed tests for edge cases (overflow, negative numbers, shifts)

### Task SC2 — Register File
- Path: rtl/single_cycle/regfile/
- Branch name: feature/regfile
- Responsibilities:
  - 32 x XLEN register file, x0 hard-wired to 0
  - 2 read ports, 1 write port
  - Synchronous write, combinational read
- Tests:
  - tb/single_cycle/regfile_tb.cpp
  - Ensure x0 cannot be modified

### Task SC3 — Program Counter & Fetch
- Path: rtl/single_cycle/pc/
- Branch name: feature/pc_fetch
- Responsibilities:
  - Program counter register, next-PC logic (pc+4, branch target, jump target)
  - Instruction memory interface (simple IMEM for single-cycle version)
- Tests:
  - tb/single_cycle/pc_tb.cpp
  - Check branch/jump behaviour and reset behaviour

### Task SC4 — Control Unit + Immediate Generator
- Paths:
  - rtl/single_cycle/control/
  - rtl/single_cycle/immgen/
- Branch name: feature/control_imm
- Responsibilities:
  - Decode RV32I instructions and generate control signals
  - Immediate extraction for I, S, B, U, J types
- Tests:
  - tb/single_cycle/control_tb.cpp
  - Check all major instruction types map to correct control signals

### Task SC5 — Data Memory Interface
- Path: rtl/single_cycle/datapath/
- Branch name: feature/data_mem
- Responsibilities:
  - Simple data memory (LW, SW, optionally LB/LH/SB/SH)
  - Memory alignment / masking strategy
- Tests:
  - tb/single_cycle/data_mem_tb.cpp
  - Test loads/stores across boundaries

### Task SC6 — Single-Cycle Top-Level Integration
- Path: rtl/single_cycle/top/
- Branch name: feature/single_cycle_top
- Responsibilities:
  - Connect PC, IMEM, control, regfile, ALU, data memory
  - Define external interface (clock, reset, instruction/data memory, debug outputs)
- Tests:
  - tb/single_cycle/cpu_tb.cpp
  - Run basic programs from sw/reference/ (e.g. small arithmetic, branch tests)

---

## Phase 2 — Pipelined CPU (5-stage)

Goal: 5-stage RV32I pipeline (IF, ID, EX, MEM, WB) with hazards handled correctly.

### Task PL1 — Pipeline Registers
- Path: rtl/pipelined/pipeline_registers/
- Branch name: feature/pipeline_regs
- Responsibilities:
  - IF/ID, ID/EX, EX/MEM, MEM/WB pipeline registers
  - Control of stalls and flushes via enable/clear signals
- Tests:
  - tb/pipelined/pipeline_regs_tb.cpp

### Task PL2 — Hazard Detection Unit
- Path: rtl/pipelined/hazard_unit/
- Branch name: feature/hazard_unit
- Responsibilities:
  - Detect load-use hazards, control hazards, structural hazards (if any)
  - Generate stall and flush signals
- Tests:
  - tb/pipelined/hazard_tb.cpp

### Task PL3 — Forwarding Unit
- Path: rtl/pipelined/forwarding_unit/
- Branch name: feature/forwarding_unit
- Responsibilities:
  - Generate forwarding control signals to avoid unnecessary stalls
  - Handle EX/MEM → ID/EX, MEM/WB → ID/EX paths
- Tests:
  - tb/pipelined/forwarding_tb.cpp

### Task PL4 — Pipelined Top-Level Integration
- Path: rtl/pipelined/top/
- Branch name: feature/pipelined_top
- Responsibilities:
  - Connect single-cycle components into 5-stage pipeline
  - Integrate hazard & forwarding units and pipeline registers
- Tests:
  - tb/pipelined/cpu_pipelined_tb.cpp
  - Run reference programs and compare behaviour against single-cycle model

---

## Phase 3 — Memory Hierarchy (Caches + Main Memory)

Goal: Add realistic memory hierarchy to pipelined CPU.

### Task MH1 — L1 Instruction Cache
- Path: rtl/memory_hierarchy/l1i/
- Branch name: feature/l1i
- Responsibilities:
  - Direct-mapped or 2-way set associative I-cache
  - Single-cycle hit, miss handling via L2
- Tests:
  - tb/memory/l1i_tb.cpp

### Task MH2 — L1 Data Cache
- Path: rtl/memory_hierarchy/l1d/
- Branch name: feature/l1d
- Responsibilities:
  - Load/store buffer, data cache behaviour
  - Write policy (write-back or write-through)
- Tests:
  - tb/memory/l1d_tb.cpp

### Task MH3 — L2 Cache + Arbiter
- Paths:
  - rtl/memory_hierarchy/l2/
  - rtl/memory_hierarchy/arbiter/
- Branch name: feature/l2_cache
- Responsibilities:
  - Shared L2 cache between I/D caches
  - Arbitration between I-cache and D-cache requests
- Tests:
  - tb/memory/l2_tb.cpp

### Task MH4 — Main Memory Model
- Path: rtl/memory_hierarchy/main_mem/
- Branch name: feature/main_mem
- Responsibilities:
  - Simple main memory model with configurable latency
- Tests:
  - tb/memory/main_mem_tb.cpp

---

## Phase 4 — Extensions

### Task EX1 — M Extension (Mul/Div)
- Path: rtl/extensions/muldiv/
- Branch name: feature/m_extension
- Responsibilities:
  - Implement RV32M instructions
  - Integrate into ALU/control
- Tests:
  - tb/single_cycle/m_extension_tb.cpp
  - Dedicated assembly in sw/demos/

### Task EX2 — MMIO / Vbuddy Interface
- Path: rtl/extensions/mmio/
- Branch name: feature/mmio_vbuddy
- Responsibilities:
  - Map Vbuddy registers via memory-mapped I/O region
- Tests:
  - tb/vbuddy/f1_tb.cpp
  - Demo programs in sw/demos/

---

## Phase 5 — Verification & Tooling

### Task V1 — Unit Testbenches
- Path: tb/single_cycle/, tb/pipelined/, tb/memory/
- Branch name: feature/unit_tests
- Responsibilities:
  - GoogleTest + Verilator for all core modules

### Task V2 — System-Level Testbenches
- Path: tb/single_cycle/, tb/pipelined/
- Branch name: feature/system_tests
- Responsibilities:
  - Full CPU simulations running reference and demo programs

### Task V3 — CI Integration
- Path: tools/ci/
- Branch name: feature/ci
- Responsibilities:
  - GitHub Actions workflow
  - Build + run tests on each PR

