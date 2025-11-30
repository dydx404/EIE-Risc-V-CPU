module PCFlat #(
    DATA_WIDTH = 32
) (
    input   logic                     clk,
    input   logic                     rst,
    input   logic                     en,
    input   logic                     jumpE,
    input   logic                     branchE,
    input   logic                     zeroE,
    input   logic                     jalrins,
    input   logic [DATA_WIDTH-1:0]    immopE,
    input   logic [DATA_WIDTH-1:0]    alu_outE,
    input   logic [DATA_WIDTH-1:0]    pcE,
    output  logic [DATA_WIDTH-1:0]    pcF,
    output  logic [DATA_WIDTH-1:0]    pc_plus4F
);
    logic [DATA_WIDTH-1:0] pc_target;
    logic [DATA_WIDTH-1:0] pc_next;
    logic [DATA_WIDTH-1:0] jalr_target;
    logic [DATA_WIDTH-1:0] jump_buff;
    logic                  jumpen;

    // PC + 4
    assign pc_plus4F = pcF + 4;

    // PC Target
    assign pc_target = pcE + immopE;

    // JALR Target is directly from alu_out
    // assign jalr_target = alu_out;
    assign jalr_target = {alu_outE[DATA_WIDTH-1:1], 1'b0}; // Ensure LSB is 0

    assign jumpen = (branchE && zeroE) || jumpE;

    // Mux
    always_comb begin
        assign jump_buff = jumpen ? pc_plus4F : pc_target;
        assign pc_next = jalrins ? jump_buff : jalr_target;
    end

    // PC Register
    always_ff @(posedge clk or posedge rst) begin
        if (rst) begin
            pcF <= 32'hBFC0000;
        end else if (en) begin
            pcF <= pc_next;
        end
    end
    
endmodule
