
module Divider( dataOut, reset, clk, dataA, dataB, Signal );
input clk, reset ;
input [31:0] dataA, dataB ;
input [1:0] Signal ;
output [63:0] dataOut ;
reg [6:0] round ;
reg [31:0] divr, tempA, tempB ;
reg [63:0] rem, dataOut ;
reg temp ;

parameter divu = 2'b00 ;

always@( posedge clk or reset ) begin
  if ( reset ) begin
    rem <= 64'd0 ;
	dataOut <= 64'd0 ;
    round <= 0 ;
  end

  else begin
    if( Signal == divu ) begin
      if( round == 0 ) begin
	    tempA = dataA ;
		tempB = dataB ;
        temp = 1 ;
        rem = { 32'd0, tempA } ;
        rem = rem << 1 ; 
      end 
    end
	
	if( temp == 1 ) begin
	  round = round + 1 ;
      rem[63:32] = rem[63:32] - tempB ;

      // rem < 0 
      if( rem[63] == 1 ) begin
        rem[63:32] = rem[63:32] + tempB ;
        rem = rem << 1 ;
        rem[0] = 0 ;
      end 

      // rem >= 0 
      else begin
        rem = rem << 1 ;
        rem[0] = 1 ;
      end
  
      if( round == 32 ) begin
        rem[63:32] = rem[63:32] >> 1 ;
        dataOut = rem ;
	    round = 0 ;
	    temp = 0 ;
	  end 
    end
  end
end

endmodule