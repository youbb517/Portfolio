module AutoVendor( clk, reset, coin, drink_choose, total_coin, refund ) ;

input clk, reset ;
input [6:0] coin ;
input [2:0] drink_choose ;
output reg [6:0] refund, total_coin ;
reg tea, coke, coffee, milk ;
reg [1:0] state, next_state ;

parameter S0 = 2'b00 ;
parameter S1 = 2'b01 ;
parameter S2 = 2'b10 ;
parameter S3 = 2'b11 ;

always @( posedge clk or reset ) begin
    if ( reset == 1'b1 && total_coin >= 0 ) begin
	    if( refund == total_coin ) $display( "exchange 0 dollars " ) ;
	    else $display( "exchange ", total_coin, " dollars " ) ;
	    state = 2'b00 ;
		tea = 1'b0 ;
		coke = 1'b0 ;
		coffee = 1'b0 ;
		milk = 1'b0 ;
		total_coin = 7'b00000 ;
		refund = 7'b00000 ;
	end

	else if ( reset == 1'b1 ) begin
		state = 2'b00 ;
		tea = 1'b0 ;
		coke = 1'b0 ;
		coffee = 1'b0 ;
		milk = 1'b0 ;
		total_coin = 7'b00000 ;
		refund = 7'b00000 ;
	end

	else begin
		state = next_state ;
		total_coin = total_coin + coin ;
	end

end

always @( state or total_coin or clk ) begin	
	
	case ( state ) 
		S0 : 
		  begin
			if( total_coin >= 25 && ( drink_choose == 3'b001 || drink_choose == 3'b010 || 
			                          drink_choose == 3'b011 || drink_choose == 3'b100 )) 
		      begin 					  
				state = S2 ; 
				next_state = S3 ;
			  end 
			  
			else if( total_coin >= 20 && ( drink_choose == 3'b001 || drink_choose == 3'b010 ||
			                               drink_choose == 3'b011 )) 		      
			  begin 					  
				state = S2 ; 
				next_state = S3 ;
			  end 			
			  
			else if( total_coin >= 15 && ( drink_choose == 3'b001 || drink_choose == 3'b010 )) 
		      begin 					  
				state = S2 ; 
				next_state = S3 ;
			  end 
			  
			else if( total_coin >= 10 && drink_choose == 3'b001 )
		      begin 					  
				state = S2 ; 
				next_state = S3 ;
			  end 
			  
			else if( total_coin >= 10 ) next_state = S1 ;
            else next_state = S0 ;
          end
		
		S1 : 
		  begin
			if( total_coin >= 25 && ( drink_choose == 3'b001 || drink_choose == 3'b010 || 
			                          drink_choose == 3'b011 || drink_choose == 3'b100 )) 
		      begin 					  
				state = S2 ; 
				next_state = S3 ;
			  end 
			  
			else if( total_coin >= 20 && ( drink_choose == 3'b001 || drink_choose == 3'b010 ||
			                               drink_choose == 3'b011 )) 		      
			  begin 					  
				state = S2 ; 
				next_state = S3 ;
			  end 			
			  
			else if( total_coin >= 15 && ( drink_choose == 3'b001 || drink_choose == 3'b010 )) 
		      begin 					  
				state = S2 ; 
				next_state = S3 ;
			  end 
			  
			else if( total_coin >= 10 && drink_choose == 3'b001 )
		      begin 					  
				state = S2 ; 
				next_state = S3 ;
			  end 
			  
			else next_state = S0 ;
          end

		S2 : next_state = S3 ;

		S3 : next_state = S0 ; 
	endcase
end

always @( state or total_coin or drink_choose ) begin
	
	case ( state )
		S0 : 
		  begin 
		    if ( total_coin >= 25 && coin != 0 ) $display( "coin ", coin, "  total ", total_coin, " dollars  tea | coke | coffee | milk" ) ;		
			else if ( total_coin >= 20 && coin != 0 ) $display( "coin ", coin, "  total ", total_coin, " dollars  tea | coke | coffee" ) ;
			else if ( total_coin >= 15 && coin != 0 ) $display( "coin ", coin, "  total ", total_coin, " dollars  tea | coke" ) ;
			else if ( total_coin >= 10 && coin != 0 ) $display( "coin ", coin, "  total ", total_coin, " dollars  tea" ) ;
			else if ( total_coin < 10 && total_coin > 0 && coin != 0 ) $display( "coin ", coin, "  total ", total_coin, " dollars" ) ;
			//else 
		  end

		S1 : 
		  begin
		    if ( total_coin >= 25 && coin != 0 ) $display( "coin ", coin, "  total ", total_coin, " dollars  tea | coke | coffee | milk" ) ;
			else if ( total_coin >= 20 && coin != 0 ) $display( "coin ", coin, "  total ", total_coin, " dollars  tea | coke | coffee" ) ; 
			else if ( total_coin >= 15 && coin != 0 ) $display( "coin ", coin, "  total ", total_coin, " dollars  tea | coke" ) ;
			else if ( total_coin >= 10 && coin != 0 ) $display( "coin ", coin, "  total ", total_coin, " dollars  tea" ) ;
		  end

		S2 : 
		    begin
			  if ( drink_choose == 3'b001 && total_coin >= 10 ) 
			    begin 
				  tea = 1'b1 ;
				  $display( "tea out" ) ;
				end
				
			  else if ( drink_choose == 3'b010 && total_coin >= 15 ) 
			    begin 
				  coke = 1'b1 ;
				  $display( "coke out" ) ;	
				end
				
			  else if ( drink_choose == 3'b011 && total_coin >= 20 ) 
			    begin 
				  coffee = 1'b1 ;
				  $display( "coffee out" ) ;	
				end

			  else if ( drink_choose == 3'b100 && total_coin >= 25 ) 
			    begin 
				  milk = 1'b1 ;
				  $display( "milk out" ) ;	
				end
			end
			
		S3 : 
		    begin
			  if ( tea == 1 ) begin
			    refund = total_coin - 7'd10 ; // tea 
				$display( "exchange ", refund, " dollars " ) ;
			    total_coin = 0 ;
			  end 
			  
			  else if ( coke == 1 ) begin
			    refund = total_coin - 7'd15 ; // coke
				$display( "exchange ", refund, " dollars " ) ;
			    total_coin = 0 ;	
              end
			  
			  else if ( coffee == 1 ) begin
			    refund = total_coin - 7'd20 ; // coffee	
				$display( "exchange ", refund, " dollars " ) ;
			    total_coin = 0 ;	
              end
			  
			  else if ( milk == 1 ) begin
			    refund = total_coin - 7'd25 ; // milk 
				$display( "exchange ", refund, " dollars " ) ;
			    total_coin = 0 ;
			  end
			  
			  tea = 1'b0 ;
			  coke = 1'b0 ;
			  coffee = 1'b0 ;
			  milk = 1'b0 ; 
			end
    endcase
end

endmodule

