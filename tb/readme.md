# Testbenches (Verilator + C++)

All simulation and verification code.

Subfolders:
- `single_cycle/` — tests for 1-cycle CPU modules
- `pipelined/`    — tests for pipeline + hazard logic
- `memory/`       — cache + memory subsystem tests
- `vbuddy/`       — tests using Vbuddy interface
- `unit/`         — small unit tests (ALU, regfile, etc.)
- `system/`       — full CPU system tests

Goal: verify correctness at module and system level.
