
module Mux_1( out, in0, in1, sel ) ;

output  out ;
input   in0, in1, sel ;
wire    result ;

assign  result = ( sel == 1'b0 ) ? in0 : in1 ;
assign  out = result ;
 
endmodule
