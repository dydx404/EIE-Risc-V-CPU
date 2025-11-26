module ControlUnit (
    input  wire [6:0]   opcode,
    input  wire [2:0]   funct3,
    input  wire [6:0]   funct7,
    input  wire         zero,
    output reg          regwrite,
    output reg          alusrc,
    output reg  [2:0]   immsrc,
    //immsrc
    //000 = I type
    //001 = S type
    //010 = B type
    //011 = U type
    //100 = J type
    output reg  [3:0]   alucontrol,  
    output reg  [1:0]   pcsrc,
    output reg  [1:0]   resultsrc, 
    output reg  [2:0]   addressingcontrol,                                
    output reg          memwrite
);

    always @* begin

        //
        regwrite = 1'b0;
        alusrc   = 1'b0;
        immsrc   = 3'b000;
        alucontrol  = 4'b0000;
        pcsrc    = 2'b00;
        resultsrc = 2'b00;
        memwrite = 0;
        addressingcontrol = 3'b000;
        //

        case (opcode)
            7'b0010011: begin
                if (funct3 == 3'b000) begin //addi
                    regwrite = 1'b1;
                    alusrc   = 1'b1;
                    immsrc   = 3'b000;
                    alucontrol = 4'b0000;
                    pcsrc = 2'b00;
                    resultsrc = 2'b00;
                    memwrite = 0;
                    addressingcontrol = 3'b000;
                end
            end
            //LI can be implemented as ADDI rd, x0, imm

            7'b1100011: begin 
                if (funct3 == 3'b001) begin //bne
                    regwrite = 1'b0;
                    alusrc   = 1'b0;
                    immsrc   = 3'b010;
                    alucontrol  = 4'b0001;
                    pcsrc = {{1'b0},~zero};
                    resultsrc = 2'b00;
                    memwrite = 0;
                    addressingcontrol = 3'b000;
                end
            end

             7'b0110011: begin //add
                if ((funct3 == 3'b000) && (funct7 == 7'b0000000)) begin
                    regwrite = 1'b1;
                    alusrc   = 1'b0;
                    immsrc   = 3'b000;
                    alucontrol = 4'b0000;
                    pcsrc = 2'b00;
                    resultsrc = 2'b00;
                    memwrite = 0;
                    addressingcontrol = 3'b000;
                end
            end

            7'b0000011: begin 
                if (funct3 == 3'b100) begin //lbu
                    regwrite = 1'b1;
                    alusrc   = 1'b1;
                    immsrc   = 3'b000;
                    alucontrol = 4'b0000;
                    pcsrc = 2'b00;
                    resultsrc = 2'b01;
                    memwrite = 0;
                    addressingcontrol = 3'b000;
                end

                else if (funct3 == 3'b000) begin //lb
                    regwrite = 1'b1;
                    alusrc   = 1'b1;
                    immsrc   = 3'b000;
                    alucontrol = 4'b0000;
                    pcsrc = 2'b00;
                    resultsrc = 2'b01;
                    memwrite = 0;
                    addressingcontrol = 3'b000;
                end
            end

            7'b0100011: begin //sb
                if (funct3 == 3'b000) begin
                    regwrite = 1'b0;
                    alusrc   = 1'b1;
                    immsrc   = 3'b001;
                    alucontrol = 4'b0000;
                    pcsrc = 2'b00;
                    resultsrc = 2'b00;
                    memwrite = 1;
                    addressingcontrol = 3'b000;
                end

            end

            7'b1101111: begin //jal
                regwrite = 1'b1;
                alusrc   = 1'b0;
                immsrc   = 3'b100;
                alucontrol = 4'b0000;
                pcsrc = 2'b01;
                resultsrc = 2'b10;
                memwrite = 0; 
                addressingcontrol = 3'b000;
            end
            //J label can be implemented as JAL x0, label

            7'b1100111: begin //jalr, used for RET
                regwrite = 1'b0;
                alusrc   = 1'b0;
                immsrc   = 3'b000;
                alucontrol = 4'b0000;
                pcsrc = 2'b10;
                resultsrc = 2'b00;
                memwrite = 0; 
                addressingcontrol = 3'b000;
            end

            7'b0110111: begin //lui
                regwrite = 1'b1;
                alusrc   = 1'b1;
                immsrc   = 3'b011;
                alucontrol = 4'b0000; //use register 0 to add 0
                pcsrc = 2'b00;
                resultsrc = 2'b00;
                memwrite = 0; 
                addressingcontrol = 3'b000;
            end
            //RET can be implemented by JALR x0, x1, 0

            default: begin
                regwrite = 1'b0;
                alusrc   = 1'b0;
                immsrc   = 3'b000;
                alucontrol  = 4'b0000;
                pcsrc    = 2'b00;
                resultsrc = 2'b00;
                memwrite = 0;
                addressingcontrol = 3'b000;
            end
        endcase
    end

endmodule
