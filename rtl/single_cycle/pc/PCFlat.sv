module PCFlat #(
    DATA_WIDTH = 32
) (
    input   logic                     clk,
    input   logic                     rst,
    input   logic [1:0]               pcsrc,
    input   logic [DATA_WIDTH-1:0]    immop,
    input   logic [DATA_WIDTH-1:0]    alu_out,
    output  logic [DATA_WIDTH-1:0]    pc
);
    // Wires
    logic [DATA_WIDTH-1:0] pc_plus4;
    logic [DATA_WIDTH-1:0] pc_target;
    logic [DATA_WIDTH-1:0] pc_next;
    logic [DATA_WIDTH-1:0] jalr_target;

    // PC + 4
    assign pc_plus4 = pc + 4;

    // PC Target
    assign pc_target = pc + immop;

    // JALR Target is directly from alu_out
    assign jalr_target = alu_out;
    // assign jalr_target = {alu_out[DATA_WIDTH-1:1], 1'b0}; // Ensure LSB is 0

    // Mux
    always_comb begin
        unique case (pcsrc)
            2'b00: pc_next = pc_plus4;
            2'b01: pc_next = pc_target;
            2'b10: pc_next = jalr_target;
            default: pc_next = '0;
        endcase
    end

    // PC Register
    always_ff @(posedge clk or posedge rst) begin
        if (rst) begin
            pc <= '0;
        end else begin
            pc <= pc_next;
        end
    end
    
endmodule
