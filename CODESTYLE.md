# Coding Style Guide (RISC-V CPU)

## SystemVerilog

### Naming

- **Signals:** \`lower_snake_case\`  
  e.g. \`clk\`, \`rst\`, \`alu_op1\`, \`pc_src\`
- **Modules / files:** \`PascalCase\` for modules, file = module.sv  
  e.g. \`ALU.sv\`, \`ControlUnit.sv\`, \`ProgramCounter.sv\`
- **Parameters:** \`UPPER_SNAKE_CASE\`  
  e.g. \`XLEN\`, \`DATA_WIDTH\`
- **Registered vs combinational:**
  - combinational: \`foo\`
  - registered (flop output): \`foo_q\`

### Structure

- One module per file.
- Synchronous logic in \`always_ff @ (posedge clk)\`.
- Pure combinational logic in \`always_comb\`.
- No implicit nets; use \`logic\` explicitly.

## C++ Testbenches

- \`lower_snake_case\` for variables.
- \`CamelCase\` for GTest fixtures.
- Put all testbenches under \`tb/\` with a clear name:
  - \`tb/unit/alu_tb.cpp\`
  - \`tb/system/riscv_single_cycle_tb.cpp\`

