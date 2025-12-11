module HazardUnit(
    input  logic [4:0]   rs1E,
    input  logic [4:0]   rs2E,
    input  logic [4:0]   rs1D,
    input  logic [4:0]   rs2D,
    input  logic [4:0]   rdE,
    input  logic [4:0]   rdM,
    input  logic [4:0]   rdW,
    input  logic         regWriteM,
    input  logic         regWriteW,
    input  logic         memReadE,
    input  logic         pcsrcE,

    // NEW: stall from data cache / MEM stage
    input  logic         cacheStall,

    output logic [1:0]   forwardAE,
    output logic [1:0]   forwardBE,
    output logic         stallF,
    output logic         stallD,
    output logic         flushD,
    output logic         flushE
);

    // ========================
    // Forwarding logic (unchanged)
    // ========================
    always_comb begin
        forwardAE = 2'b00;
        forwardBE = 2'b00;

        // EX hazard
        if (regWriteM && (rdM != 5'd0) && (rdM == rs1E))
            forwardAE = 2'b10;

        if (regWriteM && (rdM != 5'd0) && (rdM == rs2E))
            forwardBE = 2'b10;

        // MEM hazard
        if (regWriteW && (rdW != 5'd0) && (rdW == rs1E) && (forwardAE == 2'b00))
            forwardAE = 2'b01;

        if (regWriteW && (rdW != 5'd0) && (rdW == rs2E) && (forwardBE == 2'b00))
            forwardBE = 2'b01;
    end

    // ========================
    // Stalls & flushes
    // ========================
    always_comb begin
        // defaults
        stallF = cacheStall;  // freeze IF if cache is stalling
        stallD = cacheStall;  // freeze ID if cache is stalling
        flushD = 1'b0;
        flushE = 1'b0;

        // Only insert *new* hazards when cache is NOT already stalling
        if (!cacheStall) begin
            // Load-use hazard
            if (memReadE && (rdE != 5'd0) &&
                (rdE == rs1D || rdE == rs2D)) begin

                stallF = 1'b1;
                stallD = 1'b1;
                flushE = 1'b1;  // insert bubble in EX
            end

            // Branch / jump hazard
            if (pcsrcE) begin
                flushD = 1'b1;
                flushE = 1'b1;
            end
        end
    end

endmodule
