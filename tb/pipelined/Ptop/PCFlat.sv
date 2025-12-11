module PCFlat #(
    parameter DATA_WIDTH = 32
) (
    input   logic                     clk,
    input   logic                     rst,
    input   logic                     en,
    input   logic                     branchE,
    input   logic                     jalrinsE,
    input   logic [DATA_WIDTH-1:0]    pc_targetE,
    input   logic [DATA_WIDTH-1:0]    alu_outE,
    output  logic [DATA_WIDTH-1:0]    pcF,
    output  logic [DATA_WIDTH-1:0]    pc_plus4F
);

    logic [DATA_WIDTH-1:0] pc_next;
    logic [DATA_WIDTH-1:0] jalr_target;

    // PC + 4
    assign pc_plus4F = pcF + 32'd4;

    // JALR target must be aligned
    assign jalr_target = {alu_outE[DATA_WIDTH-1:1], 1'b0};


    always_comb begin
        pc_next = pc_plus4F;        // default

        if (branchE)
            pc_next = pc_targetE;

        if (jalrinsE)
            pc_next = jalr_target;
    end

    // ✅ PC Register
    always_ff @(posedge clk or posedge rst) begin
        if (rst)
            pcF <= 32'b0;           // ✅ start at 0 for now (safe)
        else if (en)
            pcF <= pc_next;
    end

endmodule
