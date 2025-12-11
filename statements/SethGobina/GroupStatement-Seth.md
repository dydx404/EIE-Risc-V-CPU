The Control Unit is the interpreter of the CPU -- it fetches the 32 bit
instruction and, analysing it, determines what control signals to output
to the other components in the structure. Control signals dictate the
outputs for MUXs, and determine what operations different modules like
the ALU perform, and what values are passed in as operands. The primary
dictators of the type of instruction are the opcode, funct3, and funct7
values. The other values in the 32 bit instruction are mainly reserved
for immediate values. The opcode is a broad, general identifier, while
funct3 and funct7 can be used to narrow the selection down to the
specific instruction -- many instructions share the same opcode. In
order to differentiate between the different instructions, we used the
case() operation for each of the opcode, funct3 and funct7. The control
signals each have a specific role in managing and organising the flow of
data within the CPU in accordance with the current instruction. They are
laid out as follows: RegWrite --decides whether or not the Register File
can be written to\
ALUControl -- determines the type of operation executed by the ALU
ALUSrc -- determines the source of the second operand for the ALU
MemWrite -- decides whether or not the Data Memory can be written to
PCSrc -- determines the next position in the Program Counter ResultSrc
-- determines the source of the data to be stored in the Register File
ImmSrc -- determines which bits to use to reconstruct the Immediate in
the ImmGen AddressingControl -- determines the type of reconstruction to
perform in the Data Memory The Immediate Generator receives the
instruction from ROM and ImmSrc from the Control Unit. Depending on the
type of immediate, the bits are arranged in a set order within the range
\[31:7\], and the Generator's job is to correctly extract these bits and
reconstruct it into the appropriate Immediate. We achieve this, too,
using case() on the ImmSrc value. If receiving an ImmSrc value
corresponding to an I-Immediate, then the Generator extracts the upper
12 bits from the instruction (from bit 31 to bit 20), and then sign
extends it by 20 bits -- this is the structure of an I-Immediate. The
other immediate types work in much the same way.
