
module ALU_1( out, carryOut, a, b, c, invert, less, sel ) ;
  
  input a, b, c, invert, less ;
  input [2:0] sel ;
  output out, carryOut ;
  wire andResult, orResult, e1, sum, cout, sltResult ;
  
  parameter ALU_add = 3'b010;
  parameter ALU_sub = 3'b110;
  parameter ALU_and = 3'b000;
  parameter ALU_or  = 3'b001;
  parameter ALU_slt = 3'b111;
  
  and( andResult, a, b ) ; 
  or( orResult, a, b ) ;
  
  xor( e1, b, invert ) ;
  FA U_FA( .sum(sum), .cout(cout), .a(a), .b(e1), .c(c) ) ;
  assign carryOut = cout ;
  assign sltResult = less ? 1'b1 : 1'b0 ;
  
  assign out = ( sel == ALU_and ) ? andResult : ( sel == ALU_or ) ? orResult : 
               ( sel == ALU_add ) ? sum : ( sel == ALU_sub ) ? sum :
			   ( sel == ALU_slt ) ? sltResult : 1'b0 ;

endmodule