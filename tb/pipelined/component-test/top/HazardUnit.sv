module HazardUnit(
    // From pipeline
    input  logic [4:0] rs1E,
    input  logic [4:0] rs2E,
    input  logic [4:0] rs1D,
    input  logic [4:0] rs2D,
    input  logic [4:0] rdE,
    input  logic [4:0] rdM,
    input  logic [4:0] rdW,

    input  logic       regWriteM,
    input  logic       regWriteW,
    input  logic       memReadE,    
    input  logic       pcsrcE,
    
    output logic [1:0] forwardAE,
    output logic [1:0] forwardBE,
    output logic       stallF,
    output logic       stallD,
    output logic       flushD,
    output logic       flushE
);

 
    always_comb begin
        forwardAE = 2'b00;
        forwardBE = 2'b00;

        if (regWriteM && rdM != 5'd0 && rdM == rs1E)
            forwardAE = 2'b10;
        if (regWriteM && rdM != 5'd0 && rdM == rs2E)
            forwardBE = 2'b10;

        if (regWriteW && rdW != 5'd0 && rdW == rs1E && forwardAE == 2'b00)
            forwardAE = 2'b01;
        if (regWriteW && rdW != 5'd0 && rdW == rs2E && forwardBE == 2'b00)
            forwardBE = 2'b01;
    end


    always_comb begin
        stallF = 1'b0;
        stallD = 1'b0;
        flushD = 1'b0;
        flushE = 1'b0;


        if (memReadE && (rdE == rs1D || rdE == rs2D)) begin
            stallF = 1'b1;
            stallD = 1'b1;
            flushE = 1'b1;
        end


        if (pcsrcE) begin
            flushD = 1'b1;
            flushE = 1'b1;
        end
    end

endmodule


