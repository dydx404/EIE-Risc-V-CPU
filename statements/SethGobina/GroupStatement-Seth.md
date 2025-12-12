# Control Unit & Immediate Generator

## Contents
- [Control Unit](#control-unit)  
- [Instruction Fields](#instruction-fields)  
- [Control Signals](#control-signals)  
- [Immediate Generator](#immediate-generator)  
- [Example: I-type Immediate](#example-i-type-immediate)

---

## Control Unit

The Control Unit is the **interpreter** of the CPU. It fetches the 32-bit instruction and determines the control signals required by the rest of the architecture. These control signals dictate the outputs of multiplexers, the operations executed by the ALU, and which values are passed in as operands.

### Instruction Fields

There are 3 flags that determine the instruction we will perform. They are extracted from the 32-bit instruction:

* **Opcode** – a broad identifier indicating the general instruction type.
* **Funct3** and **Funct7** – additional fields used to narrow down the exact instruction, since many share the same opcode.
* The remaining bits primarily encode immediate values.

To differentiate between all possible instructions, we used `case()` statements on the opcode, funct3, and funct7 fields.

### Control Signals

Each control signal plays a specific role in directing data flow through the CPU:

* **RegWrite** – controls whether the Register File is written to.
* **ALUControl** – selects the ALU operation.
* **ALUSrc** – chooses the source of the ALU’s second operand.
* **MemWrite** – controls whether the Data Memory is written to.
* **PCSrc** – determines the next Program Counter value.
* **ResultSrc** – selects the data written back to the Register File.
* **ImmSrc** – identifies the immediate format so the ImmGen can reconstruct it.
* **AddressingControl** – determines how Data Memory reconstructs loaded data (e.g., sign or zero extension).

---

## Immediate Generator

The Immediate Generator receives:

* the **full 32-bit instruction** from ROM, and
* the **ImmSrc** signal from the Control Unit.

Depending on the immediate type, bits within the range `[31:7]` are arranged differently. The Generator extracts these fields and reconstructs the appropriate immediate value using a `case()` on the ImmSrc for each of the 5 different immediate types.

### Example: I-type Immediate

For an **I-Immediate**, the Generator:

1. Extracts bits **[31:20]**.
2. Sign-extends the 12-bit value to 32 bits.

This is correctly extracted according to the structure predefined for I-Immediates. Other immediate types follow similar patterns, each with its own bit arrangement and reconstruction rules.

---
