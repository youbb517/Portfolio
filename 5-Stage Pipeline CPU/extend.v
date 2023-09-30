/*
	Title: Sign Extend
	Author: Selene (Computer System and Architecture Lab, ICE, CYCU)
	
	Input Port
		1. immed_in: 讀入欲做sign extend資料
	Output Port
		1. ext_immed_out: 輸出已完成sign extend資料
*/
module extend( opcode ,immed_in, ext_immed_out );
    input[5:0] opcode;
	input[15:0] immed_in;
	output[31:0] ext_immed_out;
	assign ext_immed_out = ( opcode == 6'b001101 ) ? { 16'b0, immed_in } : { {16{immed_in[15]}}, immed_in }; 
endmodule
