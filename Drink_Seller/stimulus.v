module stimulus() ;

reg clk, reset ;
reg [6:0] coin ;
reg [2:0]  drink_choose ;
wire [6:0] total_coin, refund ;

AutoVendor a( clk, reset, coin, drink_choose, total_coin, refund ) ;

initial 
  begin
    clk = 1'b1 ;
    reset = 1'b1 ;
  end 

always #5 clk = ~clk ;

always @ ( reset ) 
  begin
    if( reset == 1'b1 ) begin
      coin = 7'b0000000;
      drink_choose = 3'b000;
      reset = 1'b0;
	end 
  end	

initial	
  begin
    #10 coin = 10 ;		
    #10 coin = 5 ;		
	#10 reset = 1 ;
    #10 coin = 10 ;		
    #10 coin = 10 ;	
    #10 coin = 0 ;
    #10 drink_choose = 2 ; // 3 = coffee  coffee out
    #10 drink_choose = 0 ; // exchange 6 dollars
	#10 coin = 10 ;	
	#10 coin = 10 ;	
	#10 coin = 0 ;
    #10 drink_choose = 4  ; // 3 = coffee  coffee out
    #10 drink_choose = 0 ; // exchange 6 dollars
    #10 coin = 10;
	#10 coin = 0 ;
    #10 drink_choose = 1 ; // 3 = coffee  coffee out
    #10 drink_choose = 0 ; // exchange 6 dollars
    #10 coin = 50;
	#10 coin = 0 ;
    #10 drink_choose = 3; // 3 = coffee  coffee out
    #10 drink_choose = 0 ; // exchange 6 dollars	
  end

endmodule