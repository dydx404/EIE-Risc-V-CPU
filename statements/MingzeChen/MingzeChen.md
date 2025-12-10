# Mingze Chen Personal Statement

## Content

- Overview
- RTL Codes
  - PC
  - Instruction Memory
  - Fetch
  - Hazard Unit
  - Pipeline Top
  - Decode [^1]
  - Execute [^1]
  - Pipeline Registers [^1]
- Testbenches
  - Vbuddy Implementation
    - F1
    - Reference [^1]
      - Sine
      - Triangle
      - Noisy
      - Gaussian
- Key Influences
- Shortcomings
- Reflection

[^1]: For these I only took part of the credit

## Overview

Originally, I was designated to be responsible for the PC/fetch related RTL codes and debugging. However, we are short on manpower on other sections, so I took as many tasks as possible to speed up the progress. This comes with an unexpected benefit that I touched all parts and workflow of the processor therefore gained solid understanding on each of the components and their underlying principles.

## RTL Codes

In this section, I’ll detail all the RTL code that I write for the single-cycle and pipeline RISC-V processor.
This includes the code that I started from scratch and my modification to others code in order for it to work with Each other.

### PC

For the program counter I originally took the approach of separate components and a top to interface with other components, but in the end, this turned out to be tedious and over-complicated. So, I turned to a flat implementation of the program counter.

```sv
  module PCFlat #(
    DATA_WIDTH = 32
) (
    input   logic                     clk,
    input   logic                     rst,
    input   logic [1:0]               pcsrc,
    input   logic [DATA_WIDTH-1:0]    immop,
    input   logic [DATA_WIDTH-1:0]    alu_out,
    output  logic [DATA_WIDTH-1:0]    pc
);
    // Wires
    logic [DATA_WIDTH-1:0] pc_plus4;
    logic [DATA_WIDTH-1:0] pc_target;
    logic [DATA_WIDTH-1:0] pc_next;
    logic [DATA_WIDTH-1:0] jalr_target;

    // PC + 4
    assign pc_plus4 = pc + 4;

    // PC Target
    assign pc_target = pc + immop;

    // JALR Target is directly from alu_out
    assign jalr_target = alu_out;
    // assign jalr_target = {alu_out[DATA_WIDTH-1:1], 1'b0}; // Ensure LSB is 0

    // Mux
    always_comb begin
        unique case (pcsrc)
            2'b00: pc_next = pc_plus4;
            2'b01: pc_next = pc_target;
            2'b10: pc_next = jalr_target;
            default: pc_next = '0;
        endcase
    end

    // PC Register
    always_ff @(posedge clk or posedge rst) begin
        if (rst) begin
            pc <= '0; // Set to value needed on rst
        end else begin
            pc <= pc_next;
        end
    end
endmodule
```

### Instruction Memory

For the instruction memory We've decided to go with 32-bit data width Because our assembler I'll output 32-bit machine code per new-line. This meant that We are going to hard wire The instruction memory to BFC0 In the Top And this turned out to work fine.

```sv
module InstrMem #(
    DATA_WIDTH = 32,
    ADDR_WIDTH = 12 // Sets how big the memory is
) (
    input   logic [DATA_WIDTH-1:0]        addr,
    output  logic [DATA_WIDTH-1:0]        instr
);
    // Memory Declaration
    logic [DATA_WIDTH-1:0] memory [0:(1<<ADDR_WIDTH)-1];

    // Initialize Instruction Memory
    initial begin
        $readmemh("program.mem", memory);
        $display("[InstrMem] memory[0] = %h", memory[0]); // For debugging
        $display("[InstrMem] memory[1] = %h", memory[1]);
        $display("[InstrMem] memory[2] = %h", memory[2]);
    end

    // Read Instruction
    assign instr = memory[addr[ADDR_WIDTH+1:2]]; // Word-aligned address
endmodule
```

### Fetch

We decided that for each stage of the pipeline processor, there should be a dedicated hierarchy for better management. Since I work on the PC and instruction memory I work on fetch. And for them to work in the pipeline processor I did some minor modifications.

```sv
module fetch #(
    DATA_WIDTH = 32
) (
    input   logic                     clk,
    input   logic                     rst,
    input   logic                     en,
    input   logic                     branchE,
    input   logic                     jalrinsE,
    input   logic [DATA_WIDTH-1:0]    pc_targetE,
    input   logic [DATA_WIDTH-1:0]    alu_outE,
    output  logic [DATA_WIDTH-1:0]    instrF,
    output  logic [DATA_WIDTH-1:0]    pcF,
    output  logic [DATA_WIDTH-1:0]    pc_plus4F
);

    //PC
    PCFlat pc (
        .clk(clk),
        .rst(rst),
        .en(en),
        .branchE(branchE),
        .jalrinsE(jalrinsE),
        .pc_targetE(pc_targetE),
        .alu_outE(alu_outE),
        .pcF(pcF),
        .pc_plus4F(pc_plus4F)
    );

    //Instruction memory
    InstrMem instrmem (
        .addr(pcF),
        .instrF(instrF)
    );

endmodule
```

In PCFlat I added "en" to stall fetch and break up "pcsrc" into "branchE" and "jalrinsE" to better suit the pipeline structure.

```sv
    always_comb begin
        pc_next = pc_plus4F;        // default

        if (branchE)
            pc_next = pc_targetE;

        if (jalrinsE)
            pc_next = jalr_target;
    end

    // PC Register
    always_ff @(posedge clk or posedge rst) begin
        if (rst)
            pcF <= 32'b0;           // start at 0 for now
        else if (en)
            pcF <= pc_next;
    end
```

### Hazard Unit

In the hazard unit, I took different signals from different stages and used different logic to determine what state the processor is in, and forward data or stall and flush stages according to the case. This took a lot of back and forth in testing to finally decide which signal is reliable and make sure the hazard unit triggers at the right time.

```sv
module HazardUnit(
    input logic [4:0]   rs1E,
    input logic [4:0]   rs2E,
    input logic [4:0]   rs1D,
    input logic [4:0]   rs2D,
    input logic [4:0]   rdE,
    input logic [4:0]   rdM,
    input logic [4:0]   rdW,
    input logic         regWriteM,
    input logic         regWriteW,
    input logic         memReadE,
    input logic         pcsrcE,
    output logic [1:0]  forwardAE,
    output logic [1:0]  forwardBE,
    output logic        stallF,
    output logic        stallD,
    output logic        flushD,
    output logic        flushE
);

    // Forwarding logic for the EX stage
    always_comb begin
        // default: nothing to forward
        forwardAE = 2'b00;
        forwardBE = 2'b00;

        // forwarding from EX/MEM stage (most recent result)
        if (regWriteM && rdM != 5'd0 && rdM == rs1E)
            forwardAE = 2'b10;

        if (regWriteM && rdM != 5'd0 && rdM == rs2E)
            forwardBE = 2'b10;

        // forwarding from MEM/WB stage (only if EX/MEM didn't match)
        if (regWriteW && rdW != 5'd0 && rdW == rs1E && forwardAE == 2'b00)
            forwardAE = 2'b01;

        if (regWriteW && rdW != 5'd0 && rdW == rs2E && forwardBE == 2'b00)
            forwardBE = 2'b01;
    end

    always_comb begin
        // default: no stalls or flushes
        stallF = 1'b0;
        stallD = 1'b0;
        flushD = 1'b0;
        flushE = 1'b0;

        // Load-use hazard detection
        if (memReadE && (rdE != 5'd0) &&
    (rdE == rs1D || rdE == rs2D)) begin

            stallF = 1'b1;
            stallD = 1'b1;
            flushE = 1'b1;
        end

        // Hazard detection logic for branch hazards
        if (pcsrcE) begin
            flushD = 1'b1;
            flushE = 1'b1;
        end
    end
endmodule
```

### Pipeline Top

### Decode*

### Execute*

### Pipeline Registers*

## Testbenches

### Vbuddy Implementation

#### F1

#### Reference*

##### Sine

##### Triangle

##### Noisy

##### Gaussian

## Key Influences

## Shortcomings

## Reflection
