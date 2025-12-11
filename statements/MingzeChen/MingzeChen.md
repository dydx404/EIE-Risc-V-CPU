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

After completing all the articles that I am assigned to, I started helping with the testing. Mainly implement the processer on the vbuddy and debug anything that seems not right.

### Vbuddy Implementation

[Back to top](#top)

In this section, I wrote the test bench for the top for it to work on vbuddy and show the results clearly.
I decided to create a mixed testbench that both works with F1 output and PDF output, so that way when I go back and forth between them, I didn't need to change the testbench as well. I just need to change the program memory.

```cpp
int main(int argc, char **argv, char **env)
{
    int simcyc;
    int tick;

    char prog = argv[argc - 1][0];

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vtop *top = new Vtop;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("CPU.vcd");

    // init Vbuddy
    if (vbdOpen() != 1)
    {
        return (-1);
    }
    vbdHeader("CPU");
    vbdSetMode(1);

    // intialise
    top->clk = 1;
    top->rst = 1;
    step_cycle(top);
    top->rst = 0;

    // run simulation for MAX_SIM_CYC clock cycles
    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
    {
        // dump variables into VCD file and toggle clock
        for (tick = 0; tick < 2; tick++)
        {
            tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        // Test data
        if (simcyc > 37530) // gaussian=123705, noisy=204890, triangle=316018, sine=37530
        {
            vbdPlot(top->a0, 0, 255);
            vbdBar(top->a0 & 0xFF);
            vbdCycle(simcyc);
        }

        // either simulation finished, or 'q' is pressed
        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
}
```

The "simcyc" is only for pdf. I've discovered that different data input gives their results at a different cycle. So, I only allow the PC to send to vbuddy when the results are calculated. This minimizes waiting time.

#### F1

The purpose of F1 test is to verify that finite state machines can be run on the processor, so I wrote two assembly codes, one with simple loop and one with finite state machines. They ended up with the same result shown below.

```s
# Loop ver
main:
    JAL ra, init
    j   main

init:
    li s2, 0x0   
    li s3, 0xff  # load s3 with 0xff
    li a0, 0x0   # result reg init at 0
    li s4, 0xf  

loopi:
    slli s2, s2, 1    # shift left by 1
    addi s2, s2, 1    # add 1
    and  a0, s3, s2   # and with 0x11111111 

wait:
    addi s4, s4, -1
    BNE s4, zero, wait
    addi s4, zero, 0xf
    bne s3, s2, loopi
    ADDI a0, zero, 0
    RET
```

```s
# FSM ver
_start:
    # Initialize addresses
    li s0, 0x10000008        
    li s1, 0x10000004        
    li s2, 0x10000000        
    li s4, output_table      
    
    # Initialize state
    li s3, 0                 # curr_state = S0
    sw s3, 0(s0)
    
fsm_loop:
    
    # State transition
    addi s3, s3, 1           # Increment state
    li t0, 9
    blt s3, t0, update_state
    li s3, 0                 # Wrap around
    
update_state:
    j after_reset
      
after_reset:
    # Store state
    sw s3, 0(s0)
    
    # Get output from lookup table
    add t0, s4, s3           # Calculate table offset
    lb t1, 0(t0)             # Load output value
    addi a0, t1, 0           # Output
    
    li t0, 1000
delay:
    addi t0, t0, -1
    bne t0, zero, delay
    
    j fsm_loop

# Output lookup table
.data
.align 2
output_table:
    .byte 0x00    # S0: 0
    .byte 0x01    # S1: 1
    .byte 0x03    # S2: 3
    .byte 0x07    # S3: 7
    .byte 0x0F    # S4: 15
    .byte 0x1F    # S5: 31
    .byte 0x3F    # S6: 63
    .byte 0x7F    # S7: 127
    .byte 0xFF    # S8: 255
```

Add the video of them working
[F1](https://github.com/dydx404/EIE-Risc-V-CPU/tree/main/statements/MingzeChen/video/F1.mp4)

#### Reference

[^1]

For the reference testing with pdf, I didn't write the testing code but helped with debugging and implement the final testing on vbuddy to confirm the results.

Like mentioned, different data inputs require different sim cycles to acquire the results at the right time. Through testing they appear to be gaussian=123705, noisy=204890, triangle=316018, sine=37530.

Folowing are the videos of pdf results.

##### [Sine](https://github.com/dydx404/EIE-Risc-V-CPU/tree/main/statements/MingzeChen/video/Sine.mp4)

##### [Triangle](https://github.com/dydx404/EIE-Risc-V-CPU/tree/main/statements/MingzeChen/video/Triangle.mp4)

##### [Noisy](https://github.com/dydx404/EIE-Risc-V-CPU/tree/main/statements/MingzeChen/video/Noisy.mp4)

##### [Gaussian](https://github.com/dydx404/EIE-Risc-V-CPU/tree/main/statements/MingzeChen/video/Gaussian.mp4)

## Key Influences

[Back to top](#top)

As I am working on the top-level implementation of the pipeline processor, a lot of my key influences come from this. Originally in the single cycle processor implementation, we separate the processor into two main paths, namely control path and data path. But this separation does not make sense when grouping files and assign tasks in the pipeline processor. So, I decided that we should construct multiple stage modules that group smaller basic components together for us to better cooperate and debug the whole processor.

I also work on the program counter of both the single cycle and pipeline processor. And I decided instead of creating a mid-level top for PC and its smaller modules, it should be implemented all in a single file do not over complicate things.

## Shortcomings

[Back to top](#top)

In the beginning of the project, I am too excited and dived straight in, without reading the criteria of the testing in the GitHub page. This created a lot of issues later that I have to go back and fix my own mistakes.

Also, I am sort of the head of the pipeline processor, but my leadership skills do not seem to accompany that. I often give too little instructions to my teammates, so they end up giving me something that is not what I imagined, and I have to fix their stuff and waste precious time.

## Reflection

[Back to top](#top)

Like one always said, read instructions before doing anything! I need to read through all the instructions and come up with a plan with clear deadlines and specifications with measurable progress indicator, so that I can work efficiently instead of plunging straight in and ended up revising and redo all the stuff I’ve done and waste time.

A good repo structure also accompanies with that. A clear file structure there will be less confusing and have less asking which thing is where. This could significantly improve our efficiency.

Also, I need to practice my communications skill. When I need somebody else to do something for me, I need a clear specification of what I need and why the thing is needed to achieve my goal. This will build context for them and reduce the case, like I mentioned before, of Communicating back and forth and revising tasks.
