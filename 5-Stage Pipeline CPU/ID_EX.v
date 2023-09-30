
module ID_EX( wb_out, m_out, ex_out_RegDst, ex_out_ALUOp, ex_out_ALUSrc, pc_out, shamt_out, BEQ_BNE_out, jump_out, RD1_out, RD2_out, immed_extend_out, j_out, rt_out, rd_out,
              clk, rst, wb_in, m_in, ex_in, BEQ_BNE_in, jump_in, pc_in, RD1_in, RD2_in, immed_extend_in, rt_in, rd_in, j_in, shamt_in );
			  
  input clk, rst, BEQ_BNE_in, jump_in ;
  input [1:0] wb_in ;
  input [2:0] m_in ;
  input [3:0] ex_in ;
  input [4:0] rt_in, rd_in, shamt_in ;
  input [25:0] j_in ;
  input [31:0] pc_in, RD1_in, RD2_in, immed_extend_in ;
  output reg ex_out_ALUSrc, ex_out_RegDst, BEQ_BNE_out, jump_out ;
  output reg[1:0] wb_out, ex_out_ALUOp ;
  output reg[2:0] m_out ;
  output reg[4:0] rt_out, rd_out, shamt_out ;
  output reg[25:0] j_out ;
  output reg[31:0] pc_out, RD1_out, RD2_out, immed_extend_out ;
  
  always @ ( posedge clk ) begin 
    if ( rst == 1 ) begin
	  wb_out <= 2'd0 ;
	  m_out <= 3'd0 ;
	  ex_out_RegDst <= 1'd0 ;
	  ex_out_ALUSrc <= 1'd0 ;
	  ex_out_ALUOp <= 2'd0 ;
	  pc_out <= 32'd0 ;
	  RD1_out <= 32'd0 ;
	  RD2_out <= 32'd0 ;
	  immed_extend_out <= 32'd0 ;
	  rt_out <= 5'd0 ;
	  rd_out <= 5'd0 ;
	  BEQ_BNE_out <= 1'b0 ;
	  jump_out <= 1'b0 ;
	  j_out <= 26'b0 ;
	  shamt_out <= 5'b0 ;
	end 
	
	else begin
	  wb_out <= wb_in ;
	  m_out <= m_in ;
	  ex_out_RegDst <= ex_in[3] ;
	  ex_out_ALUSrc <= ex_in[0] ;
	  ex_out_ALUOp <= ex_in[2:1] ;
	  pc_out <= pc_in ;
	  RD1_out <= RD1_in ;
	  RD2_out <= RD2_in ;
	  immed_extend_out <= immed_extend_in ;
	  rt_out <= rt_in ;
	  rd_out <= rd_in ;
	  BEQ_BNE_out <= BEQ_BNE_in ;
	  jump_out <= jump_in ;
	  j_out <= j_in ;
	  shamt_out <= shamt_in ;
	end 
  end
endmodule