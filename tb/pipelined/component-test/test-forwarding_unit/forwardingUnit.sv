// Forwarding logic for the EX stage.
// Checks if the needed register value is coming from later pipeline stages.

module ForwardingUnit (
    // regs used by the instruction currently in EX
    input  logic [4:0] rs1E,
    input  logic [4:0] rs2E,

    // EX/MEM stage
    input  logic       regWriteM,
    input  logic [4:0] rdM,

    // MEM/WB stage
    input  logic       regWriteW,
    input  logic [4:0] rdW,

    // 00 = normal ID/EX values
    // 10 = take EX/MEM result
    // 01 = take MEM/WB result
    output logic [1:0] forwardAE,
    output logic [1:0] forwardBE
);

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

endmodule
