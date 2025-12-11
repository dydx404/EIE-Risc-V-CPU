I worked on the Control Unit and Immediate Generator for the
Single-Cycle, and the Decode section for the Pipeline CPU. When
approaching their design, I initially underestimated how many subtle
details and fine tuning would be involved. Both modules seemed
conceptually straightforward on paper, but in particular implementing
the Control Unit in hardware quickly revealed several hidden
complexities. My process ended up being far more iterative than
expected, and most of my challenges were a result of lapses in judgement
and misapplying the provided datapath. To begin working on the Control
Unit, I first needed to determine what commands we would be required to
execute, all of which were outlined on the Project Brief page. After
examining the pdf.s file and cross-referencing it with the RISC-V
specification table, it became clear that some commands, such as RET or
J, weren't provided in the specification. I figured that we would need
to approach them as pseudo-instructions, implementing them by adeptly
employing the commands available to us -- through my research I realised
that LI could be implemented as ADDI rd, x0, imm, J as JAL x0, imm, and
JALR x0, x1, 0. Beginning was fairly straightforward -- the architecture
provided to us on the Project Brief page outlined which inputs and
outputs were relevant for the Control Unit, so I correspondingly began
by extracting the (opcode, funct3 and funct7) signals from the
Instruction, and so too set the outputs accordingly. I later realised
that this was unreliable. Understanding which control signals were in
certain states for certain Instructions required a deep understanding of
the function of each of them. Once I had successfully mapped each
section of the initial Instruction to the opcode, funct3 and funct7, it
was a process of tracing the data along the architecture and analysing
what signals would be necessary for the specified outcome. It was as I
reached the JAL and JALR instructions that I realised the structure I
had been using was incomplete. Relying on the provided datapath too much
was one of the biggest hurdles, and let to several setbacks, including
the aforementioned. Assuming it was the structure we were required to
follow, I faithfully referenced it for all my connections and signals.
Things like the WD3 MUX lacked output options that would be needed for a
JUMP and RETURN, the corresponding control signal ResultSrc had too few
bits, and there was no way to access the PCPlus4 signal in order to use
it for JAL. With some essential functions practically unable to be
programmed, we began discussion within the group as to how we could
modify this structure to fulfil the task. Our next iteration saw an
increase in control signals and MUXs, and provided us with the necessary
tools to be able to execute all of our programs. Although I do not have
an image of the final form of this iteration, the structure resembled
the following image, with each block representing a separate
instruction. The Immediate Extension did not prove too difficult, aside
from many small mistakes on my part. It was a simple task of deciphering
the structure of the immediates as outlined in the RISV-V specification,
and accurately joining them together. Here, human error was the largest
hurdle. However, there were many other points of confusion overall. I
found myself confused as to how I would establish a difference between
an LB and an LBU instruction -- i.e.., I was unsure as to how I could
differentiate between the processing of a signed and an unsigned bit.
The provided architecture did not provide a control signal, so I
considered the possibility of implementing our own, where I could then
distinguish between signed extension and non-signed extension in the
Immediate Extension module. However, further research revealed that this
is non-standard practice, and subsequent discussion with the group
prompted me to consider the option of handling it in another module
outside of the Control Unit or Immediate Extension. Once I felt I had
completed my sections, it was necessary to test them and ensure that the
input signals yielded the expected output signals. I created C++ test
benches for the Control Unit and Immediate Extension separately, and
also created a Top Module, for which I also provided a test bench. These
tests were successful and prompted me to submit my next iteration. The
actual final iteration we see for the single-cycle CPU, however, is the
result of heavy alteration stemming from cooperation between myself and
other group members in an attempt to make the code more easy to debug
during testing. The new structure, rather than distinct blocks for each
command, features a nested layout, which handles different control
signals in sequence. This groups together similar instructions, such as
those with the same Immediate Type, or those classified as Branch
Instructions, to create a cohesive final product. For the Pipeline, my
job was to create the Decoder, which was as simple as creating a top
module for the Control Unit, Register File, and Extend module. It was a
straightforward task, but we did have to resolve a couple of
incompatibilities between components; for example, some components were
programmed to take PCSrc as a control signal, while others dealt with a
Jump, Branch and JALR signal separately. This was merely a lack of
communication. Overall, I learnt a lot about CPU architecture and the
implementation of the Control Unit. I also learnt about how
instructions, such as JALR, interact with different parts of the CPU,
and how they execute their commands.
