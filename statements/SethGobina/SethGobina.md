# CPU Control Unit & Immediate Generator Reflection

## Contents
- [Introduction](#introduction)
- [Beginning the Control Unit](#beginning-the-control-unit)
- [Challenges in Control Signal Design](#challenges-in-control-signal-design)
  - [Overreliance on the Provided Datapath](#overreliance-on-the-provided-datapath)
- [Immediate Generator](#immediate-generator)
- [Testing](#testing)
- [Pipeline CPU – Decoder](#pipeline-cpu--decoder)
- [Diagrams](#diagrams)
- [Conclusion](#conclusion)

---

## Introduction

I worked on the Control Unit and Immediate Generator for the Single-Cycle CPU, and the Decode section for the Pipeline CPU. When approaching their design, I initially underestimated how many subtle details and fine‑tuning would be involved. Both modules seemed conceptually straightforward on paper, but implementing the Control Unit in hardware quickly revealed several hidden complexities. My process ended up being far more iterative than expected, and most of my challenges were a result of lapses in judgement and misapplying the provided datapath.

---

## Beginning the Control Unit

To begin working on the Control Unit, I first needed to determine what commands we would be required to execute, all of which were outlined on the Project Brief page. After examining the `pdf.s` file and cross‑referencing it with the RISC‑V specification table, it became clear that some commands, such as **RET** or **J**, were not provided in the specification. I figured that we would need to approach them as pseudo‑instructions, implementing them using the commands available to us. Through my research, I realised that:

* **LI** could be implemented as `ADDI rd, x0, imm`
* **J** as `JAL x0, imm`
* **RET** as `JALR x0, x1, 0`

Beginning was fairly straightforward – the architecture provided to us outlined which inputs and outputs were relevant for the Control Unit, so I started by extracting the opcode, funct3 and funct7 signals from the instruction and setting outputs accordingly. I later realised that blindly following that architecture was unreliable.

---

## Challenges in Control Signal Design

Understanding which control signals were in certain states for certain instructions required a deeper understanding of the function of each of them. Once I had successfully mapped each section of the initial instruction to its opcode, funct3 and funct7, it became a process of tracing the data along the architecture and analysing what signals would be necessary for the specified outcome.

It was when I reached the **JAL** and **JALR** instructions that I realised the structure I had been using was incomplete.

### Overreliance on the Provided Datapath
![](https://github.com/EIE2-IAC-Labs/Project_Brief/raw/main/images/single-cycle.jpg)

Relying on the provided datapath too much became one of the biggest hurdles and led to several setbacks. For example:

* The **WD3 MUX** lacked output options needed for Jump and Return.
* The control signal **ResultSrc** had too few bits.
* There was no way to access **PCPlus4** to use it for JAL.

With some essential functions practically impossible to program, we discussed how to modify the structure to fulfil the task. Our next iteration saw an increase in control signals and MUXes, giving us the necessary tools to execute all of our programs. It was then a straightforward task of using the case() function to detect each instruction.

<img width="6050" height="3970" alt="single cycle" src="https://github.com/user-attachments/assets/61653d9a-83be-4130-b9fc-9189d635cf18" />

---

## Immediate Generator

The Immediate Extension module was not too difficult, aside from many small mistakes on my part. It was mainly a task of deciphering the immediate formats in the RISC‑V specification and accurately connecting them. Human error was the largest hurdle.

One major point of confusion was distinguishing between **LB** and **LBU**, since I was unsure how to differentiate signed vs. unsigned processing. The provided architecture did not include a control signal for this, so I considered implementing our own. However, further research showed this was non‑standard, and group discussion prompted us to handle it outside of the Control Unit or Immediate Extension.

---

## Testing

Once I felt I had completed my sections, I tested them by creating C++ test benches for the Control Unit and Immediate Extension separately, and also created a Top Module with its own test bench. These tests were successful and prompted the submission of my next iteration.

The final iteration of the single‑cycle CPU, however, included heavy alterations resulting from collaboration within the group, primarily to make debugging easier. Instead of distinct blocks for each instruction, the new structure featured a **nested layout**, grouping similar instructions together, such as those with the same Immediate Type or those classified as Branch Instructions. 

```sv
module ControlUnit #(
    //parameter DATA_WIDTH = 32
) (
    input  logic        zero,    // used for BEQ/BNE
    input  logic [6:0]  op,      // opcode
    input  logic [2:0]  funct3,  // funct3
    input  logic [6:0]  funct7,  // funct7

    output logic        RegWrite,
    output logic [3:0]  ALUControl,
    output logic        ALUSrc,
    output logic        MemWrite,
    output logic [1:0]  PCSrc,
    output logic [1:0]  ResultSrc,
    output logic [2:0]  ImmSrc,
    output logic [2:0]  AddressingControl
);

    // ALU op encoding
    localparam [3:0] ALU_ADD  = 4'b0000;
    localparam [3:0] ALU_SUB  = 4'b0001;
    localparam [3:0] ALU_AND  = 4'b0010;
    localparam [3:0] ALU_OR   = 4'b0011;
    localparam [3:0] ALU_XOR  = 4'b0100;
    localparam [3:0] ALU_SLT  = 4'b0101;
    localparam [3:0] ALU_SLTU = 4'b0110;
    localparam [3:0] ALU_SLL  = 4'b0111;
    localparam [3:0] ALU_SRL  = 4'b1000;
    localparam [3:0] ALU_SRA  = 4'b1011;
    localparam [3:0] ALU_LUI  = 4'b1111;

    always_comb begin
        // ---------------------- DEFAULT VALUES ----------------------
        RegWrite          = 1'b0;
        ALUControl        = ALU_ADD;
        ALUSrc            = 1'b0;
        MemWrite          = 1'b0;
        PCSrc             = 2'b00; // PC+4
        ResultSrc         = 2'b00; // ALU result
        ImmSrc            = 3'b000; // default I-type
        AddressingControl = 3'b000;

        // ---------------------- OPCODE DECODER ----------------------
        case (op)

        // ====================== R-TYPE ==============================
        7'b0110011: begin
            RegWrite  = 1'b1;
            ALUSrc    = 1'b0;
            ResultSrc = 2'b00;
            PCSrc     = 2'b00;

            case (funct3)
                3'b000: begin
                    if (funct7 == 7'b0100000)
                        ALUControl = ALU_SUB;
                    else
                        ALUControl = ALU_ADD;
                end
                3'b001: ALUControl = ALU_SLL;
                3'b010: ALUControl = ALU_SLT;
                3'b011: ALUControl = ALU_SLTU;
                3'b100: ALUControl = ALU_XOR;
                3'b110: ALUControl = ALU_OR;
                3'b111: ALUControl = ALU_AND;
                3'b101: begin
                    if (funct7 == 7'b0000000)
                        ALUControl = ALU_SRL;
                    else
                        ALUControl = ALU_SRA;
                end
                default: begin end
            endcase
        end

        // ====================== BRANCHES ============================
        7'b1100011: begin
            RegWrite   = 1'b0;
            ImmSrc     = 3'b010; // B-type
            ALUSrc     = 1'b0;
            ALUControl = ALU_SUB;

            case (funct3)
                3'b000: PCSrc = zero ? 2'b01 : 2'b00; // BEQ
                3'b001: PCSrc = zero ? 2'b00 : 2'b01; // BNE
                default: PCSrc = 2'b00;
            endcase
        end

        // ====================== I-TYPE ALU ==========================
        7'b0010011: begin
            RegWrite  = 1'b1;
            ImmSrc    = 3'b000; // I-type
            ALUSrc    = 1'b1;
            ResultSrc = 2'b00;
            PCSrc     = 2'b00;

            case (funct3)
                3'b000: ALUControl = ALU_ADD;   // ADDI
                3'b010: ALUControl = ALU_SLT;   // SLTI
                3'b011: ALUControl = ALU_SLTU;  // SLTIU
                3'b001: ALUControl = ALU_SLL;   // SLLI
                3'b100: ALUControl = ALU_XOR;   // XORI
                3'b110: ALUControl = ALU_OR;    // ORI
                3'b111: ALUControl = ALU_AND;   // ANDI

                3'b101: begin
                    if (funct7 == 7'b0000000)
                        ALUControl = ALU_SRL;  // SRLI
                    else
                        ALUControl = ALU_SRA;  // SRAI
                end
                default: begin end
            endcase
        end

        // ====================== JAL ================================
        7'b1101111: begin
            RegWrite  = 1'b1;
            ImmSrc    = 3'b011;  // J-type
            ResultSrc = 2'b10;   // PC+4
            PCSrc     = 2'b01;   // PC + imm
            ALUControl = ALU_ADD;
            ALUSrc    = 1'b0;
        end

        // ====================== LOAD ===============================
        7'b0000011: begin
            RegWrite          = 1'b1;
            ImmSrc            = 3'b000; // I-type
            ALUSrc            = 1'b1;
            ALUControl        = ALU_ADD;
            ResultSrc         = 2'b01; // memory
            PCSrc             = 2'b00;
            MemWrite          = 1'b0;
            AddressingControl = funct3;
        end

        // ====================== STORE ==============================
        7'b0100011: begin
            RegWrite          = 1'b0;
            ImmSrc            = 3'b001; // S-type
            ALUSrc            = 1'b1;
            ALUControl        = ALU_ADD;
            PCSrc             = 2'b00;
            MemWrite          = 1'b1;
            AddressingControl = funct3;
        end

        // ====================== JALR ===============================
        7'b1100111: begin
            RegWrite  = 1'b1;
            ImmSrc    = 3'b000; // I-type
            ResultSrc = 2'b10;  // PC+4
            PCSrc     = 2'b10;  // JALR path
            ALUControl = ALU_ADD;
            ALUSrc    = 1'b1;
        end

        // ====================== LUI ================================
        7'b0110111: begin
            RegWrite  = 1'b1;
            ImmSrc    = 3'b100; // U-type
            ResultSrc = 2'b00;  // ALU
            PCSrc     = 2'b00;
            ALUSrc    = 1'b1;
            ALUControl = ALU_LUI;
        end

        // ====================== DEFAULT ============================
        default: begin end

        endcase
    end

endmodule

```
---

## Pipeline CPU – Decoder

For the Pipeline CPU, my task was to create the Decoder, which was essentially a top module for the Control Unit, Register File, and Extend module. It was straightforward, but we did need to resolve a few incompatibilities between components. For example, some modules used **PCSrc** as a control signal, while others used **Jump**, **Branch**, and **JALR** separately. This was simply a communication issue.

---

## Diagrams

I also worked on the diagrams for the single-cycle and pipeline CPUs. Besides the challenge of finding a suitable program proving futile, leading me to hand-draw it entirely, it proved difficult to create a consistent diagram that reconciled all of the components into one coherent module, as different components used different names for the same signal. In the end, though, it was simply a matter of settling on one of the names, as the diagrams simply serve to provide a visual representation of the architecture we are based on.

---

## Conclusion

Overall, I learned a lot about CPU architecture and the implementation of the Control Unit. I also learned how instructions such as **JALR** interact with different parts of the CPU and how they execute their commands. If I were to do it again, I would pace myself a lot better at the start, taking time to understand the architecture before blindly referencing the one provided to us. A bit of research on industry standards in architecture may have helped with developing an intuition. In general, the experience showed me how iterative the design process can be and how important it is to understand the datapath in depth before relying on it.
