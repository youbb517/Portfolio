// 資工三甲 10927109 陳宥蓁 2022/12/29 

# include<stdio.h>
# include<iostream>
# include<fstream>
# include<vector>
# include<string>
# include<string.h>

using namespace std;

struct Token{
  vector <int> tokenType ;
  vector <int> tokenValue ;
  vector <string> token ;
};

vector <Token> tokenArray ;
vector <string> errorMessage ;
int nowFunctionIndex = 0 ;

vector <Token> tokenArray1 ;

class Table {    
  public:
  	Table() { 	  
	  table.clear() ; 
	  table.push_back("") ; // index從1開始 
	} // constructor
	
	vector<string> table ;  
  	
    void insert(string temp) {  //加入表中
      table.push_back(temp) ;
    } // insert()
    
    void initial(){
      table.clear() ; 
	  table.push_back("") ; // index從1開始 
	} // initial
	
};

struct IdentifierTableField{
  string identifier ;
  int subroutine ;
  int type ;
  vector <int> pointer ; // pointer[0] => table6 or table7 // pointer[1] => table 的位置 
} ;

class IdentifierTable {    
  public:
  	IdentifierTable() { 
	  table.clear() ; 
	  table.resize( tableSize ) ;
	  table[0].identifier = "" ; // index從1開始 
	  table[0].subroutine = 0 ; // index從1開始 
	  table[0].type = 0 ; // index從1開始 
	  for( int i = 0 ; i < tableSize ; i++ ) {
	  	table[i].pointer.push_back(0) ;
	  	table[i].pointer.push_back(0) ;
	  } // for()
	} // constructor
	
	void initial(){
	  table.clear() ; 
	  table.resize( tableSize ) ;
	  table[0].identifier = "" ; // index從1開始 
	  table[0].subroutine = 0 ; // index從1開始 
	  table[0].type = 0 ; // index從1開始 
	  for( int i = 0 ; i < tableSize ; i++ ) {
	  	table[i].pointer.push_back(0) ;
	  	table[i].pointer.push_back(0) ;
	  } // for()
	} // initial()
	
	vector<IdentifierTableField> table ;  
	int tableSize = 100 ;
    
    int hashFunction( string temp ) {
      int key = 0 ;
      
      for( int i = 0 ; i < temp.size() ; i++ ) key += (int)temp[i] ;
      key = key % tableSize ;	// ASICC除以雜湊表大小取餘數
	  
	  while( table[key].identifier.size() != 0 ){
	    if( key == 99 ) key = 0 ;	
	    else key++ ;
	  } // while()
	  
	  table[key].identifier = temp ; 
	  return key ; 
	} // hashFunction()

	bool contain( string temp, int & num ) { //查詢某一字元是否有temp字串
      for( int i = 0 ; i < table.size() ; i++ ) {
        if( table[i].identifier == temp ){
          num = i ;
          return true ;
	    } // if()
      } // for()

      return false ;
    } // contain()
    
    bool find( string temp, int & num ) { //查詢某一字元是否有temp字串
      for( int i = 0 ; i < table.size() ; i++ ) {
        if( table[i].identifier == temp && table[i].subroutine == nowFunctionIndex ){
          num = i ;
          return true ;
	    } // if()
      } // for()

      return false ;
    } // find()
};
 
struct Intermediate{
  int num ;
  vector <int> one ;
  vector <int> two ;
  vector <int> three ;
  vector <int> four ;
  string str ;	
};

Table TEMP ; // Table0
Table delimiter ; // Table1
Table reservedWord ; // Table2
Table integer ; // Table3
Table realNumber ; // Table4
IdentifierTable identifier ; // Table5
vector <Intermediate> intermediate ; // Table6
vector <int> information ; // Table7
int havePROGRAMEnd = 0 ; // PROGRAM ... ENP ;
int haveSUBROUTINEEnd = 0 ; // SUBROUTINE ... ENS ;
int T = 1 ;

void ReadTableData( Table & table, string fileName ) {
  ifstream inputFile;
  string tempLine;
  inputFile.open( fileName );
  
  while( getline( inputFile, tempLine ) ) table.insert( tempLine ) ; 
  inputFile.close();
} // ReadTableData()

bool isInt( string temp ){
  for( int i = 0 ; i < temp.size() ; i++ )  if( '0' > temp[i] || temp[i] > '9' ) return false ;
  return true ;
} // isInt

bool isInt( char temp ){
  if( '0' > temp || temp > '9' ) return false ;
  return true ;
} // isInt

bool isFloat( string temp ){
  int num = 0 ; // 記下 "." 的位置 
  
  for( int i = 0 ; i < temp.size() ; i++ ) {
    if( temp[i] == '.' ) {
	  num = i ;
	  break ;
    } // if()
  } // for()
  
  if( num == 0 ) return false ;
  
  else if( num != 0 && temp.size() < 2 ) return false ;
      
  else if( num != 0 && temp.size() >= 2 ){
    for( int j = 0 ; j < num ; j++ ) if( '0' > temp[j] || temp[j] > '9' ) return false ;
    for( int j = num+1 ; j < temp.size() ; j++ ) if( '0' > temp[j] || temp[j] > '9' ) return false ;
  } // if()
  
  return true ;
} // isFloat

bool isChar( char temp ){
  if( ( (int)temp >= 65 && (int)temp <= 90 ) || ( (int)temp >= 97 && (int)temp <= 122 ) ) return true ;
  return false ; 
} // isChar()

bool contain( Table & table, string temp, int & num ) { //查詢某一字元是否有temp字串
  string temp1 = "", temp2 = "" ;
  
  for( int i = 0 ; i < temp.size() ; i++ ) {
  	temp1 += toupper(temp[i]) ;
  	temp2 += tolower(temp[i]) ;
  } // for()
  
  for( int i = 0 ; i < table.table.size() ; i++ ) {
    if( table.table[i] == temp1 || table.table[i] == temp2 ){
      num = i ;
      return true ;
	} // if()
  } // for()

  return false ;
} // contain()

bool Identifier( string temp ){
  if( !isChar( temp[0] ) ) return false ;	
	
  for( int i = 1 ; i < temp.size() ; i++ ){
    if( !isInt( temp[i] ) && !isChar( temp[i] ) ) return false ;
  } // for()
  
  return true ;
} // Identifier()

bool Array( Token temp, int start, int end ){ // start => "("  // end => ")"
  int number = 0 ;
  int common = 0 ;
  int index ;
 
  if( !Identifier( temp.token[start-1] ) || temp.tokenType[start-1] == 2 ) return false ;
  
  for( int i = start+1 ; i <= end ; i++ ){
	if( temp.token[i] == "," || temp.token[i] == ")" ){
  	  if( temp.token[i] == "," ) common++ ;
  	  if( !isInt( temp.token[i-1] ) ) return false ;
  	  else  number++ ;
	} // if()
	
	else if( isInt( temp.token[i] ) && i < end ){
	  if( temp.token[i+1] != "," && temp.token[i+1] != ")" ) return false ;
	} // else if()
  } // for()
		
  if( number - common != 1 ) return false ;
	  
  return true ;
} // Array() 

void Array( Token temp, int type, int now, int start, int end ){  // start => "("  // end => ")"
  vector <string> numTemp ;
  int index ;
  
  if( identifier.contain( temp.token[start-1], index ) && identifier.table[index].subroutine == nowFunctionIndex ){
    Intermediate in ;
    in.num = intermediate.size() ;
    in.str = temp.token[start-1] ;
    in.one.push_back( 5 ) ;
    in.one.push_back( index ) ;
    in.two.push_back( 0 ) ;
    in.two.push_back( 0 ) ;
    in.three.push_back( 0 ) ;
    in.three.push_back( 0 ) ;
    in.four.push_back( 0 ) ;
    in.four.push_back( 0 ) ;
    intermediate.push_back( in ) ;
  } // else if()
  
  else if( !identifier.contain( temp.token[start-1], index ) || 
         ( identifier.contain( temp.token[start-1], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){
  	information.push_back(type) ;
  	index = identifier.hashFunction( temp.token[start-1] ) ;
    identifier.table[index].subroutine = nowFunctionIndex ;
    identifier.table[index].type = 1 ; // ARRAY
    identifier.table[index].pointer[0] = 7 ;
    identifier.table[index].pointer[1] = information.size() - 1 ;
    Intermediate in ;
    in.num = intermediate.size() ;
    in.str = temp.token[start-1] ;
    in.one.push_back( 5 ) ;
    in.one.push_back( index ) ;
    in.two.push_back( 0 ) ;
    in.two.push_back( 0 ) ;
    in.three.push_back( 0 ) ;
    in.three.push_back( 0 ) ;
    in.four.push_back( 0 ) ;
    in.four.push_back( 0 ) ;
    intermediate.push_back( in ) ;
  } // else if()
  
  for( int i = start ; i <= end ; i++ ){
    if( temp.token[i] == "," || temp.token[i] == ")" ){
  	  numTemp.push_back(temp.token[i-1]) ;
  	  if( contain( integer, temp.token[i-1], index ) ) integer.insert( temp.token[i-1] ) ;
	} // if()
  } // for()
  
  information.push_back(numTemp.size()) ;
  for( int i = 0 ; i < numTemp.size() ; i++ ) information.push_back( atoi(numTemp[i].c_str()) ) ;
} // Array() 

bool variable( Token temp, int start, int end ){
  if( Identifier( temp.token[start] ) && temp.tokenType[start] != 2 ) {
  	if( start == end ) return true ;
  	int id = 0 ;
    int common = 0 ;
    if( start + 1 <= end ) {
      if( temp.token[start+1] != "(" ) return false ;
	} // if()
    
	for( int i = start + 1 ; i <= end ; i++ ){
      if( ( temp.token[i] == "," || temp.token[i] == ")" ) ) {
        if( temp.token[i] == "," ) common++ ;
        if( ( Identifier( temp.token[i-1] ) && temp.tokenType[i-1] != 2 ) || isInt( temp.token[i-1] ) ) id++ ;
        else return false ;
	  }	// if
	} // for()
	
    if( id - common != 1 ) return false ;	
    return true ;
  } // if()
  
  else return false ;
} // variable()

bool FACTOR( Token temp, int start, int end ) ;
bool TERM( Token temp, int start, int end ) ;
bool EXP( Token temp, int start, int end ) ;
void FACTOR( Token temp, int start, int end, int now ) ;
void TERM( Token temp, int start, int end, int now ) ;
void EXP( Token temp, int start, int end, int now ) ;
bool FACTOR1( Token temp, int start, int end ) ;
bool TERM1( Token temp, int start, int end ) ;
bool EXP1( Token temp, int start, int end ) ;
void FACTOR1( Token temp, int start, int end, int now ) ;
void TERM1( Token temp, int start, int end, int now ) ;
void EXP1( Token temp, int start, int end, int now ) ;

bool EXP( Token temp, int start, int end ){
  int term = 0 ;
  int operatorNum = 0 ;
  bool have = false ;
  for( int i = start ; i <= end ; i++ ) {
  	if( temp.token[i] == "(" ) have = true ;
	if( temp.token[i] == ")" ) have = false ;
  	if( ( have == false && ( temp.token[i] == "+" || temp.token[i] == "-" || temp.token[i] == "OR" )) || i == end ){
  	  if( temp.token[i] == "+" || temp.token[i] == "-" || temp.token[i] == "OR" ) operatorNum++ ;
  	  if( i == end ){
  	    if( TERM( temp, start, end ) ){
  	  	  term++ ;
  	  	  start = i + 1 ;
	    } // if()
	  
	    else {
	  	  //cout << "EXP Error111111!!!" << endl ;
  	      return false ;
	    } // else
	    
	    break ;
      } // if()
      
      else {
      	if( TERM( temp, start, i-1 ) ){
  	  	  term++ ;
  	  	  start = i + 1 ;
	    } // if()
	  
	    else {
	  	  //cout << "EXP Error111111!!!" << endl ;
  	      return false ;
	    } // else
	  } // else
	} // if()
  } // for()
  
  if( term - operatorNum != 1 ){
  	//cout << "EXP Error!!!222222" << endl ;
  	return false ;
  } // if()
  
  return true ;
} // EXP()

bool TERM( Token temp, int start, int end ){
  int factor = 0 ;
  int operatorNum = 0 ;
  bool have = false ;
  for( int i = start ; i <= end ; i++ ) {
	if( temp.token[i] == "(" ) have = true ;
	if( temp.token[i] == ")" ) have = false ;
  	if( ( have == false && ( temp.token[i] == "*" || temp.token[i] == "/" || temp.token[i] == "AND" || temp.token[i] == "^" )) || i == end ){
  	  if( temp.token[i] == "*" || temp.token[i] == "/" || temp.token[i] == "AND" || temp.token[i] == "^" ) operatorNum++ ;
  	  if( i == end ){
  	  	if( FACTOR( temp, start, end ) ){
  	  	  factor++ ;
  	  	  start = i + 1 ;
	    } // if()
	  
	    else {
	  	  //cout << "TERM Error!!!" << endl ;
  	      return false ;
	    } // else
	    
	    break ;
      } // if()
  	  
  	  else {
  	  	if( FACTOR( temp, start, i-1 ) ){
  	  	  factor++ ;
  	  	  start = i + 1 ;
	    } // if()
	  
	    else {
	  	  //cout << "TERM Error!!!" << endl ;
  	      return false ;
	    } // else
	  } // else
	} // if()
  } // for()
  
  if( factor - operatorNum != 1 ){
  	//cout << "TERM Error!!!" << endl ;
  	return false ;
  } // if()
  
  return true ;
} // TERM()

bool FACTOR( Token temp, int start, int end ){
  string str = "" ;
  int index ;
  for( int i = start ; i <= end ; i++ ) str = str + temp.token[i] ;

  if( ( Identifier( str ) && temp.tokenType[start] != 2 ) || isInt( str ) || isFloat( str ) || 
      ( temp.token[start] == "(" && temp.token[end] == ")" && EXP( temp, start+1, end-1 ) )) {
  	return true ;
  } // if()
  
  else {
  	//cout << "FACTOR ERROR!!!" << endl ;
  	return false ;
  } // else 
} // FACTOR() 

void EXP( Token temp, int start, int end, int now ){
  bool have = false ;
  for( int i = start ; i <= end ; i++ ) {
  	if( temp.token[i] == "(" ) have = true ;
	if( temp.token[i] == ")" ) have = false ;
  	if( ( have == false && ( temp.token[i] == "+" || temp.token[i] == "-" || temp.token[i] == "OR" )) || i == end ){
  	  if( i == end ){
  	    TERM( temp, start, end, now ) ;
	    break ;
      } // if()
      
      else {
      	TERM( temp, start, i-1, now ) ;
  	  	start = i + 1 ;
	  } // else
	} // if()
  } // for()
} // EXP()

void TERM( Token temp, int start, int end, int now ){
  bool have = false ;
  for( int i = start ; i <= end ; i++ ) {
	if( temp.token[i] == "(" ) have = true ;
	if( temp.token[i] == ")" ) have = false ;
  	if( ( have == false && ( temp.token[i] == "*" || temp.token[i] == "/" || temp.token[i] == "AND" || temp.token[i] == "^" )) || i == end ){
      if( i == end ){
  	  	FACTOR( temp, start, end, now ) ;
	    break ;
      } // if()
  	  
  	  else {
  	  	FACTOR( temp, start, i-1, now ) ;
  	  	start = i + 1 ;
	  } // else
	} // if()
  } // for()
} // TERM()

void FACTOR( Token temp, int start, int end, int now ){
  string str = "" ;
  for( int i = start ; i <= end ; i++ ) str = str + temp.token[i] ;
  
  int index ;
  if( Identifier( str ) && temp.tokenType[start] != 2 ){
  	if( !identifier.contain( temp.token[start], index ) || 
	    ( identifier.contain( temp.token[start], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){ 
	  index = identifier.hashFunction( temp.token[start] ) ;
	  identifier.table[index].subroutine = nowFunctionIndex ;
	} // if()
  } // if()
	
  else if( isInt( str ) ){
	if( !contain( integer, str, index ) ) integer.insert( str ) ;
  }// else if()
	
  else if( isFloat( str ) ){
	if( !contain( realNumber, str, index ) ) realNumber.insert( str ) ;
  } // else if()
	
  else EXP( temp, start+1, end-1, now ) ;
} // FACTOR() 

bool TERM1( Token temp, int start, int end ){
  int factor = 0 ;
  int operatorNum = 0 ;
  bool have = false ;
  for( int i = start ; i <= end ; i++ ) {
	if( temp.token[i] == "(" ) have = true ;
	if( temp.token[i] == ")" ) have = false ;
  	if( ( have == false && ( temp.token[i] == "*" || temp.token[i] == "/" || temp.token[i] == "AND" || temp.token[i] == "^" )) || i == end ){
  	  if( temp.token[i] == "*" || temp.token[i] == "/" || temp.token[i] == "AND" || temp.token[i] == "^" ) operatorNum++ ;
  	  if( i == end ) {
  	  	if( FACTOR1( temp, start, end ) ){
  	  	  factor++ ;
  	  	  start = i + 1 ;
	    } // if()
	      
	    else {
	      //cout << "TERM1 Error!!!44444444444444" << endl ;
  	      return false ;
		} // else
		
		break ;
      } // if()
      
	  else {
        if( FACTOR1( temp, start, i-1 ) ){
  	  	  factor++ ;
  	  	  start = i + 1 ;
	    } // if()
	    
	    else {
	      //cout << "TERM1 Error2222222222222222!!!" << endl ;
  	      return false ;
		} // else
	  } // else
	} // if()
  } // for()
  
  if( factor - operatorNum != 1 ){
  	//cout << "TERM1 Error11111111111111111111!!!" << endl ;
  	return false ;
  } // if()
  
  return true ;
} // TERM()

bool EXP1( Token temp, int start, int end ){
  int term = 0 ;
  int operatorNum = 0 ;
  bool have = false ;
  for( int i = start ; i <= end ; i++ ) {
  	if( temp.token[i] == "(" ) have = true ;
	if( temp.token[i] == ")" ) have = false ;
  	if( ( have == false && ( temp.token[i] == "+" || temp.token[i] == "-" || temp.token[i] == "OR" )) || i == end ){
  	  if( temp.token[i] == "+" || temp.token[i] == "-" || temp.token[i] == "OR" ) operatorNum++ ;
	  if( i == end ) {
  	    if( TERM1( temp, start, end ) ){
  	  	  term++ ;
  	  	  start = i + 1 ;
	    } // if()
	      
	    else {
	      //cout << "EXP1 Error!!!" << endl ;
  	      return false ;
		} // else
		
		break ;
	  } // if()
	  
	  else {
  	    if( TERM1( temp, start, i-1 ) ){
  	  	  term++ ;
  	  	  start = i + 1 ;
	    } // if()
	  
	    else {
	  	  //cout << "EXP1 Error111111!!!" << endl ;
  	      return false ;
	    } // else
	  } // else 
	} // if()
  } // for()
  
  if( term - operatorNum != 1 ){
  	//cout << "EXP1 Error!!!222222" << endl ;
  	return false ;
  } // if()
  
  return true ;
} // EXP()

bool FACTOR1( Token temp, int start, int end ){
  string str = "" ;
  for( int i = start ; i <= end ; i++ ) str = str + temp.token[i] ;
  
  int index ;
  if( ( Identifier( str ) && temp.tokenType[start] != 2 ) || isInt( str ) || isFloat( str ) || 
      ( temp.token[start] == "(" && temp.token[end] == ")" && EXP1( temp, start+1, end-1 ) )) {
    return true ;
  } // if()
  
  else if( variable( temp, start, end ) ){
  	return true ;
  } // else if()
  
  else {
  	//cout << "FACTOR1 ERROR!!!4444444444" << endl ;
  	return false ;
  } // else 
} // FACTOR() 

void TERM1( Token temp, int start, int end, int now ){
  bool have = false ;
  for( int i = start ; i <= end ; i++ ) {
	if( temp.token[i] == "(" ) have = true ;
	if( temp.token[i] == ")" ) have = false ;
  	if( ( have == false && ( temp.token[i] == "*" || temp.token[i] == "/" || temp.token[i] == "AND" || temp.token[i] == "^" )) || i == end ){
  	  if( i == end ) {
  	  	FACTOR1( temp, start, end, now ) ;
		break ;
      } // if()
      
	  else {
        FACTOR1( temp, start, i-1, now ) ;
  	  	start = i + 1 ;
	  } // else
	} // if()
  } // for()
} // TERM()

void EXP1( Token temp, int start, int end, int now ){
  bool have = false ;
  for( int i = start ; i <= end ; i++ ) {
  	if( temp.token[i] == "(" ) have = true ;
	if( temp.token[i] == ")" ) have = false ;
  	if( ( have == false && ( temp.token[i] == "+" || temp.token[i] == "-" || temp.token[i] == "OR" )) || i == end ){
  	  if( i == end ) {
  	    TERM1( temp, start, end, now ) ;
		break ;
	  } // if()
	  
	  else {
  	    TERM1( temp, start, i-1, now ) ;
  	  	start = i + 1 ;
	  } // else 
	} // if()
  } // for()
} // EXP()

void FACTOR1( Token temp, int start, int end, int now ){
  string str = "" ;
  for( int i = start ; i <= end ; i++ ) str = str + temp.token[i] ;
  
  int index ;
  
  if( Identifier( str ) && temp.tokenType[start] != 2 ){
  	if( !identifier.contain( temp.token[start], index ) || 
	    ( identifier.contain( temp.token[start], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){ 
	  index = identifier.hashFunction( temp.token[start] ) ;
	  identifier.table[index].subroutine = nowFunctionIndex ;
	} // if()
  } // if()
	
  else if( isInt( str ) ){
	if( !contain( integer, str, index ) ) integer.insert( str ) ;
  }// else if()
	
  else if( isFloat( str ) ){
	if( !contain( realNumber, str, index ) ) realNumber.insert( str ) ;
  } // else if()
	
  else if( variable( temp, start, end ) ){
  	if( !identifier.contain( temp.token[start], index ) || 
	    ( identifier.contain( temp.token[start], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){ 
	  index = identifier.hashFunction( temp.token[start] ) ;
	  identifier.table[index].subroutine = nowFunctionIndex ;
	} // if()
  } // else if()
  
  else EXP1( temp, start+1, end-1, now ) ;	
} // FACTOR() 

bool unsigned_constant( string temp ){
  if( isInt( temp ) || isFloat( temp ) || Identifier( temp ) ) return true ;
  else return false ;
} // bool unsigned_constant

bool constant( Token temp, int index ){
  if( unsigned_constant( temp.token[index] ) ) return true ;
  else {
  	if( ( temp.token[index-1] == "+" || temp.token[index-1] == "-" ) && unsigned_constant( temp.token[index] ) ) return true ;
	else return false ;
  } // else 
} // constant() 

bool PROGRAM( Token temp, int now ) {  // PROGRAM identifier ;
  int index ;
  if( temp.token.size() == 3 ) {
    if( !Identifier( temp.token[1] ) ) {
      errorMessage.push_back( "第" + to_string(now+1) + "行: PROGRAM後接的不是id" ) ;
      cout << "第" + to_string(now+1) + "行: PROGRAM後接的不是id"  << endl ;
      return false ;
    } // if() 
  
    else if( contain( reservedWord, temp.token[1], index ) ){
  	  errorMessage.push_back( "第" + to_string(now+1) + "行: PROGRAM後接的是保留字(文法錯誤)" ) ;
      cout << "第" + to_string(now+1) + "行: PROGRAM後接的是保留字(文法錯誤)" << endl ;
      return false ;
    } // else if()
	
    else if( temp.token[2] != ";" ){
      errorMessage.push_back( "第" + to_string(now+1) + "行: 最後字元非結束指令 ';'" ) ;
      cout << "第" + to_string(now+1) + "行: 最後字元非結束指令 ';'" << endl ;
      return false ;      
    } // else if	
    
    return true ;
  } // if
  
  else {
  	errorMessage.push_back( "第" + to_string(now+1) + "行: PROGRAM(文法錯誤)" ) ;
    cout << "第" + to_string(now+1) + "行: PROGRAM(文法錯誤)" << endl ;
    return false ;
  } // else 
} // PROGRAM()

bool SUBROUTINE( Token temp, int now ){ // SUBROUTINE A3(INTEGER: X ,Y ,K); 
  int index ;
  if( !Identifier( temp.token[1] ) || temp.tokenType[1] == 2 ){
  	errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE 後面的名稱不是id" ) ;
    cout << "第" + to_string(now+1) + "行: SUBROUTINE 後面的名稱不是id" << endl ;
    return false ;
  } // if()
  
  if( temp.token.size() >= 8 && temp.token[2] == "(" && ( temp.token[3] == "BOOLEAN" || temp.token[3] == "INTEGER" || temp.token[3] == "REAL" ) && 
                                temp.token[temp.token.size()-2] == ")" && temp.token[temp.token.size()-1] == ";" ) {
    for( int i = 4 ; i < temp.token.size()-1 ; i++ ) {
      if( temp.token[i] == "BOOLEAN" || temp.token[i] == "INTEGER" || temp.token[i] == "REAL" ){
      	if( i + 2 < temp.token.size()-1 ){
	      if( temp.token[i+1] != ":" ) {
	      	errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE type後面不是:" ) ;
            cout << "第" + to_string(now+1) + "行: SUBROUTINE type後面不是:" << endl ;
      	    return false ;
		  } // if()
		  
		  else if( !Identifier( temp.token[i+2] ) || temp.tokenType[i+2] == 2 ) {
		  	errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE 變數不是id:" ) ;
            cout << "第" + to_string(now+1) + "行: SUBROUTINE 變數不是id:" << endl ;
      	    return false ;
          } // else if()
          
		  else {
		  	int start = i + 2 ;
		  	int idORArray = 0 ; // id數 
            int common = 0 ; // ,數 
            
		  	for( int j = i + 2 ; j < temp.token.size()-1 ; j++ ){
              if( ( temp.token[j] == "," && !( temp.token[j+1] == "BOOLEAN" || temp.token[j+1] == "INTEGER" || temp.token[j+1] == "REAL" )) || 
											 ( temp.token[j] == ")" && ( j + 1 == temp.token.size()-1 || 
											 ( j + 1 == temp.token.size()-2 && temp.token[j-1] == "(" ) ))){
				
				if( !Identifier( temp.token[start] ) || temp.tokenType[start] == 2 ) {
		  	      errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE 變數不是id:" ) ;
                  cout << "第" + to_string(now+1) + "行: SUBROUTINE 變數不是id:" << endl ;
      	          return false ;
                } // else if()
				  
				else idORArray++ ;
			   
			    if( temp.token[j] == ")" && ( j + 1 == temp.token.size()-1 || j + 1 == temp.token.size()-2 ) ) break ;
				if( temp.token[j] == "," ) {
				  common++ ;
				  start = j + 1 ;
				} // if()
			  } // if()
			  
			  else if( temp.token[j-1] == "," && ( temp.token[j] == "BOOLEAN" || temp.token[j] == "INTEGER" || temp.token[j] == "REAL" ) ){
				if( !Identifier( temp.token[start] ) || temp.tokenType[start] == 2 ) {
		  	      errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE 變數不是id:" ) ;
                  cout << "第" + to_string(now+1) + "行: SUBROUTINE 變數不是id:" << endl ;
      	          return false ;
                } // else if()
				
				else idORArray++ ;
				
	            i = j - 1 ;
	            break ;
			  } // else if()
			  
			  else if( temp.token[j-1] != "," && ( temp.token[j] == "BOOLEAN" || temp.token[j] == "INTEGER" || temp.token[j] == "REAL" ) ){
		  	    errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE 不同型別之間不是用逗號區隔" ) ;
                cout << "第" + to_string(now+1) + "行: SUBROUTINE 不同型別之間不是用逗號區隔" << endl ;
      	        return false ;
		      } // else if()
		      
		      else if( Identifier( temp.token[j] ) && Identifier( temp.token[j-1] )) {
		  	    errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE 變數之間不是逗號" ) ;
                cout << "第" + to_string(now+1) + "行: SUBROUTINE 變數之間不是逗號" << endl ;
      	        return false ;
		      } // else if()
			} // for()
			
            if( idORArray - common != 1 ) {
              errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE 變數之間不是逗號!!!" ) ;
              cout << "第" + to_string(now+1) + "行: SUBROUTINE 變數之間不是逗號!!!" << endl ;
              return false ;
	        } // if()
		  } // else 
		} // if()
		
		else {
          errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE(文法錯誤)" ) ;
          cout << "第" + to_string(now+1) + "行: SUBROUTINE(文法錯誤)" << endl ;
          return false ;
		} // else
      } // if()
    } // for()
    
    ////////對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對對//////
  
    int type ;
    for( int i = 3 ; i < temp.token.size()-1 ; i++ ) {
      if( temp.token[i] == "BOOLEAN" || temp.token[i] == "INTEGER" || temp.token[i] == "REAL" ){
      	
		if( temp.token[i] == "BOOLEAN" ) type = 2 ;	
	    else if( temp.token[i] == "INTEGER" ) type = 4 ; 
	    else if( temp.token[i] == "REAL" ) type = 6 ;
      	
      	if( i + 2 < temp.token.size()-1 ){
		  int start = i + 2 ;
		  for( int j = i + 2 ; j < temp.token.size()-1 ; j++ ){
            if( ( temp.token[j] == "," && !( temp.token[j+1] == "BOOLEAN" || temp.token[j+1] == "INTEGER" || temp.token[j+1] == "REAL" )) || 
										   ( temp.token[j] == ")" && ( j + 1 == temp.token.size()-1 || 
										   ( j + 1 == temp.token.size()-2 && temp.token[j-1] == "(" ) ))){
			  
			  int index = identifier.hashFunction( temp.token[start] ) ;
              identifier.table[index].subroutine = nowFunctionIndex ;						   	
			  if( temp.token[j] == ")" && j + 1 == temp.token.size()-2 && temp.token[j-1] == "(" ) identifier.table[index].type = 1 ; 
			  else identifier.table[index].type = type ; 
              Intermediate in ;
              in.num = intermediate.size() ;
              in.str = temp.token[start] ;
              in.one.push_back( 5 ) ;
              in.one.push_back( index ) ;
              in.two.push_back( 0 ) ;
              in.two.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              in.four.push_back( 0 ) ;
              in.four.push_back( 0 ) ;
              intermediate.push_back( in ) ;
			
			  if( temp.token[j] == ")" && ( j + 1 == temp.token.size()-1 || j + 1 == temp.token.size()-2 ) ) break ;
			  if( temp.token[j] == "," ) start = j + 1 ; 
			} // if()
			  
			else if( temp.token[j-1] == "," && ( temp.token[j] == "BOOLEAN" || temp.token[j] == "INTEGER" || temp.token[j] == "REAL" ) ){
			  
			  int index = identifier.hashFunction( temp.token[start] ) ;
              identifier.table[index].subroutine = nowFunctionIndex ;						   	
			  if( temp.token[start+2] == ")" && temp.token[start+1] == "(" ) identifier.table[index].type = 1 ; 
			  else identifier.table[index].type = type ; 
              Intermediate in ;
              in.num = intermediate.size() ;
              in.str = temp.token[start] ;
              in.one.push_back( 5 ) ;
              in.one.push_back( index ) ;
              in.two.push_back( 0 ) ;
              in.two.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              in.four.push_back( 0 ) ;
              in.four.push_back( 0 ) ;
              intermediate.push_back( in ) ;
				
	          i = j - 1 ;
	          break ;
			} // else if()
		  } // for()
		} // if()
      } // if()
    } // for() 
    
    return true ;
  } // if()
 
  else {
    errorMessage.push_back( "第" + to_string(now+1) + "行: SUBROUTINE(文法錯誤)" ) ;
    cout << "第" + to_string(now+1) + "行: SUBROUTINE(文法錯誤)" << endl ;
    return false ;
  } // else 
} // SUBROUTINE()

bool VARIABLE( Token temp, int now ) {          // VARIABLE INTRGER: U,V,M ;
  if( temp.token.size() >= 5 && temp.token[2] == ":" && temp.token[temp.token.size()-1] == ";" 
                             && ( temp.token[1] == "BOOLEAN" || temp.token[1] == "INTEGER" || temp.token[1] == "REAL" )) { 	
    int id = 0 ; // id數 
    int common = 0 ; // ,數 
    
    for( int i = 3 ; i < temp.token.size()-1 ; i++ ) {
      if( i % 2 == 1 && ( !Identifier( temp.token[i] ) || temp.tokenType[i] == 2 ) ) {
      	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "VARIABLE 宣告的變數非id" ) ;
        cout << "第" + to_string(now+1) + "行: " + "VARIABLE 宣告的變數非id" << endl ;
      	return false ;
	  } // if()
	  
      else if( i % 2 == 0 && temp.token[i] != "," ) {
      	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "VARIABLE 變數之間不是逗號" ) ;
        cout << "第" + to_string(now+1) + "行: " + "VARIABLE 變數之間不是逗號" << endl ;
  	    return false ;
	  } // else if()
      
      if( i % 2 == 1 && Identifier( temp.token[i] ) && temp.tokenType[i] != 2 ) id++ ;
	  
      if( i % 2 == 0 && temp.token[i] == "," ) common++ ;
	} // for()
      	
    if( id - common != 1 ) {
      errorMessage.push_back( "第" + to_string(now+1) + "行: " + "VARIABLE 變數之間不是逗號" ) ;
      cout << "第" + to_string(now+1) + "行: " + "VARIABLE 變數之間不是逗號" << endl ;
      return false ;	
	} // if()

	return true ;
  } // if()
  
  else { // Error 
    errorMessage.push_back( "第" + to_string(now+1) + "行: " + "VARIABLE 文法錯誤!!!!" ) ;
    cout << "第" + to_string(now+1) + "行: " + "VARIABLE 文法錯誤!!!!" << endl ;
  	return false ;
  } // else 
} // VARIABLE()

bool LABEL( Token temp, int now ) { // LABLE L91,L92 ;
  if( temp.token.size() >= 3 && temp.token[temp.token.size()-1] == ";" ) {
    int id = 0 ; // id數 
    int common = 0 ; // ,數 

    for( int i = 1 ; i < temp.token.size()-1 ; i++ ) {
      if( i % 2 == 1 && ( !Identifier( temp.token[i] ) || temp.tokenType[i] == 2 )) {
      	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "LABEL 宣告的不是id!!!!" ) ;
        cout << "第" + to_string(now+1) + "行: " + "LABEL 宣告的不是id!!!!" << endl ;
      	return false ;
	  } // if()
	  
      else if( i % 2 == 0 && temp.token[i] != "," ) {
      	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "LABEL 變數之間不是逗號!!!!" ) ;
        cout << "第" + to_string(now+1) + "行: " + "LABEL 變數之間不是逗號!!!!" << endl ;
  	    return false ;
	  } // else if()
      
      if( i % 2 == 1 && Identifier( temp.token[i] ) && temp.tokenType[i] != 2 ) id++ ;
	  
      if( i % 2 == 0 && temp.token[i] == "," ) common++ ;
	} // for()
      	
    if( id - common != 1 ) {
      errorMessage.push_back( "第" + to_string(now+1) + "行: " + "LABEL 變數之間不是逗號!!!!" ) ;
      cout << "第" + to_string(now+1) + "行: " + "LABEL 變數之間不是逗號!!!!" << endl ;
      return false ;	
	} // if()
	
	return true ;
  } // if()
  
  else { // Error 
    errorMessage.push_back( "第" + to_string(now+1) + "行: " + "LABEL 文法錯誤!!!!" ) ;
    cout << "第" + to_string(now+1) + "行: " + "LABEL 文法錯誤!!!!" << endl ;
    return false ;
  } // else 	
} // LABLE() 

bool DIMENSION( Token temp, int now ) { // DIMENSION INTEGER:A(12); 
  if( temp.token.size() >= 8 && temp.token[2] == ":" && temp.token[temp.token.size()-1] == ";" && 
    ( temp.token[1] == "BOOLEAN" || temp.token[1] == "INTEGER" || temp.token[1] == "REAL" )) {
    int array = 0, common = 0 ;
    int type = 0, left = 0 ; 
    if( temp.token[1] == "BOOLEAN" ) type = 2 ;
	else if( temp.token[1] == "INTEGER" ) type = 4 ; 
	else if( temp.token[1] == "REAL" ) type = 6 ;

    for( int i = 3 ; i < temp.token.size() ; i++ ) {
      if( temp.token[i] == "(" ) left = i ;
      
      else if( temp.token[i] == ")" ){
      	if( !Array( temp, left, i ) ){
      	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "DIMENSION 宣告的不是array!!!!" ) ;
          cout << "第" + to_string(now+1) + "行: " + "DIMENSION 宣告的不是array!!!!" << endl ;
      	  return false ;
		} // if()
		
		else array++ ;
		
		if( i + 1 < temp.token.size() && temp.token[i+1] == ";" ) break ;
		
		else if( i + 1 < temp.token.size() && temp.token[i+1] != "," ) {
		  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "DIMENSION array之間不是逗號!!!!" ) ;
          cout << "第" + to_string(now+1) + "行: " + "DIMENSION array之間不是逗號!!!!" << endl ;
      	  return false ;
		} // if()
		
		else { 
		  common++ ;
		  i++ ;	
		} // else 
	  } // else if()
	} // for()

    if( array - common != 1 ) {
      errorMessage.push_back( "第" + to_string(now+1) + "行: " + "DIMENSION 變數之間不是逗號!!!!" ) ;
      cout << "第" + to_string(now+1) + "行: " + "DIMENSION 變數之間不是逗號!!!!" << endl ;
      return false ;
	} // if()
	
	for( int i = 3 ; i < temp.token.size() ; i++ ) {
      if( temp.token[i] == "(" ) left = i ;
      
      else if( temp.token[i] == ")" ){
		Array( temp, type, now, left, i ) ;
	 
		if( temp.token[i+1] == ";" ) break ;
		else i++ ;	
	  } // if()
	} // for()
	
	return true ;
  } // if()
  
  else { // Error 
    errorMessage.push_back( "第" + to_string(now+1) + "行: " + "DIMENSION 文法錯誤!!!!" ) ;
    cout << "第" + to_string(now+1) + "行: " + "DIMENSION 文法錯誤!!!!" << endl ;
  	return false ;
  } // else 
} // DIMENSION()

bool GTO( Token temp, int now, int start ) {
  int index ;
  
  if( temp.token.size() == 3 && Identifier( temp.token[start] ) && temp.tokenType[start] != 2 && ( temp.token[temp.token.size()-1] == ";" )){
  	return true ;
  } // else if()
  
  else {
  	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "GTO 文法錯誤!!!!" ) ;
    cout << "第" + to_string(now+1) + "行: " + "GTO 文法錯誤!!!!" << endl ;
  	return false ;
  } // else 
} // GTO() 

void addASSIGNMENT( Token temp, int now, int start ){
      vector <string> stack1 ;
      vector <string> stack2 ;
      stack1.push_back( temp.token[start-2] ) ;
      stack2.push_back( temp.token[start-1] ) ;
      for( int i = start ; i < temp.token.size() ; i++ ){
      	if( Identifier( temp.token[i] ) || isInt( temp.token[i] ) || isFloat( temp.token[i] ) ) stack1.push_back( temp.token[i] ) ;
      	else if( temp.token[i] == "(" ) stack2.push_back( temp.token[i] ) ;
      	else if( temp.token[i] == "^" ) stack2.push_back( temp.token[i] ) ;
      	else if( temp.token[i] == "*" || temp.token[i] == "/" ) {
      	  if( stack2[stack2.size()-1] != "^" ) stack2.push_back( temp.token[i] ) ;
      	  else {
      	    for( int j = stack2.size()-1 ; j >= 0 ; j-- ) {
      	  	  if( stack2[j] == "^" ) {
      	  	    string strTemp1 = stack1.back() ;
      	  	    stack1.pop_back() ;
				string strTemp2 = stack1.back() ;
				stack1.pop_back() ;
				stack2.pop_back() ;
				
				int index ;
      	  	    Intermediate in ;
                in.num = intermediate.size() ;
                in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " ^ " + strTemp1 ;
                in.one.push_back( 1 ) ;
                in.one.push_back( 9 ) ;
   
                if( identifier.find( strTemp2, index ) ){
                  in.two.push_back( 5 ) ;
                  in.two.push_back( index ) ;
				} // if()
				
				else if( contain( integer, strTemp2, index ) ){
				  in.two.push_back( 3 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else if( contain( realNumber, strTemp2, index ) ){
				  in.two.push_back( 4 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else if( contain( TEMP, strTemp2, index ) ){
				  in.two.push_back( 0 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else {
				  in.two.push_back( 0 ) ;
                  in.two.push_back( 0 ) ;	
				}

				if( identifier.find( strTemp1, index ) ){
                  in.three.push_back( 5 ) ;
                  in.three.push_back( index ) ;
				} // if()
				
				else if( contain( integer, strTemp1, index ) ){
				  in.three.push_back( 3 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else if( contain( realNumber, strTemp1, index ) ){
				  in.three.push_back( 4 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else if( contain( TEMP, strTemp1, index ) ){
				  in.three.push_back( 0 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else {
				  in.three.push_back( 0 ) ;
                  in.three.push_back( 0 ) ;
				}
               
                in.four.push_back( 0 ) ;
                in.four.push_back( T ) ;
                intermediate.push_back( in ) ;
                
                stack1.push_back( "T" + to_string(T) ) ;
                TEMP.insert( "T" + to_string(T) ) ;
                T++ ;
              } // if()
              
              else {
			    stack2.push_back( temp.token[i] ) ;
			    break ;
			  } // else 
			} // for()
		  } // else()
		} // else if
		
	    else if( temp.token[i] == "+" || temp.token[i] == "-" ) {
      	  if( stack2[stack2.size()-1] != "*" && stack2[stack2.size()-1] != "/" && stack2[stack2.size()-1] != "^" ) stack2.push_back( temp.token[i] ) ;
      	  else {
      	    for( int j = stack2.size()-1 ; j >= 0 ; j-- ) {
      	  	  if( stack2[j] == "*" || stack2[j] == "/" || stack2[j] == "^" ) {
      	  	    string strTemp1 = stack1.back() ;
      	  	    stack1.pop_back() ;
				string strTemp2 = stack1.back() ;
				stack1.pop_back() ;
				stack2.pop_back() ;
				
				int index ;
      	  	    Intermediate in ;
                in.num = intermediate.size() ;
                if( stack2[j] == "*" ) {
                  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " * " + strTemp1 ;
                  in.one.push_back( 1 ) ;
                  in.one.push_back( 7 ) ;
				} // if()
				
				else if( stack2[j] == "/" ){
				  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " / " + strTemp1 ;
                  in.one.push_back( 1 ) ;
                  in.one.push_back( 8 ) ;
				} // else
				
				else if( stack2[j] == "^" ){
				  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " ^ " + strTemp1 ;
                  in.one.push_back( 1 ) ;
                  in.one.push_back( 9 ) ;
				} // else  
                
                if( identifier.find( strTemp2, index ) ){
                  in.two.push_back( 5 ) ;
                  in.two.push_back( index ) ;
				} // if()
				
				else if( contain( integer, strTemp2, index ) ){
				  in.two.push_back( 3 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else if( contain( realNumber, strTemp2, index ) ){
				  in.two.push_back( 4 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else if( contain( TEMP, strTemp2, index ) ){
				  in.two.push_back( 0 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else {
				  in.two.push_back( 0 ) ;
                  in.two.push_back( index ) ;	
				}
				
				if( identifier.find( strTemp1, index ) ){
                  in.three.push_back( 5 ) ;
                  in.three.push_back( index ) ;
				} // if()
				
				else if( contain( integer, strTemp1, index ) ){
				  in.three.push_back( 3 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else if( contain( realNumber, strTemp1, index ) ){
				  in.three.push_back( 4 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else if( contain( TEMP, strTemp1, index ) ){
				  in.three.push_back( 0 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else{
				  in.three.push_back( 0 ) ;
                  in.three.push_back( 0 ) ;
				}

                in.four.push_back( 0 ) ;
                in.four.push_back( T ) ;
                intermediate.push_back( in ) ;
                
                stack1.push_back( "T" + to_string( T ) ) ;
                TEMP.insert( "T" + to_string(T) ) ;
                T++ ;
              } // if()
              
              else {
			    stack2.push_back( temp.token[i] ) ;
			    break ;
			  } // else 
			} // if()
		  } // for()
		} // else 
		
	    else if( temp.token[i] == ")" ) {
	      for( int z = stack2.size() - 1 ; z >= 0 ; z-- ){
	      	if( stack2[z] == "(" ) {
	      	  stack2.pop_back() ;
	      	  break ;
			} // if()
			
			else {
			  string strTemp1 = stack1.back() ;
      	  	  stack1.pop_back() ;
			  string strTemp2 = stack1.back() ;
			  stack1.pop_back() ;
			  string o = stack2.back() ;
			  stack2.pop_back() ;
				
			  int index ;
      	  	  Intermediate in ;
              in.num = intermediate.size() ;
              in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " " + o + " " + strTemp1 ;
              contain( delimiter, o, index ) ;
              in.one.push_back( 1 ) ;
              in.one.push_back( index ) ;
                
              if( identifier.find( strTemp2, index ) ){
                in.two.push_back( 5 ) ;
                in.two.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp2, index ) ){
				in.two.push_back( 3 ) ;
                in.two.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp2, index ) ){
				in.two.push_back( 4 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp2, index ) ){
				in.two.push_back( 0 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else {
			  	in.two.push_back( 0 ) ;
                in.two.push_back( 0 ) ;
			  }
				
			  if( identifier.find( strTemp1, index ) ){
                in.three.push_back( 5 ) ;
                in.three.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp1, index ) ){
				in.three.push_back( 3 ) ;
                in.three.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp1, index ) ){
				in.three.push_back( 4 ) ;
                in.three.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp1, index ) ){
				in.three.push_back( 0 ) ;
                in.three.push_back( index ) ;
			  } // else if()
			  
			  else{
				in.three.push_back( 0 ) ;
                in.three.push_back( 0 ) ;
			  }

              in.four.push_back( 0 ) ;
              in.four.push_back( T ) ;
              intermediate.push_back( in ) ;
              
              stack1.push_back( "T" + to_string( T ) ) ;
              TEMP.insert( "T" + to_string(T) ) ;
              T++ ;
            } // else ()
		  } // for()
		} // else 
		
		else {
		  for( int z = stack2.size()-1 ; z >= 0 ; z-- ) {
		  	if( stack2[z] != "=" ){
		  	  string strTemp1 = stack1.back() ;
      	  	  stack1.pop_back() ;
			  string strTemp2 = stack1.back() ;
			  stack1.pop_back() ;
			  string o = stack2.back() ;
			  stack2.pop_back() ;
				
			  int index ;
      	  	  Intermediate in ;
              in.num = intermediate.size() ;
              in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " " + o + " " + strTemp1 ;
              contain( delimiter, o, index ) ;
              in.one.push_back( 1 ) ;
              in.one.push_back( index ) ;
                
              if( identifier.find( strTemp2, index ) ){
                in.two.push_back( 5 ) ;
                in.two.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp2, index ) ){
				in.two.push_back( 3 ) ;
                in.two.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp2, index ) ){
				in.two.push_back( 4 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp2, index ) ){
				in.two.push_back( 0 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else{
			  	in.two.push_back( 0 ) ;
                in.two.push_back( 0 ) ;
			  }
				
			  if( identifier.find( strTemp1, index ) ){
                in.three.push_back( 5 ) ;
                in.three.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp1, index ) ){
				in.three.push_back( 3 ) ;
                in.three.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp1, index ) ){
				in.three.push_back( 4 ) ;
                in.three.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp1, index ) ){
				in.three.push_back( 0 ) ;
                in.three.push_back( index ) ;
			  } // else if()
			  
			  else {
			  	in.three.push_back( 0 ) ;
                in.three.push_back( 0 ) ;
			  }

              in.four.push_back( 0 ) ;
              in.four.push_back( T ) ;
              intermediate.push_back( in ) ;
              
              stack1.push_back( "T" + to_string( T ) ) ;
              TEMP.insert( "T" + to_string(T) ) ;
              T++ ;
			} // if()
			
			else {
			  string strTemp1 = stack1.back() ;
      	  	  stack1.pop_back() ;
      	  	  string strTemp2 = stack1.back() ;
      	  	  stack1.pop_back() ;
			  stack2.pop_back() ;
				
			  int index ;
      	  	  Intermediate in ;
              in.num = intermediate.size() ;
              in.str = strTemp2 + " = " + strTemp1 ;
              in.one.push_back( 1 ) ;
              in.one.push_back( 4 ) ;
                
              if( identifier.find( strTemp1, index ) ){
                in.two.push_back( 5 ) ;
                in.two.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp1, index ) ){
				in.two.push_back( 3 ) ;
                in.two.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp1, index ) ){
				in.two.push_back( 4 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp1, index ) ){
			  	in.two.push_back( 0 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else {
			  	in.two.push_back( 0 ) ;
                in.two.push_back( 0 ) ;
			  }
				
              in.three.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              
			  if( identifier.find( strTemp2, index ) ){
                in.four.push_back( 5 ) ;
                in.four.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp2, index ) ){
				in.four.push_back( 3 ) ;
                in.four.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp2, index ) ){
				in.four.push_back( 4 ) ;
                in.four.push_back( index ) ;
			  } // else if()
			  
			  else {
			  	in.four.push_back( 0 ) ;
                in.four.push_back( 0 ) ;
			  }
              
              intermediate.push_back( in ) ;
			} // else 
		  } // for
		} // else 
	  } // for()
} // addASSIGNMENT()

bool ASSIGNMENT( Token temp, int now, int start ) {  
  int index ;
  if( temp.token[temp.token.size()-1] == ";" ){
	if( EXP( temp, start, temp.token.size()-2 ) ) {
      if( !identifier.contain( temp.token[start-2], index )  || 
	    ( identifier.contain( temp.token[start-2], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){ 
		index = identifier.hashFunction( temp.token[start-2] ) ;
	    identifier.table[index].subroutine = nowFunctionIndex ;
	  } // if 
	  
	  EXP( temp, start, temp.token.size()-2, now ) ;
      
	  return true ;
   	} // if
		
	else {
	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
	  cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" << endl ;
	  return false ;
	} // else 
  } // if()
  
  else {
	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ;
	cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" << endl ;
  	return false ;
  } // else 
} // ASSIGNMENT()

void addASSIGNMENTHaveArray( Token temp, int now, int start ){
  vector <string> stack1 ;
  vector <string> stack2 ;
  int index ;
      stack1.push_back( temp.token[start-2] ) ;
      stack2.push_back( temp.token[start-1] ) ;
      for( int i = start ; i < temp.token.size() ; i++ ){
	      	if( Identifier( temp.token[i-1] ) && identifier.find( temp.token[i-1], index ) && temp.token[i] == "(" ){ // Array
	          if( i + 2 < temp.token.size() && temp.token[i+2] == ")" ){ // 一維 
	          	int index ;
	      	  	Intermediate in ;
	            in.num = intermediate.size() ;
	            in.str = "T" + to_string(T) + " = " + temp.token[i-1] + temp.token[i] + temp.token[i+1] + temp.token[i+2] ;
	            in.one.push_back( 1 ) ;
	            in.one.push_back( 4 ) ;
	            
				if( identifier.find( temp.token[i-1], index ) ){
	              in.two.push_back( 5 ) ;
	              in.two.push_back( index ) ;
				} // if()
				
				else{
				  in.two.push_back( 5 ) ;
	              in.two.push_back( 0 ) ;
				}
					
				if( identifier.find( temp.token[i+1], index ) ){
	              in.three.push_back( 5 ) ;
	              in.three.push_back( index ) ;
				} // if()
					
				else if( contain( integer, temp.token[i+1], index ) ){
				  in.three.push_back( 3 ) ;
	              in.three.push_back( index ) ;
				} // else if()
				
                else{
                  in.three.push_back( 0 ) ;
	              in.three.push_back( 0 ) ;
				}				
	
	            in.four.push_back( 0 ) ;
	            in.four.push_back( T ) ;
	            intermediate.push_back( in ) ;
	                
	            stack1.push_back( "T" + to_string(T) ) ;
	            TEMP.insert( "T" + to_string(T) ) ;
	            T++ ;
	            i = i + 2 ;
			  } // if()
			  
			  else if( i + 4 < temp.token.size() && temp.token[i+2] == "," && temp.token[i+4] == ")" ) { // 二維 
	      	  	Intermediate in ;
	            in.num = intermediate.size() ;
	            in.str = "T" + to_string(T) + " = " + temp.token[i+1] + "-1" ;
	            in.one.push_back( 1 ) ;
	            in.one.push_back( 6 ) ;
	            
				if( identifier.find( temp.token[i+1], index ) ){
	              in.two.push_back( 5 ) ;
	              in.two.push_back( index ) ;
				} // if()
				
				else if( contain( integer, temp.token[i+1], index ) ){
	              in.two.push_back( 3 ) ;
	              in.two.push_back( index ) ;
				} // if()
					
				else{
	              in.two.push_back( 0 ) ;
	              in.two.push_back( 0 ) ;					
				}
				
				integer.insert( "1" ) ;
				in.three.push_back( 3 ) ;
	            in.three.push_back( integer.table.size() -1 ) ;
	
	            in.four.push_back( 0 ) ;
	            in.four.push_back( T ) ;
	            intermediate.push_back( in ) ;
	                
	            stack1.push_back( "T" + to_string(T) ) ;
	            TEMP.insert( "T" + to_string(T) ) ;
	            T++ ;
	            
	            //////////////////////////////////////////////////
	            Intermediate in1 ;
	            identifier.find( temp.token[i-1], index ) ;
	            
	            int column = information[identifier.table[index].pointer[1]+2];
	
	            string s = stack1.back() ;
	            stack1.pop_back() ;
	            
	            in1.num = intermediate.size() ;
	            in1.str = "T" + to_string(T) + " = " + s + "*" + to_string(column) ;
	            in1.one.push_back( 1 ) ;
	            in1.one.push_back( 7 ) ;
				
				if( contain( TEMP, s, index ) ){
	              in1.two.push_back( 0 ) ;
	              in1.two.push_back( index ) ;
				} // if()
					
				integer.insert( to_string(column) ) ;
				in1.three.push_back( 3 ) ;
	            in1.three.push_back( integer.table.size() -1 ) ;
	
	            in1.four.push_back( 0 ) ;
	            in1.four.push_back( T ) ;
	            intermediate.push_back( in1 ) ;        
				  
	            stack1.push_back( "T" + to_string(T) ) ;
	            TEMP.insert( "T" + to_string(T) ) ;
	            T++ ; 
	            
	            ///////////////////////////////////////
	            Intermediate in2 ;
	            string str = stack1.back() ;
	            stack1.pop_back() ;
	            
	            in2.num = intermediate.size() ;
	            in2.str = "T" + to_string(T) + " = " + str + "+" + temp.token[i+3] ;
	            in2.one.push_back( 1 ) ;
	            in2.one.push_back( 5 ) ;
				
				if( contain( TEMP, str, index ) ){
	              in2.two.push_back( 0 ) ;
	              in2.two.push_back( index ) ;
				} // if()
				
				else{
				  in2.two.push_back( 0 ) ;
	              in2.two.push_back( 0 ) ;
				}
				
				if( identifier.find( temp.token[i+3], index ) ){
	              in2.three.push_back( 5 ) ;
	              in2.three.push_back( index ) ;
				} // if()
				
				else if( contain( integer, temp.token[i+3], index ) ){
	              in2.three.push_back( 3 ) ;
	              in2.three.push_back( index ) ;
				} // if()
				
				else{
	              in2.three.push_back( 0 ) ;
	              in2.three.push_back( 0 ) ;					
				}
	
	            in2.four.push_back( 0 ) ;
	            in2.four.push_back( T ) ;
	            intermediate.push_back( in2 ) ;
	            
	                
	            stack1.push_back( "T" + to_string(T) ) ;
	            TEMP.insert( "T" + to_string(T) ) ;
	            T++ ;
	            
	            ////////////////////////////////////////////////////////
	            Intermediate in3 ;
	            s = stack1.back() ;
	            stack1.pop_back() ;
	            
	            in3.num = intermediate.size() ;
	            in3.str = "T" + to_string(T) + " = " + temp.token[i-1] + "(" + s + ")" ;
	            in3.one.push_back( 1 ) ;
	            in3.one.push_back( 4 ) ;
				
				if( identifier.find( temp.token[i-1], index ) ){
	              in3.two.push_back( 5 ) ;
	              in3.two.push_back( index ) ;
				} // if()
				
				else{
	              in3.two.push_back( 0 ) ;
	              in3.two.push_back( 0 ) ;					
				}
				
				if( contain( TEMP, str, index ) ){
	              in3.three.push_back( 0 ) ;
	              in3.three.push_back( index ) ;
				} // if()
				
				else{
	              in3.three.push_back( 0 ) ;
	              in3.three.push_back( 0 ) ;					
				}
	
	            in3.four.push_back( 0 ) ;
	            in3.four.push_back( T ) ;
	            intermediate.push_back( in3 ) ;
	                
	            stack1.push_back( "T" + to_string(T) ) ;
	            TEMP.insert( "T" + to_string(T) ) ;
	            T++ ;
	            i = i + 4 ;
			  } // else if()
			  
			  else if( i + 6 < temp.token.size() && temp.token[i+2] == "," && temp.token[i+4] == "," && temp.token[i+6] == ")" ) { // 三維 
			  	
			  } // else if()
			} // if()
			
		else if( ( i + 1 < temp.token.size() && Identifier( temp.token[i] ) && temp.token[i+1] != "(" ) || isInt( temp.token[i] ) || isFloat( temp.token[i] ) ) stack1.push_back( temp.token[i] ) ;
	    else if( temp.token[i] == "(" ) stack2.push_back( temp.token[i] ) ;
      	else if( temp.token[i] == "^" ) stack2.push_back( temp.token[i] ) ;
      	else if( temp.token[i] == "*" || temp.token[i] == "/" ) {
      	  if( stack2[stack2.size()-1] != "^" ) stack2.push_back( temp.token[i] ) ;
      	  else {
      	    for( int j = stack2.size()-1 ; j >= 0 ; j-- ) {
      	  	  if( stack2[j] == "^" ) {
      	  	    string strTemp1 = stack1.back() ;
      	  	    stack1.pop_back() ;
				string strTemp2 = stack1.back() ;
				stack1.pop_back() ;
				stack2.pop_back() ;
				
				int index ;
      	  	    Intermediate in ;
                in.num = intermediate.size() ;
                in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " ^ " + strTemp1 ;
                in.one.push_back( 1 ) ;
                in.one.push_back( 9 ) ;
                if( identifier.find( strTemp2, index ) ){
                  in.two.push_back( 5 ) ;
                  in.two.push_back( index ) ;
				} // if()
				
				else if( contain( integer, strTemp2, index ) ){
				  in.two.push_back( 3 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else if( contain( realNumber, strTemp2, index ) ){
				  in.two.push_back( 4 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else if( contain( TEMP, strTemp2, index ) ){
				  in.two.push_back( 0 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else {
				  in.two.push_back( 0 ) ;
                  in.two.push_back( 0 ) ;
				}
				
				if( identifier.find( strTemp1, index ) ){
                  in.three.push_back( 5 ) ;
                  in.three.push_back( index ) ;
				} // if()
				
				else if( contain( integer, strTemp1, index ) ){
				  in.three.push_back( 3 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else if( contain( realNumber, strTemp1, index ) ){
				  in.three.push_back( 4 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else if( contain( TEMP, strTemp1, index ) ){
				  in.three.push_back( 0 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else{
				  in.three.push_back( 0 ) ;
                  in.three.push_back( 0 ) ;
				}

                in.four.push_back( 0 ) ;
                in.four.push_back( T ) ;
                intermediate.push_back( in ) ;
                
                stack1.push_back( "T" + to_string(T) ) ;
                TEMP.insert( "T" + to_string(T) ) ;
                T++ ;
              } // if()
              
              else {
			    stack2.push_back( temp.token[i] ) ;
			    break ;
			  } // else 
			} // for()
		  } // else()
		} // else if
		
	    else if( temp.token[i] == "+" || temp.token[i] == "-" ) {
      	  if( stack2[stack2.size()-1] != "*" && stack2[stack2.size()-1] != "/" && stack2[stack2.size()-1] != "^" ) stack2.push_back( temp.token[i] ) ;
      	  else {
      	    for( int j = stack2.size()-1 ; j >= 0 ; j-- ) {
      	  	  if( stack2[j] == "*" || stack2[j] == "/" || temp.token[i] == "^" ) {
      	  	    string strTemp1 = stack1.back() ;
      	  	    stack1.pop_back() ;
				string strTemp2 = stack1.back() ;
				stack1.pop_back() ;
				stack2.pop_back() ;
				
				int index ;
      	  	    Intermediate in ;
                in.num = intermediate.size() ;
                if( stack2[j] == "*" ) {
                  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " * " + strTemp1 ;
                  in.one.push_back( 1 ) ;
                  in.one.push_back( 7 ) ;
				} // if()
				
				else if( stack2[j] == "/" ){
				  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " / " + strTemp1 ;
                  in.one.push_back( 1 ) ;
                  in.one.push_back( 8 ) ;
				} // else 
				
				else {
				  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " ^ " + strTemp1 ;
                  in.one.push_back( 1 ) ;
                  in.one.push_back( 9 ) ;
				}
                
                if( identifier.find( strTemp2, index ) ){
                  in.two.push_back( 5 ) ;
                  in.two.push_back( index ) ;
				} // if()
				
				else if( contain( integer, strTemp2, index ) ){
				  in.two.push_back( 3 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else if( contain( realNumber, strTemp2, index ) ){
				  in.two.push_back( 4 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else if( contain( TEMP, strTemp2, index ) ){
				  in.two.push_back( 0 ) ;
                  in.two.push_back( index ) ;
				} // else if()
				
				else{
				  in.two.push_back( 0 ) ;
                  in.two.push_back( 0 ) ;
				}
				
				if( identifier.find( strTemp1, index ) ){
                  in.three.push_back( 5 ) ;
                  in.three.push_back( index ) ;
				} // if()
				
				else if( contain( integer, strTemp1, index ) ){
				  in.three.push_back( 3 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else if( contain( realNumber, strTemp1, index ) ){
				  in.three.push_back( 4 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else if( contain( TEMP, strTemp1, index ) ){
				  in.three.push_back( 0 ) ;
                  in.three.push_back( index ) ;
				} // else if()
				
				else{
				  in.three.push_back( 0 ) ;
                  in.three.push_back( 0 ) ;				
				}

                in.four.push_back( 0 ) ;
                in.four.push_back( T ) ;
                intermediate.push_back( in ) ;
                
                stack1.push_back( "T" + to_string( T ) ) ;
                TEMP.insert( "T" + to_string(T) ) ;
                T++ ;
              } // if()
              
              else {
			    stack2.push_back( temp.token[i] ) ;
			    break ;
			  } // else 
			} // if()
		  } // for()
		} // else 
		
	    else if( temp.token[i] == ")" ) {
	      for( int z = stack2.size() - 1 ; z >= 0 ; z-- ){
	      	if( stack2[z] == "(" ) {
	      	  stack2.pop_back() ;
	      	  break ;
			} // if()
			
			else {
			  string strTemp1 = stack1.back() ;
      	  	  stack1.pop_back() ;
			  string strTemp2 = stack1.back() ;
			  stack1.pop_back() ;
			  string o = stack2.back() ;
			  stack2.pop_back() ;
				
			  int index ;
      	  	  Intermediate in ;
              in.num = intermediate.size() ;
              in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " " + o + " " + strTemp1 ;
              contain( delimiter, o, index ) ;
              in.one.push_back( 1 ) ;
              in.one.push_back( index ) ;
                
              if( identifier.find( strTemp2, index ) ){
                in.two.push_back( 5 ) ;
                in.two.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp2, index ) ){
				in.two.push_back( 3 ) ;
                in.two.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp2, index ) ){
				in.two.push_back( 4 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp2, index ) ){
				in.two.push_back( 0 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else{
				in.two.push_back( 0 ) ;
                in.two.push_back( 0 ) ;			  	
			  }
				
			  if( identifier.find( strTemp1, index ) ){
                in.three.push_back( 5 ) ;
                in.three.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp1, index ) ){
				in.three.push_back( 3 ) ;
                in.three.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp1, index ) ){
				in.three.push_back( 4 ) ;
                in.three.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp1, index ) ){
				in.three.push_back( 0 ) ;
                in.three.push_back( index ) ;
			  } // else if()
			  
			  else{
				in.three.push_back( 0 ) ;
                in.three.push_back( 0 ) ;			  	
			  }

              in.four.push_back( 0 ) ;
              in.four.push_back( T ) ;
              intermediate.push_back( in ) ;
              
              stack1.push_back( "T" + to_string( T ) ) ;
              TEMP.insert( "T" + to_string(T) ) ;
              T++ ;
            } // else ()
		  } // for()
		} // else 
		
		else if( i == temp.token.size() -1 ){
		  for( int z = stack2.size()-1 ; z >= 0 ; z-- ) {
		  	if( stack2[z] != "=" ){
		  	  string strTemp1 = stack1.back() ;
      	  	  stack1.pop_back() ;
			  string strTemp2 = stack1.back() ;
			  stack1.pop_back() ;
			  string o = stack2.back() ;
			  stack2.pop_back() ;
				
			  int index ;
      	  	  Intermediate in ;
              in.num = intermediate.size() ;
              in.str = "T" + to_string(T) + " = " + strTemp2 + " " + o + " " + strTemp1 ;
              contain( delimiter, o, index ) ;
              in.one.push_back( 1 ) ;
              in.one.push_back( index ) ;
                
              if( identifier.find( strTemp2, index ) ){
                in.two.push_back( 5 ) ;
                in.two.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp2, index ) ){
				in.two.push_back( 3 ) ;
                in.two.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp2, index ) ){
				in.two.push_back( 4 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp2, index ) ){
				in.two.push_back( 0 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else{
				in.two.push_back( 0 ) ;
                in.two.push_back( 0 ) ;			  	
			  }
				
			  if( identifier.find( strTemp1, index ) ){
                in.three.push_back( 5 ) ;
                in.three.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp1, index ) ){
				in.three.push_back( 3 ) ;
                in.three.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp1, index ) ){
				in.three.push_back( 4 ) ;
                in.three.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp1, index ) ){
				in.three.push_back( 0 ) ;
                in.three.push_back( index ) ;
			  } // else if()
			  
			  else{
				in.three.push_back( 0 ) ;
                in.three.push_back( 0 ) ;			  	
			  }

              in.four.push_back( 0 ) ;
              in.four.push_back( T ) ;
              intermediate.push_back( in ) ;
              
              stack1.push_back( "T" + to_string( T ) ) ;
              TEMP.insert( "T" + to_string(T) ) ;
              T++ ;
			} // if()
			
			else {
			  string strTemp1 = stack1.back() ;
      	  	  stack1.pop_back() ;
      	  	  string strTemp2 = stack1.back() ;
      	  	  stack1.pop_back() ;
			  stack2.pop_back() ;
				
			  int index ;
      	  	  Intermediate in ;
              in.num = intermediate.size() ;
              in.str = strTemp2 + " = " + strTemp1 ;
              in.one.push_back( 1 ) ;
              in.one.push_back( 4 ) ;
                
              if( identifier.find( strTemp1, index ) ){
                in.two.push_back( 5 ) ;
                in.two.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp1, index ) ){
				in.two.push_back( 3 ) ;
                in.two.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp1, index ) ){
				in.two.push_back( 4 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else if( contain( TEMP, strTemp1, index ) ){
			  	in.two.push_back( 0 ) ;
                in.two.push_back( index ) ;
			  } // else if()
			  
			  else{
			  	in.two.push_back( 0 ) ;
                in.two.push_back( 0 ) ;
			  }
				
              in.three.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              
			  if( identifier.find( strTemp2, index ) ){
                in.four.push_back( 5 ) ;
                in.four.push_back( index ) ;
			  } // if()
				
			  else if( contain( integer, strTemp2, index ) ){
				in.four.push_back( 3 ) ;
                in.four.push_back( index ) ;
			  } // else if()
				
			  else if( contain( realNumber, strTemp2, index ) ){
				in.four.push_back( 4 ) ;
                in.four.push_back( index ) ;
			  } // else if()
              
              else{
				in.four.push_back( 0 ) ;
                in.four.push_back( 0 ) ;              	
			  }
              
              intermediate.push_back( in ) ;
			} // else 
		  } // for
		} // else 
	  } // for()
} // addASSIGNMENTHaveArray()

bool ASSIGNMENTHaveArray( Token temp, int now, int start ){
  int index ;
  if( temp.token[temp.token.size()-1] == ";" ){
	if( EXP1( temp, start, temp.token.size()-2 ) ) {	  
	  if( !identifier.contain( temp.token[start-2], index )  || 
	    ( identifier.contain( temp.token[start-2], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){ 

		index = identifier.hashFunction( temp.token[start-2] ) ;
	    identifier.table[index].subroutine = nowFunctionIndex ;
	  } // if 
	  
	  EXP1( temp, start, temp.token.size()-2, now ) ;
	  
	  return true ;
	} // if
		
	else {
	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ;
	  cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENTHaveArray 文法錯誤!!!" << endl ;
	  return false ;
	} // else 
  } // if()

  else {
	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ;
	cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENTHaveArray 文法錯誤!!!" << endl ;
  	return false ;
  } // else 
} // ASSIGNMENTHaveArray()

void addASSIGNMENTHaveArrayInHeadAndHaveArray( Token temp, int now, int start ){
  vector <string> stack1 ;
  vector <string> stack2 ;
  int index ;
  int right ;
  for( int i = start ; i < temp.token.size() ; i++ ) {
  	  if( temp.token[i] == ")" ) {
  	    right = i ;
  	    break ;
	  } // if()
  } // for
  
  string tempp = "" ;
  for( int i = start-2 ; i < right + 1 ; i++ ) tempp = tempp + temp.token[i] ;
          
  stack1.push_back( tempp ) ;
  stack2.push_back( temp.token[right+1] ) ;
  for( int i = right+2 ; i < temp.token.size() ; i++ ){
	if( Identifier( temp.token[i-1] ) && identifier.find( temp.token[i-1], index ) && temp.token[i] == "(" ){ // Array
	  if( i + 2 < temp.token.size() && temp.token[i+2] == ")" ){ // 一維 
	    int index ;
	    Intermediate in ;
	    in.num = intermediate.size() ;
	    in.str = "T" + to_string(T) + " = " + temp.token[i-1] + temp.token[i] + temp.token[i+1] + temp.token[i+2] ;
	    in.one.push_back( 1 ) ;
	    in.one.push_back( 4 ) ;
	            
		if( identifier.find( temp.token[i-1], index ) ){
	      in.two.push_back( 5 ) ;
	      in.two.push_back( index ) ;
		} // if()
					
		if( identifier.find( temp.token[i+1], index ) ){
	      in.three.push_back( 5 ) ;
	      in.three.push_back( index ) ;
		} // if()
					
		else if( contain( integer, temp.token[i+1], index ) ){
		  in.three.push_back( 3 ) ;
	      in.three.push_back( index ) ;
		} // else if()
	
	    in.four.push_back( 0 ) ;
	    in.four.push_back( T ) ;
	    intermediate.push_back( in ) ;
	                
	    stack1.push_back( "T" + to_string(T) ) ;
	    TEMP.insert( "T" + to_string(T) ) ;
	    T++ ;
	    i = i + 2 ;
	  } // if()
			  
	  else if( i + 4 < temp.token.size() && temp.token[i+2] == "," && temp.token[i+4] == ")" ) { // 二維 
	    Intermediate in ;
	    in.num = intermediate.size() ;
	    in.str = "T" + to_string(T) + " = " + temp.token[i+3] + "-1" ;
	    in.one.push_back( 1 ) ;
	    in.one.push_back( 6 ) ;
	            
		if( identifier.find( temp.token[i+3], index ) ){
	      in.two.push_back( 5 ) ;
	      in.two.push_back( index ) ;
		} // if()
				
		else if( contain( integer, temp.token[i+3], index ) ){
	      in.two.push_back( 3 ) ;
	      in.two.push_back( index ) ;
		} // if()
					
		integer.insert( "1" ) ;
		in.three.push_back( 3 ) ;
	    in.three.push_back( integer.table.size() -1 ) ;
	
	    in.four.push_back( 0 ) ;
	    in.four.push_back( T ) ;
	    intermediate.push_back( in ) ;
	                
	    stack1.push_back( "T" + to_string(T) ) ;
	    TEMP.insert( "T" + to_string(T) ) ;
	    T++ ;
	            
	    //////////////////////////////////////////////////
	    Intermediate in1 ;
	    identifier.find( temp.token[i-1], index ) ;
	    int column = information[identifier.table[index].pointer[1]+2];
	    string s = stack1.back() ;
	    stack1.pop_back() ;
	            
	    in1.num = intermediate.size() ;
	    in1.str = "T" + to_string(T) + " = " + s + "*" + to_string(column) ;
	    in1.one.push_back( 1 ) ;
	    in1.one.push_back( 7 ) ;
				
		if( contain( TEMP, s, index ) ){
	      in1.two.push_back( 0 ) ;
	      in1.two.push_back( index ) ;
		} // if()
					
		integer.insert( to_string(column) ) ;
		in1.three.push_back( 3 ) ;
        in1.three.push_back( integer.table.size() -1 ) ;

        in1.four.push_back( 0 ) ;
        in1.four.push_back( T ) ;
        intermediate.push_back( in1 ) ;        
		  
        stack1.push_back( "T" + to_string(T) ) ;
        TEMP.insert( "T" + to_string(T) ) ;
        T++ ; 

        ///////////////////////////////////////
        Intermediate in2 ;
        string str = stack1.back() ;
        stack1.pop_back() ;
        
        in2.num = intermediate.size() ;
        in2.str = "T" + to_string(T) + " = " + str + "+" + temp.token[i+1] ;
        in2.one.push_back( 1 ) ;
        in2.one.push_back( 5 ) ;
		
		if( contain( TEMP, str, index ) ){
          in2.two.push_back( 0 ) ;
          in2.two.push_back( index ) ;
		} // if()
		
		if( identifier.find( temp.token[i+1], index ) ){
          in2.three.push_back( 5 ) ;
          in2.three.push_back( index ) ;
		} // if()
		
		else if( contain( integer, temp.token[i+1], index ) ){
          in2.three.push_back( 3 ) ;
          in2.three.push_back( index ) ;
		} // if()

        in2.four.push_back( 0 ) ;
        in2.four.push_back( T ) ;
        intermediate.push_back( in2 ) ;
        
        stack1.push_back( "T" + to_string(T) ) ;
        TEMP.insert( "T" + to_string(T) ) ;
        T++ ;
        
        ////////////////////////////////////////////////////////
        Intermediate in3 ;
        s = stack1.back() ;
        stack1.pop_back() ;
        
        in3.num = intermediate.size() ;
        in3.str = "T" + to_string(T) + " = " + temp.token[i-1] + "(" + s + ")" ;
        in3.one.push_back( 1 ) ;
        in3.one.push_back( 4 ) ;
		
		if( identifier.find( temp.token[i-1], index ) ){
          in3.two.push_back( 5 ) ;
          in3.two.push_back( index ) ;
		} // if()
		
		if( contain( TEMP, s, index ) ){
          in3.three.push_back( 0 ) ;
          in3.three.push_back( index ) ;
		} // if()

        in3.four.push_back( 0 ) ;
        in3.four.push_back( T ) ;
        intermediate.push_back( in3 ) ;
            
        stack1.push_back( "T" + to_string(T) ) ;
        TEMP.insert( "T" + to_string(T) ) ;
        T++ ;
        i = i + 4 ;
	  } // else if()
	  
	  else if( i + 6 < temp.token.size() && temp.token[i+2] == "," && temp.token[i+4] == "," && temp.token[i+6] == ")" ) { // 三維 
	  	
	  } // else if()
	} // if()
	
	else if( ( i + 1 < temp.token.size() && Identifier( temp.token[i] ) && temp.token[i+1] != "(" ) || isInt( temp.token[i] ) || isFloat( temp.token[i] ) ) stack1.push_back( temp.token[i] ) ;
    else if( temp.token[i] == "(" ) stack2.push_back( temp.token[i] ) ;
  	else if( temp.token[i] == "^" ) stack2.push_back( temp.token[i] ) ;
  	else if( temp.token[i] == "*" || temp.token[i] == "/" ) {
  	  if( stack2[stack2.size()-1] != "^" ) stack2.push_back( temp.token[i] ) ;
  	  else {
  	    for( int j = stack2.size()-1 ; j >= 0 ; j-- ) {
  	  	  if( stack2[j] == "^" ) {
  	  	    string strTemp1 = stack1.back() ;
  	  	    stack1.pop_back() ;
			string strTemp2 = stack1.back() ;
			stack1.pop_back() ;
			stack2.pop_back() ;
			
			int index ;
  	  	    Intermediate in ;
            in.num = intermediate.size() ;
            in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " ^ " + strTemp1 ;
            in.one.push_back( 1 ) ;
            in.one.push_back( 9 ) ;
            if( identifier.find( strTemp2, index ) ){
              in.two.push_back( 5 ) ;
              in.two.push_back( index ) ;
			} // if()
			
			else if( contain( integer, strTemp2, index ) ){
			  in.two.push_back( 3 ) ;
              in.two.push_back( index ) ;
			} // else if()
			
			else if( contain( realNumber, strTemp2, index ) ){
			  in.two.push_back( 4 ) ;
              in.two.push_back( index ) ;
			} // else if()
			
			else if( contain( TEMP, strTemp2, index ) ){
			  in.two.push_back( 0 ) ;
              in.two.push_back( index ) ;
			} // else if()
			
			else{
			  in.two.push_back( 0 ) ;
              in.two.push_back( 0 ) ;
			}
			
			if( identifier.find( strTemp1, index ) ){
              in.three.push_back( 5 ) ;
              in.three.push_back( index ) ;
			} // if()
			
			else if( contain( integer, strTemp1, index ) ){
			  in.three.push_back( 3 ) ;
              in.three.push_back( index ) ;
			} // else if()
			
			else if( contain( realNumber, strTemp1, index ) ){
			  in.three.push_back( 4 ) ;
              in.three.push_back( index ) ;
			} // else if()
			
			else if( contain( TEMP, strTemp1, index ) ){
			  in.three.push_back( 0 ) ;
              in.three.push_back( index ) ;
			} // else if()
			
			else{
			  in.three.push_back( 0 ) ;
              in.three.push_back( 0 ) ;				
			}

            in.four.push_back( 0 ) ;
            in.four.push_back( T ) ;
            intermediate.push_back( in ) ;
            
            stack1.push_back( "T" + to_string(T) ) ;
            TEMP.insert( "T" + to_string(T) ) ;
            T++ ;
          } // if()
          
          else {
		    stack2.push_back( temp.token[i] ) ;
		    break ;
		  } // else 
		} // for()
	  } // else()
	} // else if
	
    else if( temp.token[i] == "+" || temp.token[i] == "-" ) {
  	  if( stack2[stack2.size()-1] != "*" && stack2[stack2.size()-1] != "/" && stack2[stack2.size()-1] != "^" ) stack2.push_back( temp.token[i] ) ;
  	  else {
  	    for( int j = stack2.size()-1 ; j >= 0 ; j-- ) {
  	  	  if( stack2[j] == "*" || stack2[j] == "/" || stack2[j] == "^" ) {
  	  	    string strTemp1 = stack1.back() ;
  	  	    stack1.pop_back() ;
			string strTemp2 = stack1.back() ;
			stack1.pop_back() ;
			stack2.pop_back() ;
			
			int index ;
  	  	    Intermediate in ;
            in.num = intermediate.size() ;
            if( stack2[j] == "*" ) {
              in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " * " + strTemp1 ;
              in.one.push_back( 1 ) ;
              in.one.push_back( 7 ) ;
			} // if()
			
			else if( stack2[j] == "/" ){
			  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " / " + strTemp1 ;
              in.one.push_back( 1 ) ;
              in.one.push_back( 8 ) ;
			} // else 
			
			else {
			  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " ^ " + strTemp1 ;
              in.one.push_back( 1 ) ;
              in.one.push_back( 9 ) ;
			} // else 
            
            if( identifier.find( strTemp2, index ) ){
              in.two.push_back( 5 ) ;
              in.two.push_back( index ) ;
			} // if()
			
			else if( contain( integer, strTemp2, index ) ){
			  in.two.push_back( 3 ) ;
              in.two.push_back( index ) ;
			} // else if()
			
			else if( contain( realNumber, strTemp2, index ) ){
			  in.two.push_back( 4 ) ;
              in.two.push_back( index ) ;
			} // else if()
			
			else if( contain( TEMP, strTemp2, index ) ){
			  in.two.push_back( 0 ) ;
              in.two.push_back( index ) ;
			} // else if()
			
			else{
			  in.two.push_back( 0 ) ;
              in.two.push_back( 0 ) ;
			}
			
			if( identifier.find( strTemp1, index ) ){
              in.three.push_back( 5 ) ;
              in.three.push_back( index ) ;
			} // if()
			
			else if( contain( integer, strTemp1, index ) ){
			  in.three.push_back( 3 ) ;
              in.three.push_back( index ) ;
			} // else if()
			
			else if( contain( realNumber, strTemp1, index ) ){
			  in.three.push_back( 4 ) ;
              in.three.push_back( index ) ;
			} // else if()
			
			else if( contain( TEMP, strTemp1, index ) ){
			  in.three.push_back( 0 ) ;
              in.three.push_back( index ) ;
			} // else if()
			
			else{
			  in.three.push_back( 0 ) ;
              in.three.push_back( 0 ) ;				
			}

            in.four.push_back( 0 ) ;
            in.four.push_back( T ) ;
            intermediate.push_back( in ) ;
            
            stack1.push_back( "T" + to_string( T ) ) ;
            TEMP.insert( "T" + to_string(T) ) ;
            T++ ;
          } // if()
          
          else {
		    stack2.push_back( temp.token[i] ) ;
		    break ;
		  } // else 
		} // if()
	  } // for()
	} // else 
	
    else if( temp.token[i] == ")" ) {
      for( int z = stack2.size() - 1 ; z >= 0 ; z-- ){
      	if( stack2[z] == "(" ) {
      	  stack2.pop_back() ;
      	  break ;
		} // if()
		
		else {
		  string strTemp1 = stack1.back() ;
  	  	  stack1.pop_back() ;
		  string strTemp2 = stack1.back() ;
		  stack1.pop_back() ;
		  string o = stack2.back() ;
		  stack2.pop_back() ;
			
		  int index ;
  	  	  Intermediate in ;
          in.num = intermediate.size() ;
          in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " " + o + " " + strTemp1 ;
          contain( delimiter, o, index ) ;
          in.one.push_back( 1 ) ;
          in.one.push_back( index ) ;
            
          if( identifier.find( strTemp2, index ) ){
            in.two.push_back( 5 ) ;
            in.two.push_back( index ) ;
		  } // if()
			
		  else if( contain( integer, strTemp2, index ) ){
			in.two.push_back( 3 ) ;
            in.two.push_back( index ) ;
		  } // else if()
			
		  else if( contain( realNumber, strTemp2, index ) ){
			in.two.push_back( 4 ) ;
            in.two.push_back( index ) ;
		  } // else if()
		  
		  else if( contain( TEMP, strTemp2, index ) ){
			in.two.push_back( 0 ) ;
            in.two.push_back( index ) ;
		  } // else if()
		  
		  else{
			in.two.push_back( 0 ) ;
            in.two.push_back( 0 ) ;		  	
		  }
			
		  if( identifier.find( strTemp1, index ) ){
            in.three.push_back( 5 ) ;
            in.three.push_back( index ) ;
		  } // if()
			
		  else if( contain( integer, strTemp1, index ) ){
			in.three.push_back( 3 ) ;
            in.three.push_back( index ) ;
		  } // else if()
			
		  else if( contain( realNumber, strTemp1, index ) ){
			in.three.push_back( 4 ) ;
            in.three.push_back( index ) ;
		  } // else if()
		  
		  else if( contain( TEMP, strTemp1, index ) ){
			in.three.push_back( 0 ) ;
            in.three.push_back( index ) ;
		  } // else if()
		  
		  else{
			in.three.push_back( 0 ) ;
            in.three.push_back( 0 ) ;		  	
		  }

          in.four.push_back( 0 ) ;
          in.four.push_back( T ) ;
          intermediate.push_back( in ) ;
          
          stack1.push_back( "T" + to_string( T ) ) ;
          TEMP.insert( "T" + to_string(T) ) ;
          T++ ;
        } // else ()
	  } // for()
	} // else 
	
	else if( i == temp.token.size() -1 ){
	  for( int z = stack2.size()-1 ; z >= 0 ; z-- ) {
	  	if( stack2[z] != "=" ){
	  	  string strTemp1 = stack1.back() ;
  	  	  stack1.pop_back() ;
		  string strTemp2 = stack1.back() ;
		  stack1.pop_back() ;
		  string o = stack2.back() ;
		  stack2.pop_back() ;
			
		  int index ;
  	  	  Intermediate in ;
          in.num = intermediate.size() ;
          in.str = "T" + to_string(T) + " = " + strTemp2 + " " + o + " " + strTemp1 ;
          contain( delimiter, o, index ) ;
          in.one.push_back( 1 ) ;
          in.one.push_back( index ) ;
            
          if( identifier.find( strTemp2, index ) ){
            in.two.push_back( 5 ) ;
            in.two.push_back( index ) ;
		  } // if()
			
		  else if( contain( integer, strTemp2, index ) ){
			in.two.push_back( 3 ) ;
            in.two.push_back( index ) ;
		  } // else if()
			
		  else if( contain( realNumber, strTemp2, index ) ){
			in.two.push_back( 4 ) ;
            in.two.push_back( index ) ;
		  } // else if()
		  
		  else if( contain( TEMP, strTemp2, index ) ){
			in.two.push_back( 0 ) ;
            in.two.push_back( index ) ;
		  } // else if()
		  
		  else{
			in.two.push_back( 0 ) ;
            in.two.push_back( 0 ) ;		  	
		  }
			
		  if( identifier.find( strTemp1, index ) ){
            in.three.push_back( 5 ) ;
            in.three.push_back( index ) ;
		  } // if()
			
		  else if( contain( integer, strTemp1, index ) ){
			in.three.push_back( 3 ) ;
            in.three.push_back( index ) ;
		  } // else if()
			
		  else if( contain( realNumber, strTemp1, index ) ){
			in.three.push_back( 4 ) ;
            in.three.push_back( index ) ;
		  } // else if()
		  
		  else if( contain( TEMP, strTemp1, index ) ){
			in.three.push_back( 0 ) ;
            in.three.push_back( index ) ;
		  } // else if()
		  
		  else {
			in.three.push_back( 0 ) ;
            in.three.push_back( 0 ) ;		  	
		  }

          in.four.push_back( 0 ) ;
          in.four.push_back( T ) ;
          intermediate.push_back( in ) ;
          
          stack1.push_back( "T" + to_string( T ) ) ;
          TEMP.insert( "T" + to_string(T) ) ;
          T++ ;
		} // if()
		
		else {
		  string strTemp1 = stack1.back() ;
  	  	  stack1.pop_back() ;
  	  	  string strTemp2 = stack1.back() ;
  	  	  stack1.pop_back() ;
		  stack2.pop_back() ;
			
		  int index ;
  	  	  Intermediate in ;
          in.num = intermediate.size() ;
          in.str = strTemp2 + " = " + strTemp1 ;
          in.one.push_back( 1 ) ;
          in.one.push_back( 4 ) ;
            
          if( identifier.find( strTemp1, index ) ){
            in.two.push_back( 5 ) ;
            in.two.push_back( index ) ;
		  } // if()
			
		  else if( contain( integer, strTemp1, index ) ){
			in.two.push_back( 3 ) ;
            in.two.push_back( index ) ;
		  } // else if()
			
		  else if( contain( realNumber, strTemp1, index ) ){
			in.two.push_back( 4 ) ;
            in.two.push_back( index ) ;
		  } // else if()
		  
		  else if( contain( TEMP, strTemp1, index ) ){
		  	in.two.push_back( 0 ) ;
            in.two.push_back( index ) ;
		  } // else if()
		  
		  else{
		  	in.two.push_back( 0 ) ;
            in.two.push_back( 0 ) ;		  	
		  }
		  
		  string a = "" ;
		  string str = "" ;
		  for( int i = 0 ; i < strTemp2.size() ; i++ ){
		  	if( strTemp2[i] == '(' ){
		  	  a = a + strTemp2[i+1] ;
			  break ; 
			} // if()
			
			str = str + strTemp2[i] ;
		  } // for()
		  
		  if( identifier.find( str, index ) ){
            in.three.push_back( 5 ) ;
            in.three.push_back( index ) ;
		  } // if()
		  
		  else{
            in.three.push_back( 5 ) ;
            in.three.push_back( 0 ) ;		  	
		  }
          
		  if( identifier.find( a, index ) ){
            in.four.push_back( 5 ) ;
            in.four.push_back( index ) ;
		  } // if()
			
		  else if( contain( integer, a, index ) ){
			in.four.push_back( 3 ) ;
            in.four.push_back( index ) ;
		  } // else if()
		  
		  else{
			in.four.push_back( 0 ) ;
            in.four.push_back( 0 ) ;		  	
		  }
          
          intermediate.push_back( in ) ;
		} // else 
	  } // for
	} // else 
  } // for()
} // addASSIGNMENTHaveArrayInHeadAndHaveArray

bool ASSIGNMENTHaveArrayInHeadAndHaveArray( Token temp, int now, int start ){
  int right = 0 ; // )的位置 
  int id = 0 ;
  int common = 0 ;
  int index ;
  
  	for( int i = 2 ; i < temp.token.size() ; i++ ) {
  	  if( temp.token[i] == ")" ) {
  	    right = i ;
  	    break ;
	  } // if()
	  
  	  else if( i % 2 == 0 && !( Identifier( temp.token[i] ) || isInt( temp.token[i] ) ) ) {
  	  	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
  	  	cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENTHaveArrayInHeadAndHaveArray 文法錯誤!!!" << endl ;
  	  	return false ;
	  } // if()
	  
	  else if( i % 2 == 1 && temp.token[i] != "," ) {
  	  	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
  	  	cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENTHaveArrayInHeadAndHaveArray 文法錯誤!!!" << endl ;
  	  	return false ;
	  } // if()
	  
	  else if( i % 2 == 0 && ( Identifier( temp.token[i] ) || isInt( temp.token[i] ) ) ) id++ ;
	  
	  else if( i % 2 == 1 && temp.token[i] == "," ) common++ ;
    } // for()
    
    if( id - common != 1 ) {
  	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
  	  cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENTHaveArrayInHeadAndHaveArray 文法錯誤!!!" << endl ;
  	  	return false ;
  	  return false ;
	} // if()
  		
	if( Identifier( temp.token[0] ) && temp.token[right+1] == "=" && temp.token[temp.token.size()-1] == ";" ){  
	  if( EXP1( temp, right+2, temp.token.size()-2 ) ) {
	    if( !identifier.contain( temp.token[0], index )  || 
	      ( identifier.contain( temp.token[0], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){ 

		  index = identifier.hashFunction( temp.token[0] ) ;
	      identifier.table[index].subroutine = nowFunctionIndex ;
	    } // if 
	    
	    for( int i = 2 ; i < temp.token.size() ; i++ ) {
  	      if( temp.token[i] == ")" ) {
  	        right = i ;
  	        break ;
	      } // if()
	  
	      else if( i % 2 == 0 && ( Identifier( temp.token[i] ) || isInt( temp.token[i] ) ) ) {
	      	if( Identifier( temp.token[i] ) ){
	      	  if( !identifier.contain( temp.token[i], index ) || ( identifier.contain( temp.token[i], index ) && identifier.table[index].subroutine != nowFunctionIndex  ) ){ 
		        index = identifier.hashFunction( temp.token[i] ) ;
	            identifier.table[index].subroutine = nowFunctionIndex ;
	          } // else 
			} // if
			
			else{
			  if( contain( integer, temp.token[i], index ) ) integer.insert( temp.token[i] ) ;
			} // else 
		  } // else if()
        } // for()

        EXP1( temp, right+2, temp.token.size()-2, now ) ;

	    return true ;
	  } // if
		
	  else {
  	  	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
  	  	cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENTHaveArrayInHeadAndHaveArray 文法錯誤!!!" << endl ;
	  	return false ;
	  } // else 
	} // if()
  
  else {
  	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
  	cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENTHaveArrayInHeadAndHaveArray 文法錯誤!!!" << endl ;
  	return false ;
  } // else 	
} // ASSIGNMENTHaveArrayInHeadAndHaveArray()

void addASSIGNMENTHaveArrayInHeadAndWithoutArray( Token temp, int now, int start ){
	  vector <string> stack1 ;
      vector <string> stack2 ;
      int right = 0 ;
      for( int i = start ; i < temp.token.size() ; i++ ) {
  	    if( temp.token[i] == ")" ) {
  	      right = i ;
  	      break ;
	    } // if()
      } // for
      
        string tempp = "" ;
          for( int i = start-2 ; i < right + 1 ; i++ ) tempp = tempp + temp.token[i] ;

	      stack1.push_back( tempp ) ;
	      stack2.push_back( temp.token[right+1] ) ;
	      for( int i = right+2 ; i < temp.token.size() ; i++ ){
	      	if( Identifier( temp.token[i] ) || isInt( temp.token[i] ) || isFloat( temp.token[i] ) ) stack1.push_back( temp.token[i] ) ;
	      	else if( temp.token[i] == "(" ) stack2.push_back( temp.token[i] ) ;
	      	else if( temp.token[i] == "^" ) stack2.push_back( temp.token[i] ) ;
	      	else if( temp.token[i] == "*" || temp.token[i] == "/" ) {
	      	  if( stack2[stack2.size()-1] != "^" ) stack2.push_back( temp.token[i] ) ;
	      	  else {
	      	    for( int j = stack2.size()-1 ; j >= 0 ; j-- ) {
	      	  	  if( stack2[j] == "^" ) {
	      	  	    string strTemp1 = stack1.back() ;
	      	  	    stack1.pop_back() ;
					string strTemp2 = stack1.back() ;
					stack1.pop_back() ;
					stack2.pop_back() ;
					
					int index ;
	      	  	    Intermediate in ;
	                in.num = intermediate.size() ;
	                in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " ^ " + strTemp1 ;
	                in.one.push_back( 1 ) ;
	                in.one.push_back( 9 ) ;
	                if( identifier.find( strTemp2, index ) ){
	                  in.two.push_back( 5 ) ;
	                  in.two.push_back( index ) ;
					} // if()
					
					else if( contain( integer, strTemp2, index ) ){
					  in.two.push_back( 3 ) ;
	                  in.two.push_back( index ) ;
					} // else if()
					
					else if( contain( realNumber, strTemp2, index ) ){
					  in.two.push_back( 4 ) ;
	                  in.two.push_back( index ) ;
					} // else if()
					
					else if( contain( TEMP, strTemp2, index ) ){
					  in.two.push_back( 0 ) ;
	                  in.two.push_back( index ) ;
					} // else if()
					
					else{
					  in.two.push_back( 0 ) ;
	                  in.two.push_back( 0 ) ;
					}
					
					if( identifier.find( strTemp1, index ) ){
	                  in.three.push_back( 5 ) ;
	                  in.three.push_back( index ) ;
					} // if()
					
					else if( contain( integer, strTemp1, index ) ){
					  in.three.push_back( 3 ) ;
	                  in.three.push_back( index ) ;
					} // else if()
					
					else if( contain( realNumber, strTemp1, index ) ){
					  in.three.push_back( 4 ) ;
	                  in.three.push_back( index ) ;
					} // else if()
					
					else if( contain( TEMP, strTemp1, index ) ){
					  in.three.push_back( 0 ) ;
	                  in.three.push_back( index ) ;
					} // else if()
					
					else{
				      in.three.push_back( 0 ) ;
	                  in.three.push_back( 0 ) ;	
					}
	
	                in.four.push_back( 0 ) ;
	                in.four.push_back( T ) ;
	                intermediate.push_back( in ) ;
	                
	                stack1.push_back( "T" + to_string(T) ) ;
	                TEMP.insert( "T" + to_string(T) ) ;
	                T++ ;
	              } // if()
	              
	              else {
				    stack2.push_back( temp.token[i] ) ;
				    break ;
				  } // else 
				} // for()
			  } // else()
			} // else if
			
		    else if( temp.token[i] == "+" || temp.token[i] == "-" ) {
	      	  if( stack2[stack2.size()-1] != "*" && stack2[stack2.size()-1] != "/" && stack2[stack2.size()-1] != "^" ) stack2.push_back( temp.token[i] ) ;
	      	  else {
	      	    for( int j = stack2.size()-1 ; j >= 0 ; j-- ) {
	      	  	  if( stack2[j] == "*" || stack2[j] == "/" || stack2[j] == "^" ) {
	      	  	    string strTemp1 = stack1.back() ;
	      	  	    stack1.pop_back() ;
					string strTemp2 = stack1.back() ;
					stack1.pop_back() ;
					stack2.pop_back() ;
					
					int index ;
	      	  	    Intermediate in ;
	                in.num = intermediate.size() ;
	                if( stack2[j] == "*" ) {
	                  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " * " + strTemp1 ;
	                  in.one.push_back( 1 ) ;
	                  in.one.push_back( 7 ) ;
					} // if()
					
	                else if( stack2[j] == "/" ) {
	                  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " / " + strTemp1 ;
	                  in.one.push_back( 1 ) ;
	                  in.one.push_back( 8 ) ;
					} // else if()					
					
					else {
					  in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " ^ " + strTemp1 ;
	                  in.one.push_back( 1 ) ;
	                  in.one.push_back( 9 ) ;
					} // else 
	                
	                if( identifier.find( strTemp2, index ) ){
	                  in.two.push_back( 5 ) ;
	                  in.two.push_back( index ) ;
					} // if()
					
					else if( contain( integer, strTemp2, index ) ){
					  in.two.push_back( 3 ) ;
	                  in.two.push_back( index ) ;
					} // else if()
					
					else if( contain( realNumber, strTemp2, index ) ){
					  in.two.push_back( 4 ) ;
	                  in.two.push_back( index ) ;
					} // else if()
					
					else if( contain( TEMP, strTemp2, index ) ){
					  in.two.push_back( 0 ) ;
	                  in.two.push_back( index ) ;
					} // else if()
					
					else{
					  in.two.push_back( 0 ) ;
	                  in.two.push_back( 0 ) ;	
					}
					
					if( identifier.find( strTemp1, index ) ){
	                  in.three.push_back( 5 ) ;
	                  in.three.push_back( index ) ;
					} // if()
					
					else if( contain( integer, strTemp1, index ) ){
					  in.three.push_back( 3 ) ;
	                  in.three.push_back( index ) ;
					} // else if()
					
					else if( contain( realNumber, strTemp1, index ) ){
					  in.three.push_back( 4 ) ;
	                  in.three.push_back( index ) ;
					} // else if()
					
					else if( contain( TEMP, strTemp1, index ) ){
					  in.three.push_back( 0 ) ;
	                  in.three.push_back( index ) ;
					} // else if()
					
					else{
					  in.three.push_back( 0 ) ;
	                  in.three.push_back( 0 ) ;						
					}
	
	                in.four.push_back( 0 ) ;
	                in.four.push_back( T ) ;
	                intermediate.push_back( in ) ;
	                
	                stack1.push_back( "T" + to_string( T ) ) ;
	                TEMP.insert( "T" + to_string(T) ) ;
	                T++ ;
	              } // if()
	              
	              else {
				    stack2.push_back( temp.token[i] ) ;
				    break ;
				  } // else 
				} // if()
			  } // for()
			} // else 
			
		    else if( temp.token[i] == ")" ) {
		      for( int z = stack2.size() - 1 ; z >= 0 ; z-- ){
		      	if( stack2[z] == "(" ) {
		      	  stack2.pop_back() ;
		      	  break ;
				} // if()
				
				else {
				  string strTemp1 = stack1.back() ;
	      	  	  stack1.pop_back() ;
				  string strTemp2 = stack1.back() ;
				  stack1.pop_back() ;
				  string o = stack2.back() ;
				  stack2.pop_back() ;
					
				  int index ;
	      	  	  Intermediate in ;
	              in.num = intermediate.size() ;
	              in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " " + o + " " + strTemp1 ;
	              contain( delimiter, o, index ) ;
	              in.one.push_back( 1 ) ;
	              in.one.push_back( index ) ;
	                
	              if( identifier.find( strTemp2, index ) ){
	                in.two.push_back( 5 ) ;
	                in.two.push_back( index ) ;
				  } // if()
					
				  else if( contain( integer, strTemp2, index ) ){
					in.two.push_back( 3 ) ;
	                in.two.push_back( index ) ;
				  } // else if()
					
				  else if( contain( realNumber, strTemp2, index ) ){
					in.two.push_back( 4 ) ;
	                in.two.push_back( index ) ;
				  } // else if()
				  
				  else if( contain( TEMP, strTemp2, index ) ){
					in.two.push_back( 0 ) ;
	                in.two.push_back( index ) ;
				  } // else if()
				  
				  else{
					in.two.push_back( 0 ) ;
	                in.two.push_back( 0 ) ;				  	
				  }
					
				  if( identifier.find( strTemp1, index ) ){
	                in.three.push_back( 5 ) ;
	                in.three.push_back( index ) ;
				  } // if()
					
				  else if( contain( integer, strTemp1, index ) ){
					in.three.push_back( 3 ) ;
	                in.three.push_back( index ) ;
				  } // else if()
					
				  else if( contain( realNumber, strTemp1, index ) ){
					in.three.push_back( 4 ) ;
	                in.three.push_back( index ) ;
				  } // else if()
				  
				  else if( contain( TEMP, strTemp1, index ) ){
					in.three.push_back( 0 ) ;
	                in.three.push_back( index ) ;
				  } // else if()
				  
				  else {
					in.three.push_back( 0 ) ;
	                in.three.push_back( 0 ) ;				  	
				  }
	
	              in.four.push_back( 0 ) ;
	              in.four.push_back( T ) ;
	              intermediate.push_back( in ) ;
	              
	              stack1.push_back( "T" + to_string( T ) ) ;
	              TEMP.insert( "T" + to_string(T) ) ;
	              T++ ;
	            } // else ()
			  } // for()
			} // else 
			
			else {
			  for( int z = stack2.size()-1 ; z >= 0 ; z-- ) {
			  	if( stack2[z] != "=" ){
			  	  string strTemp1 = stack1.back() ;
	      	  	  stack1.pop_back() ;
				  string strTemp2 = stack1.back() ;
				  stack1.pop_back() ;
				  string o = stack2.back() ;
				  stack2.pop_back() ;
					
				  int index ;
	      	  	  Intermediate in ;
	              in.num = intermediate.size() ;
	              in.str = "T" + to_string(T) + " " + "= " + strTemp2 + " " + o + " " + strTemp1 ;
	              contain( delimiter, o, index ) ;
	              in.one.push_back( 1 ) ;
	              in.one.push_back( index ) ;
	                
	              if( identifier.find( strTemp2, index ) ){
	                in.two.push_back( 5 ) ;
	                in.two.push_back( index ) ;
				  } // if()
					
				  else if( contain( integer, strTemp2, index ) ){
					in.two.push_back( 3 ) ;
	                in.two.push_back( index ) ;
				  } // else if()
					
				  else if( contain( realNumber, strTemp2, index ) ){
					in.two.push_back( 4 ) ;
	                in.two.push_back( index ) ;
				  } // else if()
				  
				  else if( contain( TEMP, strTemp2, index ) ){
					in.two.push_back( 0 ) ;
	                in.two.push_back( index ) ;
				  } // else if()
				  
				  else{
					in.two.push_back( 0 ) ;
	                in.two.push_back( 0 ) ;				  	
				  }
					
				  if( identifier.find( strTemp1, index ) ){
	                in.three.push_back( 5 ) ;
	                in.three.push_back( index ) ;
				  } // if()
					
				  else if( contain( integer, strTemp1, index ) ){
					in.three.push_back( 3 ) ;
	                in.three.push_back( index ) ;
				  } // else if()
					
				  else if( contain( realNumber, strTemp1, index ) ){
					in.three.push_back( 4 ) ;
	                in.three.push_back( index ) ;
				  } // else if()
				  
				  else if( contain( TEMP, strTemp1, index ) ){
					in.three.push_back( 0 ) ;
	                in.three.push_back( index ) ;
				  } // else if()
				  
				  else{
				  	in.three.push_back( 0 ) ;
	                in.three.push_back( 0 ) ;
				  }
	
	              in.four.push_back( 0 ) ;
	              in.four.push_back( T ) ;
	              intermediate.push_back( in ) ;
	              
	              stack1.push_back( "T" + to_string( T ) ) ;
	              TEMP.insert( "T" + to_string(T) ) ;
	              T++ ;
				} // if()
				
				else {
				  string strTemp1 = stack1.back() ;
	      	  	  stack1.pop_back() ;
	      	  	  string strTemp2 = stack1.back() ;
	      	  	  stack1.pop_back() ;
				  stack2.pop_back() ;
					
				  int index ;
	      	  	  Intermediate in ;
	              in.num = intermediate.size() ;
	              in.str = strTemp2 + " = " + strTemp1 ;
	              in.one.push_back( 1 ) ;
	              in.one.push_back( 4 ) ;
	                
	                
	              if( identifier.find( strTemp1, index ) ){
	                in.two.push_back( 5 ) ;
	                in.two.push_back( index ) ;
				  } // if()
					
				  else if( contain( integer, strTemp1, index ) ){
					in.two.push_back( 3 ) ;
	                in.two.push_back( index ) ;
				  } // else if()
					
				  else if( contain( realNumber, strTemp1, index ) ){
					in.two.push_back( 4 ) ;
	                in.two.push_back( index ) ;
				  } // else if()
				  
				  else if( contain( TEMP, strTemp1, index ) ){
				  	in.two.push_back( 0 ) ;
	                in.two.push_back( index ) ;
				  } // else if()
				  
				  else {
				  	in.two.push_back( 0 ) ;
	                in.two.push_back( 0 ) ;				  	
				  } 
				  
				  string a = "" ;
				  string str = "" ;
				  for( int i = 0 ; i < strTemp2.size() ; i++ ){
				  	if( strTemp2[i] == '(' ) {
				  	  a = a + strTemp2[i+1] ;
					  break ;
				    } // if()
				    
				    str = str + strTemp2[i] ;
				  } // for()
				  
				  if( identifier.find( str, index ) ){
	                in.three.push_back( 5 ) ;
	                in.three.push_back( index ) ;
				  } // if()
				  
				  else{
	                in.three.push_back( 5 ) ;
	                in.three.push_back( 0 ) ;				  	
				  }
	              
				  if( identifier.find( a, index ) ){
	                in.four.push_back( 5 ) ;
	                in.four.push_back( index ) ;
				  } // if()
					
				  else if( contain( integer, a, index ) ){
					in.four.push_back( 3 ) ;
	                in.four.push_back( index ) ;
				  } // else if()
				  
				  else{
					in.four.push_back( 3 ) ;
	                in.four.push_back( 0 ) ;				  	
				  }
	              
	              intermediate.push_back( in ) ;
				} // else 
			  } // for
			} // else 
		  } // for()  
} // ASSIGNMENTHaveArrayInHeadAndWithoutArray(){

bool ASSIGNMENTHaveArrayInHeadAndWithoutArray( Token temp, int now, int start ){
  int right = 0 ; // )的位置 
  int id = 0 ;
  int common = 0 ;
  int index ;
  

  	for( int i = 2 ; i < temp.token.size() ; i++ ) {
  	  if( temp.token[i] == ")" ) {
  	    right = i ;
  	    break ;
	  } // if()
	  
  	  else if( i % 2 == 0 && !( Identifier( temp.token[i] ) || isInt( temp.token[i] ) )) {
  	    errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
  	  	cout << "第" + to_string(now+1) + "行: " + "ASSIGNMENTHaveArrayInHeadAndWithoutArray ERROR!!!" << endl ;
  	  	return false ;
	  } // if()
	  
	  else if( i % 2 == 1 && temp.token[i] != "," ) {
	    errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
  	  	cout << "第" + to_string(now+1) + "行: " +"ASSIGNMENTHaveArrayInHeadAndWithoutArray ERROR!!!" << endl ;
  	  	return false ;
	  } // if()
	  
	  else if( i % 2 == 0 && ( Identifier( temp.token[i] ) || isInt( temp.token[i] ) ) ) id++ ;
	  
	  else if( i % 2 == 1 && temp.token[i] == "," ) common++ ;
    } // for()
    
    if( id - common != 1 ) {
      errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
      cout << "第" + to_string(now+1) + "行: " +"ASSIGNMENTHaveArrayInHeadAndWithoutArray ERROR!!!" << endl ;
  	  return false ;
	} // if()
  		
	if( Identifier( temp.token[0] ) && temp.token[right+1] == "=" && temp.token[temp.token.size()-1] == ";" ){
	  if( EXP( temp, right+2, temp.token.size()-2 ) ) {
	  	if( !identifier.contain( temp.token[0], index ) || ( identifier.contain( temp.token[0], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){ 
		  index = identifier.hashFunction( temp.token[0] ) ;
	      identifier.table[index].subroutine = nowFunctionIndex ;
	    } // if
	    
	    for( int i = 2 ; i < temp.token.size() ; i++ ) {
  	      if( temp.token[i] == ")" ) {
  	        right = i ;
  	        break ;
	      } // if()
	  
	      else if( i % 2 == 0 && ( Identifier( temp.token[i] ) || isInt( temp.token[i] ) ) ) {
	      	if( Identifier( temp.token[i] ) ){
	      	  if( !identifier.contain( temp.token[i], index ) || ( identifier.contain( temp.token[i], index ) && identifier.table[index].subroutine != nowFunctionIndex  )){ 
		        index = identifier.hashFunction( temp.token[i] ) ;
	            identifier.table[index].subroutine = nowFunctionIndex ;
	          } // else 
			} // if
			
			else{
			  if( contain( integer, temp.token[i], index ) ) integer.insert( temp.token[i] ) ;
			} // else 
		  } // else if()
        } // for() 
        
        EXP( temp, right+2, temp.token.size()-2, now ) ;
		  
	    return true ;
	  } // if
		
	  else {
	  	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
	  	cout << "第" + to_string(now+1) + "行: " +"ASSIGNMENTHaveArrayInHeadAndWithoutArray 文法錯誤!!!" << endl ;
	  	return false ;
	  } // else 
	} // if()
 
  else {
  	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "ASSIGNMENT 文法錯誤!!!" ) ; 
  	cout << "第" + to_string(now+1) + "行: " +"ASSIGNMENTHaveArrayInHeadAndWithoutArray Error!!!" << endl ;
  	return false ;
  } // else 
} // ASSIGNMENTHaveArrayInHeadAndWithoutArray()

bool CALL( Token temp, int now, int start ) {
  int index ;
  
  if( temp.token.size() >= 6 && Identifier( temp.token[start] ) && temp.tokenType[start] != 2 && 
      temp.token[start+1] == "(" && temp.token[temp.token.size()-2] == ")" && temp.token[temp.token.size()-1] == ";" ) {
    int id = 0 ; // id數 
    int common = 0 ; // ,數  
    for( int i = start+2 ; i < temp.token.size()-1 ; i++ ) {
      if( temp.token[i] == "," || temp.token[i] == ")") {
      	if( temp.token[i] == "," ) common++ ;
      	
      	if( ( Identifier( temp.token[i-1] ) && temp.tokenType[i-1] != 2 ) || isInt( temp.token[i-1] ) || isFloat( temp.token[i-1] ) ) id++ ;
      	else {
      	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "CALL Error!!! 參數型別有錯" ) ;
      	  cout << "第" + to_string(now+1) + "行: " + "CALL Error!!! 參數型別有錯" << endl ;
  	      return false ;
		} // else 
	  } // if()
	} // for()
      	
    if( id - common != 1 ) {
      errorMessage.push_back( "第" + to_string(now+1) + "行: " + "CALL Error!!! 參數中間少了逗號隔開" ) ;
      cout << "第" + to_string(now+1) + "行: " + "CALL Error!!! 參數中間少了逗號隔開" << endl ;
      return false ;	
	} // if()

	return true ;
  } // if()
  
  else {
	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "CALL Error!!!" ) ;
    cout << "第" + to_string(now+1) + "行: " + "CALL Error!!!" << endl ;
    return false ;
  } // else 
} // CALL() 

bool INPUT( Token temp, int now, int start ) {
  int index ;
  if( temp.token.size() -2 > 0 && variable( temp, start, temp.token.size() -2 ) && temp.token[temp.token.size()-1] == ";" ) return true ;
  else {
  	if( temp.token.size() -2 > 0 && !variable( temp, start, temp.token.size() -2 ) ) {
  	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "INPUT 最後不是variable(文法錯誤)!!!" ) ;
      cout << "第" + to_string(now+1) + "行: " + "INPUT 最後不是variable(文法錯誤)!!!" << endl ;
      return false ;
    } // if()
    
    else if( temp.token[temp.token.size()-1] != ";" ){
  	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "INPUT 最後字元非結束指令 ';'" ) ;
      cout << "第" + to_string(now+1) + "行: " + "INPUT 最後字元非結束指令 ';'" << endl ;
      return false ;
	} // esle if()
	
	else {
	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "INPUT Error!!!" ) ;
      cout << "第" + to_string(now+1) + "行: " + "INPUT Error!!!" << endl ;
      return false ;
	} // else 
  } // else
} // INPUT 

bool OUTPUT( Token temp, int now, int start ) {
  int index ;
  if( temp.token.size() -2 > 0 && variable( temp, start, temp.token.size() -2 ) && temp.token[temp.token.size()-1] == ";" ) return true ;
  else {
  	if( temp.token.size() -2 > 0 && !variable( temp, start, temp.token.size() -2 ) ) {
  	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "OUTPUT 最後不是variable(文法錯誤)!!!" ) ;
      cout << "第" + to_string(now+1) + "行: " + "OUTPUT 最後不是variable(文法錯誤)!!!" << endl ;
      return false ;
    } // if()
    
    else if( temp.token[temp.token.size()-1] != ";" ){
  	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "OUTPUT 最後字元非結束指令 ';'" ) ;
      cout << "第" + to_string(now+1) + "行: " + "OUTPUT 最後字元非結束指令 ';'" << endl ;
      return false ;
	} // esle if()
	
	else {
	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "OUTPUT Error!!!" ) ;
      cout << "第" + to_string(now+1) + "行: " + "OUTPUT Error!!!" << endl ;
      return false ;
	} // else 
  } // else
} // INPUT 

bool Condition( Token temp, int start, int end ){ 
  int conditionIndex = 0 ;
  for( int i = start ; i <= end ; i++ ){
  	if( temp.token[i] == "AND" ||  temp.token[i] == "OR" || temp.token[i] == "GT" || temp.token[i] == "GE" || 
	    temp.token[i] == "LE" || temp.token[i] == "LT" || temp.token[i] == "EQ" || temp.token[i] == "NOT" ) conditionIndex = i ;
  } // for()
  
  if( conditionIndex != 0 ) {
    if(( variable( temp, start, conditionIndex-1 ) || constant( temp, start ) ) && ( variable( temp, conditionIndex+1, end ) || constant( temp, conditionIndex+1 ))) {
	  return true ;
	} // if()
	
    else {
      return false ;	
	} // else 
  } // if()
              
  else {
    return false ;	
  } // else
} // Condition()

bool Statement( Token temp, int start, int end, int now ){
  if( start == end ) return true ;
  else {
  	Token t ;
  	for( int i = start ; i <= end ; i++ ){
  	  t.token.push_back( temp.token[i] ) ;
  	  t.tokenType.push_back( temp.tokenType[i] ) ;
  	  t.tokenValue.push_back( temp.tokenValue[i] ) ;
	} // for()

  	t.token.push_back( ";" ) ;
  	t.tokenType.push_back( 1 ) ;
  	t.tokenValue.push_back( 1 ) ;
  	
  	if( t.token.size() == 1 ) return false ;
  	
  	if( t.token[0] == "GTO" ){
      if( GTO( t, now, 1 ) ) return true ;
      else return false ;
    } // else if()
    
    else if( t.token[0] == "CALL" ){
      if( CALL( t, now, 1 ) ) return true ;
      else return false ;
    } // else if()
    
    else if( t.token[0] == "INPUT" ){
      if( INPUT( t, now, 1 ) ) return true ;
      else return false ;
    } // else if()
    
    else if( t.token[0] == "OUTPUT" ){
      if( OUTPUT( t, now, 1 ) ) return true ;
      else return false ;
    } // else if()
     
	else if( ( Identifier( t.token[0] ) && t.tokenType[0] == 0 && t.tokenValue[0] == 0 ) && ( t.token[1] == "=" || t.token[1] == "(" ) ){ // ASSIGNMENT !!!  
	  bool expressionHaveArray = false ;
	  int same = 0 ;
	  for( int j = 0 ; j < t.token.size() ; j++ ){
		if( t.token[j] == "=" ) {
		  same = j ;
		  break ;
		} // if()
	  } // for()
			
	  if( same != 0 ) {
		for( int j = same +1 ; j < t.token.size() ; j++ ) { 
		  if( t.token[j] == "(" && Identifier( t.token[j-1] ) ){
		  	expressionHaveArray = true ;
		  	break ;
		  } // if()
		} // for()
		  	
        if( Identifier( t.token[0] ) && t.token[1] == "=" && expressionHaveArray == true && ASSIGNMENTHaveArray( t, now, 2 ) ) return true ;
        else if( Identifier( t.token[0] ) && t.token[1] == "=" && expressionHaveArray == false && ASSIGNMENT( t, now, 2 ) ) return true ; 
        else if( Identifier( t.token[0] ) && t.token[1] == "(" && expressionHaveArray == true && ASSIGNMENTHaveArrayInHeadAndHaveArray( t, now, 2 ) ) return true ;
        else if( Identifier( t.token[0] ) && t.token[1] == "(" && expressionHaveArray == false && ASSIGNMENTHaveArrayInHeadAndHaveArray( t, now, 2 ) ) return true ;
	    else {
	      return false ;
	      //cout << "ASSIGNMENT 文法錯誤!!!!!" << endl ; 
		} // else 
	  } // if()
		    
	  else {
	    return false ;
	    //cout << "ASSIGNMENT 文法錯誤!!!!!" << endl ; 
	  } // else 
	} // else if()
	
	else {
	  return false ;
	  //cout << "ASSIGNMENT 文法錯誤!!!!!" << endl ; 
	} // else 
  } // else 
} // Statement()

bool IF( Token temp, int now, int start ) {
  if( temp.token[temp.token.size()-1] == ";" ){
  	int thenIndex = 0 ;
  	for( int i = start ; i < temp.token.size() ; i++ ) {
  	  if( temp.token[i] == "THEN" ) {
  	  	thenIndex = i ;
  	  	
  	  	if( !Condition( temp, start, i-1 ) ){
  	  	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "Condition 文法錯誤!!!" ) ; 
          cout << "第" + to_string(now+1) + "行: " + "Condition 文法錯誤!!!" << endl ;
  	      return false ;
		} // if() 
		
  	  	break ;
	  } // if()
	} // for()
	
	int elseIndex = 0 ;
	
	if( thenIndex != 0 ){
	  for( int i = thenIndex+1 ; i < temp.token.size() ; i++ ){
	  	if( temp.token[i] == "ELSE" ) elseIndex = i ;
	  	
	  	if( temp.token[i] == ";" ){
	  	  if( elseIndex != 0 ) { // 有else 
	  	  	if( !Statement( temp, thenIndex+1, elseIndex-1, now )  ) {
	  	  	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "Statement1 文法錯誤!!!" ) ; 
	  	  	  cout << "Statement1 文法錯誤!!!" << endl ;
  	          return false ;
			} // if()
			
			if( !Statement( temp, elseIndex+1, i-1, now ) ) {
	  	  	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "Statement2 文法錯誤!!!" ) ; 
	  	  	  cout << "Statement2 文法錯誤!!!" << endl ;
  	          return false ;
			} // if()
		  } // if()
		  
	  	  else { // 沒有esle 
	  	  	if( !Statement( temp, thenIndex+1, i-1, now ) ) {
	  	  	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "Statement1 文法錯誤!!!" ) ; 
	  	  	  cout << "Statement1 文法錯誤!!!" << endl ;
  	          return false ;
			} // if()
		  } // else  
		  
		  break ;
		} // if()
	  } // for()
	  
	  return true ;
	} // if()
	
	else {
	  errorMessage.push_back( "第" + to_string(now+1) + "行: " + "Statement 文法錯誤!!!" ) ; 
	  cout << "Statement 文法錯誤!!!" << endl ;
  	  return false ;
	} // else
  } // if()
  
  else {
	errorMessage.push_back( "第" + to_string(now+1) + "行: " + "IF 文法錯誤!!!" ) ; 
	cout << "IF 文法錯誤!!!" << endl ;
  	return false ;
  } // else 
} // IF() 

void addLabelIndex( int now ) {
  int index ;
  if( identifier.contain( tokenArray[now].token[0], index ) && identifier.table[index].subroutine == nowFunctionIndex ){
    identifier.table[index].type = 5 ;
    identifier.table[index].pointer[0] = 6 ;
    identifier.table[index].pointer[1] = intermediate.size() ;
  } // if()
			
  else if( !identifier.contain( tokenArray[now].token[0], index ) || 
         ( identifier.contain( tokenArray[now].token[0], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){
    index = identifier.hashFunction( tokenArray[now].token[0] ) ;
    identifier.table[index].type = 5 ;
    identifier.table[index].pointer[0] = 6 ;
    identifier.table[index].pointer[1] = intermediate.size() ;
    identifier.table[index].subroutine = nowFunctionIndex ;
  } // else if()
} // addLabelIndex()

void addVARIABLE( Token temp, int now ){
  int type = 0 ;
  if( temp.token[1] == "BOOLEAN" ) type = 2 ;
  else if( temp.token[1] == "INTEGER" ) type = 4 ; 
  else if( temp.token[1] == "REAL" ) type = 6 ;
	
  int index ;
  for( int i = 3 ; i < temp.token.size()-1 ; i = i + 2 ) {
	if( identifier.contain( temp.token[i], index ) && identifier.table[index].subroutine == nowFunctionIndex ) {
        Intermediate in ;
        in.num = intermediate.size() ;
        in.str = temp.token[i] ;
        in.one.push_back( 5 ) ;
        in.one.push_back( index ) ;
        in.two.push_back( 0 ) ;
        in.two.push_back( 0 ) ;
        in.three.push_back( 0 ) ;
        in.three.push_back( 0 ) ;
        in.four.push_back( 0 ) ;
        in.four.push_back( 0 ) ;
        intermediate.push_back( in ) ;
    } // if()
	  
	else if( !identifier.contain( temp.token[i], index ) || 
	       ( identifier.contain( temp.token[i], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ) {
	    index = identifier.hashFunction( temp.token[i] ) ;
	    identifier.table[index].subroutine = nowFunctionIndex ;
        identifier.table[index].type = type ;
        Intermediate in ;
        in.num = intermediate.size() ;
        in.str = temp.token[i] ;
        in.one.push_back( 5 ) ;
        in.one.push_back( index ) ;
        in.two.push_back( 0 ) ;
        in.two.push_back( 0 ) ;
        in.three.push_back( 0 ) ;
        in.three.push_back( 0 ) ;
        in.four.push_back( 0 ) ;
        in.four.push_back( 0 ) ;
        intermediate.push_back( in ) ;
    } // else if
  } // for()
} // void()

void addLABLE( Token temp, int now ){
    int index  ;
	for( int i = 1 ; i < temp.token.size()-1 ; i = i + 2 ) {
	  if( identifier.contain( temp.token[i], index ) && identifier.table[index].subroutine == nowFunctionIndex ) {
        Intermediate in ;
        in.num = intermediate.size() ;
        in.str = temp.token[i] ;
        in.one.push_back( 5 ) ;
        in.one.push_back( index ) ;
        in.two.push_back( 0 ) ;
        in.two.push_back( 0 ) ;
        in.three.push_back( 0 ) ;
        in.three.push_back( 0 ) ;
        in.four.push_back( 0 ) ;
        in.four.push_back( 0 ) ;
        intermediate.push_back( in ) ;
      } // if()
      
      else if( !identifier.contain( temp.token[i], index ) || 
	         ( identifier.contain( temp.token[i], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ) {
      	index = identifier.hashFunction( temp.token[i] ) ;	  
	    identifier.table[index].subroutine = nowFunctionIndex ;
        identifier.table[index].type = 5 ; // LABLE => type = 5 
        identifier.table[index].pointer[0] = 6 ;
        identifier.table[index].pointer[1] = 9999 ; // => forword reference
        Intermediate in ;
        in.num = intermediate.size() ;
        in.str = temp.token[i] ;
        in.one.push_back( 5 ) ;
        in.one.push_back( index ) ;
        in.two.push_back( 0 ) ;
        in.two.push_back( 0 ) ;
        in.three.push_back( 0 ) ;
        in.three.push_back( 0 ) ;
        in.four.push_back( 0 ) ;
        in.four.push_back( 0 ) ;
        intermediate.push_back( in ) ;
	  } // else if()
	} // for()
} // addLABLE()

void addIO( Token temp, int now, int start ){
  int index ;
  if( identifier.contain( temp.token[start], index ) && identifier.table[index].subroutine == nowFunctionIndex ) {
    Intermediate in ;
    in.num = intermediate.size() ;
    in.str = temp.token[start] ;
    in.one.push_back( 5 ) ;
    in.one.push_back( index ) ;
    in.two.push_back( 0 ) ;
    in.two.push_back( 0 ) ;
    in.three.push_back( 0 ) ;
    in.three.push_back( 0 ) ;
    in.four.push_back( 0 ) ;
    in.four.push_back( 0 ) ;
    intermediate.push_back( in ) ;
  } // if()
	  
  else if( ( !identifier.contain( temp.token[start], index ) ) || 
           ( identifier.contain( temp.token[start], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ){
  	index = identifier.hashFunction( temp.token[start] ) ;
  	identifier.table[index].subroutine = nowFunctionIndex ;
    Intermediate in ;
    in.num = intermediate.size() ;
    in.str = temp.token[start] ;
    in.one.push_back( 5 ) ;
    in.one.push_back( index ) ;
    in.two.push_back( 0 ) ;
    in.two.push_back( 0 ) ;
    in.three.push_back( 0 ) ;
    in.three.push_back( 0 ) ;
    in.four.push_back( 0 ) ;
    in.four.push_back( 0 ) ;
    intermediate.push_back( in ) ;
  } // else if()
} // addIO

void addCALL( Token temp, int now, int start ) {
  int index ;
  
  Intermediate in ;
  in.num = intermediate.size() ;
  in.str = tokenArray[now].token[start-1] + " " ;
  for( int i = start ; i < tokenArray[now].token.size()-1 ; i++ ) in.str = in.str + tokenArray[now].token[i] ;
  in.one.push_back( 2 ) ;
  in.one.push_back( 3 ) ;
  in.two.push_back( 5 ) ;
  if( identifier.contain( temp.token[start], index )  ){
  	in.two.push_back( index ) ;
  } // if()
  
  else {
  	index = identifier.hashFunction( temp.token[start] ) ;
  	identifier.table[index].subroutine = nowFunctionIndex ;
  	in.two.push_back( index ) ;
  } // else
  
  in.three.push_back( 0 ) ;
  in.three.push_back( 0 ) ;
  in.four.push_back( 7 ) ;
  in.four.push_back( information.size() +1 ) ;
  intermediate.push_back( in ) ;
  
  vector <int> tempp ;
  for( int i = start+2 ; i < temp.token.size()-1 ; i ++ ) {
    if( temp.token[i] == "," || temp.token[i] == ")") {
      if( Identifier( temp.token[i-1] )) {
	    if( !identifier.contain( temp.token[i-1], index ) || 
	        ( identifier.contain( temp.token[i-1], index ) && identifier.table[index].subroutine != nowFunctionIndex ) ) {
  	      index = identifier.hashFunction( temp.token[i-1] ) ;
  	      identifier.table[index].subroutine = nowFunctionIndex ;
  	      tempp.push_back( 5 ) ;
  	      tempp.push_back( index ) ;
  	    } // if()
  	    
  	    else if( identifier.contain( temp.token[i-1], index ) && identifier.table[index].subroutine == nowFunctionIndex ){
  	      tempp.push_back( 5 ) ;
  	      tempp.push_back( index ) ;
		} // else if()
	  } // if()
	  
	  else if( isInt( temp.token[i-1] ) ) {
	  	if( contain( integer, temp.token[i-1], index ) ){
	  	  tempp.push_back( 3 ) ;
  	      tempp.push_back( index ) ;
		} // if()
		
		else {
		  integer.insert( temp.token[i-1] ) ;
		  tempp.push_back( 3 ) ;
  	      tempp.push_back( integer.table.size() ) ;
		} // else
	  } // else 
	  
	  else { // float
	  	if( contain( realNumber, temp.token[i-1], index ) ){
	  	  tempp.push_back( 4 ) ;
  	      tempp.push_back( index ) ;
		} // if()
		
		else {
		  realNumber.insert( temp.token[i-1] ) ;
		  tempp.push_back( 4 ) ;
  	      tempp.push_back( realNumber.table.size() ) ;
		} // else
	  } // else 
	} // if()
  } // for()
  
  information.push_back( tempp.size() ) ;
  for( int i = 0 ; i < tempp.size() ; i++ ) information.push_back( tempp[i] ) ;
} // addCALL

void addGTO( Token temp, int now, int start ){
	int index ;
	if( identifier.contain( temp.token[start], index ) && identifier.table[index].subroutine == nowFunctionIndex ) {
	  Intermediate in ;
      in.num = intermediate.size() ;
      in.str = temp.token[start-1] + " " + temp.token[start] ;
      in.one.push_back( 2 ) ;
      in.one.push_back( 11 ) ;
      in.two.push_back( 0 ) ;
      in.two.push_back( 0 ) ;
      in.three.push_back( 0 ) ;
      in.three.push_back( 0 ) ;
      in.four.push_back( identifier.table[index].pointer[0] ) ;
      in.four.push_back( identifier.table[index].pointer[1] ) ;
      intermediate.push_back( in ) ;	
	} // if()
	
	else {
	  index = identifier.hashFunction( temp.token[start] ) ;
	  identifier.table[index].subroutine = nowFunctionIndex ;
	  identifier.table[index].type = 5 ;
	  identifier.table[index].pointer[0] = 6 ;
	  identifier.table[index].pointer[1] = 9999 ;
	  Intermediate in ;
      in.num = intermediate.size() ;
      in.str = temp.token[start-1] + " " + temp.token[start] ;
      in.one.push_back( 2 ) ;
      in.one.push_back( 11 ) ;
      in.two.push_back( 0 ) ;
      in.two.push_back( 0 ) ;
      in.three.push_back( 0 ) ;
      in.three.push_back( 0 ) ;
      in.four.push_back( identifier.table[index].pointer[0] ) ;
      in.four.push_back( identifier.table[index].pointer[1] ) ;
      intermediate.push_back( in ) ;
	} // else if()
} // addGTO

void addIF( Token temp, int now, int start ){
  int if_else ; 
  int addGTOIndexinTable6 ;
  int thenIndex = 0 ;
  int conditionIndex = 0 ;
  int index = 0  ;
  int elseIndex = 0 ;
  string str = "" ;
  for( int i = start ; i < temp.token.size() ; i++ ) {
  	if( temp.token[i] == "THEN" ) {
  	  thenIndex = i ;
  	  break ;
	} // if()
	
	if( temp.token[i] == "AND" ||  temp.token[i] == "OR" || temp.token[i] == "GT" || temp.token[i] == "GE" || 
	    temp.token[i] == "LE" || temp.token[i] == "LT" || temp.token[i] == "EQ" || temp.token[i] == "NOT" ) { 
		conditionIndex = i ;
	} // if()
	
	str = str + temp.token[i] + " ";
  } // for()
  
  Intermediate in ;
  in.num = intermediate.size() ;
  if( contain( reservedWord, temp.token[conditionIndex], index ) ){
    in.one.push_back( 2 ) ;
    in.one.push_back( index ) ;
  } // if()
  
  in.str = "T" + to_string(T) + " = " + str ;
  
  if( conditionIndex - start > 1 ) {	
	if( identifier.contain( temp.token[start], index ) && identifier.table[index].subroutine == nowFunctionIndex ){
  	  in.three.push_back( 5 ) ;
      in.three.push_back( index ) ;
    } // if() 
      
    else {
      index = identifier.hashFunction( temp.token[start] ) ;
      in.three.push_back( 5 ) ;
      in.three.push_back( index ) ;
	} // else 
  } // if()
  
  else if( conditionIndex - start == 1 ) {	
	if( isInt( temp.token[start] ) ){
	  if( contain( integer, temp.token[start], index ) ){
  	    in.two.push_back( 3 ) ;
        in.two.push_back( index ) ;
      } // if() 
      
      else {
      	integer.insert( temp.token[start] ) ;
      	in.two.push_back( 3 ) ;
        in.two.push_back( integer.table.size() -1 ) ;
	  } // else 
	} // if()
	
	else if( isFloat( temp.token[start] )){
	  if( contain( realNumber, temp.token[start] ,index ) ){
  	    in.two.push_back( 4 ) ;
        in.two.push_back( index ) ;
      } // if() 
      
      else {
      	realNumber.insert( temp.token[start] ) ;
      	in.two.push_back( 4 ) ;
        in.two.push_back( realNumber.table.size() -1 ) ;
	  } // else 
	} // else if
	
  	else if( Identifier( temp.token[start] )){
  	  if( identifier.contain( temp.token[start], index ) && identifier.table[index].subroutine == nowFunctionIndex ){
  	    in.two.push_back( 5 ) ;
        in.two.push_back( index ) ;
      } // if() 
      
      else {
      	index = identifier.hashFunction( temp.token[start] ) ;
      	in.two.push_back( 5 ) ;
        in.two.push_back( index ) ;
	  } // else 
	} // if()
  } // else 
  
  if( thenIndex - conditionIndex > 2 ) {
 	if( identifier.contain( temp.token[conditionIndex+1], index ) && identifier.table[index].subroutine == nowFunctionIndex ){
  	  in.three.push_back( 5 ) ;
      in.three.push_back( index ) ;
    } // if() 
      
    else {
      index = identifier.hashFunction( temp.token[conditionIndex+1] ) ;
      in.three.push_back( 5 ) ;
      in.three.push_back( index ) ;
	} // else 
  } // if()
  
  else if( thenIndex - conditionIndex == 2 ) {
  	if( isInt( temp.token[conditionIndex+1] ) ){
	  if( contain( integer, temp.token[conditionIndex+1], index ) ){
  	    in.three.push_back( 3 ) ;
        in.three.push_back( index ) ;
      } // if() 
      
      else {
      	integer.insert( temp.token[conditionIndex+1] ) ;
      	in.three.push_back( 3 ) ;
        in.three.push_back( integer.table.size() -1 ) ;
	  } // else 
	} // if()
	
	else if( isFloat( temp.token[conditionIndex+1] )){
	  if( contain( realNumber, temp.token[conditionIndex+1], index ) ){
  	    in.three.push_back( 4 ) ;
        in.three.push_back( index ) ;
      } // if() 
      
      else {
      	realNumber.insert( temp.token[conditionIndex+1] ) ;
      	in.three.push_back( 4 ) ;
        in.three.push_back( realNumber.table.size() -1 ) ;
	  } // else 
	} // else if
	
  	else if( Identifier( temp.token[conditionIndex+1]) ){
  	  if( identifier.contain( temp.token[conditionIndex+1], index ) && identifier.table[index].subroutine == nowFunctionIndex ){
  	    in.three.push_back( 5 ) ;
        in.three.push_back( index ) ;
      } // if() 
      
      else {
      	index = identifier.hashFunction( temp.token[conditionIndex+1] ) ;
      	in.three.push_back( 5 ) ;
        in.three.push_back( index ) ;
	  } // else 
	} // if()
  } // else 
  
  in.four.push_back(0) ;
  in.four.push_back(T) ;
  intermediate.push_back( in ) ;
  
  TEMP.insert( "T" + to_string(T) ) ;
  ///////////////////////////////////
  Intermediate in1 ;
  in1.num = intermediate.size() ;
  in1.str = "IF T" + to_string(T) + " GO TO " + to_string(intermediate.size()+1) + ", ELSE GO TO " ;
  in1.one.push_back( 2 ) ;
  in1.one.push_back( 12 ) ;
  in1.two.push_back( 0 ) ;
  in1.two.push_back( T ) ;
  in1.three.push_back( 6 ) ;
  in1.three.push_back( intermediate.size()+1 ) ;
  in1.four.push_back( 6 ) ;
  in1.four.push_back( 9999 ) ;
  intermediate.push_back( in1 ) ;
  if_else = intermediate.size() -1 ;
  T++ ;
  ///////////////////////////////////////
	
  if( thenIndex != 0 ){
	for( int i = thenIndex+1 ; i < temp.token.size() ; i++ ){
	  if( temp.token[i] == "ELSE" ) elseIndex = i ;
	  if( temp.token[i] == ";" ){
	  	if( elseIndex != 0 ) { // 有else 
	  	  Token t ;
  	      for( int i = thenIndex+1 ; i < elseIndex ; i++ ){
  	        t.token.push_back( temp.token[i] ) ;
  	        t.tokenType.push_back( temp.tokenType[i] ) ;
  	        t.tokenValue.push_back( temp.tokenValue[i] ) ;
	      } // for()

  	      t.token.push_back( ";" ) ;
  	      t.tokenType.push_back( 1 ) ;
  	      t.tokenValue.push_back( 1 ) ;
  	      
  	      if( t.token[0] == "GTO" ) addGTO( t, now, 1 ) ;
    
          else if( t.token[0] == "CALL" ) addCALL( t, now, 1 ) ;
    
          else if( t.token[0] == "INPUT" ) addIO( t, now, 1 ) ;
    
          else if( t.token[0] == "OUTPUT" ) addIO( t, now, 1 ) ;
          
          else if( t.token.size() > 2 && Identifier( t.token[0] ) && ( t.token[1] == "=" || t.token[1] == "(" ) ){ // ASSIGNMENT !!!  
			bool expressionHaveArray = false ;
	        int same = 0 ;
	        for( int j = 0 ; j < t.token.size() ; j++ ){
		      if( t.token[j] == "=" ) {
		        same = j ;
		        break ;
		      } // if()
	        } // for()
			
	        if( same != 0 ) {
		      for( int j = same +1 ; j < t.token.size() ; j++ ) { 
		        if( t.token[j] == "(" && Identifier( t.token[j-1] ) ){
		  	      expressionHaveArray = true ;
		  	      break ;
		        } // if()
		      } // for()
		  	
              if( Identifier( t.token[0] ) && t.token[1] == "=" && expressionHaveArray == true && ASSIGNMENTHaveArray( t, now, 2 ) ) addASSIGNMENTHaveArray( t, now, 2 ) ;
              else if( Identifier( t.token[0] ) && t.token[1] == "=" && expressionHaveArray == false && ASSIGNMENT( t, now, 2 ) ) addASSIGNMENT( t, now, 2 ) ; 
              else if( Identifier( t.token[0] ) && t.token[1] == "(" && expressionHaveArray == true && ASSIGNMENTHaveArrayInHeadAndHaveArray( t, now, 2 ) ) addASSIGNMENTHaveArrayInHeadAndHaveArray( t, now, 2 ) ;
              else if( Identifier( t.token[0] ) && t.token[1] == "(" && expressionHaveArray == false && ASSIGNMENTHaveArrayInHeadAndHaveArray( t, now, 2 ) ) addASSIGNMENTHaveArrayInHeadAndHaveArray( t, now, 2 ) ;
	        } // if()
          } // else if()
          
          /////////////////////////////////////////////////////////////
          if( t.token[0] != "GTO" ) {
            Intermediate in ;
            in.num = intermediate.size() ;
            in.str = "GO TO " ;
            in.one.push_back( 2 ) ;
            in.one.push_back( 11 ) ;
            in.two.push_back( 0 ) ;
            in.two.push_back( 0 ) ;
            in.three.push_back( 0 ) ;
            in.three.push_back( 0 ) ;
            in.four.push_back( 6 ) ; 
            in.four.push_back( 9999 ) ;
            intermediate.push_back( in ) ;
            addGTOIndexinTable6 = intermediate.size() -1 ;
          } // if()
          
		  ///////////////////////////////////////////////////////////
		  intermediate[if_else].str = intermediate[if_else].str + to_string( intermediate.size() ) ;
		  intermediate[if_else].four[1] = intermediate.size() ;
		  ////////////////////////////////////////////
          
          Token t1 ;
  	      for( int i = elseIndex+1 ; i < temp.token.size() ; i++ ){
  	        t1.token.push_back( temp.token[i] ) ;
  	        t1.tokenType.push_back( temp.tokenType[i] ) ;
  	        t1.tokenValue.push_back( temp.tokenValue[i] ) ;
	      } // for() 
  	
  	      if( t1.token[0] == "GTO" ) addGTO( t1, now, 1 ) ;
    
          else if( t1.token[0] == "CALL" ) addCALL( t1, now, 1 ) ;
    
          else if( t1.token[0] == "INPUT" ) addIO( t1, now, 1 ) ;
    
          else if( t1.token[0] == "OUTPUT" ) addIO( t1, now, 1 ) ;
                   
          else if( ( Identifier( t1.token[0] ) && t1.tokenType[0] == 0 && t1.tokenValue[0] == 0 ) && ( t1.token[1] == "=" || t1.token[1] == "(" ) ){ // ASSIGNMENT !!!  
			bool expressionHaveArray = false ;
	        int same = 0 ;
	        for( int j = 0 ; j < t1.token.size() ; j++ ){
		      if( t1.token[j] == "=" ) {
		        same = j ;
		        break ;
		      } // if()
	        } // for()
			
	        if( same != 0 ) {
		      for( int j = same +1 ; j < t1.token.size() ; j++ ) { 
		        if( t1.token[j] == "(" && Identifier( t1.token[j-1] ) ){
		  	      expressionHaveArray = true ;
		  	      break ;
		        } // if()
		      } // for()
		  	
              if( Identifier( t1.token[0] ) && t1.token[1] == "=" && expressionHaveArray == true && ASSIGNMENTHaveArray( t1, now, 2 ) ) addASSIGNMENTHaveArray( t1, now, 2 ) ;
              else if( Identifier( t1.token[0] ) && t1.token[1] == "=" && expressionHaveArray == false && ASSIGNMENT( t1, now, 2 ) ) addASSIGNMENT( t1, now, 2 ) ; 
              else if( Identifier( t1.token[0] ) && t1.token[1] == "(" && expressionHaveArray == true && ASSIGNMENTHaveArrayInHeadAndHaveArray( t1, now, 2 ) ) addASSIGNMENTHaveArrayInHeadAndHaveArray( t1, now, 2 ) ;
              else if( Identifier( t1.token[0] ) && t1.token[1] == "(" && expressionHaveArray == false && ASSIGNMENTHaveArrayInHeadAndHaveArray( t1, now, 2 ) ) addASSIGNMENTHaveArrayInHeadAndHaveArray( t1, now, 2 ) ;
	        } // if()
          } // else if()
	    
		  ///////////////////////////////////////////////////
		  if( t.token[0] != "GTO" ){
		    intermediate[addGTOIndexinTable6].str = intermediate[addGTOIndexinTable6].str + to_string(intermediate.size()) ;
		    intermediate[addGTOIndexinTable6].four[1] = intermediate.size() ;
	      } // if()
		  /////////////////////////////////////////////////
		} // if()
		  
	  	else { // 沒有esle 
	  	  Token t1 ;
  	      for( int i = thenIndex+1 ; i < temp.token.size() ; i++ ){
  	        t1.token.push_back( temp.token[i] ) ;
  	        t1.tokenType.push_back( temp.tokenType[i] ) ;
  	        t1.tokenValue.push_back( temp.tokenValue[i] ) ;
	      } // for()
  	      
  	      if( t1.token[0] == "GTO" ) addGTO( t1, now, 1 ) ;
    
          else if( t1.token[0] == "CALL" ) addCALL( t1, now, 1 ) ;
    
          else if( t1.token[0] == "INPUT" ) addIO( t1, now, 1 ) ;
    
          else if( t1.token[0] == "OUTPUT" ) addIO( t1, now, 1 ) ;
          
          else if( t1.token.size() >= 3 && Identifier( t1.token[0] ) && ( t1.token[1] == "=" || t1.token[1] == "(" ) ){ // ASSIGNMENT !!!  
	        bool expressionHaveArray = false ;
	        int same = 0 ;
	        for( int j = 0 ; j < t1.token.size() ; j++ ){
		      if( t1.token[j] == "=" ) {
		        same = j ;
		        break ;
		      } // if()
	        } // for()
			
	        if( same != 0 ) {
		      for( int j = same +1 ; j < t1.token.size() ; j++ ) { 
		        if( t1.token[j] == "(" && Identifier( t1.token[j-1] ) ){
		  	      expressionHaveArray = true ;
		  	      break ;
		        } // if()
		      } // for()
		  	
              if( Identifier( t1.token[0] ) && t1.token[1] == "=" && expressionHaveArray == true && ASSIGNMENTHaveArray( t1, now, 2 ) ) addASSIGNMENTHaveArray( t1, now, 2 ) ;
              else if( Identifier( t1.token[0] ) && t1.token[1] == "=" && expressionHaveArray == false && ASSIGNMENT( t1, now, 2 ) ) addASSIGNMENT( t1, now, 2 ) ; 
              else if( Identifier( t1.token[0] ) && t1.token[1] == "(" && expressionHaveArray == true && ASSIGNMENTHaveArrayInHeadAndHaveArray( t1, now, 2 ) ) addASSIGNMENTHaveArrayInHeadAndHaveArray( t1, now, 2 ) ;
              else if( Identifier( t1.token[0] ) && t1.token[1] == "(" && expressionHaveArray == false && ASSIGNMENTHaveArrayInHeadAndHaveArray( t1, now, 2 ) ) addASSIGNMENTHaveArrayInHeadAndHaveArray( t1, now, 2 ) ;
	        } // if()
          } // else if()
          
          ///////////////////////////////////////////////////////////
		  intermediate[if_else].str = intermediate[if_else].str + to_string( intermediate.size() ) ;
		  intermediate[if_else].four[1] = intermediate.size() ;
		  ////////////////////////////////////////////
		} // else  
		
		break ;
	  } // if()
	} // for()
  } // if()
} // addIF

void write( string temppp ){
	ofstream out; // 建立輸出串流物件
    out.open( temppp + "_output.txt" ); // 開啟即將被寫入資料的檔案
    if(out.fail()){ //確認是否開啟成功
        cout << "input file opening failed...";
        exit(1);
    } // if()
	
  	for( int i = 1 ; i < intermediate.size() ; i++ ) {
	  out << intermediate[i].num << "\t(" ;
	  if( intermediate[i].one[0] == 0 && intermediate[i].one[1] == 0 ) out << "        ," ;
	  else out << "(" << intermediate[i].one[0] << "," << intermediate[i].one[1] << ")  ," ; 
		
	  if( intermediate[i].two[0] == 0 && intermediate[i].two[1] == 0 ) out << "        ," ;
	  else out << "(" << intermediate[i].two[0] << "," << intermediate[i].two[1] << ")  ," ;
		 
  	  if( intermediate[i].three[0] == 0 && intermediate[i].three[1] == 0 ) out << "        ," ;
	  else out << "(" << intermediate[i].three[0] << "," << intermediate[i].three[1] << ")  ," ; 
		
	  if( intermediate[i].four[0] == 0 && intermediate[i].four[1] == 0 ) out << "        )" ;
	  else out << "(" << intermediate[i].four[0] << "," << intermediate[i].four[1] << ")  )" ; 
		
	  out << "\t" << intermediate[i].str << endl ;
	} // for
	
	for( int i = 0 ; i < errorMessage.size() ; i++ ){
	  out << errorMessage[i] << endl ;
	} // for()
} // write()

int main(){
  ReadTableData( delimiter, "Table1.table" ) ; 
  delimiter.table[9] = "^" ;
  delimiter.table[10] = "" ;
  delimiter.table[11] = "," ;  
  ReadTableData( reservedWord, "Table2.table" ) ; 
  
  while ( true ) {
    printf("\n***************\n");
    printf("* 0. Quit     *\n");
    printf("* 1. Compiler *\n");  
    printf("***************\n");
    printf("Input a choice(0, 1): " ) ;
    
	int choice ;
    cin >> choice ;
    
    if( choice == 0 ) break ;
    
    else { // Compiler start
      vector <string> inputArray ;
      ifstream inputFile ;
      string fileName, tempLine = "" ;
      string t = "" ;
      cout << "請輸入檔名: " ;
      cin >> fileName ;
      
	  inputFile.open( fileName + ".txt") ;
	  
      while( getline( inputFile, tempLine ) ) {
      	if( tempLine[tempLine.size()-1] != ';' ) t = t + tempLine ;
	  }
	  
	  t = t + tempLine ;
	  inputArray.push_back(t) ;
      inputFile.close();
      
      cout << t << endl ;
      
      for( int i = 0 ; i < inputArray.size() ; i++ ) { // PASS 1 
   	    string line = inputArray[i] ;
  	    Token t ;
  	    int start = 0, end = 0 ; // token的頭跟尾 
  	    int num = 0 ; // 回傳在table中的位置

		for( int j = 0 ; j <= line.size() ; j++ ) {
  	      string temp = "" ;
  	      temp.append(1, line[j]);
  	  
  	      if( /*end == line.size()-1 || */line[j] == ' ' || line[j] == '\t' || contain( delimiter, temp, num ) ) {
   	  	    string tempToken = "" ;
		    for( int y = start ; line[start] != ' ' && line[start] != '\t' && y <= end ; y++ ) tempToken = tempToken + line[y] ;
			if( line[start] != ' ' && line[start] != '\t' && !contain( delimiter, tempToken, num ) ) {
		      t.token.push_back(tempToken) ;
		      
		      if( contain( delimiter, tempToken, num ) ) { 
	   	 	    t.tokenType.push_back(1) ;
	   	 	    t.tokenValue.push_back(num) ;
		  	  } // if()
			
			  else if( contain( reservedWord, tempToken, num ) ){
			    t.tokenType.push_back(2) ;
	   	 	    t.tokenValue.push_back(num) ;
			  } // else if()
			  
			  else {
			  	t.tokenType.push_back(0) ;
			  	t.tokenValue.push_back(0) ;
			  } // else 
		    } // if() 
		    
		    if(  temp != " " && temp != "\t" && contain( delimiter, temp, num ) ) {
	   	 	  t.token.push_back(temp) ; 
	   	 	  t.tokenType.push_back(1) ;
		      t.tokenValue.push_back(num) ; 
	  		} // if
	  		
	  		if( j + 1 <= line.size()) {
		      start = j + 1 ;
		      end = start ;
		    } // if
          } // if()
          
          else end = j ;
        } // for()
        
        tokenArray.push_back(t) ;	
      } // for()
    

      Intermediate in ;
      in.num = intermediate.size() ;
      in.str = "" ;
      in.one.push_back( 0 ) ;
      in.one.push_back( 0 ) ;
      in.two.push_back( 0 ) ;
      in.two.push_back( 0 ) ;
      in.three.push_back( 0 ) ;
      in.three.push_back( 0 ) ;
      in.four.push_back( 0 ) ;
      in.four.push_back( 0 ) ;
      intermediate.push_back( in ) ;
      for( int i = 0 ; i < tokenArray.size() ; i++ ) { // 檢查文法 
        int num ;

        if( tokenArray[i].token[0] == "PROGRAM" && i == 0 ){  
          if( PROGRAM( tokenArray[i], i ) ){
			num = identifier.hashFunction( tokenArray[i].token[1] ) ;
			identifier.table[num].pointer[0] = 6 ;
            identifier.table[num].pointer[1] = 1 ;
            nowFunctionIndex = num ;
		  } // if()
	    } // if()
	    
	    else if( tokenArray[i].token[0] == "PROGRAM" && i != 0 ){
	      cout <<  "第" + to_string(i+1) + "行:" + "文法錯誤: PROGRAM 要在程式最開始的地方!!!!!" << endl ; 
	      errorMessage.push_back( "第" + to_string(i+1) + "行:" + "文法錯誤: PROGRAM 要在程式最開始的地方!!!!!" ) ; 
		} // else if()
		  
		else if( tokenArray[i].token[0] == "SUBROUTINE" && tokenArray[i].token.size() > 2 ){
		  if( identifier.contain( tokenArray[i].token[1], num ) ) ;
		  else num = identifier.hashFunction( tokenArray[i].token[1] ) ;
		  identifier.table[num].pointer[0] = 6 ;
          identifier.table[num].pointer[1] = intermediate.size() ;
          nowFunctionIndex = num ;
		  SUBROUTINE( tokenArray[i], i ) ;
		} // else if()
		  
		else if( tokenArray[i].token[0] == "VARIABLE" ){
          if( VARIABLE( tokenArray[i], i ) ) {
            addVARIABLE( tokenArray[i], i );
		  } // if()
		} // else if()
		  
		else if( tokenArray[i].token[0] == "LABEL" ){
          if( LABEL( tokenArray[i], i ) ){
          	addLABLE( tokenArray[i], i );
		  } // if()
		} // else if()
		  
		else if( tokenArray[i].token[0] == "DIMENSION" ){
          DIMENSION( tokenArray[i], i ) ;
		} // else if()
		
		else if( tokenArray[i].token[0] == ";" ){
		  if( tokenArray[i].token.size() == 1 ) {
		    Intermediate in ;
            in.num = intermediate.size() ;
            in.str = "" ;
            in.one.push_back( 0 ) ;
            in.one.push_back( 0 ) ;
            in.two.push_back( 0 ) ;
            in.two.push_back( 0 ) ;
            in.three.push_back( 0 ) ;
            in.three.push_back( 0 ) ;
            in.four.push_back( 0 ) ;
            in.four.push_back( 0 ) ;
            intermediate.push_back( in ) ;	
		  } // if()
		  
		  else {
		    cout << "第" + to_string(i+1) + "行: empty statement 文法錯誤!!!" << endl ; 
		  	errorMessage.push_back( "第" + to_string(i+1) + "行: empty statement 文法錯誤!!!" ) ; 
		  } // else 
		} // else if()
		  
		else if( tokenArray[i].token[0] == "IF" ){
          if( IF( tokenArray[i], i, 1 ) ) {
            addIF( tokenArray[i], i, 1 ) ;
		  } // if()
		} // else if()
		  
		else if( tokenArray[i].token[0] == "GTO" ){
          if( GTO( tokenArray[i], i, 1 ) ){
          	addGTO( tokenArray[i], i, 1 ) ;
		  } // if()
		} // else if()
		  
		else if( tokenArray[i].token[0] == "CALL" ){
          if( CALL( tokenArray[i], i, 1 ) ) {
          	addCALL( tokenArray[i], i, 1 ) ;
		  } // if()
		} // else if()
		  
		else if( tokenArray[i].token[0] == "INPUT" ){
          if( INPUT( tokenArray[i], i, 1 ) ) {
          	addIO( tokenArray[i], i, 1 );
		  } // if()
		} // else if()
		  
		else if( tokenArray[i].token[0] == "OUTPUT" ){
          if( OUTPUT( tokenArray[i], i, 1 ) ){
          	addIO( tokenArray[i], i, 1 );
		  } // if()
		} // else if()
		  
		else { // assignment // 有 LABLE 在最前面 !!!!! 
		  if( tokenArray[i].token[0] == "ENP" ) { 
		    if( tokenArray[i].token.size() == 2 && tokenArray[i].token[1] == ";" ){
		      Intermediate in ;
              in.num = intermediate.size() ;
              in.str = "ENP" ;
              in.one.push_back( tokenArray[i].tokenType[0] ) ;
              in.one.push_back( tokenArray[i].tokenValue[0] ) ;
              in.two.push_back( 0 ) ;
              in.two.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              in.four.push_back( 0 ) ;
              in.four.push_back( 0 ) ;
              intermediate.push_back( in ) ;
			} // if()
			
			else {
			  cout << "第" + to_string(i+1) + "行: ENP 文法錯誤!!!" << endl ; 
		  	  errorMessage.push_back( "第" + to_string(i+1) + "行: ENP 文法錯誤!!!" ) ; 
			}
		  } // if()
		    
		  else if ( tokenArray[i].token[0] == "ENS" ) { 
		    if( tokenArray[i].token.size() == 2 && tokenArray[i].token[1] == ";" ){
		      Intermediate in ;
              in.num = intermediate.size() ;
              in.str = "ENS" ;
              in.one.push_back( tokenArray[i].tokenType[0] ) ;
              in.one.push_back( tokenArray[i].tokenValue[0] ) ;
              in.two.push_back( 0 ) ;
              in.two.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              in.three.push_back( 0 ) ;
              in.four.push_back( 0 ) ;
              in.four.push_back( 0 ) ;
              intermediate.push_back( in ) ;
            } // if()
			
			else {
			  cout << "第" + to_string(i+1) + "行: ENS 文法錯誤!!!" << endl ; 
		  	  errorMessage.push_back( "第" + to_string(i+1) + "行: ENS 文法錯誤!!!" ) ; 
		    }
		  } // else if()
		  
		  else if( ( Identifier( tokenArray[i].token[0] ) && tokenArray[i].tokenType[0] != 2 ) && 
		           ( tokenArray[i].token[1] == "=" || tokenArray[i].token[1] == "(" ) ){ // ASSIGNMENT !!! 
		  	bool expressionHaveArray = false ;
		  	int same = 0 ;
		  	for( int j = 0 ; j < tokenArray[i].token.size() ; j++ ){
		  	  if( tokenArray[i].token[j] == "=" ) {
		  	  	same = j ;
		  	  	break ;
			  } // if()
			} // for()
			
			if( same != 0 ) {
		  	  for( int j = same +1 ; j < tokenArray[i].token.size() ; j++ ) { 
		  	    if( tokenArray[i].token[j] == "(" && Identifier( tokenArray[i].token[j-1] ) ){
		  	  	  expressionHaveArray = true ;
		  	  	  break ;
			    } // if()
			  } // for()
		  	
              if( Identifier( tokenArray[i].token[0] ) && tokenArray[i].token[1] == "=" && expressionHaveArray == true ) {
			    if( ASSIGNMENTHaveArray( tokenArray[i], i, 2 ) ){
			      addASSIGNMENTHaveArray( tokenArray[i], i, 2 ) ;
			    } // if()
			  } // if()
			   
              else if( Identifier( tokenArray[i].token[0] ) && tokenArray[i].token[1] == "=" && expressionHaveArray == false ) {
			    if( ASSIGNMENT( tokenArray[i], i, 2 ) ){
			      addASSIGNMENT( tokenArray[i], i, 2 ) ;
				} // if()
              } // else if()
              
			  else if( Identifier( tokenArray[i].token[0] ) && tokenArray[i].token[1] == "(" && expressionHaveArray == true ) {
			    if( ASSIGNMENTHaveArrayInHeadAndHaveArray( tokenArray[i], i, 2 ) ){
			      addASSIGNMENTHaveArrayInHeadAndHaveArray( tokenArray[i], i, 2 ) ;
				} // if()
              } // else if()
              
			  else if( Identifier( tokenArray[i].token[0] ) && tokenArray[i].token[1] == "(" && expressionHaveArray == false ) {
			    if( ASSIGNMENTHaveArrayInHeadAndWithoutArray( tokenArray[i], i, 2 ) ){
			      addASSIGNMENTHaveArrayInHeadAndWithoutArray( tokenArray[i], i, 2 );
				} // if()
			  } // else if()
			  
			  else { 
			    errorMessage.push_back( "第" + to_string(i+1) + "行: " + "ASSIGNMENT 文法錯誤!!!!!" ) ;
			    cout << "第" + to_string(i+1) + "行: " + "ASSIGNMENT 文法錯誤!!!!!"<< endl ; 
			  } // else 
		    } // if()
		    
		    else {
		      errorMessage.push_back( "第" + to_string(i+1) + "行: " + "ASSIGNMENT 文法錯誤!!!!!" ) ;
			  cout << "第" + to_string(i+1) + "行: " + "ASSIGNMENT 文法錯誤!!!!!" << endl ; 
		    } // else
		  } // else if()
		
		  // 有 LABLE 在最前面 !!!!! 
		  else if( Identifier( tokenArray[i].token[0] ) && tokenArray[i].tokenType[0] != 2 ){
            int index ;

			if( tokenArray[i].token[1] == "ENP" ) { 
			  if( tokenArray[i].token.size() == 3 && tokenArray[i].token[2] == ";" ){
			  	addLabelIndex( i ) ;
		        Intermediate in ;
                in.num = intermediate.size() ;
                in.str = "ENP" ;
                in.one.push_back( tokenArray[i].tokenType[1] ) ;
                in.one.push_back( tokenArray[i].tokenValue[1] ) ;
                in.two.push_back( 0 ) ;
                in.two.push_back( 0 ) ;
                in.three.push_back( 0 ) ;
                in.three.push_back( 0 ) ;
                in.four.push_back( 0 ) ;
                in.four.push_back( 0 ) ;
                intermediate.push_back( in ) ;
			  } // if()
			
			  else {
			    errorMessage.push_back( "第" + to_string(i+1) + "行: " + "ENP 文法錯誤!!!!" ) ;
			    cout << "第" + to_string(i+1) + "行: " + "ENP 文法錯誤" << endl ;
		      }
		    } // if()
		    
			else if ( tokenArray[i].token[1] == "ENS" ) { 
			  if( tokenArray[i].token.size() == 3 && tokenArray[i].token[2] == ";" ){
			  	addLabelIndex( i ) ;
		        Intermediate in ;
                in.num = intermediate.size() ;
                in.str = "ENS" ;
                in.one.push_back( tokenArray[i].tokenType[1] ) ;
                in.one.push_back( tokenArray[i].tokenValue[1] ) ;
                in.two.push_back( 0 ) ;
                in.two.push_back( 0 ) ;
                in.three.push_back( 0 ) ;
                in.three.push_back( 0 ) ;
                in.four.push_back( 0 ) ;
                in.four.push_back( 0 ) ;
                intermediate.push_back( in ) ;
			  } // if()
			
			  else {
			  	errorMessage.push_back( "第" + to_string(i+1) + "行: " + "ENP 文法錯誤!!!!" ) ;
			  	cout << "第" + to_string(i+1) + "行: " + "ENP 文法錯誤" << endl ;
			  }
		    } // else if()
		    
		    else if( tokenArray[i].token[1] == ";" ){
		      if( tokenArray[i].token.size() == 2 ) {
		      	addLabelIndex( i ) ;
		        Intermediate in ;
                in.num = intermediate.size() ;
                in.str = "" ;
                in.one.push_back( 0 ) ;
                in.one.push_back( 0 ) ;
                in.two.push_back( 0 ) ;
                in.two.push_back( 0 ) ;
                in.three.push_back( 0 ) ;
                in.three.push_back( 0 ) ;
                in.four.push_back( 0 ) ;
                in.four.push_back( 0 ) ;
                intermediate.push_back( in ) ;	
			  } // if()
			  
		      else {
		      	errorMessage.push_back( "第" + to_string(i+1) + "行: " + "empty statement 文法錯誤!!!!" ) ;
		      	cout << "第" + to_string(i+1) + "行: " + "empty statement 文法錯誤!!!!" << endl ; 
			  }
		    } // else if()
		    
			else if( tokenArray[i].token[1] == "IF" ){
              if( IF( tokenArray[i], i, 2 ) ) {
              	addLabelIndex( i ) ;
              	addIF( tokenArray[i], i, 2 ) ;
			  } // if()
		    } // else if()
		  
		    else if( tokenArray[i].token[1] == "GTO" ){
		      if( GTO( tokenArray[i], i, 2 ) ){
		      	addLabelIndex( i ) ;
          	    addGTO( tokenArray[i], i, 2 ) ;
		      } // if()
		    } // else if()
		  
		    else if( tokenArray[i].token[1] == "CALL" ){
              if( CALL( tokenArray[i], i, 2 ) ) {
              	addLabelIndex( i ) ;
              	addCALL( tokenArray[i], i, 2 ) ;
			  } // if()
		    } // else if()
		    
		    else if( ( Identifier( tokenArray[i].token[1] ) && tokenArray[i].tokenType[1] == 0 && tokenArray[i].tokenValue[1] == 0 ) && 
		                                                   ( tokenArray[i].token[2] == "=" || tokenArray[i].token[2] == "(" ) ){ // ASSIGNMENT !!! 
		  	  
			  bool expressionHaveArray = false ;
		  	  int same = 0 ;
		  	  
		  	  for( int j = 1 ; j < tokenArray[i].token.size() ; j++ ){
		  	    if( tokenArray[i].token[j] == "=" ) {
		  	  	  same = j ;
		  	  	  break ;
			    } // if()
			  } // for()
			  
			  if( same != 0 ) {
		  	    for( int j = same +1 ; j < tokenArray[i].token.size() ; j++ ) { 
		  	      if( tokenArray[i].token[j] == "(" && Identifier( tokenArray[i].token[j-1]) ){
		  	  	    expressionHaveArray = true ;
		  	  	    break ;
			      } // if()
			    } // for()
	  
                if( Identifier( tokenArray[i].token[1] ) && tokenArray[i].token[2] == "=" && expressionHaveArray == true ) {
			      if( ASSIGNMENTHaveArray( tokenArray[i], i, 3 ) ){
			      	addLabelIndex( i ) ;
			        addASSIGNMENTHaveArray( tokenArray[i], i, 3 ) ;
			      } // if()
			    } // if()
			   
                else if( Identifier( tokenArray[i].token[1] ) && tokenArray[i].token[2] == "=" && expressionHaveArray == false ) {
			      if( ASSIGNMENT( tokenArray[i], i, 3 ) ){
			      	addLabelIndex( i ) ;
			        addASSIGNMENT( tokenArray[i], i, 3 ) ;
				  } // if()
                } // else if()
              
			    else if( Identifier( tokenArray[i].token[1] ) && tokenArray[i].token[2] == "(" && expressionHaveArray == true ) {
			      if( ASSIGNMENTHaveArrayInHeadAndHaveArray( tokenArray[i], i, 3 ) ){
			      	addLabelIndex( i ) ;
			        addASSIGNMENTHaveArrayInHeadAndHaveArray( tokenArray[i], i, 3 ) ;
				  } // if()
                } // else if()
              
			    else if( Identifier( tokenArray[i].token[1] ) && tokenArray[i].token[2] == "(" && expressionHaveArray == false ) {
			      if( ASSIGNMENTHaveArrayInHeadAndWithoutArray( tokenArray[i], i, 3 ) ){
			      	addLabelIndex( i ) ;
			        addASSIGNMENTHaveArrayInHeadAndWithoutArray( tokenArray[i], i, 3 );
			  	  } // if()
			    } // else if()
		      
			    else {
			      errorMessage.push_back( "第" + to_string(i+1) + "行: " + "ASSIGNMENT 文法錯誤!!!!" ) ;	
			      cout << "第" + to_string(i+1) + "行: " + "ASSIGNMENT 文法錯誤!!!!" << endl ;
				}
		      } // if()
		      
		      else {
			    cout << "第" + to_string(i+1) + "行: " + "ASSIGNMENT 文法錯誤!!!!" << endl ;
			    errorMessage.push_back( "第" + to_string(i+1) + "行: " + "ASSIGNMENT 文法錯誤!!!!" ) ;
			  } 
		    } // else if()
		    
		    else if( tokenArray[i].token[1] == "INPUT" ){
		      if( INPUT( tokenArray[i], i, 2 ) ) {
		      	addLabelIndex( i ) ;
          	    addIO( tokenArray[i], i, 2 );
		      } // if()
		    } // else if()
		  
		    else if( tokenArray[i].token[1] == "OUTPUT" ){
		      if( OUTPUT( tokenArray[i], i, 2 ) ) {
		      	addLabelIndex( i ) ;
          	    addIO( tokenArray[i], i, 2 );
		      } // if()
		    } // else if()
			
			else {
			  errorMessage.push_back( "第" + to_string(i+1) + "行: " + "條件皆不符合!!!!" ) ;
		  	  cout << "第" + to_string(i+1) + "行: " + "條件皆不符合!!!!" << endl ;
		    } // else 
		  } // else if()
		  
		  else {
		    errorMessage.push_back( "第" + to_string(i+1) + "行: " + "條件皆不符合!!!!" ) ;
		  	cout << "第" + to_string(i+1) + "行: " + "條件皆不符合!!!!" << endl ;
		  } // else 
		} // else 
	  } // for() 
	  
	  int temp = 0 ;
	  string str = "" ;
	  int index ; 
	  for( int i = 0 ; i < intermediate.size() ; i++ ){
	    if( intermediate[i].four[0] == 6 && intermediate[i].four[1] == 9999 ) {
	      if( intermediate[i].one[0] == 2 && intermediate[i].one[1] == 12 ) { // IF
	      	for( int j = i + 1 ; j < intermediate.size() ; j++ ){
	      	  if( intermediate[j].one[0] == 2 && intermediate[j].one[1] == 11 ) {
				temp = j - i + 1 ;
				break ;
			  } // if()
			} // for()
			
			intermediate[i].four[1] = i + temp ;
		  } // if()
		  
		  else if( intermediate[i].one[0] == 2 && intermediate[i].one[1] == 11 ){ // GTO
		  	for( int zz = 0 ; zz < intermediate[i].str.size() ; zz++ ){
		  	  str = str + intermediate[i].str[zz] ;
		  	  if( str == "GTO" ) {
		  	  	zz = zz + 1 ;
		  	  	str = "" ;
			  } // if()
			} // for()
			
			if( identifier.contain( str, index ) ){
			  temp = identifier.table[index].pointer[1] ;
			} // if()
			
			else {
			  cout << "這個Label沒有地放可以跳!!!" << endl ; 
			} // else
			
			intermediate[i].four[1] = temp ;
		  } // else if()
		} // if()
	  } // for() 
      
      write( fileName ) ;
	  errorMessage.clear() ; 
      tokenArray.clear() ;
      TEMP.initial() ; // Table0
      integer.initial() ; // Table3
      realNumber.initial() ; // Table4
      identifier.initial() ; // Table5
      intermediate.clear() ; // Table6
      information.clear() ; // Table7
      nowFunctionIndex = 0 ;
      T = 1 ;
	} // else 
  } // while()
} // main() 
