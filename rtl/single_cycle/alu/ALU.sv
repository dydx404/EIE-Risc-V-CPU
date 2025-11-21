// rtl/single_cycle/alu/ALU.sv
module ALU #(
    parameter LEN = 32
) (
    input  logic [LEN-1:0]  aluop1,         // first operand
    input  logic [LEN-1:0]  aluop2,         // second operand
    input  logic [2:0]      alu_ctrl,       // selects operation
    output logic [LEN-1:0]  alu_result,     // ALU result
    output logic            zero            // 1 if result == 0
);

    assign aluout = 
        (alu_ctrl == 3'b000) ? (aluop1 + aluop2) :  //add
        (alu_ctrl == 3'b001) ? (aluop1 - aluop2) :  //sub
        (alu_ctrl == 3'b010) ? (aluop1 & aluop2) :  //and
        (alu_ctrl == 3'b011) ? (aluop1 | aluop2) :  //or
        (alu_ctrl == 3'b100) ? (aluop1 ^ aluop2) :  //xor
        (alu_ctrl == 3'b101) ? () :                 //slt
        (alu_ctrl == 3'b110) ? () :                 //sll
        (alu_ctrl == 3'b111) ? () :                 //srl
    
    ;
    assign zero;
    
endmodule
