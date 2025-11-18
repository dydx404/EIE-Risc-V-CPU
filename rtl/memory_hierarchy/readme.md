# Memory Hierarchy (Caches + Main Mem)

This folder contains all memory system components.

Subfolders:
- `l1i/`      — L1 instruction cache
- `l1d/`      — L1 data cache
- `l2/`       — shared L2 cache (optional)
- `main_mem/` — memory model with configurable latency
- `arbiter/`  — routes requests between I/D/L2 memories

Goal: add realistic memory behaviour for pipeline designs.
