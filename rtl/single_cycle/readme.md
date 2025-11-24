# Single-Cycle CPU (RV32I)

This folder contains the baseline 1-cycle CPU implementation.

Subfolders:
- `alu/`      — arithmetic/logic unit + ALU control
- `regfile/`  — 32×32 register file
- `pc/`       — program counter + next PC logic
- `control/`  — instruction decoder + control signals
- `immgen/`   — immediate generator (I/S/B/U/J)
- `datapath/` — data memory and helper modules
- `top/`      — single-cycle CPU top-level

Goal: functional RV32I core that can execute simple programs.
