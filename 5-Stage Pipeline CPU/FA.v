
module FA( sum, cout, a, b, c ) ;

  input a, b, c ;
  output sum, cout ;
  wire e1, e2, e3, e4 ;
  
  // sum
  xor( e1, a, b ) ;
  xor( sum, e1, c ) ;
  
  // cout
  or( e2, a, b ) ;
  and( e3, e2, c ) ;
  and( e4, a, b ) ;
  or( cout, e3, e4 ) ;

endmodule 