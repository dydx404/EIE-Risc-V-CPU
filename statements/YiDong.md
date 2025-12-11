# Overview

As the Repository Master and Lead Developer, I led the architectural design, implementation, and verification of our team’s RISC-V CPU, evolving it from a single-cycle core into a pipelined design with a cache-backed memory system. My work covered both RTL development (single-cycle, pipeline stages, cache integration) and the verification strategy (unit tests, reference programs, and structured debug instrumentation), as well as the Git workflow that allowed the team to collaborate safely and iteratively.

---

# Yi's Contribution Index

- [Overview](#overview)
- [Yi's Contribution Index](#yis-contribution-index)
  - [1. Workflow Development](#1-workflow-development)
  - [2. Single-Cycle CPU](#2-single-cycle-cpu)
    - [Development](#development)
      - [Data Memory Design](#data-memory-design)
      - [Single-Cycle Top-Level Integration](#single-cycle-top-level-integration)
      - [Control Unit Contribution \[^p\]](#control-unit-contribution-p)
    - [Unit Verifications](#unit-verifications)
    - [CPU-Level Verifications](#cpu-level-verifications)
  - [3. Pipelined CPU](#3-pipelined-cpu)
    - [Development](#development-1)
      - [Pipeline Stage Implementation](#pipeline-stage-implementation)
      - [Execute (EX) Stage](#execute-ex-stage)
      - [Memory (MEM) Stage](#memory-mem-stage)
      - [Writeback (WB) Stage](#writeback-wb-stage)
    - [Unit Verifications](#unit-verifications-1)
    - [Pipeline-Level Verifications](#pipeline-level-verifications)
  - [4. Cache System](#4-cache-system)
    - [Development](#development-2)
      - [4.1 Baseline: Behaviour Required from the Cache](#41-baseline-behaviour-required-from-the-cache)
      - [4.2 MainMemory: Simple 8 KB Word Store](#42-mainmemory-simple-8-kb-word-store)
      - [4.3 DataCache Geometry and Organisation](#43-datacache-geometry-and-organisation)
      - [4.4 Hit Detection and Replacement Policy](#44-hit-detection-and-replacement-policy)
      - [4.5 Write Policy and Partial Stores](#45-write-policy-and-partial-stores)
      - [4.6 Load Semantics and Byte/Halfword Extraction](#46-load-semantics-and-bytehalfword-extraction)
      - [4.7 Miss Handling and Stall Protocol](#47-miss-handling-and-stall-protocol)
    - [Verifications](#verifications)
  - [5. Mistakes I Made](#5-mistakes-i-made)
  - [6. Key Design Decisions](#6-key-design-decisions)
  - [7. Other Contributions](#7-other-contributions)
  - [8. Reflections](#8-reflections)
    - [What I Learned](#what-i-learned)
    - [Mistakes \& Corrections](#mistakes--corrections)
    - [Improvements for the Future](#improvements-for-the-future)
    - [Key Takeaways](#key-takeaways)
  - [9. Commit Records](#9-commit-records)
  - [Commit Appendix](#commit-appendix)

---

## 1. Workflow Development

I designed and enforced a structured Git workflow to support parallel development, verification traceability, and safe system integration across the team. The repository is organised with a clear separation between RTL (`rtl/`), testbenches (`tb/`), RISC-V software (`sw/`), documentation (`docs/`), and tooling, which keeps the project scalable and easy to navigate.

We adopted a feature-branch model (`feature/*`), with `main` reserved strictly for stable, reviewed code. Non-trivial changes were integrated via Pull Requests, using PR/Issue templates to standardise documentation, encourage review (when available), and require basic verification evidence before merge. Branch protection rules prevented direct commits to `main`, reducing regression risk and preserving a readable project history.

Task allocation started with a GitHub Project dashboard to visualise progress and ownership. As the CPU evolved into a pipelined, cached design, technical dependencies between features (e.g. hazards, forwarding, cache stalls) made work more coupled than initially expected, so we increasingly fell back to more direct coordination. Overall, the workflow emphasised isolating experimental work, controlled integration, and verifiable development—mirroring a professional hardware–software co-design environment.

---

## 2. Single-Cycle CPU

### Development

#### Data Memory Design

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

#### Single-Cycle Top-Level Integration

I implemented the **single-cycle Top** module, integrating the PC, instruction memory, control unit, immediate generator, register file, ALU, and the new byte-addressable data memory into a single-cycle RV32I datapath.

Each instruction flows through the classic five conceptual stages—fetch, decode, execute, memory, writeback—but all within one clock cycle:

- **Fetch:** PC and instruction memory provide `pc` and `instr`.  
- **Decode:** The Control Unit and Immediate Generator (Extend) decode `opcode`, `funct3`, `funct7`, and produce both control signals and the sign-extended immediate.  
- **Execute:** The ALU performs arithmetic/logic and branch comparisons, using `ALUSrc` to choose between register and immediate.  
- **Memory:** The DataMemory performs byte/halfword/word loads and stores using `access_ctrl`.  
- **Writeback:** A result multiplexer selects among ALU result, load data, and `pc+4` for register writeback.

The main challenges here were aligning control signals across modules implemented by different team members and keeping the top-level wiring readable. My contribution included standardising signal names and ensuring that the Control Unit, Immediate Generator, and DataMemory agreed on encodings (e.g. for addressing modes and result selection).

#### Control Unit Contribution [^p]

I contributed to the **Control Unit** design, particularly the generation of a structured control vector that cleanly separates instruction decode from datapath behaviour. The Control Unit decodes RV32I instruction fields into:

- register write enable  
- memory read/write enables  
- ALU source selection  
- ALU control codes  
- result selection (ALU vs memory vs PC+4)  
- branch/jump control  

This unified control scheme is reused unchanged in the transition to the pipelined CPU, which reduced the risk of regressions when we later split the datapath into stages.

---

### Unit Verifications

I verified the DataMemory using a Verilator + GoogleTest unit testbench that directly exercised all RV32I load/store widths and sign behaviours. The tests covered:

- Correct LB/LBU, LH/LHU, LW behaviour and sign/zero extension.  
- Little-endian ordering using hand-crafted patterns in memory.  
- Store correctness for SB/SH/SW, checking that only the intended bytes were modified.  
- The `$readmemh` preload offset and the mapping to the memory base address.

I also supported unit verification for the **Control Unit** and **Immediate Generator** [^p], checking that:

- Instruction decodes were consistent with the RV32I spec.  
- Immediate formats (I/S/B/U/J) were correctly extracted and sign-extended.  
- Control signals matched expectations for representative instructions (ALU ops, loads, stores, branches, jumps).

---

### CPU-Level Verifications

To validate the integrated single-cycle CPU, I helped design and/or run several focused assembly programs:

- [**ALU Test:**](../tb/single_cycle/test-alu)
  A compact program that builds operands using `addi` and verifies ADD, SUB, XOR, OR, AND, SLT, SLTU. Results are accumulated into `a0`, which ends at a known value (e.g. 11), confirming correct ALU behaviour and control decoding.

- **“Nerfed” ALU Test (SRA/SRAI Focus):**  
  A minimal program that stresses `sra` and `srai` using negative values (e.g. -16), ensuring the arithmetic right-shift behaves correctly and that immediate decoding matches the register version. Again, the final `a0` value is used as a single pass/fail signal.

- **Load/Store Test:**  
  A program that constructs a base pointer (e.g. `0x100`), performs a series of `sw`/`lw` operations, and checks that the loaded values match what was stored. The final sum in `a0` (e.g. 141) confirms correct memory address calculation, endianness, and DataMemory integration.

- **Immediate Test:**  
  A test that exercises all immediate forms—`addi`, `andi`, `ori`, `xori`, `slti`, `sltiu`, shift immediates, `lui`, and PC-relative branches/jumps. This validates the Immediate Generator and control logic across all RV32I formats in one compact sequence.

- **Reference Program (`pdf.asm`):**  
  Finally, the CPU was run against the official reference program. Correct final register and memory states, as well as correct use of subroutines (JAL), confirmed that the single-cycle core matched the expected architectural behaviour across a realistic, multi-function workload.

Together, these tests gave high confidence that the single-cycle core was architecturally correct before we moved on to pipelining.

---

## 3. Pipelined CPU

### Development

#### Pipeline Stage Implementation

To transition from a single-cycle core to a 5-stage pipeline, I implemented the **Execute (EX)**, **Memory (MEM)**, and **Writeback (WB)** stages and wired them into the IF–ID–EX–MEM–WB structure, using dedicated pipeline registers (ID/EX, EX/MEM, MEM/WB).

The work included:

- Extracting the datapath logic from the single-cycle top and placing it into stage-specific modules.  
- Defining clean interfaces for each stage (data, control, and forwarding signals).  
- Standardising control-signal propagation across pipeline boundaries.  
- Integrating the Hazard Unit so that stalls, flushes, and forwarding correctly control stage inputs.

This modularisation turned the CPU from a monolithic block into a set of well-defined stages, each easier to reason about and verify in isolation.

#### Execute (EX) Stage

The Execute stage handles:

- Operand selection and forwarding.  
- ALU execution.  
- Branch target computation.  
- Branch decision (`branchTakenE`).

Two forwarding multiplexers select the actual ALU operands:

- `forwardAE` chooses among the original `rd1E`, the MEM stage result `aluResultM`, or the WB result `resultW`.  
- `forwardBE` does the same for operand B, and its output is also used as store data (`writeDataE`).

`ALUSrcE` then chooses between the forwarded register operand and the sign-extended immediate. The ALU produces the arithmetic result and zero flag. The branch target is computed as `pcE + extImmE`, and the final branch decision is `branchTakenE = branchE && zeroE`, making the EX stage the point where both arithmetic and control-flow decisions are resolved.

#### Memory (MEM) Stage

The MEM stage is a thin wrapper around the (now pipelined) data memory:

- It uses the ALU result as the address.  
- It forwards store data from EX.  
- It drives the existing byte-addressable DataMemory with the appropriate load/store controls.

By keeping this stage simple and state-light, we made it straightforward to later swap the raw DataMemory for a cache-backed two-level memory without having to modify the EX and WB stages.

#### Writeback (WB) Stage

The WB stage is a pure combinational multiplexer that selects the final register write value:

- `00` → ALU result (`aluResultW`)  
- `01` → memory read data (`readDataW`)  
- `10` → `pcPlus4W` (for JAL/JALR)

The output (`resultW`) is written to the register file and simultaneously fed back into the forwarding network to resolve subsequent data hazards.

---

### Unit Verifications

I wrote unit tests (Verilator + GoogleTest) for several pipeline components:

- **Execute Stage:**  
  Tested operand forwarding (from MEM and WB), ALU operation with and without immediate, store-data forwarding, and branch resolution. Specific cases forced `forwardAE/forwardBE` to each source and checked the resulting ALU output and branch behaviour.

- **Hazard Unit:**  
  Verified basic behaviour with no hazards (all control lines deasserted), then tested classic load-use hazards where a load in EX feeds a dependent instruction in ID. The tests confirm that IF and ID stall, EX flushes, and forwarding paths are used only when safe. Branch resolution tests checked correct flushing of decode/execute stages when `pcsrcE` is asserted.

- **ID Stage:**  
  Confirmed that register file writes and reads behaved correctly, including enforcing `x0` as hard-wired zero. Tested decode and control-vector generation across R-type, I-type loads, S-type stores, B-type branches, JAL/JALR, and U-type instructions.

- **MEM Stage:**  
  Tested all load/store widths (byte/halfword/word) and sign behaviours, using edge cases such as `0x80` and `0x8000`, verifying both endianness and sign/zero extension. Confirmed that `memReadM` gating behaved as expected.

- **Writeback Stage:**  
  Tested all `resultSrcW` cases and ensured undefined values default safely to the ALU path, preventing accidental propagation of garbage into the register file.

These unit tests gave stage-local confidence before integrating everything into the full pipelined CPU.

---

### Pipeline-Level Verifications

For the full pipelined CPU, rather than building a heavy instruction-level unit test harness, I verified correctness by:

- Running the official reference programs in the pipeline top under Verilator.  
- Instrumenting each stage (IF, ID, EX, MEM, WB) with structured debug printouts that log key signals every cycle: PC, instruction fields, ALU inputs/outputs, branch decisions, memory accesses, and writeback values.  
- Logging hazard-control signals (stalls, flushes, forwarding selections) to understand pipeline behaviour without always opening waveforms.

This textual execution trace acts like a human-readable “waveform with commentary”. It made it far easier to see where things went wrong (e.g. a branch taken but not flushed, or a load-use hazard without stall) and iterate quickly, especially when debugging late-stage integration issues. Once the trace aligned with expected architectural behaviour for all reference programs, we had strong evidence that the pipeline was functionally correct.

---

## 4. Cache System

### Development

After the pipelined CPU was stable with a simple byte-addressable DataMemory, I extended the design into a **two-level memory hierarchy** by inserting a data cache between the pipeline and a backing MainMemory. The objective was not to build an extremely aggressive cache, but to introduce *realistic* memory latency while preserving the architectural contract previously provided by DataMemory (byte-level loads/stores, little-endian behaviour, and correct RV32I semantics).

At a high level, the new hierarchy consists of:

- a **DataCache** module directly connected to the MEM stage, and  
- a simple **MainMemory** module acting as the next level in the hierarchy.

The MEM stage now talks exclusively to the DataCache, and the DataCache is responsible for maintaining the illusion of a byte-granular, low-latency memory whenever there is a cache hit.

#### 4.1 Baseline: Behaviour Required from the Cache

The original single-cycle DataMemory implementation was a fully byte-addressable 128 KB array with the following properties:

- **Byte/halfword/word support:** LB/LBU, LH/LHU and LW were all implemented correctly, using explicit sign or zero extension.  
- **SB/SH/SW stores:** Partial-width stores merged bytes and halfwords into existing words at the correct byte offsets.  
- **Little-endian semantics:** Multi-byte values were laid out and reconstructed in little-endian order.  
- **Single-cycle timing model:** Loads were effectively combinational from the CPU’s point of view, while stores committed on the rising clock edge.  
- **Bounded address space:** Only the lower address bits were used to index memory, keeping the simulation safe and consistent with the lab’s memory map.

When adding the cache, I treated this behaviour as a **contract** the new memory subsystem must satisfy. In other words, the DataCache + MainMemory pair must appear indistinguishable from the original DataMemory as far as the pipeline is concerned, at least on architecturally visible behaviour.

#### 4.2 MainMemory: Simple 8 KB Word Store

The backing store is implemented as a compact, word-addressable memory:

- 8 KB total capacity, organised as 2048 words of 32 bits each.
- Addressing via `addr[12:2]`, i.e. word-aligned access only.
- A simple ready/valid-style interface:
  - `mem_read` / `mem_write` request signals,
  - `rdata` and `ready` response.

In the current design, each memory access returns in a single cycle once `mem_read` or `mem_write` is asserted, and `ready` is used to signal completion. This is intentionally minimalistic; all complexity of byte extraction, partial stores, and line refills are handled in the cache above it.

#### 4.3 DataCache Geometry and Organisation

The DataCache implements a **4 KB, 2-way set-associative cache with 16-byte lines**.

Key parameters:

- **Line size:** 16 B = 4 × 32-bit words (`LINE_WORDS = 4`).  
- **Associativity:** 2 ways (`NUM_WAYS = 2`).  
- **Number of sets:** 128 (`NUM_SETS = 128`).  
- **Address breakdown:**
  - `byte_offset = addrM[1:0]`
  - `word_offset = addrM[3:2]`
  - `set_index = addrM[10:4]`  (7 bits → 128 sets)
  - `tag = addrM[31:11]`       (remaining 21 bits)

Each cache line stores:

- a 4-word data array `data[way][set][word]`,  
- a tag `tag[way][set]`,  
- a valid bit `valid[way][set]`, and  
- a per-set LRU bit `lru[set]` to choose the victim on replacement.

This geometry is deliberately modest but realistic for a teaching CPU: it is large enough to hold the working sets of the test programs while still being simple enough to fully understand.

#### 4.4 Hit Detection and Replacement Policy

For any incoming memory access from the MEM stage (identified by address `addrM`):

- The cache computes **set index** and **tag**.
- It checks both ways in the selected set:
  - `hit0 = valid[0][set] && (tag[0][set] == addr_tag)`
  - `hit1 = valid[1][set] && (tag[1][set] == addr_tag)`
- A global `hit` signal is asserted when either way matches.

If both ways are valid and none match, the cache chooses a victim using a **per-set LRU bit**:

- The LRU bit flips on each hit, recording which way was most recently used.
- On replacement, if both lines are valid, the less recently used way is evicted.
- If one way is invalid, that empty slot is used instead of evicting a valid line.

This gives a simple but effective replacement policy, with very low hardware overhead.

#### 4.5 Write Policy and Partial Stores

The cache implements a **write-through, write-allocate** policy:

- **Write-through:** On a write hit, the cache updates its own data array *and* forwards the write to MainMemory. This keeps MainMemory as the single source of truth and avoids the complexity of dirty bits and write-back logic.
- **Write-allocate:** On a write miss, the cache first refills the entire line from MainMemory, and then applies the write to the new line. This keeps the cache coherent with the kinds of access patterns produced by typical RISC-V code.

Partial-width stores are handled by computing a **merged store word**:

- For SB (store byte), the updated byte is inserted into the correct 8-bit slice of the old 32-bit word.
- For SH (store halfword), the updated 16-bit halfword is inserted into the correct 16-bit slice.
- For SW (store word), the entire 32-bit word is replaced.

On write hits, this merged word is written back into the cache data array and sent to MainMemory. This logic carefully mirrors the behaviour of the original DataMemory, so the semantics of SB/SH/SW remain identical.

#### 4.6 Load Semantics and Byte/Halfword Extraction

On read hits, the cache reconstructs the loaded value using the same ideas as the original DataMemory:

- A full 32-bit word is fetched from the appropriate way/set/word.
- `byte_offset` selects which byte within that word to use for LB/LBU.
- `byte_offset[1]` selects which halfword within the word to use for LH/LHU.
- Sign/zero extension is applied based on the 3-bit `addressingModeM` control.

Concretely:

- **LB:** select the byte and sign-extend it to 32 bits.  
- **LBU:** select the byte and zero-extend it.  
- **LH:** select the 16-bit halfword and sign-extend it.  
- **LHU:** select the 16-bit halfword and zero-extend it.  
- **LW:** return the full 32-bit word.

On a hit, this logic is effectively combinational from the pipeline’s point of view: the MEM stage does not see an extra cycle of latency compared to the original DataMemory. This is important for keeping the hazard and forwarding logic valid without modification.

#### 4.7 Miss Handling and Stall Protocol

Miss handling is managed by a small finite state machine with two states:

- `IDLE` – normal operation (hits or idle cycles),
- `REFILL` – active line refill from MainMemory on a miss.

When a miss is detected (for either a read or a write):

1. The cache captures the **miss address**, its tag, and the target set.
2. It chooses a victim way (empty way if possible, otherwise LRU).
3. The **refill base address** is computed by aligning the miss address down to a 16-byte boundary.
4. The cache enters the `REFILL` state and begins reading four consecutive 32-bit words from MainMemory, one per cycle (or per `mem_ready`).
5. Each word is stored into `data[miss_way][miss_set][refill_cnt]`.
6. After the last word:
   - `tag[miss_way][miss_set]` and `valid[miss_way][miss_set]` are updated.
   - The per-set LRU bit is updated.
   - The cache returns to `IDLE`.

To maintain correctness, the cache exposes a `stallM` signal back to the pipeline:

- On hits, `stallM` is deasserted and the MEM stage behaves as in the uncached design.
- On misses, `stallM` is asserted, and the pipeline is held until the refill completes.

In the current integration, this stall is used to freeze the parts of the pipeline that depend on the MEM stage, effectively turning a cache miss into a temporary multi-cycle memory access while preserving all existing hazard behaviour.

---

### Verifications

Due to time constraints and because I was the only team member working on the cache subsystem, verification for the cached design focused on **reusing the existing reference programs** rather than building a new, cache-specific unit test suite.

Concretely, I tested the cached system using:

- The standard **F1 program** used earlier in the course.  
- The first four programs from the `tb/asm` test suite (ALU, loads/stores, immediates, and branches/jumps).  
- The official `pdf.asm` reference program used to validate overall CPU behaviour.

For the first four `tb/asm` programs, the cached pipeline produced the **same architectural results** (register contents and relevant memory locations) as the known-good uncached pipeline. This provides strong evidence that:

- cache hits correctly preserve load/store semantics,  
- misses and refills do not corrupt memory state, and  
- the stall protocol does not break the existing hazard and forwarding logic for typical instruction sequences.

The cache was instrumented with debug printouts (logging hits, misses, chosen ways, and refill beats), which made it easier to correlate pipeline state with cache events and to confirm that refills were aligned and tagged correctly.

For the full `pdf.asm` program, the cached CPU currently exhibits **PC loops** in some cases, indicating that there is still a subtle bug to be resolved. From the symptoms, this is likely related to an interaction between cache stalls, branch resolution, and/or instruction/data memory alignment rather than a simple arithmetic error. Because of time pressure near the deadline, I prioritised getting the core cache functionality working and validated on the first four tests, and I have marked the `pdf.asm` behaviour as **known outstanding work** to debug in a future iteration.

Even with this limitation, the cache subsystem already demonstrates:

- correct behaviour on a non-trivial suite of tests,  
- realistic memory latency injected into the pipeline, and  
- a clear path for further debugging and extension.

---

## 5. Mistakes I Made

Several mistakes were important learning points:

- **Underestimating Integration Complexity:**  
  I initially assumed that once single-cycle components worked, piping them together would be mostly mechanical. In practice, wiring control signals, hazards, and forwarding paths cleanly was a major source of bugs, especially when different team members had slightly different naming schemes and assumptions.

- **Git Hygiene Under Pressure:**  
  During intense debugging, I occasionally used force-pushes and worked in detached HEAD states, which complicates history and confused branch ownership. This reinforced why branch protection and disciplined PR use matter, especially in a shared repo.

- **Delayed Waveform Use:**  
  While I intentionally relied more on debug printouts than on GTKWave, there were cases where opening a waveform earlier would have exposed signal-timing issues more quickly. Finding the right balance between textual logging and waveform inspection is still a work in progress.

- **Hazard Corner Cases:**  
  Some subtle hazards (e.g. corner cases in load-use and branch-after-load) were not covered by the earliest tests. These only emerged once full programs were run through the pipeline, which highlighted gaps in my initial unit-test coverage assumptions.

---

## 6. Key Design Decisions

Some of the most important design choices were:

- **Byte-Addressable Memories:**  
  Moving both instruction and data memories to byte-addressable designs simplified support for all RV32I load/store instructions and aligned the implementation directly with the ISA’s view of memory.

- **Clean Stage Boundaries:**  
  Implementing EX, MEM, and WB as separate modules with explicit interfaces and pipeline registers made the CPU easier to debug and later extend with caching, compared to embedding all logic in one large top module.

- **Hazard Unit Centralisation:**  
  Centralising stall, flush, and forwarding logic into a dedicated Hazard Unit avoided sprinkling ad-hoc hazard fixes throughout the pipeline stages and made correctness much easier to reason about and test.

- **Debug-First Philosophy:**  
  Adding structured debug printouts in the pipeline top and stages turned Verilator into a powerful trace generator, reducing the need to open waveforms for every minor bug. This was a deliberate tradeoff favouring faster iteration.

- **Simple, Bounded Memory Map:**  
  Truncating addresses and using a 128 KB memory region kept the design realistic while avoiding simulation blowups from allocating a full 4 GB address space.

---

## 7. Other Contributions

Beyond core RTL work, I also:

- Helped define and maintain coding style and file structure conventions across the team.  
- Contributed to Vbuddy testbenches and visualisation setups, especially for earlier labs and for simple single-cycle demos.  
- Create the Team Final Statements 
- Assisted teammates in debugging their modules (e.g. aligning control encodings and resolving off-by-one or sign-extension bugs).

These “infrastructure” tasks were less visible than the main CPU pipeline, but they significantly reduced friction in day-to-day development.

---

## 8. Reflections

### What I Learned

This project was my first end-to-end experience of taking a CPU from a single-cycle teaching design to a pipelined, cache-backed implementation. I learned how quickly complexity grows once you introduce hazards, forwarding, and a memory hierarchy, and why clean interfaces and systematic verification are essential rather than optional.

### Mistakes & Corrections

I saw how early shortcuts—like loose naming or incomplete test coverage—come back as integration bugs later. I corrected this by refactoring interfaces, tightening unit tests, and adopting more disciplined Git practices. The experience made me more cautious about “quick hacks” in shared code.

### Improvements for the Future

If I were to repeat this project, I would:

- Plan the hazard and forwarding scheme earlier and build tests around it from the start.  
- Introduce a minimal CI script to run key tests on every PR.  
- Document design decisions in real time, not only at the end, so that tradeoffs are recorded while still fresh.

### Key Takeaways

- Good architecture is as much about clean boundaries and contracts as it is about internal implementation.  
- Verification must be layered: unit tests, focused assembly tests, and full reference programs all catch different classes of bugs.  
- Tooling and workflow decisions (Git, debug strategy, repo structure) have a direct impact on how fast you can debug, integrate, and extend a design.

---

## 9. Commit Records



The commit history in these branches documents both the incremental implementation of modules and the iterative debugging process (e.g. hazard fixes, forwarding corrections, and cache integration adjustments). Together, they provide an auditable record of my role in the project.

---
## Commit Appendix

| Commit ID | Date | Commit Description |
|-----------|------|--------------------|
| 343acc4 | 2025-12-11 | Remove README to prepare for merge |
| 5cd5cfe | 2025-12-11 | updated statements |
| 9197cdf | 2025-12-10 | docs: progress on individual contribution statement |
| 67fcc0f | 2025-12-10 | On cache-rebuild: temp: cache + statement mixed WIP |
| 08ae5f0 | 2025-12-10 | index on cache-rebuild: 0935cd6 first 4 test good |
| d8a4b14 | 2025-12-10 | untracked files on cache-rebuild: 0935cd6 first 4 test good |
| 6e28374 | 2025-12-10 | WIP: cache pipeline + testbench + program.mem updates |
| 0935cd6 | 2025-12-09 | first 4 test good |
| c2ef213 | 2025-12-08 | Cache rebuild baseline: pipeline + debug + corrected test program |
| 752985c | 2025-12-08 | First four test correct |
| 5f1607e | 2025-12-08 | minimal model |
| fab2db9 | 2025-12-08 | small fix |
| 945387d | 2025-12-08 | small fix |
| 212f364 | 2025-12-07 | Fix regfile debug exposure and improve WB testbench observability |
| bf82fac | 2025-12-07 | Pipeline bring-up: ALU/addi verified, WB path correct, PC stepping validated via IF trace |
| a357767 | 2025-12-07 | WIP: pipeline top integration and hazard rewiring |
| 61a0057 | 2025-12-07 | WIP: pipeline top integration, wiring pass in progress |
| 5ddab16 | 2025-12-07 | Fix true load-use hazard detection and add HazardUnit gtest |
| 85b793a | 2025-12-07 | Fix Decode memReadD signal and update gtest for load-use support |
| 25fff89 | 2025-12-07 | test(wb): verify writeback stage mux for alu mem and PC+4 |
| b90e80a | 2025-12-07 | test(mem): verify MEM stage; load/store widths and sign extension |
| f7845c4 | 2025-12-07 | test(ex): verify Execute stage; forwarding, ALUSrc, and branchTaken logic |
| f30a37c | 2025-12-07 | test(id): verify Decode stage; assert branchD and fix funct7 wiring |
| a02f06f | 2025-12-07 | test(pipelined): scaffold full component verification structure |
| b2eddff | 2025-11-30 | Add Execute stage with forwarding and ALU muxing |
| 626e3ef | 2025-11-30 | Add MEM_STAGE and DataMemory to mem-stage branch |
| ca9afbb | 2025-11-30 | Add MEM_STAGE, DataMemory copy, WB_STAGE |
| 32a9666 | 2025-11-29 | Fix DMEM preload, add MemRead support, fix control path, test-reference integration |
| 2fd7152 | 2025-11-28 | test-reference: added DataMemory preload, program.mem, gaussian.mem, and full test setup |
| 598986f | 2025-11-28 | Overwrite test-reference with fresh copy from test-general |
| c98514b | 2025-11-28 | Full general test program and verification output |
| 85b516f | 2025-11-28 | Add full SRA/SRAI correctness test and verification |
| 67f977b | 2025-11-28 | nerfed arithmatic program test |
| dc4d4a6 | 2025-11-28 | Add regfile-zero test (x0 behaviour verified) |
| 645a4df | 2025-11-28 | Merge branch 'feature/test-immediate' into feature/test-immediate |
| f0becd2 | 2025-11-28 | Add immediate-format test (SRAI still failing, others OK) |
| 6dfb968 | 2025-11-28 | Add ALU test (exposes SRAI bug) |
| 01c93eb | 2025-11-28 | Add load/store test program and testbench for feature/test-loadstore |
| 97ad589 | 2025-11-28 | Add program.s for branch test |
| 4f7d595 | 2025-11-26 | Add standalone top-level CPU integration and testbench (top.sv, top_tb.cpp, program.mem, doit.sh) |
| 022e2da | 2025-11-26 | Resolve conflict: keep CU_verification version of ControlUnit.sv |
| 8a55383 | 2025-11-26 | Fix CU + GTest + doit.sh working version |
| f4034d4 | 2025-11-25 | Add GoogleTest verification suite for ControlUnit |
| c9e8fa9 | 2025-11-25 | Extend verification: add full testbench, updated Extend module, and PC/doit fix |
| 5f4dff8 | 2025-11-24 | Add new DataMemory design |
| c3f076c | 2025-11-20 | fix structure (merge develop) |
| 73d9cab | 2025-11-20 | Restore folder structure from commit a06638c |
| ba71dae | 2025-11-20 | Merge develop → main to preserve file system |
| 0e71153 | 2025-11-20 | Add DataMemory module and complete unit verification |
| 9bd1968 | 2025-11-18 | Add develop branch, PR/issue templates, improved README |
| a06638c | 2025-11-18 | Add complete folder structure and README placeholders |
| fa43638 | 2025-11-18 | Add project structure, tasks, contribution docs |
| 98dde8b | 2025-11-18 | Initial project structure for RISC-V CPU |


[^p]: *Partial credit / supporting role.*
