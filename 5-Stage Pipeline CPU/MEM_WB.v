
module MEM_WB( wb_out_RegWrite, wb_out_MemToReg, dmem_rdata_out, ALU_result_out, rfile_wn_out ,
              clk, rst, wb_in, dmem_rdata_in, ALU_result_in, rfile_wn_in );
		  
  input clk, rst ;
  input [1:0] wb_in ;
  input [4:0] rfile_wn_in ;
  input [31:0] dmem_rdata_in, ALU_result_in ;
  output reg wb_out_RegWrite, wb_out_MemToReg;
  output reg[4:0] rfile_wn_out ;
  output reg[31:0] dmem_rdata_out, ALU_result_out ;
  
  always @ ( posedge clk ) begin 
    if ( rst ) begin
	  wb_out_RegWrite <= 1'd0 ;
	  wb_out_MemToReg <= 1'd0 ;
	  dmem_rdata_out <= 32'd0 ;
	  ALU_result_out <= 32'd0 ;
	  rfile_wn_out <= 5'd0 ;
	end 
	
	else begin
	  wb_out_RegWrite <= wb_in[1] ;
	  wb_out_MemToReg <= wb_in[0] ;
	  dmem_rdata_out <= dmem_rdata_in ;
	  ALU_result_out <= ALU_result_in ;
	  rfile_wn_out <= rfile_wn_in ;
	end 
  end
endmodule