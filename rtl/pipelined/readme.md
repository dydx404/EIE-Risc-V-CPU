# Pipelined CPU (5-Stage)

This folder contains the 5-stage pipeline version of the CPU.

Stages (classic RISC):
- `if_stage/`   — instruction fetch
- `id_stage/`   — decode + regfile read
- `ex_stage/`   — ALU + branch decision
- `mem_stage/`  — memory access
- `wb_stage/`   — writeback

Other components:
- `pipeline_registers/` — IF/ID, ID/EX, EX/MEM, MEM/WB
- `hazard_unit/`        — load-use stalls and flushes
- `forwarding_unit/`    — bypass paths to avoid stalls
- `top/`                — pipelined CPU top-level

Goal: same behaviour as single-cycle, higher throughput.
