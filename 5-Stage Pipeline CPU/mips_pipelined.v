//	Title: MIPS Single-Cycle Processor
//	Editor: Selene (Computer System and Architecture Lab, ICE, CYCU)
module mips_pipelined( clk, rst );
	input clk, rst;
	
	// instruction bus
	wire[31:0] instr_in ,instr_out ;
	
	// break out important fields from instruction
	wire [5:0] opcode, funct;
    wire [4:0] rs, rt, rd, shamt;
    wire [15:0] immed;
    wire [31:0] extend_immed_result , b_offset;
    wire [25:0] jumpoffset, j_out;
	
	// datapath signals
    wire [4:0] rfile_wn;
    wire [31:0] rfile_rd1, rfile_rd2, rfile_wd, alu_b, alu_out, b_tgt, pc_next, pc, pc_incr_in, dmem_rdata, jump_addr, branch_addr ;

    wire Zero_out, zero_1, zero_2 ;
    wire [1:0] wb_out_1, wb_out_2 ; 
	wire [2:0] m_out ;
	wire [4:0] rt_out, rd_out, rfile_wn_out_1, rfile_wn_out_2, shamt_out  ;
	wire [31:0] rfile_rd1_out, rfile_rd2_out, immed_extend_out, address, ALU_result_out_1, ALU_result_out_2, 
	            WD, dmem_rdata_out ;
				
	wire [31:0] pc_incr_out_1, pc_incr_out_2, jump_addr_result, HiOut, LoOut, final_result ;
	wire [63:0] divu_result ;

    //control signals
    wire RegWrite, Branch, PCSrc, RegDst, MemtoReg, MemRead, MemWrite, ALUSrc, Jump , ExtendSel, BEQ_OR_BNE, BEQ_OR_BNE_out1, BEQ_OR_BNE_out2, Jump_out_1, Jump_out_2;
    wire [1:0] ALUOp, signal;
    wire [2:0] Operation;
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	wire W_RegWrite, ex_RegDst, ex_ALUSrc, M_Branch, M_BEQ_BNE, M_MemRead, M_MemWrite, Zero_out_1, Zero_out_2, W_MemtoReg   ;
    wire [1:0] ex_ALUOp ;
	//////////////////////////////////////////////////////////////////////////////////////////////
	
    assign opcode = instr_out[31:26];
    assign rs = instr_out[25:21];
    assign rt = instr_out[20:16];
    assign rd = instr_out[15:11];
    assign shamt = instr_out[10:6];
    assign funct = instr_out[5:0];
    assign immed = instr_out[15:0];
    assign jumpoffset = instr_out[25:0];

	// module instantiations
	reg32 PC( .clk(clk), .rst(rst), .en_reg(1'b1), .d_in(pc_next), .d_out(pc) );
	
	add32 PCADD( .a(pc), .b(32'd4), .result(pc_incr_in) );
	
	memory InstrMem( .clk(clk), .MemRead(1'b1), .MemWrite(1'b0), .wd(32'd0), .addr(pc), .rd(instr_in) );
	
	IF_ID if_id( .pc_out(pc_incr_out_1), .instr_out(instr_out), .clk(clk), .rst(rst), .pc_incr(pc_incr_in), .instr(instr_in) ) ;
    
	control_pipelined CTL(.opcode(opcode), .RegDst(RegDst), .ALUSrc(ALUSrc), .MemtoReg(MemtoReg), .RegWrite(RegWrite), .MemRead(MemRead), 
	                       .MemWrite(MemWrite), .Branch(Branch), .Jump(Jump), .ALUOp(ALUOp), .BEQ_OR_BNE(BEQ_OR_BNE) );
						  
	reg_file RegFile( .clk(clk), .RegWrite(W_RegWrite), .RN1(rs), .RN2(rt), .WN(rfile_wn_out_2), .WD(rfile_wd), .RD1(rfile_rd1), .RD2(rfile_rd2) );					  
						  
	// extender
    extend SignExt( .opcode(opcode), .immed_in(immed), .ext_immed_out(extend_immed_result) );						  
						  
    ID_EX id_ex( .wb_out(wb_out_1), .m_out(m_out), .ex_out_RegDst(ex_RegDst), .ex_out_ALUOp(ex_ALUOp), .ex_out_ALUSrc(ex_ALUSrc), .pc_out(pc_incr_out_2), .shamt_out(shamt_out) ,
	             .BEQ_BNE_out( BEQ_OR_BNE_out1 ), .jump_out(Jump_out_1), .RD1_out(rfile_rd1_out), .RD2_out(rfile_rd2_out), .immed_extend_out(immed_extend_out), .j_out(j_out) ,.rt_out(rt_out),
				 .rd_out(rd_out), .clk(clk), .rst(rst), .wb_in({RegWrite, MemtoReg}), .m_in({Branch, MemRead, MemWrite}), .ex_in({RegDst, ALUOp, ALUSrc}), .BEQ_BNE_in( BEQ_OR_BNE ), .jump_in(Jump),
				 .pc_in(pc_incr_out_1), .RD1_in(rfile_rd1), .RD2_in(rfile_rd2), .immed_extend_in(extend_immed_result), .rt_in(rt), .rd_in(rd), .j_in(jumpoffset), .shamt_in(shamt)  );						  
						  
	mux2 #(5) RFMUX( .sel(ex_RegDst), .a(rt_out), .b(rd_out), .y(rfile_wn) );					  
						  
	mux2 #(32) ALUMUX( .sel(ex_ALUSrc), .a(rfile_rd2_out), .b(immed_extend_out), .y(alu_b) );					  
						  
	// branch offset shifter
    assign b_offset = immed_extend_out << 2; // 位置偏移量					  
						  
	add32 BRADD( .a(pc_incr_out_2), .b(b_offset), .result(b_tgt) ); // for Branch
	
	alu_ctl ALUCTL( .ALUOp(ex_ALUOp), .Funct(immed_extend_out[5:0]), .signal(signal), .ALUOperation(Operation) );
	
    alu ALU( .result(alu_out), .zero_1(zero_1), .zero_2(zero_2), .a(rfile_rd1_out), .b(alu_b), .ctl(Operation), .rst(rst), .shamt(shamt_out) );
	
	Divider divider( .dataOut(divu_result), .reset(rst), .clk(clk), .dataA(rfile_rd1_out), .dataB(rfile_rd2_out), .Signal(signal) ) ;
  
    HiLo hilo( .HiOut(HiOut), .LoOut(LoOut), .reset(rst), .clk(clk), .DivAns(divu_result)) ;
	
	mux3 mux3( .sel(signal), .a(alu_out), .b(HiOut), .c(LoOut), .y(final_result) ) ;
	
	// jump offset shifter & concatenation
	assign jump_addr = { pc_incr_out_2[31:28], j_out << 2 };

	EX_MEM ex_mem( .wb_out(wb_out_2), .m_out_Branch(M_Branch), .m_out_MemRead(M_MemRead), .m_out_MemWrite(M_MemWrite), .BEQ_BNE_out(BEQ_OR_BNE_out2), .b_address_out(address), .jump_out(Jump_out_2), 
	               .j_out(jump_addr_result), .Zero_out_1(Zero_out_1), .Zero_out_2(Zero_out_2), .ALU_result_out(ALU_result_out_1), .RD2_out(WD), .rfile_wn_out(rfile_wn_out_1) ,.clk(clk), .rst(rst), 
				   .wb_in(wb_out_1), .m_in(m_out), .b_address_in(b_tgt), .Zero_in_1(zero_1), .Zero_in_2(zero_2), .ALU_result_in(final_result), .jump_in(Jump_out_1), .j_in(jump_addr), 
			       .RD2_in(rfile_rd2_out), .rfile_wn_in(rfile_wn), .BEQ_BNE_in(BEQ_OR_BNE_out1) );	
				   
	mux2 #(1) BRANCH( .sel(BEQ_OR_BNE_out2), .a(Zero_out_1), .b(Zero_out_2), .y(Zero_out) );
	
	and BR_AND(PCSrc, M_Branch, Zero_out);
	
	mux2 #(32) PCMUX( .sel(PCSrc), .a(pc_incr_in), .b(address), .y(branch_addr) );	
	
	mux2 #(32) JMUX( .sel(Jump_out_2), .a(branch_addr), .b(jump_addr_result), .y(pc_next) );

	memory DatMem( .clk(clk), .MemRead(M_MemRead), .MemWrite(M_MemWrite), .wd(WD), .addr(ALU_result_out_1), .rd(dmem_rdata) );
	
	MEM_WB mem_wb( .wb_out_RegWrite(W_RegWrite), .wb_out_MemToReg(W_MemtoReg), .dmem_rdata_out(dmem_rdata_out), .ALU_result_out(ALU_result_out_2), 
	               .rfile_wn_out(rfile_wn_out_2), .clk(clk), .rst(rst), .wb_in(wb_out_2), .dmem_rdata_in(dmem_rdata), 
				   .ALU_result_in(ALU_result_out_1), .rfile_wn_in(rfile_wn_out_1) );

    mux2 #(32) WRMUX( .sel(W_MemtoReg), .a(dmem_rdata_out), .b(ALU_result_out_2), .y(rfile_wd) );
				   
endmodule
