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

    // PC + 4 is always based on *current* PC
    assign pc_plus4F = pcF + 32'd4;

    // JALR target must be aligned
    assign jalr_target = {alu_outE[DATA_WIDTH-1:1], 1'b0};

    // Next PC logic – fully gated by `en`
    always_comb begin
        pc_next = pcF;              // default: HOLD on stall

        if (en) begin
            pc_next = pc_plus4F;    // normal sequential

            if (branchE)
                pc_next = pc_targetE;

            if (jalrinsE)
                pc_next = jalr_target;
        end
    end

    // PC register
    always_ff @(posedge clk or posedge rst) begin
        if (rst)
            pcF <= '0;
        else
            pcF <= pc_next;
    end

endmodule
