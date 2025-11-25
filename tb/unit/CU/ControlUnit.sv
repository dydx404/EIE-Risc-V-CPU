module ControlUnit (
    input  wire [6:0]   opcode,
    input  wire [2:0]   funct3,
    input  wire [6:0]   funct7,
    input  wire         zero,
    output reg          regwrite,
    output reg          alusrc,
    output reg  [1:0]   immsrc,
    //immsrc
    //00 = I type
    //01 = S type
    //10 = B type
    //11 = J type
    output reg  [2:0]   alucontrol,  
    output reg          pcsrc,
    output reg  [1:0]   resultsrc, //alu(0), datamem(1), or PC + 4(2) result to reg
                                   //MUX needs 3 inputs in order to implement JAL, otherwise
                                   //we cannot store next instruction
                                   
    output reg          memwrite,
    output reg          jalre      //jalr enable, used for MUX before PCTarget 
);

    always @* begin

        //
        regwrite = 1'b0;
        alusrc   = 1'b0;
        immsrc   = 2'b00;
        alucontrol  = 3'b000;
        pcsrc    = 1'b0;
        resultsrc = 0;
        memwrite = 0;
        jalre = 0;
        //

        case (opcode)
            7'b0010011: begin
                if (funct3 == 3'b000) begin //addi
                    regwrite = 1'b1;
                    alusrc   = 1'b1;
                    immsrc   = 2'b00;
                    alucontrol = 3'b000;
                    pcsrc = 1'b0;
                    resultsrc = 2'b00;
                    memwrite = 0;
                    jalre = 0;
                end
            end
            //LI can be implemented as ADDI rd, x0, imm

            7'b1100011: begin 
                if (funct3 == 3'b001) begin //bne
                    regwrite = 1'b0;
                    alusrc   = 1'b0;
                    immsrc   = 2'b10;
                    alucontrol  = 3'b001;
                    pcsrc = ~zero;
                    resultsrc = 2'b00;
                    memwrite = 0;
                    jalre = 0;
                end
            end

             7'b0110011: begin //add
                if ((funct3 == 3'b000) && (funct7 == 7'b0000000)) begin
                    regwrite = 1'b1;
                    alusrc   = 1'b0;
                    immsrc   = 2'b00;
                    alucontrol = 3'b000;
                    pcsrc = 1'b0;
                    resultsrc = 2'b00;
                    memwrite = 0;
                    jalre = 0;
                end
            end

            7'b0000011: begin //lbu
                if (funct3 == 3'b100) begin
                    regwrite = 1'b1;
                    alusrc   = 1'b1;
                    immsrc   = 2'b00;
                    alucontrol = 3'b000;
                    pcsrc = 1'b0;
                    resultsrc = 2'b01;
                    memwrite = 0;
                    jalre = 0;
                    //how to implement zero extension?
                end
            end

            7'b0100011: begin //sb
                if (funct3 == 3'b000) begin
                    regwrite = 1'b0;
                    alusrc   = 1'b1;
                    immsrc   = 2'b01;
                    alucontrol = 3'b000;
                    pcsrc = 1'b0;
                    resultsrc = 2'b00;
                    memwrite = 1;
                    jalre = 0;
                end

            end

            7'b1101111: begin //jal
                regwrite = 1'b1;
                alusrc   = 1'b0;
                immsrc   = 2'b11;
                alucontrol = 3'b000;
                pcsrc = 1'b1;
                resultsrc = 2'b10;
                memwrite = 0; 
                jalre = 0;
            end
            //J label can be implemented as JAL x0, label

            7'b1100111: begin //jalr, used for RET
                regwrite = 1'b0;
                alusrc   = 1'b0;
                immsrc   = 2'b00;
                alucontrol = 3'b000;
                pcsrc = 1'b1;
                resultsrc = 2'b00;
                memwrite = 0; 
                jalre = 1;
            end
            //RET can be implemented by JALR x0, x1, 0

            default: begin
                regwrite = 1'b0;
                alusrc   = 1'b0;
                immsrc   = 2'b00;
                alucontrol  = 3'b000;
                pcsrc    = 1'b0;
                resultsrc = 2'b00;
                memwrite = 0;
                jalre = 0;
            end
        endcase
    end

endmodule
