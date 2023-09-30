 
module alu( result, zero_1, zero_2, a, b, ctl, rst, shamt ) ;
  input rst ;
  input [2:0] ctl ;
  input [4:0] shamt ;
  input [31:0] a, b ;
  output zero_1, zero_2 ;
  output [31:0] result ;
  
  wire [31:0] carry ;
  wire [31:0] sll_result ;
  wire [31:0] out ;
  
  parameter ALU_add = 3'b010;
  parameter ALU_sub = 3'b110;
  parameter ALU_and = 3'b000;
  parameter ALU_or  = 3'b001;
  parameter ALU_slt = 3'b111;
  parameter sll = 3'b011 ;

  // 如果 ctl == SUB 或是 ctl == SLT => invert = 1 
  // 如果 ctl == AND 或是 ctl == OR 或是 ctl == ADD => invert = 0
  wire cin, temp ;
  assign cin = ( ctl == ALU_sub || ctl == ALU_slt ) ? 1'b1 : 1'b0 ; 
  assign temp = ( ctl == ALU_slt && ( a < b ) ) ? 1'b1 : 1'b0 ;
  
  ALU_1 U_ALU0( .out(out[0]), .carryOut(carry[0]), .a(a[0]), .b(b[0]), .c(cin), .invert(cin), .less(temp), .sel(ctl) ) ;
  ALU_1 U_ALU1( .out(out[1]), .carryOut(carry[1]), .a(a[1]), .b(b[1]), .c(carry[0]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU2( .out(out[2]), .carryOut(carry[2]), .a(a[2]), .b(b[2]), .c(carry[1]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU3( .out(out[3]), .carryOut(carry[3]), .a(a[3]), .b(b[3]), .c(carry[2]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU4( .out(out[4]), .carryOut(carry[4]), .a(a[4]), .b(b[4]), .c(carry[3]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU5( .out(out[5]), .carryOut(carry[5]), .a(a[5]), .b(b[5]), .c(carry[4]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU6( .out(out[6]), .carryOut(carry[6]), .a(a[6]), .b(b[6]), .c(carry[5]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU7( .out(out[7]), .carryOut(carry[7]), .a(a[7]), .b(b[7]), .c(carry[6]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU8( .out(out[8]), .carryOut(carry[8]), .a(a[8]), .b(b[8]), .c(carry[7]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU9( .out(out[9]), .carryOut(carry[9]), .a(a[9]), .b(b[9]), .c(carry[8]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU10( .out(out[10]), .carryOut(carry[10]), .a(a[10]), .b(b[10]), .c(carry[9]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU11( .out(out[11]), .carryOut(carry[11]), .a(a[11]), .b(b[11]), .c(carry[10]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU12( .out(out[12]), .carryOut(carry[12]), .a(a[12]), .b(b[12]), .c(carry[11]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU13( .out(out[13]), .carryOut(carry[13]), .a(a[13]), .b(b[13]), .c(carry[12]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU14( .out(out[14]), .carryOut(carry[14]), .a(a[14]), .b(b[14]), .c(carry[13]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU15( .out(out[15]), .carryOut(carry[15]), .a(a[15]), .b(b[15]), .c(carry[14]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU16( .out(out[16]), .carryOut(carry[16]), .a(a[16]), .b(b[16]), .c(carry[15]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU17( .out(out[17]), .carryOut(carry[17]), .a(a[17]), .b(b[17]), .c(carry[16]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU18( .out(out[18]), .carryOut(carry[18]), .a(a[18]), .b(b[18]), .c(carry[17]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU19( .out(out[19]), .carryOut(carry[19]), .a(a[19]), .b(b[19]), .c(carry[18]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU20( .out(out[20]), .carryOut(carry[20]), .a(a[20]), .b(b[20]), .c(carry[19]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU21( .out(out[21]), .carryOut(carry[21]), .a(a[21]), .b(b[21]), .c(carry[20]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU22( .out(out[22]), .carryOut(carry[22]), .a(a[22]), .b(b[22]), .c(carry[21]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU23( .out(out[23]), .carryOut(carry[23]), .a(a[23]), .b(b[23]), .c(carry[22]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU24( .out(out[24]), .carryOut(carry[24]), .a(a[24]), .b(b[24]), .c(carry[23]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU25( .out(out[25]), .carryOut(carry[25]), .a(a[25]), .b(b[25]), .c(carry[24]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU26( .out(out[26]), .carryOut(carry[26]), .a(a[26]), .b(b[26]), .c(carry[25]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU27( .out(out[27]), .carryOut(carry[27]), .a(a[27]), .b(b[27]), .c(carry[26]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU28( .out(out[28]), .carryOut(carry[28]), .a(a[28]), .b(b[28]), .c(carry[27]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU29( .out(out[29]), .carryOut(carry[29]), .a(a[29]), .b(b[29]), .c(carry[28]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU30( .out(out[30]), .carryOut(carry[30]), .a(a[30]), .b(b[30]), .c(carry[29]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  ALU_1 U_ALU31( .out(out[31]), .carryOut(carry[31]), .a(a[31]), .b(b[31]), .c(carry[30]), .invert(cin), .less(1'b0), .sel(ctl) ) ;
  
  Shifter shifter( .dataA(a), .dataB(shamt), .dataOut(sll_result) ) ;
  
  assign zero_1 = ( ctl == ALU_sub && out == 32'd0 ) ? 1 : 0 ; // EQ
  assign zero_2 = ( ctl == ALU_sub && out == 32'd0 ) ? 0 : 1 ; // NE
  assign result = ( ctl == sll ) ? sll_result : out ;
  
endmodule
