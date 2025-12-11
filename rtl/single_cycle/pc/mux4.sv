module mux4 #(
    DATA_WIDTH = 32
) (
    input   logic [DATA_WIDTH-1:0]  in0,
    input   logic [DATA_WIDTH-1:0]  in1,
    input   logic [DATA_WIDTH-1:0]  in2,
    input   logic [DATA_WIDTH-1:0]  in3,
    input   logic [1:0]            sel,
    output  logic [DATA_WIDTH-1:0]  out
);
    assign out = (sel == 2'b00) ? in0 :
                 (sel == 2'b01) ? in1 :
                 (sel == 2'b10) ? in2 :
                                  in3 ;

endmodule
