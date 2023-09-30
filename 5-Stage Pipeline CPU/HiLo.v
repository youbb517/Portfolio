
module HiLo( HiOut, LoOut, reset, clk, DivAns );
input clk, reset ;
input [63:0] DivAns ;
output reg[31:0] HiOut, LoOut ;
reg [63:0] HiLo ;

parameter mfhi = 2'b10 ;
parameter mfLo = 2'b01 ;

always@( posedge clk or reset ) begin
  if ( reset ) begin
    HiLo = 64'b0 ;
  end

  else begin
    HiLo = DivAns ;
	LoOut = HiLo[31:0] ; // °Ó¼Æ
    HiOut = HiLo[63:32] ;  // ¾l¼Æ
  end
end
endmodule