module RegFile #(
    parameter DATA_WIDTH = 32,
    parameter ADDR_WIDTH = 5,
    parameter NUM_REGS   = (1 << ADDR_WIDTH)
)(
    input  logic                     clk,
    input  logic                     we3,              // write enable
    input  logic [ADDR_WIDTH-1:0]    ad1, ad2, ad3,    // rs1, rs2, rd
    input  logic [DATA_WIDTH-1:0]    wd3,             // write data
    output logic [DATA_WIDTH-1:0]    rd1, rd2,        // read data

    // debug / test port
    input  logic [ADDR_WIDTH-1:0]    testRegAddress,
    output logic [DATA_WIDTH-1:0]    testRegData
);

    logic [DATA_WIDTH-1:0] reg_file [NUM_REGS-1:0];

    // synchronous write
    always_ff @(posedge clk) begin
        if (we3) begin
            if (ad3 != '0)
                reg_file[ad3] <= wd3;   // normal write
            else
                reg_file[0]   <= '0;    // keep x0 hard-wired to 0
        end
    end

    // asynchronous reads and debug tap
    always_comb begin
        rd1         = (ad1 == '0) ? '0 : reg_file[ad1];
        rd2         = (ad2 == '0) ? '0 : reg_file[ad2];
        testRegData = (testRegAddress == '0) ? '0 : reg_file[testRegAddress];
    end

endmodule

