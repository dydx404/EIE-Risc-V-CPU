# EIE RISC-V CPU

A teaching / experimental RISC-V CPU project for Imperial EIE.

## High-level layout

- \`rtl/\` — SystemVerilog RTL source
  - \`single_cycle/\` — baseline single-cycle core
  - \`pipelined/\` — pipelined core
  - \`cache/\` — cache / memory hierarchy
  - \`common/\` — shared components (muxes, adders, etc.)
  - \`top/\` — top-level SoC / integration modules
- \`tb/\` — testbenches
  - \`unit/\` — unit-level testbenches (ALU, regfile, PC, CU, etc.)
  - \`system/\` — full CPU testbenches
  - \`vbuddy/\` — Vbuddy-specific testbenches and configs
- \`sw/\` — RISC-V programs and test software
  - \`reference/\` — reference programs provided by the course
  - \`demos/\` — custom demo programs
- \`tools/\` — scripts and CI helpers
- \`docs/\` — design and verification documentation

This repo is structured to support:
- clean separation of RTL / testbench / software / tooling
- scalable extensions (pipeline, caches, branch prediction, etc.)
- professional GitHub workflow with CI and code review.
