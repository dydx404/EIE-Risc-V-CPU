# Contributing Guidelines — EIE RISC-V CPU

We use a **feature-branch + PR** workflow. No one pushes directly to `main`.

## Branches

- `main` — always stable, known-good CPU version.
- `develop` — integration branch for features (we’ll create it after the skeleton is ready).
- `feature/*` — one branch per task/feature.

### Feature branch naming

Use names that match tasks in `docs/TASKS.md`, for example:

- `feature/alu`
- `feature/regfile`
- `feature/control_imm`
- `feature/pc_fetch`
- `feature/data_mem`
- `feature/single_cycle_top`
- `feature/pipeline_regs`
- `feature/hazard_unit`
- `feature/forwarding_unit`
- `feature/l1i`
- `feature/l1d`
- `feature/l2_cache`
- etc.

## Basic workflow

1. **Sync `develop`**

   ```bash
   git checkout develop
   git pull origin develop
2. **Create a feature branch**
   ```bash
   git checkout -b feature/<name>
3. **Implement the feature**
   RTL goes in the relevant rtl/ subfolder.

   Testbenches go in the relevant tb/ subfolder.

   If you change the architecture or interfaces, update docs/ (especially docs/TASKS.md or design notes).
4. **Run tests locally**
   Make sure the code compiles with Verilator.

   Run the relevant unit/system testbenches.

   Do not open PRs that are known to be broken.
5. **Commit**
   ```bash
   git add <files>
   git commit -m "Short, meaningful summary"
6. **Push**
   ```bash
   git push -u origin feature/<name>
7. **Open a Pull Request**
   Target branch: `develop`

   Link the corresponding task in docs/TASKS.md.

   Describe:

   What you implemented

   How you tested it

   Any limitations or TODOs
   
8. **Review & Merge**

    At least one other team member reviews and approves.

    CI (once set up) must pass.

    After approval, the PR is merged into develop.


