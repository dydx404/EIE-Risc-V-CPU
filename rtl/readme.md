# RTL (Register-Transfer Level)

This directory contains all hardware modules for the CPU project.

Folder layout (quick):
- `single_cycle/`  — RV32I 1-cycle CPU
- `pipelined/`     — 5-stage pipeline (IF/ID/EX/MEM/WB)
- `memory_hierarchy/` — L1/L2 caches + main memory
- `extensions/`    — Extra ISA (M-ext), CSRs, MMIO
- `common/`        — Reusable blocks (mux, adders, utils)

All modules are written in SystemVerilog (.sv).
