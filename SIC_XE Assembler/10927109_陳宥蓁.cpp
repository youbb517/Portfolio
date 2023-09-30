// 資工三甲 10927109 陳宥蓁 2022/11/13

# include<stdio.h>
# include<iostream>
# include<fstream>
# include<vector>
# include<string>
# include<string.h>
# include<cctype>
# include<algorithm>
# include<sstream>
# include<math.h> 
#include <bitset>
#include <iomanip>

using namespace std;

string opCodeArr[59][2] = { {"ADD", "18"}, {"ADDF", "58"}, {"ADDR", "90"}, {"AND", "40" }, {"CLEAR", "B4"}, {"COMP", "28"}, {"COMPF", "88"}, {"COMPR", "A0"}, 
                             {"DIV", "24"}, {"DIVF", "64"}, {"DIVR", "9C"}, {"FIX", "C4"}, {"FLOAT", "C0"}, {"HIO", "F4"}, {"J", "3C"}, {"JEQ", "30"}, 
							 {"JGT", "34"}, {"JLT", "38"}, {"JSUB", "48"}, {"LDA", "00"}, {"LDB", "68"}, {"LDCH", "50"}, {"LDF", "70"}, {"LDL", "08"}, {"LDS", "6C"}, 
							 {"LDT", "74"}, {"LDX", "04"}, {"LPS", "D0"}, {"MUL", "20"}, {"MULF", "60"}, {"MULR", "98"}, {"NORM", "C8"}, {"OR", "44"}, 
							 {"RD", "D8"}, {"RMO", "AC"}, {"RSUB", "4C"}, {"SHIFTL", "A4"}, {"SHIFTR", "A8"}, {"SIO", "F0"}, {"SSK", "EC"}, {"STA", "0C"}, 
							 {"STB", "78"}, {"STCH", "54"}, {"STF", "80"}, {"STI", "D4"}, {"STL", "14"}, {"STS", "7C"}, {"STSW", "E8"}, {"STT", "84"}, 
							 {"STX", "10"}, {"SUB", "1C"}, {"SUBF", "5C"}, {"SUBR", "94"}, {"SVC", "B0"}, {"TD", "E0"}, {"TIO", "F8"}, {"TIX", "2C"},
							 {"TIXR", "B8"}, {"WD", "DC"} };

// Yes => SIC 跟 SIC/XE 都可用  
// No => only SIC/XE  
string opCodeFormatArr[59][2] = { {"3/4", "Yes"}, {"3/4", "No"}, {"2", "No"}, {"3/4", "Yes"}, {"2", "No"}, {"3/4", "Yes"}, {"3/4", "No"}, {"2", "No"}, 
                                  {"3/4", "Yes"}, {"3/4", "No"}, {"2", "No"}, {"1", "No"}, {"1", "No"}, {"1", "No"}, {"3/4", "Yes"}, {"3/4", "Yes"}, 
								  {"3/4", "Yes"}, {"3/4", "Yes"}, {"3/4", "Yes"}, {"3/4", "Yes"}, {"3/4", "No"}, {"3/4", "Yes"}, {"3/4", "No"}, 
								  {"3/4", "Yes"}, {"3/4", "No"}, {"3/4", "No"}, {"3/4", "Yes"}, {"3/4", "No"}, {"3/4", "Yes"}, {"3/4", "No"}, {"2", "No"},
								  {"1", "No"}, {"3/4", "Yes"}, {"3/4", "Yes"}, {"2", "No"}, {"3/4", "Yes"}, {"2", "No"}, {"2", "No"}, {"1", "No"}, 
								  {"3/4", "No"}, {"3/4", "Yes"}, {"3/4", "No"}, {"3/4", "Yes"}, {"3/4", "No"}, {"3/4", "No"}, {"3/4", "Yes"}, {"3/4", "No"}, 
								  {"3/4", "Yes"}, {"3/4", "No"}, {"3/4", "Yes"}, {"3/4", "Yes"}, {"3/4", "No"}, {"2", "No"}, {"2", "No"}, {"3/4", "Yes"},
								  {"1", "No"}, {"3/4", "Yes"}, {"2", "No"}, {"3/4", "Yes"} };

// 1 => 0001 							 
string opcodeToMachineCode( string temp ){
  string opcode = "" ;
  for( int i = 0 ; i < temp.size() ; i++ ){
  	if( temp[i] == '0' ) opcode = opcode + "0000" ;
  	else if( temp[i] == '1' ) opcode = opcode + "0001" ;
  	else if( temp[i] == '2' ) opcode = opcode + "0010" ;
  	else if( temp[i] == '3' ) opcode = opcode + "0011" ;
  	else if( temp[i] == '4' ) opcode = opcode + "0100" ;
  	else if( temp[i] == '5' ) opcode = opcode + "0101" ;
  	else if( temp[i] == '6' ) opcode = opcode + "0110" ;
  	else if( temp[i] == '7' ) opcode = opcode + "0111" ;
  	else if( temp[i] == '8' ) opcode = opcode + "1000" ;
  	else if( temp[i] == '9' ) opcode = opcode + "1001" ;
  	else if( temp[i] == 'A' ) opcode = opcode + "1010" ;
  	else if( temp[i] == 'B' ) opcode = opcode + "1011" ;
  	else if( temp[i] == 'C' ) opcode = opcode + "1100" ;
  	else if( temp[i] == 'D' ) opcode = opcode + "1101" ;
  	else if( temp[i] == 'E' ) opcode = opcode + "1110" ;
  	else opcode = opcode + "1111" ; // F
  } // for()
  
  return opcode ;
} // opcodeToMachineCode()

int searchOpCodeIndex( string temp ){
  string temp1 = "", temp2 = "" ;
  
  for( int i = 0 ; i < temp.size() ; i++ ) {
  	temp1 += toupper(temp[i]) ;
    temp2 += tolower(temp[i]) ;
  } // for()
 
  for( int i = 0 ; i < 59 ; i++ ) if( opCodeArr[i][0] == temp1 || opCodeArr[i][0] == temp2 ) return i ;
} // searchOpCodeIndex()

string searchOpCodeFormat( string temp ){
  return opCodeFormatArr[searchOpCodeIndex(temp)][0] ;
} // searchOpCodeFormat()

bool SIC_Can_Use( string temp ){
  if( opCodeFormatArr[searchOpCodeIndex(temp)][1] == "Yes" ) return true ;
  else return false ;
} // SIC_Can_Use()

struct MachineCode{
  int line ;
  string loc ;
  string sourceStatement ;
  string objectCode ;
} ;

vector <MachineCode> machineCode ;

struct Token{
  vector <int> tokenType ;
  vector <int> tokenValue ;
  vector <string> token ;
  int startAddress = 0 ;
  string start = "" ;
  int length = 0 ;
};

vector <Token> tokenArray ;

struct symbolAndLiteral{
  string thing ;
  string address ;
  string strOrDec ;
};

class ConstantTable {    
  public:
  	ConstantTable() { 	  
	  table.clear() ; 
	  table.push_back("") ; 
	} // constructor
	
	vector<string> table ;  
  	
    void insert(string temp) {  //加入表中
      table.push_back(temp) ;
    } // insert()
};

class Hash {		
  public:
  	vector<string> hashTable ;	// 雜湊表
	int hashTableSize ;		// 雜湊表大小
	
	void reset() { 
	  hashTableSize = 100 ; 
	  hashTable.resize( hashTableSize ) ;
	} // reset()   
	
    Hash() { reset() ; }	// constructor
    
    int hashFunction( string temp ) {
      int key = 0 ;
      bool findSame = false ;
      
      for( int i = 0 ; i < temp.size() ; i++ ) key += (int)temp[i] ;
      key = key % hashTableSize ;	// ASICC除以雜湊表大小取餘數
	  
	  while( hashTable[key].size() != 0 ){
	  	if( hashTable[key] == temp ){
	  	  findSame = true ;
	  	  break ;
		} // if()
		
	    if( key == 99 ) key = 0 ;	
	    else key++ ;
	  } // while()
	  
	  if( findSame == false ) hashTable[key] = temp ; 
	  return key ; 
	} // hashFunction()
	
	bool contain( string temp, int & num ) { //查詢某一字元是否有temp字串
      string temp1 = "", temp2 = "" ;
  
      for( int i = 0 ; i < temp.size() ; i++ ) {
  	    temp1 += toupper(temp[i]) ;
  	    temp2 += tolower(temp[i]) ;
      } // for()
  
      for( int i = 0 ; i < hashTable.size() ; i++ ) {
        if( hashTable[i] == temp1 || hashTable[i] == temp2 ){
          num = i ;
          return true ;
	    } // if()
      } // for()

      return false ;
    } // contain()
} ; // Hash

class SymbolTable { // 5
  public:
  	vector<symbolAndLiteral> hashTable ;	// 雜湊表
  	int hashTableSize ;		// 雜湊表大小
  	
  	void reset() { 
	  hashTableSize = 100 ; 
	  hashTable.resize( hashTableSize ) ;
	} // reset()   
  	
    SymbolTable() { reset() ; } // constructor
    
    int hashFunction(string temp) {
      int key = 0 ;
      
      for( int i = 0 ; i < temp.size() ; i++ ) key += (int)temp[i] ;
      key = key % hashTableSize ;	// ASICC除以雜湊表大小取餘數
	  
	  while( hashTable[key].thing.size() != 0 ){
	  	if( hashTable[key].thing == temp ){
	  	  return -9999 ;
		} // if()
		
	    if( key == 99 ) key = 0 ;	
	    else key++ ;
	  } // while()
	  
	  hashTable[key].thing = temp ; 
	  return key ; 
	} // hashFunction()

	bool contain( string temp, int & num ) { //查詢某一字元是否有temp字串
      string temp1 = "", temp2 = "" ;
  
      for( int i = 0 ; i < temp.size() ; i++ ) {
  	    temp1 += toupper(temp[i]) ;
  	    temp2 += tolower(temp[i]) ;
      } // for()
  
      for( int i = 0 ; i < hashTable.size() ; i++ ) {
        if( hashTable[i].thing == temp1 || hashTable[i].thing == temp2 ){
          num = i ;
          return true ;
	    } // if()
      } // for()

      return false ;
    } // contain()
};

class IntegerTable : public Hash { // 6
  public: 
    IntegerTable() { Hash() ; } // constructor
};

class StringTable : public Hash { // 7
  public: 
    StringTable() { Hash() ; } // constructor
    
    bool contain( string temp, int & num ) { //查詢某一字元是否有temp字串
      for( int i = 0 ; i < hashTable.size() ; i++ ) {
        if( hashTable[i] == temp ){
          num = i ;
          return true ;
	    } // if()
      } // for()

      return false ;
    } // contain()
};

class LiteralTable { // 8
  public:
  	vector<symbolAndLiteral> hashTable ;	// 雜湊表
  	int hashTableSize ;		// 雜湊表大小
  	
  	void reset() { 
	  hashTableSize = 100 ; 
	  hashTable.resize( hashTableSize ) ;
	} // reset() 
  	
    LiteralTable() { reset() ; } // constructor
	
	int hashFunction(string temp) {
      int key = 0 ;
      bool findSame = false ;
	  
	  while( hashTable[key].thing.size() != 0 ){
	  	if( hashTable[key].thing == temp ){
	  	  findSame = true ;
	  	  break ;
		} // if()
		
	    if( key == 99 ) key = 0 ;	
	    else key++ ;
	  } // while()
	  
	  if( findSame == false ) {
	    hashTable[key].thing = temp ;
	  } // if()
	  
	  return key ; 
	} // hashFunction() 
	
	bool contain( string temp, int & num ) { //查詢某一字元是否有temp字串
      string temp1 = "", temp2 = "" ;
  
      for( int i = 0 ; i < temp.size() ; i++ ) {
  	    temp1 += toupper(temp[i]) ;
  	    temp2 += tolower(temp[i]) ;
      } // for()
  
      for( int i = 0 ; i < hashTable.size() ; i++ ) {
        if( hashTable[i].thing == temp1 || hashTable[i].thing == temp2 ){
          num = i ;
          return true ;
	    } // if()
      } // for()

      return false ;
    } // contain()
};

void ReadTableData( ConstantTable & table, string fileName ) {
  ifstream inputFile;
  string tempLine;
  inputFile.open( fileName );
  
  while( getline( inputFile, tempLine ) ) table.insert( tempLine ) ; 
  inputFile.close();
} // ReadTableData()

bool contain( ConstantTable & table, string temp, int & num ) { //查詢某一字元是否有temp字串
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

bool isInt( string temp ){
  for( int i = 0 ; i < temp.size() ; i++ )  if( '0' > temp[i] || temp[i] > '9' ) return false ;
  return true ;
} // isInt

string decToHex_addr( int decimal ){
  stringstream stream2;
  stream2 << setfill('0') << setw(4) << hex << decimal ;
  string result = stream2.str() ;
  string temp ;
  
  for( int i = 0 ; i < result.size() ; i++ ) {
  	temp += toupper(result[i]) ;
  } // for()
  
  return temp;
} // decToHex()

string decToHex( int decimal ){
  stringstream stream2;
  stream2 << hex << decimal ;
  string result = stream2.str() ;
  string temp ;
  
  for( int i = 0 ; i < result.size() ; i++ ) {
  	temp += toupper(result[i]) ;
  } // for()
  
  return temp;
} // decToHex()

int hexToDec( string temp ){
  return (int)strtol(temp.c_str(), NULL, 16) ;
} // hexToDec() 

string convertBinToHex( string binText ){
  stringstream res;
  string binary_str(binText);
  bitset<16> set(binary_str);
  res << setfill('0') << setw(4) << hex << uppercase << set.to_ulong();
  return res.str();
} // if()

string convertBinToHex_12( string binText ){
  stringstream res;
  string binary_str(binText);
  bitset<12> set(binary_str);
  res << setfill('0') << setw(3) << hex << uppercase << set.to_ulong();
  return res.str();
} // if()

string dexToBin(int n){
  bitset<16> r(n) ;
  stringstream res;
  res << r ;
  return res.str();
} // toBinary()

string diff( string temp ){
  string t; 
  for( int i = 0 ; i < temp.size() ; i++ ){
  	if( temp[i] == '0' ) t = t + "1" ;
  	else t = t + "0" ;
  } // for() 
  	
  return t ;	
} // diff()

string binAdd( string temp1, string temp2 ){
  string sum ;
  if( temp2.size() < temp1.size() ) for( int i = temp2.size() ; i < temp1.size() ; i++ ) temp2 = "0" + temp2 ;
  
  int carry = 0 ;
  for( int i = temp1.size() - 1 ; i >= 0 ; i-- ){
    if( carry == 0 && temp1[i] == '0' && temp2[i] == '0' ) {
      sum = "0" + sum ;
      carry = 0 ;
	} // if()
	
	else if( carry == 0 && temp1[i] == '1' && temp2[i] == '0' ) {
      sum = "1" + sum ;
      carry = 0 ;
	} // else if()
	
	else if( carry == 0 && temp1[i] == '0' && temp2[i] == '1' ) {
      sum = "1" + sum ;
      carry = 0 ;
	} // else if()
	
	else if( carry == 0 && temp1[i] == '1' && temp2[i] == '1' ) {
      sum = "0" + sum ;
      carry = 1 ;
	} // else if()
	
	else if( carry == 1 && temp1[i] == '0' && temp2[i] == '0' ) {
      sum = "1" + sum ;
      carry = 0 ;
	} // else if()
	
	else if( carry == 1 && temp1[i] == '1' && temp2[i] == '0' ) {
      sum = "0" + sum ;
      carry = 1 ;
	} // else if()
	
	else if( carry == 1 && temp1[i] == '0' && temp2[i] == '1' ) {
      sum = "0" + sum ;
      carry = 1 ;
	} // else if()
	
	else if( carry == 1 && temp1[i] == '1' && temp2[i] == '1' ) {
      sum = "1" + sum ;
      carry = 1 ;
	} // else if()
  } // for()
  
  return sum;
} // binAdd()

string convertHexToBin(string text){
    int no = 0;
    string binaryResult = "";
    for (int i = 0 ; i < text.length() ; i++){
      string temp = "";
      if (text[i] >= '0' && text[i] <= '9'){
        no = int(text[i]) - 48;
        while (no > 0){
          temp += char(no % 2 + 48);
          no /= 2;
        } // while()
      
	    while (temp.length() != 4){
          temp += '0';
        } // while()
      
	    no = 0;
      } // if()
      
      else {
        no = text[i] - 'A' + 10;
        while (no > 0){
          temp += char(no % 2 + 48);
          no /= 2;
        } // while()
      } // else 
      
      for (int j = 3 ; j >= 0 ; j--){
        binaryResult += temp[j];
      } // for()
    }// for()
    
    return binaryResult;
} // convertHexToBin()

void writeFile( string temppp ) { 
    ofstream out; // 建立輸出串流物件
    out.open( temppp + "_output.txt" ); // 開啟即將被寫入資料的檔案
    if(out.fail()){ //確認是否開啟成功
        cout << "input file opening failed...";
        exit(1);
    } // if()
    
    out << "Line  Location  Source code		Object code" << endl ;
    out << "----  -------- ------------		-----------"  << endl ;
    for( int i = 0 ; i < machineCode.size() ; i++ ){ 
	  out << setw(4) << machineCode[i].line << "  " << machineCode[i].loc << " " << machineCode[i].sourceStatement << "			" << machineCode[i].objectCode << endl ;
	} // for()   
} // writeFile()

int main(){
  ConstantTable instruction ;
  ConstantTable pseudo ;
  ConstantTable reg ;
  ConstantTable delimiter ;
  
  ReadTableData( instruction, "Table1.table" ) ; 
  ReadTableData( pseudo, "Table2.table" ) ; 
  ReadTableData( reg, "Table3.table" ) ;  
  ReadTableData( delimiter, "Table4.table" ) ; 
  
  while ( true ) {
    printf("\n***************\n");
    printf("* 0. Quit     *\n");
    printf("* 1. SIC      *\n");  
    printf("* 2. SIC/XE   *\n");
    printf("***************\n");
    printf("Input a choice(0, 1, 2): " ) ;
    
    SymbolTable symbol ;
    IntegerTable integer ;
    StringTable str ;
    LiteralTable literal ;
    
    int choice = 0 ; 
    int numLines = 0;
    int numEQU ; // 遇到EQU時存在 tokenArray 的位置 
    int PC = 0 ;
	string B ;
	bool haveBase = false ;
  
    string tempLine, fileName;
    vector <string> inputArray ;
    
    cin >> choice ;
    
    if( choice == 0 ) break ;
    
    else if( choice == 1 ) { // SIC
      ifstream inputFile ;
      cout << "請輸入檔名: " ;
      string temppp ;
      cin >> temppp ;
      
	  inputFile.open( temppp + "_input.txt") ;
      while( getline( inputFile, tempLine ) ) if( tempLine != "" ) inputArray.push_back(tempLine) ; 
      inputFile.close();
      vector <int> undefindSymbol ;
      
      for( int i = 0 ; i < inputArray.size() ; i++ ) { // PASS 1 
  	    string line = inputArray[i] ;
  	    bool findError = false ;
  	    bool isString = false, isLiteral = false, isEQU = false, isLTORG = false, isEND = false ; 
  	    Token t ;
  	    int start = 0, end = 0 ; // token的頭跟尾 
  	    int num = 0 ; // 回傳在table中的位置
	    int lineBytes = 0 ; 

		for( int j = 0 ; j <= line.size() ; j++ ) {
  	      string temp = "" ;
  	      temp.append(1, line[j]);
  	  
  	      if( end == line.size()-1 || line[j] == ' ' || line[j] == '\t' || contain( delimiter, temp, num ) ) {
  	  	    string tempToken = "" ;
		    for( int y = start ; line[start] != ' ' && line[start] != '\t' && y <= end ; y++ ) tempToken = tempToken + line[y] ;
		    if( line[start] != ' ' && line[start] != '\t' && !contain( delimiter, tempToken, num ) ) {
		      t.token.push_back(tempToken) ;
		  
		      if( t.token.size() >= 2 ) { // isLitreal
		  	    if( t.token[t.token.size()-2] == "=" ) { // + instruction =6666 or instruction =6666
			      isLiteral = true ;
		        } // if()
		      }// if()
		  
		      if( t.token.size() >= 3 ) { // isLitreal or string 
		        if( t.token[t.token.size()-2] == "'" && temp == "'" && t.token[t.token.size()-4] == "=" ) { // (labol) (+) instruction =C'EOF'
		          isLiteral = true ;
			    } // if()
			    
			    else if( t.token[t.token.size()-2] == "'" && temp == "'" && t.token[t.token.size()-4] != "=" ){ // (labol) BYTE X'EOF'
			      isString = true ;
				} // else if()
		      } // if() 
		      
		      if( ( tempToken == "c" || tempToken == "C" || tempToken == "x" || tempToken == "X" ) && temp == "'" ){
		      	t.tokenType.push_back(9) ;
		      	t.tokenValue.push_back(0) ;
			  } // if() 
		  
		      else if( contain( instruction, tempToken, num ) && isString == false && isLiteral == false ) {
		  	    t.tokenType.push_back(1) ;
		  	    t.tokenValue.push_back(num) ;
		      } // else if()
		  
			  else if( contain( pseudo, tempToken, num ) && isString == false && isLiteral == false ) {
			  	t.tokenType.push_back(2) ;
			  	t.tokenValue.push_back(num) ;
			  } // else if()
		  
			  else if( contain( reg, tempToken, num ) && isString == false && isLiteral == false ){
			  	t.tokenType.push_back(3) ;
			  	t.tokenValue.push_back(num) ;
			  } // else if()
		  
			  else {
			  	if( t.token.size() == 1 && !isInt( tempToken ) && isString == false && isLiteral == false ) { // symbol
			  	  int num = symbol.hashFunction(tempToken) ;
			  	  
			  	  if( num == -9999 ) { // 重複定義 
			  	  	findError = true ;
				  } // if()
				  
				  else {
			  	    t.tokenType.push_back(5) ;
			  	    t.tokenValue.push_back(num) ;
			      } // else 
				} // if()
			
				else if( t.token.size() > 1 && !isInt( tempToken ) && isString == false && isLiteral == false && symbol.contain( tempToken, num ) ) { // Backword Reference
				  t.tokenType.push_back(5) ;
			  	  t.tokenValue.push_back(num) ;
				} // if()
			
			  	else if( isInt( tempToken ) && isString == false && isLiteral == false ){ // int
			  	  if( integer.contain( tempToken, num ) ){
			  	  	t.tokenType.push_back(6) ;
			  	    t.tokenValue.push_back(num) ;
				  } // if()
				  
				  else {
				  	t.tokenType.push_back(6) ;
			  	    t.tokenValue.push_back(integer.hashFunction(tempToken)) ;
				  } // else ()
				} // else if()
			
				else if( isString == true && isLiteral == false ){ // string 
				  if( start - 2 >= 0 ) {
				    if( line[start-2] == 'C' || line[start-2] == 'c' ) {
				  	  t.tokenType.push_back(7) ;
			  	      t.tokenValue.push_back(str.hashFunction(tempToken)) ;
			  	    } // if()
		  	    
			  	    else if( line[start-2] == 'X' || line[start-2] == 'x' ) {
			  	      if( integer.contain( tempToken, num ) ){
			  	  	    t.tokenType.push_back(6) ;
			  	        t.tokenValue.push_back(num) ;
				      } // if()
				  
				      else {
				  	    t.tokenType.push_back(6) ;
			  	        t.tokenValue.push_back(integer.hashFunction(tempToken)) ;
				      } // else ()
					} // esle if()
				  } // if()
				} // else if()
			
				else if( isLiteral == true && isString == false ){ // 還有 8(literal) =6666 (有空白)
				  if( literal.contain( tempToken, num )){
				  	t.tokenType.push_back(8) ;
		 	 	    t.tokenValue.push_back(num) ;
				  } // if()
				  
				  else {
				  	t.tokenType.push_back(8) ;
		 	 	    t.tokenValue.push_back(literal.hashFunction(tempToken)) ;	
				  } // else 
				} // else 
			
				else {
				  t.tokenType.push_back(0) ;
			  	  t.tokenValue.push_back(0) ;
				} // else 
			  } // else  
	   		} // if ()
	    
			if( contain( delimiter, temp, num ) ) {
	   	 	  t.token.push_back(temp) ; 
	   	 	  t.tokenType.push_back(4) ;
		      t.tokenValue.push_back(num) ;
	  		} // if
		
		    if( j + 1 <= line.size()) {
		      start = j + 1 ;
		      end = start ;
		    } // if 
	      } // if
	  	  
	      else end = j ;
  	    } // for() 
  	    
  	    if( findError == true ) lineBytes = 0 ;
	    
	    else { // findError == false 
	     if( t.token.size() > 0 ) { // 算每道指令的長度  => 算address!!! 
          if( t.tokenType[0] == 5 ){ // 第一個是symbol 
            if( t.token.size() == 2 ){
              if( t.tokenType[1] == 1 && SIC_Can_Use( t.token[1] ) == true ){ // labol instruction 沒有 operand 
          	    lineBytes = 3 ;
		      } // if
		      
		      else if( t.token[1] == "LTORG" || t.token[1] == "ltorg" ) { // labol LTORG 
		        isLTORG = true ; 
		        lineBytes = 0 ;
			  } // else 
			  
			  else if( t.token[1] == "END" || t.token[1] == "end" ){ // labol END
		        isEND = true ; 
		        lineBytes = 0 ;
			  } // else
			  
			  else { // ERROR
			  	findError = true ;
			  	lineBytes = 0 ;
			  } // else
	  	    } // if()
        
      	    else if( t.token.size() == 3 ){
      	      if( ( t.token[1] == "RESW" || t.token[1] == "resw" ) && t.tokenType[2] == 6 ){ // label pseudo(RESW) 立即值(dec) 
      	  	    lineBytes = stoi(t.token[2]) * 3 ;
		      } // if()
		  
		      else if( ( t.token[1] == "RESB" || t.token[1] == "resb" ) && t.tokenType[2] == 6 ){ // label pseudo(RESB) 立即值(dec) 
		  	    lineBytes = stoi(t.token[2]) ;
		      } // else if()
		  
		      else if( ( t.token[1] == "WORD" || t.token[1] == "word" ) && t.tokenType[2] == 6 ){ // label pseudo(WORD) 立即值(dec) 
		  	    lineBytes = 3 ;
		      } // else if()
		      
		      else if( ( t.token[1] == "BYTE" || t.token[1] == "byte" ) && t.tokenType[2] == 6 ){ // label pseudo(BYTE) 立即值(dec) 
		  	    lineBytes = stoi(t.token[2]) ;
		      } // else if()
		  
		      else if( ( t.token[1] == "EQU" || t.token[1] == "equ" ) && ( t.tokenType[2] == 5 || t.tokenType[2] == 6 || t.tokenType[2] == 0 ) ){  // label pseudo(EQU) symbol or 立即值
		  	    isEQU = true ;	    
			    if( t.tokenType[2] == 6 ) { // labol EQU 3 
			      t.startAddress = stoi(t.token[2]) ; // str -> int
			    } // if()
			    
			    else if( t.tokenType[2] == 0 ){
			      t.startAddress = hexToDec( "0000" ) ;
				} // else if()
			
			    else t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) ; // labol EQU labol(backword refer)
		      } // else if()
		  
		      else if( ( t.token[1] == "START" || t.token[1] == "start" ) ){ // 處理了 labol START 1000 but labol START F1 ????
		  	    t.startAddress = hexToDec( t.token[2].c_str() ) ; 
		  	    lineBytes = 0 ;
		      } // else if()
		  
		      else if( ( t.token[1] == "END" || t.token[1] == "end" ) && ( t.tokenType[2] == 5 || t.tokenType[2] == 0 ) ) { // labol pseudo(END) labol
		      	isEND = true ; 
		      	lineBytes = 0 ;
			  } // else if()
			  
			  /*
			  else if( ( t.token[1] == "BASE" || t.token[1] == "base" ) && ( t.tokenType[2] == 5 || t.tokenType[2] == 6 || t.tokenType[2] == 0 ) ) {
			  	haveBase = true ;
			    lineBytes = 0 ;
			  } // else if
			  */
			  
			  else if( t.tokenType[1] == 1 && SIC_Can_Use(t.token[1]) == true && ( t.tokenType[2] == 5 || t.tokenType[2] == 0 ) ){ // labol instruction 有 1個 operand 
			  	lineBytes = 3 ;
		      } // else if()
		
			  else { // ERROR
			  	findError = true ;
			  	lineBytes = 0 ;
			  } // else 
		    } // else if()
		
		    else if( t.token.size() == 5 ){
		      if( t.tokenType[1] == 1 && SIC_Can_Use( t.token[1] ) && t.token[3] == "," && t.tokenType[4] == 3 &&
			      ( t.tokenType[2] == 5 || t.tokenType[2] == 0 ) && ( t.token[4] == "X" || t.token[4] == "x" ) ) { // labol instruction 有 2個 operand 
        
			  	lineBytes = 3 ;
		      } // if()
		      
		      else if( ( t.token[1] == "EQU" || t.token[1] == "equ" ) && t.tokenType[2] == 5 && t.tokenType[3] == 4 && t.tokenType[4] == 5 ) {
		      	if( t.token[3] == "+" ) t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) + hexToDec( symbol.hashTable[t.tokenValue[4]].address ) ; // str -> int
			    else if( t.token[3] == "-" ) t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) - hexToDec( symbol.hashTable[t.tokenValue[4]].address ) ; // str -> int
			    else if( t.token[3] == "*" ) t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) * hexToDec( symbol.hashTable[t.tokenValue[4]].address ) ; // str -> int
			    else if( t.token[3] == "/" ) t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) / hexToDec( symbol.hashTable[t.tokenValue[4]].address ) ; // str -> int
			    else { // ERROR
			      findError = true ;
			  	  lineBytes = 0 ;
				} // else
			  } // else if()
			  
		      else { // ERROR
			  	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		
		    else if( t.token.size() == 6 ){
		      if( ( t.token[1] == "BYTE" || t.token[1] == "byte" ) && t.token[3] == "\'" && t.token[5] == "\'" && ( t.tokenType[4] == 6 || t.tokenType[4] == 7 ) ){ // labol pseudo(BYTE) 立即值 or string 
		  	    if( t.token[2] == "X" || t.token[2] == "x" ) lineBytes = t.token[4].size() / 2 ;
		  	    else if( t.token[2] == "C" || t.token[2] == "c" ) lineBytes = t.token[4].size() ;
		  	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()

			  else if( ( t.token[1] == "WORD" || t.token[1] == "word" ) && t.token[3] == "\'" && t.token[5] == "\'" && ( t.tokenType[4] == 6 || t.tokenType[4] == 7 ) ){ // labol pseudo(BYTE) 立即值 or string 
		  	    lineBytes = 3 ; 
		      } // else if()
		      
		      else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		    
			else {
			  findError = true ;
			  lineBytes = 0 ;
			} // else 
	      } // if()
	  
	      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	               
	      else if( t.tokenType[0] == 1 || t.tokenType[0] == 2 ){ // 第一個是instruction or pseudo 
	        if( t.token.size() == 1 ){ 
	          if( t.tokenType[0] == 1 && SIC_Can_Use( t.token[0] ) ) { // instruction 沒有 operand 
			  	lineBytes = 3 ;
			  } // if()
			  
			  else if( t.tokenType[0] == 2 && ( t.token[0] == "LTORG" || t.token[0] == "LTORG" ) ){
			  	isLTORG = true ;
			  	lineBytes = 0 ;
			  } // else if()

			  else if( t.tokenType[0] == 2 && ( t.token[0] == "END" || t.token[0] == "end" ) ){
			  	isEND = true ;
			  	lineBytes = 0 ;
			  } // else if()
			  
			  else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // if()
        
      	    else if( t.token.size() == 2 ){
      	      if( ( t.token[0] == "RESW" || t.token[0] == "resw" ) && t.tokenType[1] == 6 ){ // pseudo(RESW) 立即值(dec)  
      	  	    lineBytes = stoi(t.token[1]) * 3 ;
		      } // if()
		  
		      else if( ( t.token[0] == "RESB" || t.token[0] == "resb" ) && t.tokenType[1] == 6 ){ // pseudo(RESB) 立即值(dec)  
		  	    lineBytes = stoi(t.token[1]) ;
		      } // else if()
		  
		      else if( ( t.token[0] == "WORD" || t.token[0] == "word" ) && t.tokenType[1] == 6 ){ // pseudo(WORD) 立即值(dec) 
		  	    lineBytes = 3 ;
		      } // else if()
		      
		      else if( ( t.token[0] == "BYTE" || t.token[0] == "byte" ) && t.tokenType[1] == 6 ){ // pseudo(BYTE) 立即值(dec) 
		  	    lineBytes = stoi(t.token[1]) ;
		      } // else if()
		    
		      else if( ( t.token[0] == "START" || t.token[0] == "start" ) ){ // 處理了  START 1000 but  START F1 ????
		  	    t.startAddress = hexToDec( t.token[1].c_str() ) ; 
		  	    lineBytes = 0 ;
		      } // else if()
		    
		      else if( ( t.token[0] == "END" || t.token[0] == "end" ) && ( t.tokenType[1] == 5 || t.tokenType[1] == 0 )) { // pseudo(END) labol
		      	isEND = true ; 
		      	lineBytes = 0 ;
			  } // else if()
			  
			  /*
			  else if( ( t.token[0] == "BASE" || t.token[0] == "base" ) && ( t.tokenType[1] == 5 || t.tokenType[1] == 6 || t.tokenType[1] == 0 ) ) {
			    haveBase = true ;
			    lineBytes = 0 ;
			  } // else if
			  */
			  
			  else if( t.tokenType[0] == 1 && SIC_Can_Use(t.token[0]) == true && ( t.tokenType[1] == 5 || t.tokenType[1] == 0 ) ){ // labol instruction 有 1個 operand 
			  	lineBytes = 3 ;
		      } // else if()
		      
		      else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		
		    else if( t.token.size() == 4 ){
		      if( t.tokenType[0] == 1 && SIC_Can_Use( t.token[0] ) && t.token[2] == "," && t.tokenType[3] == 3 &&
			      ( t.tokenType[1] == 5 || t.tokenType[1] == 0 ) && ( t.token[3] == "X" || t.token[3] == "x" ) ) { // labol instruction 有 2個 operand 
        
			  	lineBytes = 3 ;
		      } // if()	
		    	
		      else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		
		    else if( t.token.size() == 5 ){
			  if( ( t.token[0] == "BYTE" || t.token[0] == "byte" ) && t.token[2] == "\'" && t.token[4] == "\'" && ( t.tokenType[3] == 6 || t.tokenType[3] == 7 ) ){ // labol pseudo(BYTE) 立即值 or string 
		  	    if( t.token[1] == "X" || t.token[1] == "x" ) lineBytes = t.token[3].size() / 2 ;
		  	    else if( t.token[1] == "C" || t.token[1] == "c" ) lineBytes = t.token[3].size() ;
		  	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // if()
		      
			  else if( ( t.token[0] == "WORD" || t.token[0] == "word" ) && t.token[2] == "\'" && t.token[4] == "\'" && ( t.tokenType[3] == 6 || t.tokenType[3] == 7 ) ){ // labol pseudo(BYTE) 立即值 or string 
		  	    lineBytes = 3 ; 
		      } // else if()
		      		      
			  else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()

			else {
			  findError = true ;
			  lineBytes = 0 ; 
			} // else 
	      } // else if()
	     } // if()
        } // else() 
	    
		if( findError == false && tokenArray.size() == 0 && t.token.size() != 0 ) {
	      t.length = lineBytes ;
	      t.start = decToHex_addr(t.startAddress) ;
	      tokenArray.push_back(t) ;	
	    } // if()
	
	    else if( t.token.size() != 0 && findError == false ){
	      if( t.token[0] == "." ) t.length = 0 ; 
	      else t.length = lineBytes ;
	  
	      if( isEQU == true ){
	        numEQU = tokenArray.size()+1 ; 
	      } // if()
	
	      else if( numEQU == tokenArray.size() ){ 
			for( int i = tokenArray.size()-2 ; i >= 0 ; i-- ){
	          if( tokenArray[i].token.size() >= 1 ) {
	          	if( tokenArray[i].token[1] != "EQU" && tokenArray[i].token[1] != "equ" ){
				  t.startAddress = tokenArray[i].length + tokenArray[i].startAddress ;
				  break ;
				} // if()
			  } // if()
			} // for()
          } // else if()
          
  	      else t.startAddress = tokenArray[tokenArray.size()-1].length + tokenArray[tokenArray.size()-1].startAddress ;
	  
	      t.start = decToHex_addr(t.startAddress) ;
	      tokenArray.push_back(t) ;	
  	    } // else if()
  	    
  	    else if( findError == true ){
  	      t.token.clear() ;
  	      t.tokenType.clear() ;
  	      t.tokenValue.clear() ;
  	      t.token.push_back("ERROR!!!") ;
  	      t.tokenType.push_back(-9999) ;
  	      t.tokenValue.push_back(-9999) ;
  	      t.length = 0 ;
  	      
  	      if( numEQU == tokenArray.size() ){  // 連續EQU
			for( int i = tokenArray.size()-2 ; i >= 0 ; i-- ){
	          if( tokenArray[i].token.size() >= 1 ) {
	          	if( tokenArray[i].token[1] != "EQU" && tokenArray[i].token[1] != "equ" ){
				  t.startAddress = tokenArray[i].length + tokenArray[i].startAddress ;
				  break ;
				} // if()
			  } // if()
			} // for()
          } // else if()
          
  	      else t.startAddress = tokenArray[tokenArray.size()-1].length + tokenArray[tokenArray.size()-1].startAddress ;
  	      
		  t.start = decToHex_addr(t.startAddress) ;
	      tokenArray.push_back(t) ;	
		} // else if()
	
	    if( t.tokenType[0] == 5 && symbol.contain( t.token[0], num ) ) {
		  if( symbol.hashTable[num].address == "" ) symbol.hashTable[num].address = t.start ;
	    } // if()
      } // for()

	  for( int i = 0 ; i < tokenArray.size() ; i++ ){ // 將forword reference的值給定義 
      	for( int j = 0 ; j < tokenArray[i].token.size() ; j++ ){
      	  if( tokenArray[i].token[j] == "." || tokenArray[i].token[j] == "END" || tokenArray[i].token[j] == "end" ){
      	  	break ;
		  } // if()
		  
      	  else if( tokenArray[i].tokenType[j] == 0 && tokenArray[i].tokenValue[j] == 0 ){
      	  	int num ;
      	  	
      	  	if( symbol.contain( tokenArray[i].token[j], num )){
      	  	  tokenArray[i].tokenType[j] = 5 ;
      	  	  tokenArray[i].tokenValue[j] = num ;
			} // if() 
			
			else {
	     	  tokenArray[i].token.clear() ;
              tokenArray[i].tokenType.clear() ;
  	          tokenArray[i].tokenValue.clear() ;
  	          tokenArray[i].token.push_back("ERROR!!!") ;
  	          tokenArray[i].tokenType.push_back(-9999) ;
  	          tokenArray[i].tokenValue.push_back(-9999) ;
			  undefindSymbol.push_back(i) ;
			} // else()
		  } // else if()
		} // for()
	  } // for()
	  
	  for( int i = 0 ; i < undefindSymbol.size() ; i++ ){
	  	for( int j = undefindSymbol[i]+1 ; j < tokenArray.size() ; j++ ){
	  	  tokenArray[j].startAddress = tokenArray[j].startAddress - tokenArray[undefindSymbol[i]].length ;
	  	  tokenArray[j].start = decToHex_addr( tokenArray[j].startAddress ) ;
		} // for()
	  } // for() 
      
      bool meetEND = false ;
      for( int i = 0 ; i < tokenArray.size() ; i++ ) { // PASS 2

        MachineCode m ;
  	    
  	    if( meetEND == true && tokenArray[i].tokenType[0] != 8 ) break ;
  	    
        m.line = (i+1) * 5 ;
  	    if( tokenArray[i].token[0] != "ERROR!!!" && tokenArray[i].token[0] != "." ) m.loc = tokenArray[i].start ;
  	    else m.loc = "    " ;
  	    
  	    if( i < inputArray.size() && meetEND == false && tokenArray[i].token[0] != "ERROR!!!" ) m.sourceStatement = inputArray[i] ;
  	    else if( tokenArray[i].token[0] == "ERROR!!!" ) m.sourceStatement = tokenArray[i].token[0] ;
  	    else m.sourceStatement = "" ;
  	
  	    for( int j = 0 ; j < tokenArray[i].token.size() ; j++ ){
  	      if( ( tokenArray[i].tokenType[j] == 2 && tokenArray[i].token[j] != "WORD" && tokenArray[i].token[j] != "word" &&
		        tokenArray[i].token[j] != "BYTE" && tokenArray[i].token[j] != "byte" ) || tokenArray[i].token[j] == "." ){ // pseudo(除了WORD跟BYTE) or (.)就不翻 
  		    
			if( tokenArray[i].token[j] == "END" || tokenArray[i].token[j] == "end" ) {
			  meetEND = true ;
			  m.loc = "    " ;
			} // if()

            // BASE !!!!!!			
            // LTORG !!!!  
			
			m.objectCode = "" ;
  		    break ;
	      } // if
	      
	      else if( tokenArray[i].tokenType[j] == 2 && ( tokenArray[i].token[j] == "WORD" || tokenArray[i].token[j] == "word" )) {
	      	if( tokenArray[i].tokenType[j+1] == 6 ){
	      	  m.objectCode = decToHex( atoi( tokenArray[i].token[j+1].c_str())) ;
	      	  if( m.objectCode.size() == 1 ) m.objectCode = "00000" + m.objectCode ;
	      	  else if( m.objectCode.size() == 2 ) m.objectCode = "0000" + m.objectCode ;
	      	  else if( m.objectCode.size() == 3 ) m.objectCode = "000" + m.objectCode ;
	      	  else if( m.objectCode.size() == 4 ) m.objectCode = "00" + m.objectCode ;
	      	  else if( m.objectCode.size() == 5 ) m.objectCode = "0" + m.objectCode ; 
  		      break ;
			} // if()
		  } // else if()
		  
		  else if( tokenArray[i].tokenType[j] == 2 && ( tokenArray[i].token[j] == "BYTE" || tokenArray[i].token[j] == "byte" )) {
	      	if( tokenArray[i].token[j+1] == "X" || tokenArray[i].token[j+1] == "x" ){
	      	  m.objectCode = tokenArray[i].token[j+3] ;
  		      break ;
			} // if()
			  
			else if( tokenArray[i].token[j+1] == "C" || tokenArray[i].token[j+1] == "c" ){
			  for( int y = 0 ; y < tokenArray[i].token[j+3].size() ; y++ ){
			  	m.objectCode = m.objectCode + decToHex( (int)tokenArray[i].token[j+3][y] ) ;
			  } // for()
			   
  		      break ;
			} // else if()
		  } // else if()
		  
		  else if( tokenArray[i].tokenType[j] == 1 && SIC_Can_Use(tokenArray[i].token[j]) ){ // 是 instruction  
	  	    string temp, address ;
		    temp = opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1]  ;
			
	        if( j + 1 == tokenArray[i].token.size()-1 ){
	  	      if( tokenArray[i].tokenType[j+1] == 5 ){
	  	      	m.objectCode = temp + symbol.hashTable[tokenArray[i].tokenValue[j+1]].address ;
			  } // if()
			} // if()
			
			else if( j + 3 == tokenArray[i].token.size()-1 ){ 
			  if( tokenArray[i].tokenType[j+1] == 5 && ( tokenArray[i].token[j+3] == "X" || tokenArray[i].token[j+3] == "x" )) {
			    address = "1" + convertHexToBin( symbol.hashTable[tokenArray[i].tokenValue[j+1]].address ).substr(1,15) ;
			    m.objectCode = temp + convertBinToHex( address ) ;
			  } // if()
			} // else if()
			
			else{
	  	      address = "0000000000000000" ;
	  	      m.objectCode = temp + convertBinToHex( address ) ;
		    } // else
 
	  	    break ;
	   	  } // else if() 
	    } // for()
	    
	    machineCode.push_back(m) ;
      } // for() 
	  
	  writeFile( temppp ) ;
	} // else if()
	
	else if( choice == 2 ){ // SIC/XE
	  ifstream inputFile ;
      cout << "請輸入檔名: " ;
      string temppp ;
      cin >> temppp ;
      inputFile.open( temppp + "_input.txt") ;

      while( getline( inputFile, tempLine ) ) if( tempLine != "" ) inputArray.push_back(tempLine) ; 
      inputFile.close();
      vector <int> undefindSymbol ;
      
      for( int i = 0 ; i < inputArray.size() ; i++ ) { // PASS 1 
        
        bool findError = false ;
  	    string line = inputArray[i] ;
  	    bool isString = false, isLiteral = false, isEQU = false, isLTORG = false, isEND = false ; 
  	    Token t ;
  	    int start = 0, end = 0 ; // token的頭跟尾 
  	    int num = 0 ; // 回傳在table中的位置
	    int lineBytes = 0 ; 
	
  	    for( int j = 0 ; j <= line.size() ; j++ ) {
  	      string temp = "" ;
  	      temp.append(1, line[j]);
  	  
  	      if( end == line.size()-1 || line[j] == ' ' || line[j] == '\t' || contain( delimiter, temp, num ) ) {
  	  	    string tempToken = "" ;
		    for( int y = start ; line[start] != ' ' && line[start] != '\t' && y <= end ; y++ ) tempToken = tempToken + line[y] ;
		    if( line[start] != ' ' && line[start] != '\t' && !contain( delimiter, tempToken, num ) ) {
		      t.token.push_back(tempToken) ;
		  
		      if( t.token.size() >= 2 ) { // isLitreal
		  	    if( t.token[t.token.size()-2] == "=" ) { // + instruction =6666 or instruction =6666
			      isLiteral = true ;
		        } // if()
		      }// if()
		  
		      if( t.token.size() >= 3 ) { // isLitreal or string 
		        if( t.token[t.token.size()-2] == "'" && temp == "'" && t.token[t.token.size()-4] == "=" ) { // (labol) (+) instruction =C'EOF'
		          isLiteral = true ;
			    } // if()
			    
			    else if( t.token[t.token.size()-2] == "'" && temp == "'" && t.token[t.token.size()-4] != "=" ){ // (labol) BYTE X'EOF'
			      isString = true ;
				} // else if()
		      } // if() 
		      
		      if( ( tempToken == "c" || tempToken == "C" || tempToken == "x" || tempToken == "X" ) && temp == "'" ){
		      	t.tokenType.push_back(9) ;
		      	t.tokenValue.push_back(0) ;
			  } // if() 
		  
		      else if( contain( instruction, tempToken, num ) && isString == false && isLiteral == false ) {
		  	    t.tokenType.push_back(1) ;
		  	    t.tokenValue.push_back(num) ;
		      } // else if()
		  
			  else if( contain( pseudo, tempToken, num ) && isString == false && isLiteral == false ) {
			  	t.tokenType.push_back(2) ;
			  	t.tokenValue.push_back(num) ;
			  } // else if()
		  
			  else if( contain( reg, tempToken, num ) && isString == false && isLiteral == false ){
			  	t.tokenType.push_back(3) ;
			  	t.tokenValue.push_back(num) ;
			  } // else if()
		  
			  else {
			  	if( t.token.size() == 1 && !isInt( tempToken ) && isString == false && isLiteral == false ) { // symbol
			  	  int num = symbol.hashFunction(tempToken) ;
			  	  
			  	  if( num == -9999 ) { // 重複定義 
			  	  	findError = true ;
				  } // if()
				  
				  else {
			  	    t.tokenType.push_back(5) ;
			  	    t.tokenValue.push_back(num) ;
			      } // else 
				} // if()
			
				else if( t.token.size() > 1 && !isInt( tempToken ) && isString == false && isLiteral == false && symbol.contain( tempToken, num ) ) { // Backword Reference
				  t.tokenType.push_back(5) ;
			  	  t.tokenValue.push_back(num) ;
				} // if()
			
			  	else if( isInt( tempToken ) && isString == false && isLiteral == false ){ // int
			  	  if( integer.contain( tempToken, num ) ){
			  	  	t.tokenType.push_back(6) ;
			  	    t.tokenValue.push_back(num) ;
				  } // if()
				  
				  else {
				  	t.tokenType.push_back(6) ;
			  	    t.tokenValue.push_back(integer.hashFunction(tempToken)) ;
				  } // else ()
				} // else if()
			
				else if( isString == true && isLiteral == false ){ // string 
				  if( start - 2 >= 0 ) {
				    if( line[start-2] == 'C' || line[start-2] == 'c' ) {
				  	  t.tokenType.push_back(7) ;
			  	      t.tokenValue.push_back(str.hashFunction(tempToken)) ;
			  	    } // if()
		  	    
			  	    else if( line[start-2] == 'X' || line[start-2] == 'x' ) {
			  	      if( integer.contain( tempToken, num ) ){
			  	  	    t.tokenType.push_back(6) ;
			  	        t.tokenValue.push_back(num) ;
				      } // if()
				  
				      else {
				  	    t.tokenType.push_back(6) ;
			  	        t.tokenValue.push_back(integer.hashFunction(tempToken)) ;
				      } // else ()
					} // esle if()
				  } // if()
				} // else if()
			
				else if( isLiteral == true && isString == false ){ // 還有 8(literal) = 6666 (有空白)
				  if( literal.contain( tempToken, num )){
				  	t.tokenType.push_back(8) ;
		 	 	    t.tokenValue.push_back(num) ;
				  } // if()
				  
				  else {
				  	t.tokenType.push_back(8) ;
		 	 	    t.tokenValue.push_back(literal.hashFunction(tempToken)) ;	
				  } // else 
				} // else 
			
				else {
				  t.tokenType.push_back(0) ;
			  	  t.tokenValue.push_back(0) ;
				} // else 
			  } // else  
	   		} // if ()
	    
			if( contain( delimiter, temp, num ) ) {
	   	 	  t.token.push_back(temp) ; 
	   	 	  t.tokenType.push_back(4) ;
		      t.tokenValue.push_back(num) ;
	  		} // if
		
		    if( j + 1 <= line.size()) {
		      start = j + 1 ;
		      end = start ;
		    } // if 
	      } // if
	  	  
	      else end = j ;
  	    } // for()
	
	    if( findError == true ) lineBytes = 0 ;
	    
	    else { // findError == false 
	     if( t.token.size() > 0 ) { // 算每道指令的長度  => 算address!!! 
          if( t.tokenType[0] == 5 ){ // 第一個是symbol 
            if( t.token.size() == 2 ){
              if( t.tokenType[1] == 1 ){ // labol instruction 沒有 operand 
          	    if( searchOpCodeFormat(t.token[1]) == "1" ) lineBytes = 1 ;  // format 1
          	    else if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 3 ;  // format 3
          	    else {
		      	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // if
		      
		      else if( t.token[1] == "LTORG" || t.token[1] == "ltorg" ) { // labol LTORG 
		        isLTORG = true ; 
		        lineBytes = 0 ;
			  } // else 
			  
			  else if( t.token[1] == "END" || t.token[1] == "end" ){ // labol END
		        isEND = true ; 
		        lineBytes = 0 ;
			  } // else
			  
			  else { // ERROR
			  	findError = true ;
			  	lineBytes = 0 ;
			  } // else
	  	    } // if()
        
      	    else if( t.token.size() == 3 ){
      	      if( ( t.token[1] == "RESW" || t.token[1] == "resw" ) && t.tokenType[2] == 6 ){ // label pseudo(RESW) 立即值(dec) 
      	  	    lineBytes = stoi(t.token[2]) * 3 ;
		      } // if()
		  
		      else if( ( t.token[1] == "RESB" || t.token[1] == "resb" ) && t.tokenType[2] == 6 ){ // label pseudo(RESB) 立即值(dec) 
		  	    lineBytes = stoi(t.token[2]) ;
		      } // else if()
		  
		      else if( ( t.token[1] == "WORD" || t.token[1] == "word" ) && t.tokenType[2] == 6 ){ // label pseudo(WORD) 立即值(dec) 
		  	    lineBytes = 3 ;
		      } // else if()
		      
		      else if( ( t.token[1] == "BYTE" || t.token[1] == "byte" ) && t.tokenType[2] == 6 ){ // label pseudo(BYTE) 立即值(dec) 
		  	    if(  t.token[2].size() % 2 != 0 ) lineBytes = t.token[2].size() / 2 +1 ;
				else lineBytes = t.token[2].size() / 2 ;
		      } // else if()
		  
		      else if( ( t.token[1] == "EQU" || t.token[1] == "equ" ) && ( t.tokenType[2] == 5 || t.tokenType[2] == 6 || t.tokenType[2] == 0 ) ){  // label pseudo(EQU) symbol or 立即值
		  	    isEQU = true ;	    
			    if( t.tokenType[2] == 6 ) { // labol EQU 3 
			      t.startAddress = stoi(t.token[2]) ; // str -> int
			    } // if()
			    
			    else if( t.tokenType[2] == 0 ){
			      t.startAddress = hexToDec( "0000" ) ;
				} // else if()
			
			    else t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) ; // labol EQU labol(backword refer)
		      } // else if()
		  
		      else if( ( t.token[1] == "START" || t.token[1] == "start" ) ){ // 處理了 labol START 1000 but labol START F1 ????
		  	    t.startAddress = hexToDec( t.token[2].c_str() ) ; 
		  	    lineBytes = 0 ;
		      } // else if()
		  
		      else if( ( t.token[1] == "END" || t.token[1] == "end" ) && ( t.tokenType[2] == 5 || t.tokenType[2] == 0 ) ) { // labol pseudo(END) labol
		      	isEND = true ; 
		      	lineBytes = 0 ;
			  } // else if()
			  
			  else if( ( t.token[1] == "BASE" || t.token[1] == "base" ) && ( t.tokenType[2] == 5 || t.tokenType[2] == 6 || t.tokenType[2] == 0 ) ) {
			  	haveBase = true ;
			    lineBytes = 0 ;
			  } // else if
			  
			  else if( t.tokenType[1] == 1 && ( (( t.token[1] == "TIXR" || t.token[1] == "tixr" || t.token[1] == "clear" || t.token[1] == "CLEAR" ) && t.tokenType[2] == 3 ) ||
			          (( t.token[1] == "SVC" || t.token[1] == "svc" ) && t.tokenType[2] == 6 && (( atoi(t.token[2].c_str()) >= 0 && atoi(t.token[2].c_str()) <= 6 ) || ( atoi(t.token[2].c_str()) >= 8 && atoi(t.token[2].c_str()) <= 9 ))) ||
					  (( t.tokenType[2] == 5 || t.tokenType[2] == 0 || t.tokenType[2] == 6 ) && t.token[1] != "TIXR" && t.token[1] != "tixr" && 
					     t.token[1] != "clear" && t.token[1] != "CLEAR" && t.token[1] != "SVC" && t.token[1] != "svc" ))){ // labol instruction 有 1個 operand 

          	    if( searchOpCodeFormat(t.token[1]) == "2" ) lineBytes = 2 ; // format 2
          	    else if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		  
		      else if( t.token[1] == "+" && t.tokenType[2] == 1 ){ // labol (+)instruction 沒有 operand 
		  	    if( searchOpCodeFormat(t.token[2]) == "3/4" ) lineBytes = 4 ;  // format 4
		  	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
		      else if( t.tokenType[0] == 1 && t.token[1] == "=" && literal.contain( t.token[2], num ) ){ // instruction =6666
          	    if( searchOpCodeFormat(t.token[0]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
			  } // else if()
		      
			  else { // ERROR
			  	findError = true ;
			  	lineBytes = 0 ;
			  } // else 
		    } // else if()
		
		    else if( t.token.size() == 4 ){		  
		      if( t.token[1] == "+" && t.tokenType[2] == 1 && ( t.tokenType[3] == 5 || t.tokenType[3] == 0 || t.tokenType[3] == 6 ) ){ // labol (+)instruction 有 1個 operand 
		  	    if( searchOpCodeFormat(t.token[2]) == "3/4" ) lineBytes = 4 ;  // format 4
		  	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // if()
		      
		      else if( t.token[2] == "@" && t.tokenType[1] == 1 && ( t.tokenType[3] == 5 || t.tokenType[3] == 0 ) ){ // labol instruction @有 1個 operand 
          	    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
		      else if( t.token[2] == "#" && t.tokenType[1] == 1 && ( t.tokenType[3] == 5 || t.tokenType[3] == 0 || t.tokenType[3] == 6 ) ){ // labol instruction #有 1個 operand 
          	    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
		      else if( t.tokenType[1] == 1 && t.token[2] == "=" && literal.contain( t.token[3], num ) ){ // labol instruction =6666
          	    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
			  } // else if()
		      
		      else { // ERROR
			  	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		
		    else if( t.token.size() == 5 ){
		      if( t.tokenType[1] == 1 && ( ( t.tokenType[2] == 3 && t.tokenType[4] == 3 && ( t.token[1] == "ADDR" || t.token[1] == "addr" || 
			      t.token[1] == "COMPR" || t.token[1] == "compr" || t.token[1] == "DIVR" || t.token[1] == "divr" || t.token[1] == "MULR" ||
				  t.token[1] == "mulr" || t.token[1] == "RMO" || t.token[1] == "rmo" || t.token[1] == "SUBR" || t.token[1] == "subr" )) ||
				  ( t.tokenType[2] == 3 && t.tokenType[4] == 6 && (( atoi(t.token[4].c_str()) >= 0 && atoi(t.token[4].c_str()) <= 6 ) || ( atoi(t.token[4].c_str()) >= 8 && atoi(t.token[4].c_str()) <= 9 )) &&
				  ( t.token[1] == "SHIFTL" || t.token[1] == "shiftl" || t.token[1] == "SHIFTR" || t.token[1] == "shiftr" )) ||
				  (( t.tokenType[2] == 5 || t.tokenType[2] == 0 ) && t.tokenType[4] == 3 && t.token[1] != "ADDR" && t.token[1] != "addr" && t.token[1] != "COMPR" && 
				     t.token[1] != "compr" && t.token[1] != "DIVR" && t.token[1] != "divr" && t.token[1] != "MULR" && t.token[1] != "mulr" && t.token[1] != "RMO" && 
					 t.token[1] != "rom" && t.token[1] != "SUBR" && t.token[1] != "subr" && t.token[1] != "SHIFTL" && t.token[1] != "shiftl" && 
					 t.token[1] != "SHIFTR" && t.token[1] != "shiftr" )) && t.token[3] == "," ) { // labol instruction 有 2個 operand 
          	
          	    if( searchOpCodeFormat(t.token[1]) == "2" ) lineBytes = 2 ; // format 2
          	    else if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 3 ; // format 3	
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // if()
		      
		      else if( t.token[1] == "+" && t.tokenType[2] == 1 && t.token[3] == "@" && ( t.tokenType[4] == 5 || t.tokenType[4] == 0 ) ){ // labol (+)instruction @有 1個 operand 
		  	    if( searchOpCodeFormat(t.token[2]) == "3/4" ) lineBytes = 4 ;  // format 4
		  	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // if()
		      
		      else if( t.token[1] == "+" && t.tokenType[2] == 1 && t.token[3] == "#" && ( t.tokenType[4] == 5 || t.tokenType[4] == 0 || t.tokenType[4] == 6 ) ){ // labol (+)instruction @有 1個 operand 
		  	    if( searchOpCodeFormat(t.token[2]) == "3/4" ) lineBytes = 4 ;  // format 4
		  	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // if()
		      
		      else if( ( t.token[1] == "EQU" || t.token[1] == "equ" ) && t.tokenType[2] == 5 && t.tokenType[3] == 4 && t.tokenType[4] == 5 ) {
		      	if( t.token[3] == "+" ) t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) + hexToDec( symbol.hashTable[t.tokenValue[4]].address ) ; // str -> int
			    else if( t.token[3] == "-" ) t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) - hexToDec( symbol.hashTable[t.tokenValue[4]].address ) ; // str -> int
			    else if( t.token[3] == "*" ) t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) * hexToDec( symbol.hashTable[t.tokenValue[4]].address ) ; // str -> int
			    else if( t.token[3] == "/" ) t.startAddress = hexToDec( symbol.hashTable[t.tokenValue[2]].address ) / hexToDec( symbol.hashTable[t.tokenValue[4]].address ) ; // str -> int
			    else { // ERROR
			      findError = true ;
			  	  lineBytes = 0 ;
				} // else
			  } // else if()
			  
			  else if( t.token[1] == "+" && t.tokenType[2] == 1 && t.token[3] == "=" && literal.contain( t.token[4], num ) ){ // labol +instruction =6666
          	    if( searchOpCodeFormat(t.token[2]) == "3/4" ) lineBytes = 4 ; // format 4
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
			  } // else if()
			  
		      else { // ERROR
			  	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		
		    else if( t.token.size() == 6 ){
		      if( t.token[1] == "+" && t.tokenType[2] == 1 && ( t.tokenType[3] == 5 || t.tokenType[3] == 0 ) && t.token[4] == "," && t.tokenType[5] == 3 ){ // labol (+)instruction 有 2個 operand 
			    if( searchOpCodeFormat(t.token[2]) == "3/4" ) lineBytes = 4 ;  // format 4
			    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // if()
		    
		      else if( ( t.token[1] == "BYTE" || t.token[1] == "byte" ) && t.token[3] == "\'" && t.token[5] == "\'" && ( t.tokenType[4] == 6 || t.tokenType[4] == 7 ) ){ // labol pseudo(BYTE) 立即值 or string 
		  	    if( t.token[2] == "X" || t.token[2] == "x" ) lineBytes = t.token[4].size() / 2 ;
		  	    else if( t.token[2] == "C" || t.token[2] == "c" ) lineBytes = t.token[4].size() ;
		  	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()

			  else if( ( t.token[1] == "WORD" || t.token[1] == "word" ) && t.token[3] == "\'" && t.token[5] == "\'" && ( t.tokenType[4] == 6 || t.tokenType[4] == 7 ) ){ // labol pseudo(BYTE) 立即值 or string 
		  	    lineBytes = 3 ; 
		      } // else if()
		      
		      else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		    
		    else if( t.token.size() == 7 ){ // labol instruction literal
		      int num ;
		      if( t.tokenType[1] == 1 && t.token[2] == "=" && t.token[4] == "\'" && t.token[6] == "\'" && literal.contain( t.token[5], num ) ){
		      	
		      	literal.hashTable[num].strOrDec = t.token[3] ;
          	    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
			  } // if()
			  
			  else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
			} // else if()
			
			else if( t.token.size() == 8 ){ // labol +instruction literal
			  int num ;
		      if( t.token[1] == "+" && t.tokenType[2] == 1 && t.token[3] == "=" && t.token[5] == "\'" && t.token[7] == "\'" && literal.contain( t.token[6], num ) ){
		      	
		      	literal.hashTable[num].strOrDec = t.token[4] ;
                if( searchOpCodeFormat(t.token[2]) == "3/4" ) lineBytes = 4 ;  // format 4
                else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
			  } // if()
			  
			  else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
			} // else if()
			
			else {
			  findError = true ;
			  lineBytes = 0 ;
			} // else 
	      } // if()
	  
	      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	  
	      else if( t.tokenType[0] == 1 || t.tokenType[0] == 2 || ( t.tokenType[0] == 4 && t.token[0] == "+" ) ){ // 第一個是instruction or pseudo or (+)instruction
	        if( t.token.size() == 1 ){ 
	          if( t.tokenType[0] == 1 ) { // instruction 沒有 operand 
                if( searchOpCodeFormat(t.token[0]) == "1" ) lineBytes = 1 ;  // format 1
                else if( searchOpCodeFormat(t.token[0]) == "3/4" ) lineBytes = 3 ; // format 3
                else {
		      	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
			  } // if()
			  
			  else if( t.tokenType[0] == 2 && ( t.token[0] == "LTORG" || t.token[0] == "LTORG" ) ){
			  	isLTORG = true ;
			  	lineBytes = 0 ;
			  } // else if()

			  else if( t.tokenType[0] == 2 && ( t.token[0] == "END" || t.token[0] == "end" ) ){
			  	isEND = true ;
			  	lineBytes = 0 ;
			  } // else if()
			  
			  else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // if()
        
      	    else if( t.token.size() == 2 ){
      	      if( ( t.token[0] == "RESW" || t.token[0] == "resw" ) && t.tokenType[1] == 6 ){ // pseudo(RESW) 立即值(dec)  
      	  	    lineBytes = stoi(t.token[1]) * 3 ;
		      } // if()
		  
		      else if( ( t.token[0] == "RESB" || t.token[0] == "resb" ) && t.tokenType[1] == 6 ){ // pseudo(RESB) 立即值(dec)  
		  	    lineBytes = stoi(t.token[1]) ;
		      } // else if()
		  
		      else if( ( t.token[0] == "WORD" || t.token[0] == "word" ) && t.tokenType[1] == 6 ){ // pseudo(WORD) 立即值(dec) 
		  	    lineBytes = 3 ;
		      } // else if()
		      
		      else if( ( t.token[0] == "BYTE" || t.token[0] == "byte" ) && t.tokenType[1] == 6 ){ // pseudo(BYTE) 立即值(dec) 
		  	    if(  t.token[1].size() % 2 != 0 ) lineBytes = t.token[1].size() / 2 +1 ;
				else lineBytes = t.token[1].size() / 2 ;
		      } // else if()
		    
		      else if( ( t.token[0] == "START" || t.token[0] == "start" ) ){ // 處理了  START 1000 but  START F1 ????
		  	    t.startAddress = hexToDec( t.token[1].c_str() ) ; 
		  	    lineBytes = 0 ;
		      } // else if()
		    
		      else if( ( t.token[0] == "END" || t.token[0] == "end" ) && ( t.tokenType[1] == 5 || t.tokenType[1] == 0 )) { // pseudo(END) labol
		      	isEND = true ; 
		      	lineBytes = 0 ;
			  } // else if()
			  
			  else if( ( t.token[0] == "BASE" || t.token[0] == "base" ) && ( t.tokenType[1] == 5 || t.tokenType[1] == 6 || t.tokenType[1] == 0 ) ) {
			    haveBase = true ;
			    lineBytes = 0 ;
			  } // else if
					     
			  else if( t.tokenType[0] == 1 && ((( t.token[0] == "TIXR" || t.token[0] == "tixr" || t.token[0] == "clear" || t.token[0] == "CLEAR" ) && t.tokenType[1] == 3 ) ||
			          (( t.token[0] == "SVC" || t.token[0] == "svc" ) && t.tokenType[1] == 6 && (( atoi(t.token[1].c_str()) >= 0 && atoi(t.token[1].c_str()) <= 6 ) || ( atoi(t.token[1].c_str()) >= 8 && atoi(t.token[1].c_str()) <= 9 ))) ||
					  (( t.tokenType[1] == 5 || t.tokenType[1] == 0 || t.tokenType[1] == 6 ) && t.token[0] != "TIXR" && t.token[0] != "tixr" && 
					     t.token[0] != "clear" && t.token[0] != "CLEAR" && t.token[0] != "SVC" && t.token[0] != "svc" ))){ // labol instruction 有 1個 operand 
					     
          	    if( searchOpCodeFormat(t.token[0]) == "2" ) lineBytes = 2 ; // format 2
          	    else if( searchOpCodeFormat(t.token[0]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
		      else if( t.token[0] == "+" && t.tokenType[1] == 1 ){ // (+)instruction 沒有 operand 
		  	    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 4 ;  // format 4
		  	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
		      else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		
		    else if( t.token.size() == 3 ){
		      if( t.token[0] == "+" && t.tokenType[1] == 1 && ( t.tokenType[2] == 5 || t.tokenType[2] == 0 || t.tokenType[2] == 6 ) ){ //  (+)instruction 有 1個 operand 
			    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 4 ;  // format 4
			    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // if()
		      
		      else if( t.tokenType[0] == 1 && t.token[1] == "@" && ( t.tokenType[2] == 5 || t.tokenType[2] == 0 ) ){ // instruction @有 1個 operand 
          	    if( searchOpCodeFormat(t.token[0]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
		      else if( t.tokenType[0] == 1 && t.token[1] == "#" && ( t.tokenType[2] == 5 || t.tokenType[2] == 0 || t.tokenType[2] == 6 ) ){ // instruction #有 1個 operand 
          	    if( searchOpCodeFormat(t.token[0]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
		      else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		
		    else if( t.token.size() == 4 ){
		      if( t.tokenType[0] == 1 && ( ( t.tokenType[1] == 3 && t.tokenType[3] == 3 && ( t.token[0] == "ADDR" || t.token[0] == "addr" || 
			      t.token[0] == "COMPR" || t.token[0] == "compr" || t.token[0] == "DIVR" || t.token[0] == "divr" || t.token[0] == "MULR" ||
				  t.token[0] == "mulr" || t.token[0] == "RMO" || t.token[0] == "rmo" || t.token[0] == "SUBR" || t.token[0] == "subr" )) ||
				  ( t.tokenType[1] == 3 && t.tokenType[3] == 6 && (( atoi(t.token[3].c_str()) >= 0 && atoi(t.token[3].c_str()) <= 6 ) || ( atoi(t.token[3].c_str()) >= 8 && atoi(t.token[3].c_str()) <= 9 )) &&
				  ( t.token[0] == "SHIFTL" || t.token[0] == "shiftl" || t.token[0] == "SHIFTR" || t.token[0] == "shiftr" )) ||
				  (( t.tokenType[1] == 5 || t.tokenType[1] == 0 ) && t.tokenType[3] == 3 && t.token[0] != "ADDR" && t.token[0] != "addr" && t.token[0] != "COMPR" && 
				     t.token[0] != "compr" && t.token[0] != "DIVR" && t.token[0] != "divr" && t.token[0] != "MULR" && t.token[0] != "mulr" && t.token[0] != "RMO" && 
					 t.token[0] != "rom" && t.token[0] != "SUBR" && t.token[0] != "subr" && t.token[0] != "SHIFTL" && t.token[0] != "shiftl" && 
					 t.token[0] != "SHIFTR" && t.token[0] != "shiftr" )) && t.token[2] == "," ) { //  instruction 有 2個 operand 
					 
          	    if( searchOpCodeFormat(t.token[0]) == "2" ) lineBytes = 2 ; // format 2
          	    else if( searchOpCodeFormat(t.token[0]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } //  if()
		      
		      else if( t.token[0] == "+" && t.tokenType[1] == 1 && t.token[2] == "@" && ( t.tokenType[3] == 5 || t.tokenType[3] == 0 )){ //  (+)instruction @有 1個 operand 
			    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 4 ;  // format 4
			    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
		      else if( t.token[0] == "+" && t.tokenType[1] == 1 && t.token[2] == "#" && ( t.tokenType[3] == 5 || t.tokenType[3] == 0 || t.tokenType[3] == 6 )){ //  (+)instruction #有 1個 operand 
			    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 4 ;  // format 4
			    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
		      else if( t.token[0] == "+" && t.tokenType[1] == 1 && t.token[2] == "=" && literal.contain( t.token[3], num ) ){ // +instruction =6666
          	    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 4 ; // format 4
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
			  } // else if()
		      
		      else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		
		    else if( t.token.size() == 5 ){
		      if( t.token[0] == "+" && t.tokenType[1] == 1 && ( t.tokenType[2] == 5 || t.tokenType[2] == 0 ) && t.token[3] == "," && t.tokenType[4] == 3 ){ // (+)instruction 有 2個 operand 
			    if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 4 ;  // format 4
			    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // if()
		      
			  else if( ( t.token[0] == "BYTE" || t.token[0] == "byte" ) && t.token[2] == "\'" && t.token[4] == "\'" && ( t.tokenType[3] == 6 || t.tokenType[3] == 7 ) ){ // labol pseudo(BYTE) 立即值 or string 
		  	    if( t.token[1] == "X" || t.token[1] == "x" ) lineBytes = t.token[3].size() / 2 ;
		  	    else if( t.token[1] == "C" || t.token[1] == "c" ) lineBytes = t.token[3].size() ;
		  	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
		      } // else if()
		      
			  else if( ( t.token[0] == "WORD" || t.token[0] == "word" ) && t.token[2] == "\'" && t.token[4] == "\'" && ( t.tokenType[3] == 6 || t.tokenType[3] == 7 ) ){ // labol pseudo(BYTE) 立即值 or string 
		  	    lineBytes = 3 ; 
		      } // else if()
		      		      
			  else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
		    } // else if()
		    
		    else if( t.token.size() == 6 ){ // instruction literal
		      int num ;
		      if( t.tokenType[0] == 1 && t.token[1] == "=" && t.token[3] == "\'" && t.token[5] == "\'" && literal.contain( t.token[4], num ) ){
		      	
		      	literal.hashTable[num].strOrDec = t.token[2] ;
          	    if( searchOpCodeFormat(t.token[0]) == "3/4" ) lineBytes = 3 ; // format 3
          	    else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
			  } // if()
			  
			  else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
			} // else if()
			
			else if( t.token.size() == 7 ){ // +instruction literal
			  int num ;
		      if( t.token[0] == "+" && t.tokenType[1] == 1 && t.token[2] == "=" && t.token[4] == "\'" && t.token[6] == "\'" && literal.contain( t.token[5], num ) ){
		      	
		      	literal.hashTable[num].strOrDec = t.token[3] ;
                if( searchOpCodeFormat(t.token[1]) == "3/4" ) lineBytes = 4 ;  // format 4
                else { // ERROR
			  	  findError = true ;
			  	  lineBytes = 0 ;
			    } // else
			  } // if()
			  
			  else {
		      	findError = true ;
			  	lineBytes = 0 ;
			  } // else
			} // else if()
			
			else {
			  findError = true ;
			  lineBytes = 0 ; 
			} // else 
	      } // if()
	     } // if()
        } // else() 
	    
		if( findError == false && tokenArray.size() == 0 && t.token.size() != 0 ) {
	      t.length = lineBytes ;
	      t.start = decToHex_addr(t.startAddress) ;
	      tokenArray.push_back(t) ;	
	    } // if()
	
	    else if( t.token.size() != 0 && findError == false ){
	      if( t.token[0] == "." ) t.length = 0 ; 
	      else t.length = lineBytes ;
	  
	      if( isEQU == true ){
	        numEQU = tokenArray.size()+1 ; 
	      } // if()
	
	      else if( numEQU == tokenArray.size() ){  // 連續EQU還沒處理   
			for( int i = tokenArray.size()-2 ; i >= 0 ; i-- ){
	          if( tokenArray[i].token.size() >= 1 ) {
	          	if( tokenArray[i].token[1] != "EQU" && tokenArray[i].token[1] != "equ" ){
				  t.startAddress = tokenArray[i].length + tokenArray[i].startAddress ;
				  break ;
				} // if()
			  } // if()
			} // for()
          } // else if()
          
  	      else t.startAddress = tokenArray[tokenArray.size()-1].length + tokenArray[tokenArray.size()-1].startAddress ;
	  
	      t.start = decToHex_addr(t.startAddress) ;
	      tokenArray.push_back(t) ;	
  	    } // else if()
  	    
  	    else if( findError == true ){
  	      t.token.clear() ;
  	      t.tokenType.clear() ;
  	      t.tokenValue.clear() ;
  	      t.token.push_back("ERROR!!!") ;
  	      t.tokenType.push_back(-9999) ;
  	      t.tokenValue.push_back(-9999) ;
  	      t.length = 0 ;
  	      
  	      if( numEQU == tokenArray.size() ){  // 連續EQU
			for( int i = tokenArray.size()-2 ; i >= 0 ; i-- ){
	          if( tokenArray[i].token.size() >= 1 ) {
	          	if( tokenArray[i].token[1] != "EQU" && tokenArray[i].token[1] != "equ" ){
				  t.startAddress = tokenArray[i].length + tokenArray[i].startAddress ;
				  break ;
				} // if()
			  } // if()
			} // for()
          } // else if()
          
  	      else t.startAddress = tokenArray[tokenArray.size()-1].length + tokenArray[tokenArray.size()-1].startAddress ;
  	      
		  t.start = decToHex_addr(t.startAddress) ;
	      tokenArray.push_back(t) ;	
		} // else if()
	
	    if( t.tokenType[0] == 5 && symbol.contain( t.token[0], num ) ) {
		  if( symbol.hashTable[num].address == "" ) symbol.hashTable[num].address = t.start ;
	    } // if()
	    
	    if( isLTORG == true || isEND == true ){
	      int num = 0 ;
	      for( int i = 0 ; i < literal.hashTable.size() ; i++ ){
	      	Token t1 ;
	      	if( literal.hashTable[i].thing != "" && literal.hashTable[i].address == "" ) {
	      	  num++ ;
	      	  if( literal.hashTable[i].strOrDec == "C" || literal.hashTable[i].strOrDec == "c" ) {
				t1.length = literal.hashTable[i].thing.size() ;
			    t1.token.push_back( literal.hashTable[i].thing ) ;
			    t1.tokenType.push_back(8) ;
				t1.tokenValue.push_back(i) ;
			  } // if() 
			  
	      	  else if( literal.hashTable[i].strOrDec == "X" || literal.hashTable[i].strOrDec == "x" ) { 
				t1.length = literal.hashTable[i].thing.size() / 2 ;
				t1.token.push_back( literal.hashTable[i].thing ) ;
				t1.tokenType.push_back(8) ;
				t1.tokenValue.push_back(i) ;
			  } // else
			  
			  else {
			  	t1.length = 3 ;
				t1.token.push_back( literal.hashTable[i].thing ) ;
				t1.tokenType.push_back(8) ;
				t1.tokenValue.push_back(i) ;
			  } // else 
	      	  
	      	  if( num == 1 ) t1.startAddress = tokenArray[tokenArray.size()-2].startAddress + tokenArray[tokenArray.size()-2].length ;
	      	  else t1.startAddress = tokenArray[tokenArray.size()-1].startAddress + tokenArray[tokenArray.size()-1].length ;

	      	  t1.start = decToHex_addr(t1.startAddress) ;
	      	  literal.hashTable[i].address = t1.start ;
	      	  tokenArray.push_back(t1) ;	
			} // if()
		  } // for()
		} // if()
      } // for()
      
      for( int i = 0 ; i < tokenArray.size() ; i++ ){ // 將forword reference的值給定義 
      	for( int j = 0 ; j < tokenArray[i].token.size() ; j++ ){
      	  if( tokenArray[i].token[j] == "." || tokenArray[i].token[j] == "END" || tokenArray[i].token[j] == "end" ){
      	  	break ;
		  } // if()
		  
      	  else if( tokenArray[i].tokenType[j] == 0 && tokenArray[i].tokenValue[j] == 0 ){
      	  	int num ;
      	  	
      	  	if( symbol.contain( tokenArray[i].token[j], num )){
      	  	  tokenArray[i].tokenType[j] = 5 ;
      	  	  tokenArray[i].tokenValue[j] = num ;
			} // if() 
			
			else {
	     	  tokenArray[i].token.clear() ;
              tokenArray[i].tokenType.clear() ;
  	          tokenArray[i].tokenValue.clear() ;
  	          tokenArray[i].token.push_back("ERROR!!!") ;
  	          tokenArray[i].tokenType.push_back(-9999) ;
  	          tokenArray[i].tokenValue.push_back(-9999) ;
			  undefindSymbol.push_back(i) ;
			} // else()
		  } // else if()
		} // for()
	  } // for()
	  
	  for( int i = 0 ; i < undefindSymbol.size() ; i++ ){
	  	for( int j = undefindSymbol[i]+1 ; j < tokenArray.size() ; j++ ){
	  	  tokenArray[j].startAddress = tokenArray[j].startAddress - tokenArray[undefindSymbol[i]].length ;
	  	  tokenArray[j].start = decToHex_addr( tokenArray[j].startAddress ) ;
		} // for()
	  } // for()
	  
	  bool meetEND = false ; 
	  for( int i = 0 ; i < tokenArray.size() ; i++ ) { // PASS 2
        int num1, num2 ;
        
        if( i + 1 < tokenArray.size() ) PC = tokenArray[i+1].startAddress ;
        MachineCode m ;
  	    
  	    if( meetEND == true && tokenArray[i].tokenType[0] != 8 ) break ;
  	    
        m.line = (i+1) * 5 ;
  	    if( tokenArray[i].token[0] != "ERROR!!!" && tokenArray[i].token[0] != "." ) m.loc = tokenArray[i].start ;
  	    else m.loc = "    " ;
  	    
  	    if( i < inputArray.size() && meetEND == false && tokenArray[i].token[0] != "ERROR!!!" ) m.sourceStatement = inputArray[i] ;
  	    else if( tokenArray[i].token[0] == "ERROR!!!" ) m.sourceStatement = tokenArray[i].token[0] ;
  	    else m.sourceStatement = "" ;
  	
  	    for( int j = 0 ; j < tokenArray[i].token.size() ; j++ ){
  	      if( ( tokenArray[i].tokenType[j] == 2 && tokenArray[i].token[j] != "WORD" && tokenArray[i].token[j] != "word" &&
		        tokenArray[i].token[j] != "BYTE" && tokenArray[i].token[j] != "byte" ) || tokenArray[i].token[j] == "." ){ // pseudo(除了WORD跟BYTE) or (.)就不翻 
  		    
			if( tokenArray[i].token[j] == "END" || tokenArray[i].token[j] == "end" ) {
			  meetEND = true ;
			  m.loc = "    " ;
			} // if()
			
			else if( tokenArray[i].token[j] == "BASE" || tokenArray[i].token[j] == "base" ){
			  int num ;
		  	  if( tokenArray[i].tokenType[j+1] == 5 && symbol.contain(tokenArray[i].token[j+1], num)){ //  pseudo(BASE) labol
			    B = symbol.hashTable[num].address ;
			  } // if()
				
			  else if( tokenArray[i].tokenType[j+1] == 6 ){ //  pseudo(BASE) 立即值(dec)
		        B = tokenArray[i].token[j+1] ;
			  } // else 
			  
			  else { 
			    B = "" ;
			    haveBase = false ;
			  } // else
			  
			  m.loc = "    " ;
			} // else if()
			
			else if( tokenArray[i].token[j] == "LTORG" || tokenArray[i].token[j] == "ltorg" ){
			  m.loc = "    " ; 
			} // else if() 
			
			m.objectCode = "" ;
  		    break ;
	      } // if
	      
	      else if( tokenArray[i].tokenType[j] == 2 && ( tokenArray[i].token[j] == "WORD" || tokenArray[i].token[j] == "word" )) {
	      	if( tokenArray[i].tokenType[j+1] == 6 ){
	      	  m.objectCode = decToHex( atoi( tokenArray[i].token[j+1].c_str())) ;
	      	  if( m.objectCode.size() == 1 ) m.objectCode = "00000" + m.objectCode ;
	      	  else if( m.objectCode.size() == 2 ) m.objectCode = "0000" + m.objectCode ;
	      	  else if( m.objectCode.size() == 3 ) m.objectCode = "000" + m.objectCode ;
	      	  else if( m.objectCode.size() == 4 ) m.objectCode = "00" + m.objectCode ;
	      	  else if( m.objectCode.size() == 5 ) m.objectCode = "0" + m.objectCode ; 
	      	  break ;
			} // if()
			
			else if( tokenArray[i].token[j+1] == "X" || tokenArray[i].token[j+1] == "x" ){
	      	  m.objectCode = tokenArray[i].token[j+3] ;
  		      break ;
			} // else if()
			  
			else if( tokenArray[i].token[j+1] == "C" || tokenArray[i].token[j+1] == "c" ){
			  for( int y = 0 ; y < tokenArray[i].token[j+3].size() ; y++ ){
			  	m.objectCode = m.objectCode + decToHex( (int)tokenArray[i].token[j+3][y] ) ;
			  } // for()
			   
  		      break ;
			} // else if()
		  } // else if()
		  
		  else if( tokenArray[i].tokenType[j] == 2 && ( tokenArray[i].token[j] == "BYTE" || tokenArray[i].token[j] == "byte" )) {
		  	if( tokenArray[i].tokenType[j+1] == 6 ){
		  	  m.objectCode = decToHex( atoi( tokenArray[i].token[j+1].c_str())) ;
		  	  int num = m.objectCode.size() ;
		  	  while( num % 2 != 0 ) {
		  	    m.objectCode = "0" + m.objectCode ;
				num = num / 2 ;
			  } // while()
	      	  
	      	  break ;
			} // if()
			
	      	if( tokenArray[i].token[j+1] == "X" || tokenArray[i].token[j+1] == "x" ){
	      	  m.objectCode = tokenArray[i].token[j+3] ;
  		      break ;
			} // if()
			  
			else if( tokenArray[i].token[j+1] == "C" || tokenArray[i].token[j+1] == "c" ){
			  for( int y = 0 ; y < tokenArray[i].token[j+3].size() ; y++ ){
			  	m.objectCode = m.objectCode + decToHex( (int)tokenArray[i].token[j+3][y] ) ;
			  } // for()
			   
  		      break ;
			} // else if()
		  } // else if()
	  
	      else if( tokenArray[i].tokenType[j] == 8 ){
	      	int num ; 
	   	    if( literal.contain( tokenArray[i].token[j], num ) == true ) {
	   	      if( literal.hashTable[num].strOrDec == "C" || literal.hashTable[num].strOrDec == "c" || literal.hashTable[num].strOrDec == "X" || literal.hashTable[num].strOrDec == "x" ){
			     m.sourceStatement = "=" + literal.hashTable[num].strOrDec + "'" + tokenArray[i].token[j] + "'" ; 
			  } // if()
			  
			  else {
			  	m.sourceStatement = "=" + tokenArray[i].token[j]  ; 
			  } // else 
			} // if() 
			
			m.objectCode = "" ;
	  	    break ;
		  } // else if()
		  
		  else if( tokenArray[i].tokenType[j] == 1 ){ // 是 instruction  
	   	    if( searchOpCodeFormat( tokenArray[i].token[j] ) == "1" ) { // format1
	  	      m.objectCode = opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ;
	  	      break ;
		    } // if
		
		    else if( searchOpCodeFormat( tokenArray[i].token[j] ) == "2" ){ // format2
		      if( j + 1 == tokenArray[i].token.size()-1 && contain( reg, tokenArray[i].token[j+1], num1 ) ) {
		        m.objectCode = opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] + decToHex(num1-1) + "0" ;
	          } // if()   
			  
			  else if( j + 1 == tokenArray[i].token.size()-1 && tokenArray[i].tokenType[j+1] == 6 ){
			  	m.objectCode = opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] + tokenArray[i].token[j+1] + "0" ;
			  } // else if()
	      
	          else if( j + 3 == tokenArray[i].token.size()-1 && contain( reg, tokenArray[i].token[j+1], num1 ) && contain( reg, tokenArray[i].token[j+3], num2 ) ){
	      	    m.objectCode = opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] + decToHex(num1-1) + decToHex(num2-1) ;
		      } // else if()

		      break ;
		    } // else if()
		
		    else { // format 3/4 (尚未防呆)
		      if( j - 1 >= 0 ){
		        if( tokenArray[i].token[j-1] == "+" ) { // format4
		  	      if( j + 1 == tokenArray[i].token.size()-1 ) {
		    	    string temp, temp1 = "", address ;
		  	        if( tokenArray[i].tokenType[j+1] == 5 ) {  // (+)instruction labol   
				      temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
				      for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ;
				      temp = temp1 + "110001" ;
			    	  address = "0" + symbol.hashTable[tokenArray[i].tokenValue[j+1]].address ;
			    	  m.objectCode = convertBinToHex_12( temp ) + address ;
			        } // if() 
			  
			        else if( tokenArray[i].tokenType[j+1] == 6 ){  // (+)instruction 1000(address)  
			          temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			     	  for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ;
			    	  temp = temp1 + "110001" ;
			    	  if( tokenArray[i].token[j+1].size() == 1 ) address = "0000" + tokenArray[i].token[j+1] ;
                      else if( tokenArray[i].token[j+1].size() == 2 ) address = "000" + tokenArray[i].token[j+1] ;
                      else if( tokenArray[i].token[j+1].size() == 3 ) address = "00" + tokenArray[i].token[j+1] ;
                      else if( tokenArray[i].token[j+1].size() == 4 ) address = "0" + tokenArray[i].token[j+1] ;
                      else address = tokenArray[i].token[j+1] ;
			    	  m.objectCode = convertBinToHex_12( temp ) + address ;
			        } // else if() 
			      } // if()
			      
			      else if( j + 2 == tokenArray[i].token.size()-1 ){
			      	string temp, temp1 = "", address ; 
			      	int num ;
			      	if( tokenArray[i].token[j+1] == "#" && ( tokenArray[i].tokenType[j+2] == 5 || tokenArray[i].tokenType[j+2] == 6 ) ) {
			      	  temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
				      for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ;
					  temp = temp1 + "010001" ;
					  
					  if( tokenArray[i].tokenType[j+2] == 5 && symbol.contain(tokenArray[i].token[j+2], num) ){
					  	address = "0" + symbol.hashTable[num].address ;
					  } // if()
					  
					  else {
					  	if( tokenArray[i].token[j+2].size() == 1 ) address = "0000" + decToHex( atoi(tokenArray[i].token[j+2].c_str()));
					  	else if( tokenArray[i].token[j+2].size() == 2 ) address = "000" + decToHex( atoi(tokenArray[i].token[j+2].c_str())) ;
					  	else if( tokenArray[i].token[j+2].size() == 3 ) address = "00" + decToHex( atoi(tokenArray[i].token[j+2].c_str())) ;
					  	else if( tokenArray[i].token[j+2].size() == 4 ) address = "0" + decToHex( atoi(tokenArray[i].token[j+2].c_str())) ;
					  	else address = decToHex( atoi(tokenArray[i].token[j+2].c_str())) ;
					  } // else () 
					  
					  m.objectCode = convertBinToHex_12( temp ) + address ;
					} // if()
					
					else if( tokenArray[i].token[j+1] == "@" && tokenArray[i].tokenType[j+2] == 5 ){
					  string temp, temp1 = "", address ;
					  temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
				      for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ;
					  temp = temp1 + "100001" ;
					  address = "0" + symbol.hashTable[tokenArray[i].tokenValue[j+2]].address ;
			    	  m.objectCode = convertBinToHex_12( temp ) + address ;
					} // else if()
					
					else if(  tokenArray[i].token[j+1] == "=" && literal.contain( tokenArray[i].token[j+2], num ) ){ // literal
			          temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
				      for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ;
				      temp = temp1 + "110001" ;
				      address = "0" + literal.hashTable[num].address ;
				      m.objectCode = convertBinToHex_12( temp ) + address ;
					} // else if ()
				  } // else if()
			      
			      else if( j + 3 == tokenArray[i].token.size()-1 ){
			      	if( tokenArray[i].tokenType[j+1] == 5 && ( tokenArray[i].token[j+3] == "X" || tokenArray[i].token[j+3] == "x" )) {
					  string temp, temp1 = "", address ;
					  temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
				      for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ;
				      temp = temp1 + "111001" ;
			    	  address = "0" + symbol.hashTable[tokenArray[i].tokenValue[j+1]].address ;
			    	  m.objectCode = convertBinToHex_12( temp ) + address ;
					} // if()
				  } // else if()
				  
				  else { // +lda =c'EOF' // +LDA =6666
			      	string temp, temp1 = "", address ;
			      	int num ;
					
		  	        if( tokenArray[i].token[j+1] == "=" && literal.contain( tokenArray[i].token[j+4], num ) ) {  // (+)instruction literal   
				      temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
				      for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ;
				      temp = temp1 + "110001" ;
                      address = "0" + literal.hashTable[num].address ;
					  m.objectCode = convertBinToHex_12( temp ) + address ;
			        } // if() 
				  } // else 
		        } // if()
		        
		        else { // format3
		        if( j + 3 == tokenArray[i].token.size()-1 ){
		    	  if(( tokenArray[i].token[j+3] == "x" || tokenArray[i].token[j+3] == "X" ) && tokenArray[i].tokenType[j+1] == 5 ) {  // instruction symbol,X
			    	string temp, temp1 = "", disp ; 
		     	    int TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+1]].address ) ;
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
		     	    
			    	temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
		    	  	
		    	  	if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "111010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "111100" ;
		    	  	} // else if()
		    		 
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else 
		      	    
		    		m.objectCode = convertBinToHex_12( temp ) + disp ;
		    	  } // if()
				} // if()
				
				else if( j + 2 == tokenArray[i].token.size()-1 ){
				  int num ;
				  string temp, temp1 = "", disp ;
				  
				  if( tokenArray[i].token[j+1] == "#" && tokenArray[i].tokenType[j+2] == 5 ) {
		     	    int TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+2]].address ) ;
		     	    
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
		     	    
					temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
				    
				    if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "010010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "010100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else 
					
					m.objectCode = convertBinToHex_12( temp ) + disp ;
				  } // if()
				  
				  else if( tokenArray[i].token[j+1] == "#" && tokenArray[i].tokenType[j+2] == 6 ) {
		     	    int TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+2]].address ) ;	    	  
					temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
			    	temp = temp1 + "010000" ;
			    	disp = decToHex( atoi(tokenArray[i].token[j+2].c_str())) ;
		  	        if( disp.size() == 1 ) disp = "00" + disp ;
		  	        else if( disp.size() == 2 ) disp = "0" + disp ;
					m.objectCode = convertBinToHex_12( temp ) + disp ;
				  } // else if()
				
				  else if( tokenArray[i].token[j+1] == "@" && tokenArray[i].tokenType[j+2] == 5 ){
		     	    int TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+2]].address ) ;	    	  
					temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
			    	
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
		     	    
					if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "100010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "100100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else 
					
					m.objectCode = convertBinToHex( temp ) + disp ;
				  } // else if()	
				  
				  else if( tokenArray[i].token[j+1] == "=" && literal.contain( tokenArray[i].token[j+2], num )) {  // instruction literal  
		    		temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
		    		for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
					int TA = hexToDec( literal.hashTable[num].address ) ;
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
					
					if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "110010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "110100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else

	                m.objectCode = convertBinToHex_12( temp ) + disp ;
			      } // else if()
				} // else if()
			
		      	else if( j + 1 == tokenArray[i].token.size()-1 ){
		      	  string temp, temp1 = "", disp ;
		      	  int TA ;
		  	  
		      	  if( tokenArray[i].tokenType[j+1] == 5 ) {  // instruction labol  
		    		TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+1]].address ) ;
		    		temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
		    		for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
	                
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
		    		
		    		if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "110010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "110100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else 
		  	        
			    	m.objectCode = convertBinToHex_12( temp ) + disp ;
			      } // if() 
			  
			      else if( tokenArray[i].tokenType[j+1] == 6 ){  // instruction 1000(address)  
			    	temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit

				    if( tokenArray[i].token[j+1].size() <= 12 ){
		      	      disp = tokenArray[i].token[j+1] ;
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 2 ) disp = disp.substr(disp.size()-3, disp.size()-1) ;
		     	      temp = temp1 + "110000" ;
			    	} // if()
		  	    
			    	m.objectCode = convertBinToHex_12( temp ) + disp ;
			      } // else if() 
		    	} // else if()  
		      
		        else if( j == tokenArray[i].token.size()-1 ) {
		          string temp, temp1 = "" ;
		    	  temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
		    	  for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
	              temp = temp1 + "110000" ;
			      m.objectCode = convertBinToHex_12( temp ) + "000" ;
				} // else if()
		      
			    else { // instruction literal
		
			      string temp, temp1 = "", disp ;
			      int num, TA ;
			      
		  	      if( tokenArray[i].token[j+1] == "=" && literal.contain( tokenArray[i].token[j+4], num )) {  // instruction literal  
		    		temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
		    		for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
					TA = hexToDec( literal.hashTable[num].address ) ;
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
					
					if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "110010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "110100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else

	                m.objectCode = convertBinToHex_12( temp ) + disp ;
			      } // if() 
				} // else()
			   } // else()
		      } // if()
		      
		      else { // format3
		        if( j + 3 == tokenArray[i].token.size()-1 ){
		    	  if(( tokenArray[i].token[j+3] == "x" || tokenArray[i].token[j+3] == "X" ) && tokenArray[i].tokenType[j+1] == 5 ) {  // instruction symbol,X
			    	string temp, temp1 = "", disp ; 
		     	    int TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+1]].address ) ;
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
		     	    
			    	temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
		    	  	
		    	  	if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "111010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "111100" ;
		    	  	} // else if()
		    		 
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else 
		      	    
		    		m.objectCode = convertBinToHex_12( temp ) + disp ;
		    	  } // if()
				} // if()
				
				else if( j + 2 == tokenArray[i].token.size()-1 ){
				  string temp, temp1 = "", disp ;
				  int num ;
				  
				  if( tokenArray[i].token[j+1] == "#" && tokenArray[i].tokenType[j+2] == 5 ) {
		     	    int TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+2]].address ) ;
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
		     	    
					temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
				    
				    if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "010010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "010100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else 
					
					m.objectCode = convertBinToHex_12( temp ) + disp ;
				  } // if()
				  
				  else if( tokenArray[i].token[j+1] == "#" && tokenArray[i].tokenType[j+2] == 6 ) {
		     	    int TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+2]].address ) ;	    	  
					temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
			    	temp = temp1 + "010000" ;
			    	disp = decToHex( atoi(tokenArray[i].token[j+2].c_str())) ;
		  	        if( disp.size() == 1 ) disp = "00" + disp ;
		  	        else if( disp.size() == 2 ) disp = "0" + disp ;
					m.objectCode = convertBinToHex_12( temp ) + disp ;
				  } // else if()
				
				  else if( tokenArray[i].token[j+1] == "@" && tokenArray[i].tokenType[j+2] == 5 ){
		     	    int TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+2]].address ) ;	    	  
					temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
		     	    
					if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "100010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "100100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else 
					
					m.objectCode = convertBinToHex_12( temp ) + disp ;
				  } // else if()	
				  
				  else if( tokenArray[i].token[j+1] == "=" && literal.contain( tokenArray[i].token[j+2], num )) {  // instruction literal  
		    		temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
		    		for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
					int TA = hexToDec( literal.hashTable[num].address ) ;
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
					
					if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "110010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "110100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else

	                m.objectCode = convertBinToHex_12( temp ) + disp ;
			      } // else if()
				} // else if()
			
		      	else if( j + 1 == tokenArray[i].token.size()-1 ){
		      	  string temp, temp1 = "", disp ;
		      	  int TA ;
		  	  
		      	  if( tokenArray[i].tokenType[j+1] == 5 ) {  // instruction labol  
		    		TA = hexToDec( symbol.hashTable[tokenArray[i].tokenValue[j+1]].address ) ;
		    		temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
		    		for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
		    		
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
		    		
		    		if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "110010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "110100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else 
		  	        
			    	m.objectCode = convertBinToHex_12( temp ) + disp ;
			      } // if() 
			  
			      else if( tokenArray[i].tokenType[j+1] == 6 ){  // instruction 1000(address)  
			    	temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
			    	for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit

				    if( tokenArray[i].token[j+1].size() <= 12 ){
		      	      disp = tokenArray[i].token[j+1] ;
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 2 ) disp = disp.substr(disp.size()-3, disp.size()-1) ;
		     	      temp = temp1 + "110000" ;
			    	} // if()
		  	    
			    	m.objectCode = convertBinToHex_12( temp ) + disp ;
			      } // else if() 
		    	} // else if()  
		      
		        else if( j == tokenArray[i].token.size()-1 ) {
		          string temp, temp1 = "" ;
		    	  temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
		    	  for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
	              temp = temp1 + "110000" ;
			      m.objectCode = convertBinToHex_12( temp ) + "000" ;
				} // else if()
		      
			    else { // instruction literal
		
			      string temp, temp1 = "", disp ;
			      int num, TA ;
			      
		  	      if( tokenArray[i].token[j+1] == "=" && literal.contain( tokenArray[i].token[j+4], num ) ) {  // instruction literal  
		    		temp = opcodeToMachineCode( opCodeArr[searchOpCodeIndex(tokenArray[i].token[j])][1] ) ;
		    		for( int i = 0 ; i < 6 ; i++ ) temp1 = temp1 + temp[i] ; // 6bit
					TA = hexToDec( literal.hashTable[num].address ) ;
		     	    bool usePC = false, useB = false ;
		     	    if( TA - PC >= 0 && decToHex( TA - PC ).size() <= 3 ) usePC = true ;
		     	    else if( TA - PC < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1")).size() <= 16 && 
					       ((binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '0' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '0'  ) || 
					       ( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[0] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[1] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[2] == '1' && 
							 binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))[3] == '1'  ))) usePC = true ;
							 
		     	    else if( TA - hexToDec(B) >= 0 && decToHex(TA - hexToDec(B)).size() <= 3 ) useB = true ;
		     	    else if( TA - hexToDec(B) < 0 && binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1")).size() <= 16 && 
					       (( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '0' &&  
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[1] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[2] == '0' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[3] == '0' ) ||
							( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' && 
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1' &&
							  binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))[0] == '1'  )))  useB = true ;
					
					if( usePC == true ){
		      	      if( TA - PC >= 0 ) disp = decToHex( TA - PC ) ;
		      	      else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(PC).c_str()), "1"))) ;
		      	      
		      	      if( disp.size() == 1 ) disp = "00" + disp ;
		     	      else if( disp.size() == 2 ) disp = "0" + disp ;
		     	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		     	      temp = temp1 + "110010" ;
			    	} // if()
                      
		    	  	else if( haveBase == true && useB == true ){ // BASE    
		  	          if( TA - hexToDec(B) >= 0 ) disp = decToHex( TA - hexToDec(B) ) ;
		    	  	  else disp = convertBinToHex( binAdd(dexToBin(TA), binAdd(diff(dexToBin(hexToDec(B)).c_str()), "1"))) ;
		    
		  	          if( disp.size() == 1 ) disp = "00" + disp ;
		      	      else if( disp.size() == 2 ) disp = "0" + disp ;
		      	      else if( disp.size() > 3 ) disp = disp.substr(1,3) ;
		      	      temp = temp1 + "110100" ;
		    	  	} // else if()
		    		
		    		else { // ERROR
		    		  m.objectCode = "error!" ;
		    		} // else

	                m.objectCode = convertBinToHex_12( temp ) + disp ;
			      } // if() 
				} // else()
			  } // else()
		  
		      break ;
		    } // else
	      } // else if()
	    } // for()
	    
	    machineCode.push_back(m) ;
      } // for()

      writeFile( temppp ) ;
    } // else if()
    
    machineCode.clear() ;
    tokenArray.clear() ;
  } // while()
} // main()
