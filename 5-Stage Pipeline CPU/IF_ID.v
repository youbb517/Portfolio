
module IF_ID( pc_out, instr_out, clk, rst, pc_incr, instr );
  input clk, rst ;
  input [31:0] pc_incr, instr ;
  output reg[31:0] pc_out, instr_out ;

  always @ ( posedge clk ) begin 
    if ( rst == 1 ) begin
	  pc_out <= 32'd0 ;
	  instr_out <= 32'd0 ;
	end

	else begin
	  pc_out <= pc_incr ;
	  instr_out <= instr ;
	end 
  end
endmodule
