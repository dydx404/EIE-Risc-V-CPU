# Mingze Chen Personal Statement {#top}

- [Mingze Chen Personal Statement {#top}](#mingze-chen-personal-statement-top)
  - [Overview](#overview)
  - [RTL Codes](#rtl-codes)
    - [PC](#pc)
    - [Instruction Memory](#instruction-memory)
    - [Fetch](#fetch)
    - [Hazard Unit](#hazard-unit)
    - [Pipeline Top](#pipeline-top)
    - [Decode](#decode)
    - [Execute](#execute)
    - [Pipeline Registers](#pipeline-registers)
  - [Testbenches](#testbenches)
    - [Vbuddy Implementation](#vbuddy-implementation)
      - [F1](#f1)
      - [Reference](#reference)
        - [Sine](#sine)
        - [Triangle](#triangle)
        - [Noisy](#noisy)
        - [Gaussian](#gaussian)
  - [Key Influences](#key-influences)
  - [Shortcomings](#shortcomings)
  - [Reflection](#reflection)

[^1]: *For these I only took part of the credit

## Overview

[Back to top](#top)

Originally, I was designated to be responsible for the PC/fetch related RTL codes and debugging. However, we are short on manpower on other sections, so I took as many tasks as possible to speed up the progress. This comes with an unexpected benefit that I touched all parts and workflow of the processor therefore gained solid understanding on each of the components and their underlying principles.

## RTL Codes

[Back to top](#top)

In this section, I’ll detail all the RTL code that I write for the single-cycle and pipeline RISC-V processor.
This includes the code that I started from scratch and my modification to others code in order for it to work with Each other.

### PC

[Back to top](#top)

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

[Back to top](#top)

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

[Back to top](#top)

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

[Back to top](#top)

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

[Back to top](#top)

The pipeline top connects all the stages together and exposes all the important pins for the user and for debugging. The code is too long to be shown here.
It is simple overall, but it is always the final integration that you find out that previous decisions are not compatible with each other, so I had to do a lot of fixing on each stage to make sure the wire names are right and missing features are added back to complete RV32I.

### Decode

[^1]
[Back to top](#top)

Decode is one of the stages that I need to fix and add new features in.
Because the fetch changed and no longer uses “pcsrc”, CU needs to change too.  Mainly separate all things with “pcsrc” to “branch”, “jump” and “jalr” for logic in other stages. Also, I decided to implement “BLT/U”, “BGE/U”, so CU needs to send new control signals to the ALU.

```sv
 // ====================== JAL ================================
        7'b1101111: begin
            RegWrite  = 1'b1;
            ImmSrc    = 3'b011;  // J-type
            ResultSrc = 2'b10;   // PC+4
            //PCSrc     = 2'b01;   // PC + imm
            branchD   = 1'b0;
            jumpD     = 1'b1;
            jalrD     = 1'b0;
            ALUControl = ALU_ADD;
            ALUSrc    = 1'b0;
        end

// ====================== BRANCHES ============================
        7'b1100011: begin
            RegWrite   = 1'b0;
            ImmSrc     = 3'b010; // B-type
            ALUSrc     = 1'b0;
            ALUControl = ALU_SUB;
            jumpD     = 1'b0;
            jalrD     = 1'b0;
            branchD   = 1'b1;

            case (funct3)
                3'b000 : ALUControl = ALU_SUB;   // beq
                3'b001 : ALUControl = 4'b1100;   // bne
                3'b100 : ALUControl = ALU_SLT;   // blt
                3'b101 : ALUControl = 4'b1001;   // bge
                3'b110 : ALUControl = ALU_SLTU;   // bltu
                3'b111 : ALUControl = 4'b1010;   // bgeu
                default: branchD = 1'b1;
            endcase
        end
```

This is just one example of each. CU is more than this (of course).

### Execute

[^1]
[Back to top](#top)

Execute is another one of the stages that needs to be modified. Same reason with Decode, to implement new branch instructions (BLT/U, BGE/U), ALU needs to receive the new signal and act correspondingly.

```sv
 always_comb begin
        unique case (alu_ctrl)
            // SLT (signed) // BLT
            4'b0101: begin
                if ($signed(aluop1) < $signed(aluop2))
                    aluout = {{(LEN-1){1'b0}}, 1'b1};
                else
                    aluout = {LEN{1'b0}};
            zero = (aluout == {LEN{1'b1}});
            end
            // SLTU (unsigned) // BLTU
            4'b0110: begin
                if (aluop1 < aluop2)
                    aluout = {{(LEN-1){1'b0}}, 1'b1};
                else
                    aluout = {LEN{1'b0}};
            zero = (aluout == {LEN{1'b1}});
            end
            4'b1001: begin
                     aluout = (aluop1 >= aluop2) ? 1 : 0;       // BGE
                     zero = (aluout == {LEN{1'b1}});
            end
            4'b1010: begin
                     aluout = ($unsigned(aluop1) >= $unsigned(aluop2)) ? 1 : 0;
                     zero = (aluout == {LEN{1'b1}});              // BGEU
            end
            4'b1100: begin
                     aluout = aluop1 - aluop2;
                     zero = (aluout != {LEN{1'b0}});               // BNE
            end     
        endcase
    end
```

Code segments that I modified.

### Pipeline Registers

[^1]
[Back to top](#top)

For the Pipeline registers, due to some miscommunications, some signals that were not needed got passed through and some needed didn’t. The mistakes in the process will be elaborated in [Shortcomings](#shortcomings).

## Testbenches

[Back to top](#top)

### Vbuddy Implementation

[Back to top](#top)

#### F1

#### Reference

[^1]

##### Sine

##### Triangle

##### Noisy

##### Gaussian

## Key Influences

[Back to top](#top)

## Shortcomings

[Back to top](#top)

## Reflection

[Back to top](#top)
