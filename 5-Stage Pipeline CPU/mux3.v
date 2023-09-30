//-----------------------------------------------------------------------------
// Title         : 3-1 multiplexer
//-----------------------------------------------------------------------------

module mux3( sel, a, b, c, y );
  input [1:0] sel;
  input  [31:0] a, b, c;
  output [31:0] y;
	
  parameter divu = 2'b00 ;
  parameter mfhi = 2'b10 ;
  parameter mfLo = 2'b01 ; 
  parameter alu = 2'b11 ; // 不是除法
 
  assign y = ( sel == mfhi ) ? b : ( sel == mfLo ) ? c : a ;
endmodule
