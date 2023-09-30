
module EX_MEM( wb_out, m_out_Branch, m_out_MemRead, m_out_MemWrite, BEQ_BNE_out, b_address_out, jump_out, j_out, Zero_out_1, Zero_out_2, ALU_result_out, RD2_out, rfile_wn_out ,
              clk, rst, wb_in, m_in, b_address_in, Zero_in_1, Zero_in_2, ALU_result_in, jump_in, j_in, RD2_in, rfile_wn_in, BEQ_BNE_in );
	  
  input clk, rst, Zero_in_1, Zero_in_2, BEQ_BNE_in, jump_in ;
  input [1:0] wb_in ;
  input [2:0] m_in ;
  input [4:0] rfile_wn_in ;
  input [31:0] b_address_in, ALU_result_in, RD2_in, j_in ;
  output reg m_out_Branch, m_out_MemRead, m_out_MemWrite, Zero_out_1, Zero_out_2, BEQ_BNE_out, jump_out ;
  output reg[1:0] wb_out ;
  output reg[4:0] rfile_wn_out ;
  output reg[31:0] b_address_out, ALU_result_out, RD2_out, j_out ;
  
  always @ ( posedge clk ) begin 
    if ( rst ) begin
	  wb_out <= 2'd0 ;
	  m_out_Branch <= 1'd0 ;
	  m_out_MemRead <= 1'd0 ;
	  m_out_MemWrite <= 1'd0 ;
	  b_address_out <= 32'd0 ;
	  Zero_out_1 <= 1'd0 ;
	  Zero_out_2 <= 1'd0 ;
	  ALU_result_out <= 32'd0 ;
	  RD2_out <= 32'd0 ;
	  rfile_wn_out <= 5'd0 ;
	  BEQ_BNE_out <= 1'b0 ;
	  jump_out <= 1'b0 ;
	  j_out <= 32'b0 ;
	end 
	
	else begin
	  wb_out <= wb_in ;
	  m_out_Branch <= m_in[2] ;
	  m_out_MemRead <= m_in[1] ;
	  m_out_MemWrite <= m_in[0] ;
	  b_address_out <= b_address_in ;
	  Zero_out_1 <= Zero_in_1 ;
	  Zero_out_2 <= Zero_in_2 ;
	  ALU_result_out <= ALU_result_in ;
	  RD2_out <= RD2_in ;
	  rfile_wn_out <= rfile_wn_in ;
	  BEQ_BNE_out <= BEQ_BNE_in ;
	  jump_out <= jump_in ;
	  j_out <= j_in ;
	end 
  end
endmodule