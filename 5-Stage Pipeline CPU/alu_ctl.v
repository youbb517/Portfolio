/*
	Title:	ALU Control Unit
	Author: Garfield (Computer System and Architecture Lab, ICE, CYCU)
	Input Port
		1. ALUOp: ����alu�O�n��+�٬O-�άO��L���O
		2. Funct: �p�G�O��L���O�h�γo��6�X�P�_
	Output Port
		1. ALUOperation: �̫�ѽX���������O
*/

module alu_ctl(ALUOp, Funct, signal, ALUOperation);
    input [1:0] ALUOp ;
    input [5:0] Funct;
	output reg[1:0] signal ;
    output reg[2:0] ALUOperation;

    // symbolic constants for instruction function code
    parameter F_add = 6'd32 ;
    parameter F_sub = 6'd34 ;
    parameter F_and = 6'd36 ;
    parameter F_or  = 6'd37 ;
    parameter F_slt = 6'd42 ; 
	parameter F_sll = 6'd0 ;
	parameter F_divu = 6'd27 ;
	parameter F_mfhi = 6'd16 ;
	parameter F_mflo = 6'd18 ;

    // symbolic constants for ALU Operations
    parameter ALU_add = 3'b010;
    parameter ALU_sub = 3'b110;
    parameter ALU_and = 3'b000;
    parameter ALU_or  = 3'b001;
    parameter ALU_slt = 3'b111;
	parameter ALU_sll = 3'b011;
	
	parameter divu = 2'b00 ;
	parameter mfhi = 2'b10 ;
	parameter mfLo = 2'b01 ;
	
    always @(ALUOp or Funct) begin
	  signal = 2'b11 ; // ���O���k
        case (ALUOp) 
            2'b00 : ALUOperation = ALU_add;
            2'b01 : ALUOperation = ALU_sub;
			2'b11 : ALUOperation = ALU_or ;
            2'b10 : case (Funct) 
                        F_add : ALUOperation = ALU_add;
                        F_sub : ALUOperation = ALU_sub;
                        F_and : ALUOperation = ALU_and;
                        F_or  : ALUOperation = ALU_or;
                        F_slt : ALUOperation = ALU_slt;
						F_sll : ALUOperation = ALU_sll ;
						F_divu : begin 
						  signal = divu ;
						  ALUOperation = 3'bxxx;
						end 
						
						F_mfhi : begin 
						  signal = mfhi ;
						  ALUOperation = 3'bxxx;
						end 
					    
						F_mflo : begin 
						  signal = mfLo ;
						  ALUOperation = 3'bxxx;
						end 
						
                        default ALUOperation = 3'bxxx; 
                    endcase
            default ALUOperation = 3'bxxx;
        endcase
    end
endmodule

