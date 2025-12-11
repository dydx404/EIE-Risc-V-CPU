# Project Tasks Overview — EIE RISC-V CPU

This file defines WHAT we are building and WHERE the work lives in the repo.

---

## Phase 1 — Single-Cycle CPU (baseline)

Goal: Fully functional RV32I single-cycle CPU that passes reference programs.

### Task SC1 — ALU
- Path: `rtl/single_cycle/alu/`
- Branch: `feature/alu`
- Responsibilities:
  - Implement ALU for RV32I ops (ADD, SUB, AND, OR, XOR, SLT, SLTU, SLL, SRL, SRA, etc.)
  - ALU control (decode funct3/funct7 → ALU opcode)
- Tests:
  - `tb/single_cycle/alu_tb.cpp`

### Task SC2 — Register File
- Path: `rtl/single_cycle/regfile/`
- Branch: `feature/regfile`
- Responsibilities:
  - 32×XLEN registers, x0 hard-wired to 0
  - 2 read ports, 1 write port
- Tests:
  - `tb/single_cycle/regfile_tb.cpp`

### Task SC3 — PC & Fetch
- Path: `rtl/single_cycle/pc/`
- Branch: `feature/pc_fetch`
- Responsibilities:
  - Program counter, next-PC logic (pc+4, branch/jump target)
  - Interface to instruction memory
- Tests:
  - `tb/single_cycle/pc_tb.cpp`

### Task SC4 — Control Unit + ImmGen
- Paths: `rtl/single_cycle/control/`, `rtl/single_cycle/immgen/`
- Branch: `feature/control_imm`
- Responsibilities:
  - Decode RV32I instructions to control signals
  - Immediate generator for I/S/B/U/J types
- Tests:
  - `tb/single_cycle/control_tb.cpp`

### Task SC5 — Data Memory / Datapath Helpers
- Path: `rtl/single_cycle/datapath/`
- Branch: `feature/data_mem`
- Responsibilities:
  - Data memory interface (LW, SW; optionally byte/halfword variants)
  - Masking/alignment strategy
- Tests:
  - `tb/single_cycle/data_mem_tb.cpp`

### Task SC6 — Single-Cycle Top-Level
- Path: `rtl/single_cycle/top/`
- Branch: `feature/single_cycle_top`
- Responsibilities:
  - Wire PC, IMEM, control, regfile, ALU, data memory into one core
  - Define external interface (clk, rst, memory buses, debug)
- Tests:
  - `tb/single_cycle/cpu_single_cycle_tb.cpp`
  - Programs from `sw/reference/`

---

## Phase 2 — Pipelined CPU (5-stage)

Goal: 5-stage RV32I pipeline (IF, ID, EX, MEM, WB) matching single-cycle behaviour.

### Task PL1 — Pipeline Registers
- Path: `rtl/pipelined/pipeline_registers/`
- Branch: `feature/pipeline_regs`
- Responsibilities:
  - IF/ID, ID/EX, EX/MEM, MEM/WB registers
  - Stall/flush control inputs
- Tests: `tb/pipelined/pipeline_regs_tb.cpp`

### Task PL2 — Hazard Unit
- Path: `rtl/pipelined/hazard_unit/`
- Branch: `feature/hazard_unit`
- Responsibilities:
  - Load-use hazard detection
  - Control hazard handling (branch/jump)
- Tests: `tb/pipelined/hazard_tb.cpp`

### Task PL3 — Forwarding Unit
- Path: `rtl/pipelined/forwarding_unit/`
- Branch: `feature/forwarding_unit`
- Responsibilities:
  - Select forwarding paths from EX/MEM, MEM/WB
- Tests: `tb/pipelined/forwarding_tb.cpp`

### Task PL4 — Pipelined Top-Level
- Path: `rtl/pipelined/top/`
- Branch: `feature/pipelined_top`
- Responsibilities:
  - Connect stages, pipeline registers, hazard and forwarding units
- Tests:
  - `tb/pipelined/cpu_pipelined_tb.cpp`
  - Compare against single-cycle behaviour.

---

## Phase 3 — Memory Hierarchy

### Task MH1 — L1 Instruction Cache
- Path: `rtl/memory_hierarchy/l1i/`
- Branch: `feature/l1i`

### Task MH2 — L1 Data Cache
- Path: `rtl/memory_hierarchy/l1d/`
- Branch: `feature/l1d`

### Task MH3 — L2 + Arbiter
- Paths: `rtl/memory_hierarchy/l2/`, `rtl/memory_hierarchy/arbiter/`
- Branch: `feature/l2_cache`

### Task MH4 — Main Memory Model
- Path: `rtl/memory_hierarchy/main_mem/`
- Branch: `feature/main_mem`

---

## Phase 4 — Extensions

### Task EX1 — M Extension (Mul/Div)
- Path: `rtl/extensions/muldiv/`
- Branch: `feature/m_extension`

### Task EX2 — MMIO / Vbuddy
- Path: `rtl/extensions/mmio/`
- Branch: `feature/mmio_vbuddy`

---

## Phase 5 — Verification & Tooling

### Task V1 — Unit Testbenches
- Path: `tb/single_cycle/`, `tb/pipelined/`, `tb/memory/`
- Branch: `feature/unit_tests`

### Task V2 — System Testbenches
- Path: `tb/single_cycle/`, `tb/pipelined/`
- Branch: `feature/system_tests`

### Task V3 — CI Integration
- Path: `tools/ci/`
- Branch: `feature/ci`
