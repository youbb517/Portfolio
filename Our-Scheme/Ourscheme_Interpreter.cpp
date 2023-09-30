# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <iomanip>
# include <string>  
# include <string.h>
# include <vector> 
# include <ctype.h>
# include <cmath>

using namespace std ;

vector <string> g_tokenList ;
string g_first_num ;
string g_temp_token = "\0" ;
string g_ErrorMessage = "\0" ;
string g_pretty_str = "\0" ;
string g_primitive[43] = { "car", "cdr", "cons", "define", "quote", "exit", "list", "nil", "#t", "#f", 
                           "string=?", "+", "-", "*", "/", ">", "<", "<=", ">=", "=", "atom?",
                           "pair?", "list?", "null?", "integer?", "real?", "number?", "string?", "boolean?",
                           "symbol?", "not", "and", "or", "string-append", "string>?", "string<?", "eqv?",
                           ".", "equal?", "begin", "if", "cond", "clean-environment" } ;

string g_functions[39] = { "car", "cdr", "cons", "define", "quote", "exit", "list", "string=?", "+", 
                           "-", "*", "/", ">", "<", "<=", ">=", "=", "atom?", "pair?", "list?", "null?", 
                           "integer?", "real?", "number?", "string?", "boolean?","symbol?", "not", "and", 
                           "or", "string-append", "string>?", "string<?", "eqv?", "equal?", 
                           "begin", "if", "cond", "clean-environment" } ;

int g_line = 1 ;
int g_column = 0 ;
int g_token_column = 0 ;
int g_space = 0 ;
int g_function_call_number = 0 ;
int g_let_func_call_number = 0 ;

bool g_end_of_line = false ;
bool g_end_of_file = false ;
bool g_first_need_lp = false ;
bool g_now_state = false ; // ???????get?????char?????????????imiter 
bool g_exit = false ;
bool g_change = true ;
string  g_now_function = "\0" ;
bool g_local = false ;
string g_Isfrom = "\0" ; 
bool g_hasRecursive = false ;

struct Tree_node {
  string left_str ;
  string right_str ; 
  Tree_node *left ;      
  Tree_node *right ;
};

struct User_define {
  string symbol ;
  string function ;
  string result ;
  int index ; // default by self
  bool change ;
};

vector < User_define > g_define_list ;           // ?????????????????????
// vector < User_define > g_local_define_list ;     // local??????????????
vector < User_define > g_lambda_define_list ;    // lambda??????????????
vector < User_define > g_temp_vector ; 

bool EvalSExp( Tree_node *walk, string &result, bool single ) ;
string Print_Result( string str ) ;

bool IsFunction( string str ) {
  for ( int i = 0 ; i < 39 ; i++ ) {
    if ( str == ( "#<procedure " + g_functions[i] + ">" ) ) return true ;
  } // for

  for ( int i = 0 ; i < g_define_list.size() ; i++ ) {
    if ( str == g_define_list[i].function && str != "\0" ) {
      // cout << "*********************************" << endl ;
      return true ;
    } // if
  } // for
  
  return false ;
} // IsFunction()

bool IsFunction( string str, string &value ) {
  for ( int i = 0 ; i < 39 ; i++ ) {
    if ( str == ( "#<procedure " + g_functions[i] + ">" ) ) {
      value = g_functions[i] ;
      return true ;
    } // if
  } // for

  for ( int i = 0 ; i < g_define_list.size() ; i++ ) {
    if ( str == g_define_list[i].function && str != "\0" ) {
      // cout << "*********************************" << endl ;
      value = g_define_list[i].function ;
      return true ;
    } // if
  } // for
  
  return false ;
} // IsFunction()

bool IsDefine( string str, int & index ) {
  for ( int i = 0 ; i < g_define_list.size() ; i++ ) {
    if ( ( str == g_define_list[i].function || str == g_define_list[i].symbol ) && str != "\0" ) {
      index = i ;
      return true ;
    } // if
  } // for
  
  return false ;
} // IsDefine()

bool IsUserDefine( string str, int & index ) {
  for ( int i = 0 ; i < 39 ; i++ ) {
    if ( str == ( "#<procedure " + g_functions[i] + ">" ) ) return false ;
  } // for

  for ( int i = 0 ; i < g_define_list.size() ; i++ ) {
    if ( str == g_define_list[i].function && str != "\0" ) {
      index = i ;
      return true ;
    } // if
  } // for
  
  return false ;
} // IsUserDefine()

void Init( Tree_node *node ) {
  node -> left_str = "\0" ; 
  node -> right_str = "\0" ; 
  node -> left = NULL ; 
  node -> right = NULL ; 
} // Init()

string GetToken( ) {
  if ( g_end_of_line == true ) {
    g_line++ ;
    g_column = 0 ;
    g_end_of_line = false ; 
  } // if()  

  string token = "\0" ;
  if ( !g_end_of_file && g_temp_token == "\0" ) {
    char ch = getchar() ;
    g_column++ ;
    while ( ch != EOF ) {
      if ( g_end_of_line == true ) {
        g_line++ ;
        g_column = 1 ;
        g_end_of_line = false ; 
      } // if()

      if ( ch != ' ' && ch != '\t' && ch != '\n' && ch != '(' && 
           ch != ')' && ch != ';' && ch != '\'' && ch != '\"' ) {
        token = token + ch ;
        g_now_state = false ;
      } // if

      else {
        if ( ch == ' ' || ch == '\t' || ch == '\n' ) {
          if ( ch == '\n' ) g_end_of_line = true ;
          
          if ( token != "\0" ) {
            g_token_column = g_column - token.size() ;
            g_tokenList.push_back( token ) ;
            return token ;
          } // if

          g_now_state = false ;
        } // if

        else if ( ch == '(' || ch == ')' || ch == ';' || ch == '\'' || ch == '\"' ) {
          if ( ch == ';' && token == "\0" ) {
            g_temp_token = "\0" ;
            ch = getchar() ;
            while ( ch != '\n' && ch != EOF ) ch = getchar() ;
            
            if ( ch == EOF ) {
              g_end_of_file = true ;
              return token ;
            } // if

            g_end_of_line = true ;
            token = GetToken() ;
            return token ;
          } // if

          if ( token == "\0" ) {
            token = token + ch ;
            g_token_column = g_column ;
          } // if

          else {
            g_temp_token = ch ;
            g_token_column = g_column - 1 ;
          } // else

          if ( token == "\'" ) {
            g_tokenList.push_back( "(" ) ;
            g_tokenList.push_back( token ) ;
          } // if

          else if ( ch != '\"' || token != "\0" ) g_tokenList.push_back( token ) ;

          g_now_state = true ;
          return token ;
        } // else if
      } // else

      ch = getchar() ;
      g_column++ ;
    } // while

    g_end_of_file = true ;
    g_tokenList.push_back( token ) ;
    return token ;
  } // if

  else if ( !g_end_of_file && g_temp_token != "\0" ) {
    if ( g_temp_token == ";" ) {
      g_temp_token = "\0" ;
      char ch = getchar() ;
      while ( ch != '\n' && ch != EOF ) ch = getchar() ;
      
      if ( ch == EOF ) {
        g_end_of_file = true ;
        return g_temp_token ;
      } // if

      g_end_of_line = true ;
      token = GetToken() ;
      return token ;
    } // if

    else {
      g_now_state = true ;
      token = g_temp_token ;
      g_temp_token = "\0" ;
      if ( g_column == 0 ) g_token_column = 1 ;
      else g_token_column = g_column ;

      if ( token == "\'" ) g_tokenList.push_back( "(" ) ;
      g_tokenList.push_back( token ) ;
      return token ;
    } // else 
    
  } // else if 

  else {
    return token ;
  } // else
} // GetToken()

bool IsDigit( string str ) {
  int i = 0 ;
  if ( str == "\0" ) return false ;

  if ( str[i] == '+' || str[i] == '-' ) {
    if ( str.size() == 1 ) return false ;
    else i++ ;
  } // if

  for (  ; i < str.size() ; i++ ) {
    if ( ! isdigit( str[i] )  ) return false ;
  } // for

  return true ;
} // IsDigit()

bool IsFloat( string str ) {
  int i = 0, dotnum = 0 ;
  if ( str == "." || str == "+." || str == "-." || str == "+" || str == "-" || str == "\0" ) return false ; 
  
  if ( str[i] == '+' || str[i] == '-' ) i++ ;
  bool hasDot = false ;
  for ( ; i < str.size() ; i++ ) {
    if ( str[i] == '.' ) {
      dotnum = i ;
      if ( hasDot ) return false ;
      else hasDot = true ;
    } // if

    else if ( ! isdigit( str[i] ) ) return false ;
  } // for
  
  if ( !hasDot ) return false ;
  return true ;
} // IsFloat()

string Float( string str ) {
  string temp = "\0" ;
  int i = 0 ;
  for ( ; str[i] != '.' ; i++ ) 
    temp = temp + str[i] ;
  
  temp = temp + str[i] + str[i+1] + str[i+2] + str[i+3] ;
  return temp ;
} // Float()

string Int2str( int num ) {
  char c[100] = {'\0'} ;
  string temp = "\0" ;
  sprintf( c, "%d", num ) ;
  for ( int i = 0 ; i < 100 && c[i] != '\0' ; i++ ) { 
    temp = temp + c[i] ;
  } // for

  return temp ;
} // Int2str()

string Float2str( float num ) {
  char c[100] = {'\0'} ;
  string temp = "\0" ;
  sprintf( c, "%.3f", num );
  for ( int i = 0 ; i < 100 && c[i] != '\0' ; i++ ) {
    temp = temp + c[i] ;
  } // for
  
  return temp ;
} // Float2str()

int Float2Int( float f ) {
  string str = Float2str( f ) ;
  string s = "\0" ;
  for ( int i = 0 ; i < str.size() ; i++ ) {
    if ( str[i] != '.' ) s = s + str[i] ;
    else i = str.size() ;
  } // for

  int num = atoi( s.c_str() ) ;
  return num ;
} // Float2Int()

string Space( bool new_line ) {
  string str = "\0" ;
  for ( int i = 0 ; i < g_space && new_line ; i++ ) {
    str = str + " " ;
  } // for
  
  return str ;
} // Space()

bool IsString( string token ) {  
  if ( token == "\"" ) {
    string temp = token ; // ???"???????
    char ch = getchar() ;
    g_column++ ;
    g_token_column = g_column ;
    while ( ch != '\n' && ch != EOF ) {

      temp = temp + ch ;
      if ( ch == '\\' ) {
        ch = getchar() ;
        temp = temp + ch ;
        g_column++ ;
        g_token_column = g_column ;
      } // if

      else if ( ch == '\"' ) {
        g_tokenList.pop_back() ;
        g_tokenList.push_back( temp ) ;
        return true ;
      } // else if()

      ch = getchar() ;
      g_column++ ;
      g_token_column = g_column ;
    } // while
    
    if ( ch == '\n' ) g_end_of_line = true ;
    else if ( ch == EOF ) g_end_of_file = true ;

    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (no closing quote) : END-OF-LINE encountered at Line " + Int2str( g_line ) + 
                       " Column " + Int2str( g_token_column ) ;

    return false ;
  } // if
  
  else return false ;
} // IsString() 

bool Atom( string token ) {
  if ( IsDigit( token ) ) return true ;
  else if ( IsFloat( token ) ) return true ;
  else if ( !g_end_of_file && IsString( token ) ) return true ;
  else if ( token == "nil" || token == "#f" ) return true ;
  else if ( token == "t" || token == "#t" ) return true ;
  else if ( token != "(" && token != ")" && token != "\'" && token != "\"" && token != ";" && token != "." ) 
    return true ; // Symbol
  return false ;
} // Atom()

bool S_exp( string token ) {
  if ( token == "\'" && !g_end_of_file ) {
    token = GetToken() ;
    if ( !g_end_of_file && S_exp( token ) ) {
      g_tokenList.push_back( ")" ) ;
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) 
        g_ErrorMessage = "ERROR (unexpected token) : atom or '(' expected when token at Line " + 
            Int2str( g_line ) + " Column " + Int2str( g_token_column ) + " is >>" + token + "<<" ;
      return false ;
    } // else
  } // if
 
  else if ( token == "(" && !g_end_of_file ) {
    token = GetToken() ;
    if ( !g_end_of_file && token == ")" ) {
      g_tokenList.pop_back() ;
      g_tokenList.pop_back() ;
      g_tokenList.push_back( "()" ) ;
      return true ; // atom
    } // if

    else if ( !g_end_of_file && S_exp( token ) ) {
      token = GetToken() ;
      while ( !g_end_of_file && ( token == "(" || token == "\'" || Atom( token ) ) ) {
        bool has_quote = false ;
        if ( token == "\'" || token == "(" ) {
          if ( token == "\'" ) {
            has_quote = true ;
            token = GetToken() ;
          } // if

          if ( S_exp( token ) ) {
            if ( has_quote ) g_tokenList.push_back( ")" ) ;
            token = GetToken() ;
            has_quote = false ;
          } // if

          else return false ;
        } // if()

        else token = GetToken() ; 
      } // while

      if ( !g_end_of_file && token == "." ) {
        token = GetToken() ;
        if ( !g_end_of_file && S_exp( token ) ) {
          token = GetToken() ;
          // cout << token << "    11" << endl ; 
          if ( !g_end_of_file && token == ")" ) return true ;
          else {
            if ( g_ErrorMessage == "\0" ) 
              g_ErrorMessage = "ERROR (unexpected token) : ')' expected when token at Line " + 
                    Int2str( g_line ) + " Column " + Int2str( g_token_column ) + " is >>" + token + "<<" ;
            
            return false ;
          } // else  
        } // if

        else {
          if ( g_ErrorMessage == "\0" ) 
            g_ErrorMessage = "ERROR (unexpected token) : atom or '(' expected when token at Line " + 
                    Int2str( g_line ) + " Column " + Int2str( g_token_column ) + " is >>" + token + "<<" ;
          return false ;
        } // else
      } // if	  

      else if ( !g_end_of_file && token == ")" ) return true ;

      else {
        if ( g_ErrorMessage == "\0" ) 
          g_ErrorMessage = "ERROR (unexpected token) : ')' expected when token at Line " + 
                    Int2str( g_line ) + " Column " + Int2str( g_token_column ) + " is >>" + token + "<<" ;
        return false ;
      } // else
    } // else if

    else {
      if ( g_ErrorMessage == "\0" ) 
        g_ErrorMessage = "ERROR (unexpected token) : atom or '(' expected when token at Line " + 
                    Int2str( g_line ) + " Column " + Int2str( g_token_column ) + " is >>" + token + "<<" ;
      return false ;
    } // else 
  } // else if
  
  else if ( Atom( token ) ) return true ;
  
  else {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (unexpected token) : atom or '(' expected when token at Line " + 
                    Int2str( g_line ) + " Column " + Int2str( g_token_column ) + " is >>" + token + "<<" ;
    return false ;
  } // else 
} // S_exp()

void PrettyPrint( Tree_node *walk, bool &new_line ) {
  if ( walk -> left_str != "\0" ) {
    g_pretty_str = g_pretty_str + Space( new_line ) + walk -> left_str + "\n" ;
    new_line = true ;
  } // if
  
  if ( walk -> left != NULL ) {
    g_pretty_str = g_pretty_str + Space( new_line ) + "( " ;
    g_space = g_space + 2 ;
    Tree_node *temp = walk -> left ;
    new_line = false ;
    PrettyPrint( temp, new_line ) ;
  } // if
  
  if ( walk -> right != NULL ) {
    Tree_node *temp = walk -> right ;
    PrettyPrint( temp, new_line ) ;
  } // if 
  
  if ( walk -> right_str != "\0" ) {
    g_pretty_str = g_pretty_str + Space( new_line ) + "." + "\n" + Space( new_line ) + 
                   walk -> right_str + "\n" ;
    g_space = g_space - 2 ;
    g_pretty_str = g_pretty_str + Space( new_line ) + ")" + "\n" ;
    new_line = true ;
  } // if

  if ( g_first_need_lp && walk -> right == NULL && walk -> right_str == "\0" ) {
    g_space = g_space - 2 ;
    g_pretty_str = g_pretty_str + Space( new_line ) + ")" + "\n" ;
    new_line = true ;
  } // if
} // PrettyPrint()

void BuildDS( Tree_node *now, int &i ) {
  bool isRL = false ;
  for ( ; i < g_tokenList.size() && !isRL ; i++ ) {
    if ( g_tokenList[i] == "("  ) {
      if ( now -> left == NULL && now -> left_str == "\0" ) {
        Tree_node *temp = now ;
        now -> left = new Tree_node ;
        Init( now -> left ) ;
        now = now -> left ;
        i++ ;
        BuildDS( now, i ) ;
        now = temp ;
      } // if

      else {
        now -> right = new Tree_node ;
        Init( now -> right ) ;
        now = now -> right ;
        BuildDS( now, i ) ;
        isRL = true ;
        i-- ;
      } // else 
    } // if()
    
    else if ( g_tokenList[i] == "." ) {
      i++ ;
      if ( g_tokenList[i] == "(" ) {
        now -> right = new Tree_node ;
        Init( now -> right ) ;
        Tree_node *temp = now ;
        now = now -> right ; 
        i++ ;
        BuildDS( now, i ) ;
        now = temp ;
      } // if

      else {
        if ( IsDigit( g_tokenList[i] ) ) now -> right_str = Int2str( atoi( g_tokenList[i].c_str() ) ) ;
        else if ( IsFloat( g_tokenList[i] ) ) 
          now -> right_str = Float2str( atof( g_tokenList[i].c_str() ) )  ; 
        else if ( IsString( g_tokenList[i] ) ) now -> right_str = g_tokenList[i] ;
        else if ( g_tokenList[i] == "#t" || g_tokenList[i] == "t" ) now -> right_str = "#t" ;
        else if ( g_tokenList[i] == "\'" ) now -> right_str = "quote" ;
        else if ( g_tokenList[i] != "#f" && g_tokenList[i] != "nil" && g_tokenList[i] != "()" )
          now -> right_str = g_tokenList[i] ;
      } // else 
    } // else if()

    else if ( g_tokenList[i] == ")" ) {
      isRL = true ;
      i-- ;
    } // else if

    else if ( now -> left_str != "\0" || ( now -> left_str == "\0" && now -> left != NULL ) ) {
      now -> right = new Tree_node ;
      Init( now -> right ) ;
      now = now -> right ; 
      if ( IsDigit( g_tokenList[i] ) ) now -> left_str = Int2str( atoi( g_tokenList[i].c_str() ) ) ;
      else if ( IsFloat( g_tokenList[i] ) ) 
        now -> left_str = Float2str( atof( g_tokenList[i].c_str() ) ) ; 
      else if ( IsString( g_tokenList[i] ) ) now -> left_str = g_tokenList[i] ;
      else if ( g_tokenList[i] == "#t" || g_tokenList[i] == "t" ) now -> left_str = "#t" ;
      else if ( g_tokenList[i] == "#f" || g_tokenList[i] == "nil" || g_tokenList[i] == "()" ) 
        now -> left_str = "nil" ;
      else if ( g_tokenList[i] == "\'" ) now -> left_str = "quote" ;
      else now -> left_str = g_tokenList[i] ;
    } // else if

    else {
      if ( IsDigit( g_tokenList[i] ) ) now -> left_str = Int2str( atoi( g_tokenList[i].c_str() ) ) ;
      else if ( IsFloat( g_tokenList[i] ) ) 
        now -> left_str = Float2str( atof( g_tokenList[i].c_str() ) ) ; 
      else if ( IsString( g_tokenList[i] ) ) now -> left_str = g_tokenList[i] ;
      else if ( g_tokenList[i] == "#t" || g_tokenList[i] == "t" ) now -> left_str = "#t" ;
      else if ( g_tokenList[i] == "#f" || g_tokenList[i] == "nil" || g_tokenList[i] == "()" ) 
        now -> left_str = "nil" ;
      else if ( g_tokenList[i] == "\'" ) now -> left_str = "quote" ;
      else now -> left_str = g_tokenList[i] ;
    } // else
  } // for()
} // BuildDS()

void BuildTree( Tree_node *root ) {
  if ( g_tokenList.size() == 1 && IsDigit( g_tokenList[0] ) ) 
    root -> left_str = Int2str( atoi( g_tokenList[0].c_str() ) ) ;
  else if ( g_tokenList.size() == 1 && IsFloat( g_tokenList[0] ) ) 
    root -> left_str = Float2str( atof( g_tokenList[0].c_str() ) ) ; 
  else if ( g_tokenList.size() == 1 && IsString( g_tokenList[0] ) ) 
    root -> left_str = g_tokenList[0] ;
  else if ( g_tokenList.size() == 1 && ( g_tokenList[0] == "#t" || g_tokenList[0] == "t" ) ) 
    root -> left_str = "#t" ;
  else if ( g_tokenList.size() == 1 && ( g_tokenList[0] == "#f" || g_tokenList[0] == "nil" || 
                                         g_tokenList[0] == "()" ) ) root -> left_str = "nil" ;
  else if ( g_tokenList[0] == "(" ) {
    int i = 0 ;
    // cout << "( " ;
    g_pretty_str = g_pretty_str + "( " ;
    g_space =  g_space + 2 ;
    if ( g_tokenList[0] == "(" ) i = 1 ;
    BuildDS( root, i ) ;
    g_first_need_lp = true ;
  } // else if()
  
  else root -> left_str = g_tokenList[0] ;
} // BuildTree()

int g_index = -100 ;

bool InList( string str, int &index, string &value ) {
  for ( int i = 0 ; i < g_define_list.size() ; i++ ) {
    if ( str != "\0" && ( str == g_define_list[i].symbol || str == g_define_list[i].function ) ) {
      index = i ;
      g_index = i ;

      if ( g_define_list[i].function == "\0" || g_pretty_str == "\0" ) value = g_define_list[i].result ;
      else value = g_define_list[i].function ;
      g_Isfrom = "G" ;
      return true ;
    } // if
  } // for

  return false ;
} // InList()

bool InPrimitiveList( string str ) {
  if ( g_local == true ) { // ???local
    for ( int i = g_lambda_define_list.size() -1 ; i >= 0 ; i-- ) {
      if ( str == g_lambda_define_list[i].symbol ) {
        return true ;
      } // if
    } // for

    for ( int i = 0 ; i < g_define_list.size() ; i++ ) {
      if ( str == g_define_list[i].symbol ) {
        return true ;
      } // if
    } // for

    return false ;
  } // if()

  else {
    for ( int i = 0 ; i < g_define_list.size() ; i++ ) {
      if ( str == g_define_list[i].symbol ) {
        return true ;
      } // if
    } // for

    return false ;
  } // else 
} // InPrimitiveList()

bool IsPrimitive( string str ) {
  for ( int i = 0 ; i < 43 ; i++ ) {
    if ( g_primitive[i] == str ) return true ;
  } // for

  return false ;
} // IsPrimitive()

bool IsPrimitive_function( string str ) {
  for ( int i = 0 ; i < 39 ; i++ ) {
    if ( ( "#<procedure " + g_functions[i] + ">" ) == str ) return true ;
  } // for

  return false ;
} // IsPrimitive_function()

bool IsPrimitive_function_PRO( string str ) {
  for ( int i = 0 ; i < 39 ; i++ ) {
    if ( g_functions[i] == str ) return true ;
  } // for

  return false ;
} // IsPrimitive_function_PRO()

bool IsSymbol( string str ) {
  if ( !IsDigit( str ) && !IsFloat( str ) && ! ( ( str[0] == '\"' ) && ( str[str.size()-1] == '\"' ) ) &&
       ! ( ( str[0] == '(' ) && ( str[str.size()-1] == ')' ) ) && str != "#t" && str != "nil" ) 
    return true ;

  return false ;
} // IsSymbol()

string Error_print( string errorMessage ) {
  string return_result = "\0" ;
  string temp = "\0" ;
  for ( int i = 0 ; i < errorMessage.size() ; i++ ) {
    if ( ( errorMessage[i] == ' ' || errorMessage[i] == '\n' ) && temp != "#<procedure" ) {
      if ( temp != "\0" ) {
        if ( IsFloat( temp ) ) temp = Float( Float2str( round( atof( temp.c_str() ) * 1000 ) / 1000.0 ) ) ;
        return_result = return_result + temp ;
        temp = "\0" ;
      } // if

      return_result = return_result + errorMessage[i] ;
    } // if

    else if ( errorMessage[i] == '\"' ) {
      temp = temp + errorMessage[i] ;
      i++ ;
      while ( errorMessage[i] != '\"' ) {
        if ( errorMessage[i] == '\\' && i + 1 < errorMessage.size() ) {
          i++ ;
          if ( errorMessage[i] == 'n' ) temp = temp + "\n" ;
          else if ( errorMessage[i] == 't' ) temp = temp + "\t" ;
          else if ( errorMessage[i] == '\"' ) temp = temp + "\"" ;
          else if ( errorMessage[i] == '\\' ) temp = temp + "\\" ;
          else temp = temp + errorMessage[i-1] + errorMessage[i] ;
        } // if

        else {
          temp = temp + errorMessage[i] ;
        } // else

        i++ ;
      } // while

      temp = temp + errorMessage[i] ;
      return_result = return_result + temp ;
      temp = "\0" ;
    } // else if

    else {
      temp = temp + errorMessage[i] ;
      string tempstr = "\0" ;
      if ( IsPrimitive_function( temp ) ) {
        for ( int z = 0 ; z < temp.size() ; z++ ) {
          if ( temp[z] == ' ' ) {
            for ( int s = z+1 ; s < temp.size()-1 ; s++ ) {
              tempstr = tempstr + temp[s] ;
            } // for ()
            
            temp = tempstr ;
            z = temp.size() ;
          } // if()
        } // for()
      } // if ()
    } // else 
  } // for

  if ( temp != "\0" ) return_result = return_result + temp ;
  return return_result ;
} // Error_print()

bool Eval_judge( string str, Tree_node *walk, string &result, Tree_node *root ) {
  int num = 0 ;
  string temp = "\0" ;

  if ( walk != NULL && walk -> right == NULL ) {
    if ( ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) && 
         walk -> left_str != "\0" && EvalSExp( walk, temp, false ) ) {
      if ( str == "#<procedure integer?>" && IsDigit( temp ) ) result = "#t" ;

      else if ( ( str == "#<procedure real?>" || str == "#<procedure number?>" ) && 
                ( IsDigit( temp ) || IsFloat( temp ) ) ) result = "#t" ;

      else if ( str == "#<procedure boolean?>" && ( temp == "#t" || temp == "nil" ) ) result = "#t" ;

      else if ( str == "#<procedure string?>" && temp[0] == '\"' && temp[temp.size()-1] == '\"' ) 
        result = "#t" ;

      else if ( str == "#<procedure null?>" && temp == "nil" ) result = "#t" ;

      else if ( ( str == "#<procedure atom?>" ) && 
                ( IsDigit( temp ) || IsFloat( temp ) || IsSymbol( temp ) || temp == "#t" || temp == "nil" ||
                  ( temp[0] == '\"' && temp[temp.size()-1] == '\"' ) ) ) result = "#t" ;

      else if ( ( str == "#<procedure symbol?>" ) && IsSymbol( temp ) ) result = "#t" ;

      else if ( ( str == "#<procedure pair?>" || str == "#<procedure list?>" ) && 
                ( ( temp[0] == '(' && temp[temp.size()-1] == ')' ) || 
                  temp == "()" || temp == "#f" || temp == "nil" ) ) {
        if ( ( str == "pair?" || str == "#<procedure pair?>" ) && 
             temp[0] == '(' && temp[temp.size()-1] == ')' ) result = "#t" ;
        else if ( ( str == "list?" || str == "#<procedure list?>" ) && 
                  temp[0] == '(' && temp[temp.size()-1] == ')' ) {
          result = "#t" ;
          for ( int i = 0 ; i < temp.size() ; i++ ) {
            if ( temp[i] == '.' && i+1 < temp.size() && temp[i+1] == '\n' ) {
              Tree_node *temp_tree = new Tree_node ;
              Init( temp_tree ) ;
              walk = temp_tree ;
              g_tokenList.clear() ;
              string str = "\0" ;

              for ( int i = 0 ; i < temp.size() ; i++ ) {
                if ( ( temp[i] == ' ' || temp[i] == '(' || temp[i] == ')' || temp[i] == '\"' ||
                       ( i+1 < temp.size() && temp[i-1] == ' ' && temp[i] == '.' && temp[i+1] == ' ' ) || 
                       temp[i] == '\n' ) && str != "#<procedure" ) {
                  if ( str != "\0" ) {
                    g_tokenList.push_back( str ) ;
                    str = "\0" ;
                  } // if

                  if ( temp[i] == '\"' ) {
                    str = str + temp[i] ;
                    i++ ;
                    while ( temp[i] != '\"' ) {
                      str = str + temp[i] ;

                      if ( temp[i] == '\\' ) {
                        if ( i + 1 < temp.size() ) {
                          i++ ;
                          str = str + temp[i] ;
                        } // if()
                      } // if()

                      i++ ;
                    } // while

                    str = str + temp[i] ;
                  } // if

                  else if ( temp[i] != ' ' && temp[i] != '\n' ) {
                    str = temp[i] ;
                    g_tokenList.push_back( str ) ;
                    str = "\0" ;
                  } // else if
                } // if

                else str = str + temp[i] ;
              } // for

              BuildTree( temp_tree ) ;
              Tree_node *walk_temp = temp_tree ;
              while ( walk_temp != NULL ) {
                if ( walk_temp -> right_str != "\0" && walk_temp -> right_str != "nil" ) {
                  result = "nil" ;
                  walk_temp = NULL ;
                } // if

                else walk_temp = walk_temp -> right ;
              } // while 
            } // if
              
          } // for
        } // else if

        else if ( ( str == "list?" || str == "#<procedure list?>" ) && 
                  ( temp == "()" || temp == "#f" || temp == "nil" ) ) {
          result = "#t" ;
        } // else if

        else result = "nil" ;
      } // else if

      else result = "nil" ;
      return true ;
    } // if

    else if ( walk -> right_str != "\0" && walk -> right == NULL && walk -> right_str != "nil" ) {
      if ( g_ErrorMessage == "\0" ) {
        bool new_line = false ;
        PrettyPrint( root, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
      } // if
        
      return false ; 
    } // else if

    else {
      Tree_node *temp_walk = walk ;
      temp = "\0" ;
      int index ;
      string value = "\0" ;
      while ( temp_walk -> left != NULL ) temp_walk = temp_walk -> left ;

      if ( temp_walk -> left_str == "#<procedure clean-environment>" || 
           temp_walk -> left_str == "#<procedure define>" || 
           temp_walk -> left_str == "#<procedure exit>" ||
           ( InList( temp_walk -> left_str, index, value ) && 
             ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
               value == "#<procedure exit>" ) ) ) {
        string now = "\0" ;
        if ( InList( temp_walk -> left_str, index, value ) ) now = value ;
        else now = temp_walk -> left_str ;

        if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) 
          g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
        else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" )
          g_ErrorMessage = "ERROR (level of DEFINE)" ;
        else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) 
          g_ErrorMessage = "ERROR (level of EXIT)" ;
        return false ;
      } // if()

      else if ( walk -> left != NULL && EvalSExp( walk -> left, temp, true ) ) {
        if ( ( str == "integer?" || str == "#<procedure integer?>" ) && IsDigit( temp ) ) result = "#t" ;
          
        else if ( ( str == "real?" || str == "number?" || str == "#<procedure real?>" || 
                    str == "#<procedure number?>" ) &&
                  ( IsDigit( temp ) || IsFloat( temp ) ) ) result = "#t" ;
                      
        else if ( ( str == "boolean?" || str == "#<procedure boolean?>" ) && 
                  ( temp == "#t" || temp == "nil" ) ) result = "#t" ;
            
        else if ( ( str == "string?" || str == "#<procedure string?>" ) && 
                  temp[0] == '\"' && temp[temp.size()-1] == '\"' ) result = "#t" ;
            
        else if ( ( str == "null?" || str == "#<procedure null?>" ) && temp == "nil" ) result = "#t" ;
            
        else if ( ( str == "atom?" || str == "#<procedure atom?>" ) && 
                  ( IsDigit( temp ) || IsFloat( temp ) || IsSymbol( temp ) || temp == "#t" ||
                    ( temp[0] == '\"' && temp[temp.size()-1] == '\"' ) || temp == "nil" ) ) 
          result = "#t" ;
              
        else if ( ( str == "symbol?" || str == "#<procedure symbol?>" ) && IsSymbol( temp ) ) result = "#t" ;

        else if ( ( str == "pair?" || str == "list?" || str == "#<procedure pair?>" || 
                    str == "#<procedure list?>" ) && 
                  ( ( temp[0] == '(' && temp[temp.size()-1] == ')' ) || 
                    temp == "()" || temp == "#f" || temp == "nil" ) ) {
          if ( ( str == "pair?" || str == "#<procedure pair?>" ) && temp[0] == '(' && 
               temp[temp.size()-1] == ')' ) result = "#t" ;

          else if ( ( str == "list?" || str == "#<procedure list?>" ) && 
                    temp[0] == '(' && temp[temp.size()-1] == ')' ) {
            result = "#t" ;
            for ( int i = 0 ; i < temp.size() ; i++ ) {
              if ( temp[i] == '.' && i+1 < temp.size() && temp[i+1] == '\n' ) {
                Tree_node *temp_tree = new Tree_node ;
                Init( temp_tree ) ;
                walk = temp_tree ;
                g_tokenList.clear() ;
                string str = "\0" ;

                for ( int i = 0 ; i < temp.size() ; i++ ) {
                  if ( ( temp[i] == ' ' || temp[i] == '(' || temp[i] == ')' || temp[i] == '\"' ||
                         ( i+1 < temp.size() && temp[i-1] == ' ' && temp[i] == '.' && temp[i+1] == ' ' ) || 
                         temp[i] == '\n' ) && str != "#<procedure" ) {
                    if ( str != "\0" ) {
                      g_tokenList.push_back( str ) ;
                      str = "\0" ;
                    } // if

                    if ( temp[i] == '\"' ) {
                      str = str + temp[i] ;
                      i++ ;
                      while ( temp[i] != '\"' ) {
                        str = str + temp[i] ;

                        if ( temp[i] == '\\' ) {
                          if ( i + 1 < temp.size() ) {
                            i++ ;
                            str = str + temp[i] ;
                          } // if()
                        } // if()

                        i++ ;
                      } // while

                      str = str + temp[i] ;
                    } // if

                    else if ( temp[i] != ' ' && temp[i] != '\n' ) {
                      str = temp[i] ;
                      g_tokenList.push_back( str ) ;
                      str = "\0" ;
                    } // else if
                  } // if

                  else str = str + temp[i] ;
                } // for

                BuildTree( temp_tree ) ;
                Tree_node *walk_temp = temp_tree ;
                while ( walk_temp != NULL ) {
                  if ( walk_temp -> right_str != "\0" && walk_temp -> right_str != "nil" ) {
                    result = "nil" ;
                    walk_temp = NULL ;
                  } // if

                  else walk_temp = walk_temp -> right ;
                } // while 
              } // if
                
            } // for
          } // else if

          else if ( ( str == "list?" || str == "#<procedure list?>" ) && 
                    ( temp == "()" || temp == "#f" || temp == "nil" ) ) result = "#t" ;

          else result = "nil" ;
        } // else if

        else result = "nil" ;
        return true ;
      } // else if

      else {
        if ( walk -> right != NULL || walk -> right_str != "\0" ) {
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( str ) ;
          while ( walk -> right != NULL ) walk = walk -> right ;
          if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
            g_pretty_str = "( " ;
            g_space = 2 ;
            g_first_need_lp = true ;
            bool l = false ;
            PrettyPrint( root, l ) ;
            g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
            g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
          } // if

          return false ;
        } // if ()

        else {
          if ( g_ErrorMessage != "\0" ) return false ;
          else {
            result = "nil" ;
            return true ;
          } // else
        } // else
      } // else 
    } // else
  } // if
    
  else {
    g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( str ) ;
    while ( walk -> right != NULL ) walk = walk -> right ;
    if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
      g_pretty_str = "( " ;
      g_space = 2 ;
      g_first_need_lp = true ;
      bool l = false ;
      PrettyPrint( root, l ) ;
      g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
      g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
    } // if

    return false ;
  } // else
} // Eval_judge()

bool Eval_define( Tree_node *walk, string &result, Tree_node *root ) {
  User_define temp ;
  string str, value ;
  int local_index = 0 ;
  if ( walk != NULL ) {
    if ( walk -> left == NULL && walk -> left_str != "\0" && walk -> right_str == "\0" && 
         walk -> right != NULL && IsSymbol( walk -> left_str ) && 
         !IsPrimitive_function( walk -> left_str ) ) {

      str = walk -> left_str ;
      walk = walk -> right ;
      if ( walk -> right == NULL && 
           ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) && 
           walk -> left_str != "\0" ) {
        int index = 0 ;
        int index1 = 0 ;

        if ( IsSymbol( walk -> left_str ) && ! InList( walk -> left_str, index1, value ) && 
             ! IsPrimitive_function( walk -> left_str ) ) {
          if ( g_ErrorMessage == "\0" ) 
            g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
          return false ;
        } // if ()

        else if ( InList( str, index, value ) && ! InList( walk -> left_str, index1, value ) ) {
          if ( IsFunction( walk -> left_str ) == false ) {
            g_define_list.erase( g_define_list.begin() + index ) ;
            temp.symbol = str ;
            temp.function = "\0" ;
            temp.result = walk -> left_str ;
            temp.index = g_define_list.size() ;
            temp.change = g_change ;
            g_define_list.push_back( temp ) ;
          } // if

          else {
            g_define_list.erase( g_define_list.begin() + index ) ;
            temp.symbol = str ;
            temp.function = walk -> left_str ;
            temp.result = walk -> left_str ;
            temp.index = g_define_list.size() ;
            temp.change = g_change ;
            g_define_list.push_back( temp ) ;
          } // else
        } // if ()

        else if ( InList( str, index, value ) && InList( walk -> left_str, index1, value ) ) {
          if ( IsFunction( walk -> left_str ) == false ) {
            g_define_list[index].result = g_define_list[index1].result ;
            g_define_list[index].function = "\0" ;
            g_define_list[index].index = g_define_list[index1].index ;
            g_define_list[index].change = g_define_list[index1].change ;
          } // if()

          else {
            g_define_list[index].function = g_define_list[index1].function ;
            g_define_list[index].result = g_define_list[index1].function ;
            g_define_list[index].index = g_define_list[index1].index ;
            g_define_list[index].change = g_define_list[index1].change ;
          } // else
        } // if ()

        else if ( ! InList( str, index, value ) && InList( walk -> left_str, index1, value ) ) {
          // cout << "!!!!!!!!!!!!!!!!@@@@@@@@@@@@@" << endl ;
          if ( IsFunction( walk -> left_str ) == false ) {
            temp.symbol = str ;
            temp.function = "\0" ;
            temp.result = g_define_list[index1].result ;
            temp.index = g_define_list[index1].index ;  
            temp.change = g_define_list[index1].change ;
            g_define_list.push_back( temp ) ;
          } // if ()

          else {
            temp.symbol = str ;
            temp.function = g_define_list[index1].function ;
            temp.result = g_define_list[index1].function ;
            temp.index = g_define_list[index1].index ;  
            temp.change = g_define_list[index1].change ;
            g_define_list.push_back( temp ) ;
          } // else
        } // else if ()

        else {
          if ( IsFunction( walk -> left_str ) == false ) {
            temp.symbol = str ;
            temp.function = "\0" ;
            temp.result = walk -> left_str ;
            temp.index = g_define_list.size() ; // default // 
            temp.change = g_change ;
            g_define_list.push_back( temp ) ;
          } // if()

          else {
            temp.symbol = str ;
            temp.function = walk -> left_str ;
            temp.result = walk -> left_str ;
            temp.index = g_define_list.size() ; // default // 
            temp.change = g_change ;
            g_define_list.push_back( temp ) ;
          } // else
        } // else ()

        result = str ;
        return true ;
      } // if

      else if ( walk -> right == NULL && walk-> right_str != "\0" && walk -> right_str != "nil" ) {
        if ( g_ErrorMessage == "\0" ) {
          bool new_line = false ;
          PrettyPrint( root, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        } // if
        
        return false ;
      } // else if
    
      else if ( walk -> right == NULL && walk -> left != NULL ) {
        Tree_node *temp_walk = walk ;
        int index ;
        string value = "\0" ;
        while ( temp_walk -> left != NULL ) temp_walk = temp_walk -> left ;

        if ( temp_walk -> left_str == "#<procedure clean-environment>" || 
             temp_walk -> left_str == "#<procedure define>" || 
             temp_walk -> left_str == "#<procedure exit>" ||
             ( InList( temp_walk -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( temp_walk -> left_str, index, value ) ) now = value ;
          else now = temp_walk -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) 
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" )
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) 
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          return false ;
        } // if()

        if ( EvalSExp( walk -> left, result, true ) ) {
          if ( walk -> right != NULL || ( walk -> right_str != "\0" && walk -> right_str != "nil" ) ) {
            if ( g_ErrorMessage == "\0" ) {
              bool new_line = false ;
              PrettyPrint( root, new_line ) ;
              g_ErrorMessage = "ERROR (DEFINE format) : " + Error_print( g_pretty_str ) ;
              g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
            } // if

            return false ;
          } // if

          else {
            int index ;
            while ( walk -> left != NULL ) walk = walk -> left ;
            if ( walk -> left_str == "#<procedure clean-environment>" || 
                 walk -> left_str == "#<procedure define>" || 
                 walk -> left_str == "#<procedure exit>" || 
                 ( InList( walk -> left_str, index, value ) && 
                   ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                     value == "#<procedure exit>" ) ) ) {
              string now = "\0" ;
              if ( InList( walk -> left_str, index, value ) ) now = value ;
              else now = walk -> left_str ;

              if ( now == "#<procedure clean-environment>" ) 
                g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
              else if ( now == "#<procedure define>" ) 
                g_ErrorMessage = "ERROR (level of DEFINE)" ;
              else if ( now == "#<procedure exit>" )
                g_ErrorMessage = "ERROR (level of EXIT)" ;
              return false ;
            } // if()

            if ( g_index != -100 && walk -> left_str != "#<procedure cons>" && 
                 walk -> left_str != "#<procedure list>" ) {
              int index = 0 ;
              int tempppp = g_index ;
              if ( InList( str, index, value ) ) {
                g_define_list[index].symbol = str ;
                g_define_list[index].function = "\0" ;
                g_define_list[index].result = result ;
                g_define_list[index].index = index ; // default // 
                g_define_list[index].change = g_change ;
              } // if ()

              else {
                temp.symbol = str ;
                temp.function = "\0" ;
                temp.result = result ;
                temp.index = index ; // default // 
                temp.change = g_change ;
                g_define_list.push_back( temp ) ;
              } // else

              g_index = -100 ;
            } // if()

            else {
              if ( IsFunction( result ) && g_change ) {
                if ( IsDefine( str, local_index ) ) 
                  g_define_list.erase( g_define_list.begin() + local_index ) ;

                temp.symbol = str ;
                temp.function = result ;
                temp.result = result ;
                temp.index = g_define_list.size() ; // default // 
                temp.change = g_change ;
                g_define_list.push_back( temp ) ;
              } // if ()

              else {
                if ( IsDefine( str, local_index ) ) 
                  g_define_list.erase( g_define_list.begin() + local_index ) ;

                temp.symbol = str ;
                temp.function = "\0" ;
                temp.result = result ;
                temp.index = g_define_list.size() ; // default // 
                temp.change = g_change ;
                g_define_list.push_back( temp ) ;
              } // else
            } // else

            result = str ;
            return true ;
          } // else

        } // if ()

        else { // ????????????????????? ex: (define a 3 4) 4??????????????
          if ( g_ErrorMessage == "\0" ) {
            bool new_line = false ;
            PrettyPrint( root, new_line ) ;
            g_ErrorMessage = "ERROR (DEFINE format) : " + Error_print( g_pretty_str ) ;
            g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
          } // if
          
          return false ;
        } // else
      } // else if

      else { // ????????????????????? ex: (define a 3 4) 4??????????????
        if ( g_ErrorMessage == "\0" ) {
          bool new_line = false ;
          PrettyPrint( root, new_line ) ;
          g_ErrorMessage = "ERROR (DEFINE format) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        } // if

        return false ;
      } // else
    } // if
    
    else {
      if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        if ( g_ErrorMessage == "\0" ) {
          bool new_line = false ;
          PrettyPrint( root, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        } // if
      } // if

      else {
        if ( g_ErrorMessage == "\0" ) {
          bool new_line = false ;
          PrettyPrint( root, new_line ) ;
          g_ErrorMessage = "ERROR (DEFINE format) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        } // if
      } // else 
      
      return false ;
    } // else
  } // if
  
  else {
    if ( g_ErrorMessage == "\0" ) {
      bool new_line = false ;
      PrettyPrint( root, new_line ) ;
      g_ErrorMessage = "ERROR (DEFINE format) : " + Error_print( g_pretty_str ) ;
      g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
    } // if

    return false ;
  } // else
} // Eval_define()

bool Eval_car( Tree_node *walk, string &result, Tree_node *root ) {
  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : car" ;
    return false ;
  } // if()

  else {
    string temp, value ;
    bool ss = false ;
    int index = -1 ;

    if ( walk -> left_str != "\0" || walk -> left != NULL ) {
      if ( walk -> right == NULL && 
           ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) && 
           IsPrimitive_function( walk -> left_str ) == false &&
           InList( walk -> left_str, index, value ) && walk -> left == NULL ) {

        Tree_node *temp_tree = new Tree_node ;
        Init( temp_tree ) ;
        walk = temp_tree ;
        g_tokenList.clear() ;
        string str = "\0" ;
        temp = value ;

        if ( temp.size() > 0 && temp[0] != '(' && temp[temp.size()-1] != ')' ) {
          if ( g_ErrorMessage == "\0" ) {
            if ( IsPrimitive_function( temp ) )
              g_ErrorMessage = "ERROR (car with incorrect argument type) : " + temp ;
            else 
              g_ErrorMessage = "ERROR (car with incorrect argument type) : " + Error_print( temp ) ;
          } // if

          return false ;
        } // if 

        // cout<<"pp= "<<g_pretty_str<<endl;
        for ( int i = 0 ; i < temp.size() ; i++ ) {
          if ( ( temp[i] == ' ' || temp[i] == '(' || temp[i] == ')' || temp[i] == '\"' ||
                 ( i+1 < temp.size() && temp[i-1] == ' ' && temp[i] == '.' && temp[i+1] == ' ' ) || 
                 temp[i] == '\n' ) && str != "#<procedure" ) {
            if ( str != "\0" ) {
              g_tokenList.push_back( str ) ;
              str = "\0" ;
            } // if

            if ( temp[i] == '\"' ) {
              str = str + temp[i] ;
              i++ ;
              while ( temp[i] != '\"' ) {
                str = str + temp[i] ;

                if ( temp[i] == '\\' ) {
                  if ( i + 1 < temp.size() ) {
                    i++ ;
                    str = str + temp[i] ;
                  } // if()
                } // if()

                i++ ;
              } // while

              str = str + temp[i] ;
            } // if

            else if ( temp[i] != ' ' && temp[i] != '\n' ) {
              str = temp[i] ;
              g_tokenList.push_back( str ) ;
              str = "\0" ;
            } // else if
          } // if

          else str = str + temp[i] ;
        } // for

        BuildTree( temp_tree ) ;

        bool new_line = false ;
        if ( temp_tree -> left != NULL ) {
          walk = temp_tree -> left ;
          g_pretty_str = "( " ;
          g_first_need_lp = true ;
          g_space = 2 ;
          g_first_need_lp = true ;
        } // if

        else {
          walk -> left_str = temp_tree -> left_str ;
          walk -> left = NULL ;
          walk -> right = NULL ;
          walk -> right_str = "\0" ;
          g_pretty_str = "\0" ;
          g_space = 0 ;
          g_first_need_lp = false ;
        } // else

        PrettyPrint( walk, new_line ) ;
        if ( g_pretty_str.size() > 0 ) {
          result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
          return true ;
        } // if
        
        else {
          if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (car with incorrect argument type) : nil" ;
          return false ;
        } // else
      } // if()

      else if ( walk -> right == NULL && walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        g_pretty_str = "( " ;
        g_space = 2 ;
        bool new_line = false ;
        g_first_need_lp = true ;
        PrettyPrint( root, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        return false ;
      } // else if()

      else if ( walk -> right == NULL && walk -> right_str == "\0" && walk -> left == NULL ) {
        int ii = 0 ;
        if ( IsDigit( walk -> left_str ) || IsFloat( walk -> left_str ) || 
             ( walk -> left_str[0] == '\"' && walk -> left_str[walk -> left_str.size()-1] == '\"' ) ||
             walk -> left_str == "#t" || walk -> left_str == "nil" || IsFunction( walk -> left_str ) ) {
          if ( g_ErrorMessage == "\0" && IsFunction( walk -> left_str ) ) 
            // g_ErrorMessage = "ERROR (car with incorrect argument type) : #<procedure " + 
            //                  walk -> left_str + ">" ;
            g_ErrorMessage = "ERROR (car with incorrect argument type) : " + walk -> left_str ;
          
          else if ( g_ErrorMessage == "\0" )
            g_ErrorMessage = "ERROR (car with incorrect argument type) : " + 
                             Error_print( walk -> left_str ) ;
        } // if

        else if ( !InList( walk -> left_str, ii, value ) ) {
          if ( g_ErrorMessage == "\0" ) 
            g_ErrorMessage = "ERROR (unbound symbol) : " + ( walk -> left_str ) ;
        } // else if
        
        return false ;
      } // else if

      else {
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;
        if ( tw -> left_str == "#<procedure clean-environment>" || 
             tw -> left_str == "#<procedure define>" || 
             tw -> left_str == "#<procedure exit>" || 
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( walk -> left != NULL && EvalSExp( walk -> left, temp, true ) ) {
          if ( walk -> right != NULL ) {
            if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : car" ;
            return false ;
          } // if() 

          else {
            if ( temp[0] == '(' && temp[temp.size()-1] == ')' ) {
              Tree_node *temp_tree = new Tree_node ;
              Init( temp_tree ) ;
              walk = temp_tree ;
              g_tokenList.clear() ;
              string str = "\0" ;

              for ( int i = 0 ; i < temp.size() ; i++ ) {
                if ( ( temp[i] == ' ' || temp[i] == '(' || temp[i] == ')' || temp[i] == '\"' ||
                       ( i+1 < temp.size() && temp[i-1] == ' ' && temp[i] == '.' && temp[i+1] == ' ' ) || 
                       temp[i] == '\n' ) && str != "#<procedure" ) {
                  if ( str != "\0" ) {
                    g_tokenList.push_back( str ) ;
                    str = "\0" ;
                  } // if

                  if ( temp[i] == '\"' ) {
                    str = str + temp[i] ;
                    i++ ;
                    while ( temp[i] != '\"' ) {
                      str = str + temp[i] ;

                      if ( temp[i] == '\\' ) {
                        if ( i + 1 < temp.size() ) {
                          i++ ;
                          str = str + temp[i] ;
                        } // if()
                      } // if()

                      i++ ;
                    } // while

                    str = str + temp[i] ;
                  } // if

                  else if ( temp[i] != ' ' && temp[i] != '\n' ) {
                    str = temp[i] ;
                    g_tokenList.push_back( str ) ;
                    str = "\0" ;
                  } // else if
                } // if

                else str = str + temp[i] ;
              } // for

              BuildTree( temp_tree ) ;

              bool new_line = false ;

              if ( temp_tree -> left != NULL ) {
                walk = temp_tree -> left ;
                g_pretty_str = "( " ;
                g_space = 2 ;
                g_first_need_lp = true ;
              } // if

              else {
                walk -> left_str = temp_tree -> left_str ;
                walk -> left = NULL ;
                walk -> right = NULL ;
                walk -> right_str = "\0" ;
                g_pretty_str = "\0" ;
                g_space = 0 ;
                g_first_need_lp = false ;
              } // else

              PrettyPrint( walk, new_line ) ;
              if ( g_pretty_str.size() > 0 ) {
                result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
                return true ;
              } // if
              
              else {
                if ( g_ErrorMessage == "\0" ) 
                  g_ErrorMessage = "ERROR (car with incorrect argument type) : nil" ;
                return false ;
              } // else
            } // if()

            else {
              if ( g_ErrorMessage == "\0" ) 
                g_ErrorMessage = "ERROR (car with incorrect argument type) : " + Error_print( temp ) ;
              return false ;
            } // else 
          } // else
        } // else if

        else {
          if ( walk -> right != NULL && walk -> right_str == "\0" ) {
            if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : car" ;
          } // if
            
          return false ;
        } // else 
      } // else 
    } // if() 
  } // else 

  return false ;
} // Eval_car()

bool Eval_cdr( Tree_node *walk, string &result, Tree_node *root ) {
  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : cdr" ;
    return false ;
  } // if()

  else {
    string temp, value ;
    bool ss = false ;
    int index = -1 ;

    if ( walk -> left_str != "\0" || walk -> left != NULL ) {
      if ( walk -> right == NULL && walk -> right_str == "\0" && 
           IsPrimitive_function( walk -> left_str ) == false &&
           InList( walk -> left_str, index, value ) && walk -> left == NULL ) {

        Tree_node *temp_tree = new Tree_node ;
        Init( temp_tree ) ;
        walk = temp_tree ;
        g_tokenList.clear() ;
        string str = "\0" ;
        temp = value ;

        if ( temp.size() > 0 && temp[0] != '(' && temp[temp.size()-1] != ')' ) {
          if ( g_ErrorMessage == "\0" ) {
            if ( IsPrimitive_function( temp ) )
              g_ErrorMessage = "ERROR (cdr with incorrect argument type) : " + temp ;
            else g_ErrorMessage = "ERROR (cdr with incorrect argument type) : " + Error_print( temp ) ;
          } // if

          return false ;
        } // if

        for ( int i = 0 ; i < temp.size() ; i++ ) {
          if ( ( temp[i] == ' ' || temp[i] == '(' || temp[i] == ')' || temp[i] == '\"' ||
                 ( i+1 < temp.size() && temp[i-1] == ' ' && temp[i] == '.' && temp[i+1] == ' ' ) || 
                 temp[i] == '\n' ) && str != "#<procedure" ) {
            if ( str != "\0" ) {
              g_tokenList.push_back( str ) ;
              str = "\0" ;
            } // if

            if ( temp[i] == '\"' ) {
              str = str + temp[i] ;
              i++ ;
              while ( temp[i] != '\"' ) {
                str = str + temp[i] ;

                if ( temp[i] == '\\' ) {
                  if ( i + 1 < temp.size() ) {
                    i++ ;
                    str = str + temp[i] ;
                  } // if()
                } // if()

                i++ ;
              } // while

              str = str + temp[i] ;
            } // if

            else if ( temp[i] != ' ' && temp[i] != '\n' ) {
              str = temp[i] ;
              g_tokenList.push_back( str ) ;
              str = "\0" ;
            } // else if
          } // if

          else str = str + temp[i] ;
        } // for

        BuildTree( temp_tree ) ;
        
        walk = temp_tree ;
        int l = 0 ;
        bool jump = false ;
        if ( walk -> left == NULL && walk -> right != NULL ) walk = walk -> right ;

        else if ( walk -> left != NULL && walk -> right == NULL && walk -> right_str == "\0" ) {
          result = "nil" ;
          return true ;
        } // else if() 

        else if ( walk -> left_str != "\0" && walk -> left == NULL && walk -> right == NULL &&
                  walk -> right_str == "\0" ) {
          result = "nil" ;
          return true ;
        } // else if()

        else if ( walk -> left != NULL ) {
          while ( walk -> left != NULL && jump == false ) {
            if ( walk -> right != NULL ) {
              walk -> left = NULL ;
              jump = true ;
              walk = walk -> right ;
            } // if ()

            else if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
              walk -> left = NULL ;
              jump = true ; 
              walk -> left_str = walk -> right_str ;
              walk -> right_str = "\0" ;
              g_pretty_str = "\0" ;
              g_space = 0 ;
              bool new_line = false ;
              g_first_need_lp = false ;
              PrettyPrint( walk, new_line ) ;
              if ( g_pretty_str.size() > 0 ) {
                result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
                return true ;
              } // if
              
              else {
                if ( g_ErrorMessage == "\0" ) 
                  g_ErrorMessage = "ERROR (cdr with incorrect argument type) : nil" ;
                return false ;
              } // else
            } // else if ()

            else {
              walk = walk -> left ;
              l++ ;
            } // else
          } // while ()
        } // else if()

        else if ( walk -> left == NULL && walk -> right == NULL && walk -> right_str != "\0" ) {
          walk -> left_str = walk -> right_str ;
          walk -> right_str = "\0" ;
          bool new_line = false ;
          g_first_need_lp = false ;
          g_pretty_str = "\0" ;
          g_space = 0 ;
          PrettyPrint( walk, new_line ) ;
          if ( g_pretty_str.size() > 0 ) {
            result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
            return true ;
          } // if
          
          else {
            if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (cdr with incorrect argument type) : nil" ;
            return false ;
          } // else
        } // else if ()

        else if ( walk -> left == NULL && walk -> left_str == "\0" &&
                  walk -> right == NULL && 
                  ( walk -> right_str == "\0" || 
                    ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) ) {
          if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (cdr with incorrect argument type) : nil" ;
          return false ;
        } // else if

        bool new_line = false ;
        g_pretty_str = "( " ;
        g_space = 2 ;
        g_first_need_lp = true ;
        PrettyPrint( walk, new_line ) ;
        if ( g_pretty_str.size() > 0 ) {
          result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
          return true ;
        } // if
        
        else {
          if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (cdr with incorrect argument type) : nil" ;
          return false ;
        } // else
      } // if()

      else if ( walk -> right == NULL && walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        bool new_line = false ;
        PrettyPrint( root, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        return false ;
      } // else if()

      else if ( walk -> right == NULL && walk -> right_str == "\0" && walk -> left == NULL ) {
        int ii = 0 ;
        if ( IsDigit( walk -> left_str ) || IsFloat( walk -> left_str ) || 
             ( walk -> left_str[0] == '\"' && walk -> left_str[walk -> left_str.size()-1] == '\"' ) ||
             walk -> left_str == "#t" || walk -> left_str == "nil" || IsFunction( walk -> left_str ) ) {
          if ( g_ErrorMessage == "\0" && IsFunction( walk -> left_str ) ) 
            // g_ErrorMessage = "ERROR (cdr with incorrect argument type) : #<procedure " + 
            //                  walk -> left_str + ">" ;
            g_ErrorMessage = "ERROR (cdr with incorrect argument type) : " + walk -> left_str ;
          
          else if ( g_ErrorMessage == "\0" )
            g_ErrorMessage = "ERROR (cdr with incorrect argument type) : " + 
                             Error_print( walk -> left_str ) ;
        } // if

        else if ( !InList( walk -> left_str, ii, value ) ) {
          if ( g_ErrorMessage == "\0" ) 
            g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
        } // else if
        
        return false ;
      } // else if

      else {
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;
        if ( tw -> left_str == "#<procedure clean-environment>" || tw -> left_str == "#<procedure define>" ||
             tw -> left_str == "#<procedure exit>" ||
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( walk -> left != NULL && EvalSExp( walk -> left, temp, true ) ) {
          if ( walk -> right != NULL ) {
            if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : cdr" ;
            return false ;
          } // if()

          else {
            if ( temp[0] == '(' && temp[temp.size()-1] == ')' ) {
              Tree_node *temp_tree = new Tree_node ;
              Init( temp_tree ) ;
              g_tokenList.clear() ;
              string str = "\0" ;

              for ( int i = 0 ; i < temp.size() ; i++ ) {
                if ( ( temp[i] == ' ' || temp[i] == '(' || temp[i] == ')' || temp[i] == '\"' ||
                       ( i+1 < temp.size() && temp[i-1] == ' ' && temp[i] == '.' && temp[i+1] == ' ' ) || 
                       temp[i] == '\n' ) && str != "#<procedure" ) {
                  if ( str != "\0" ) {
                    g_tokenList.push_back( str ) ;
                    str = "\0" ;
                  } // if

                  if ( temp[i] == '\"' ) {
                    str = str + temp[i] ;
                    i++ ;
                    while ( temp[i] != '\"' ) {
                      str = str + temp[i] ;

                      if ( temp[i] == '\\' ) {
                        if ( i + 1 < temp.size() ) {
                          i++ ;
                          str = str + temp[i] ;
                        } // if()
                      } // if()

                      i++ ;
                    } // while

                    str = str + temp[i] ;
                  } // if

                  else if ( temp[i] != ' ' && temp[i] != '\n' ) {
                    str = temp[i] ;
                    g_tokenList.push_back( str ) ;
                    str = "\0" ;
                  } // else if
                } // if

                else str = str + temp[i] ;
              } // for

              BuildTree( temp_tree ) ;

              walk = temp_tree ;
              int l = 0 ;
              bool jump = false ;
              
              if ( walk -> left == NULL && walk -> right != NULL ) walk = walk -> right ;
              else if ( walk -> left != NULL && walk -> right == NULL && walk -> right_str == "\0" ) {
                result = "nil" ;
                return true ;
              } // else if() 

              else if ( walk -> left_str != "\0" && walk -> left == NULL && walk -> right == NULL &&
                        walk -> right_str == "\0" ) {
                result = "nil" ;
                return true ;
              } // else if()

              else if ( walk -> left != NULL ) {
                while ( walk -> left != NULL && jump == false ) {
                  if ( walk -> right != NULL ) {
                    walk -> left = NULL ;
                    jump = true ;
                    walk = walk -> right ;
                  } // if ()

                  else if ( walk -> right_str != "\0" ) {
                    walk -> left = NULL ;
                    jump = true ; 
                    walk -> left_str = walk -> right_str ;
                    walk -> right_str = "\0" ;
                    g_pretty_str = "\0" ;
                    g_space = 0 ;
                    bool new_line = false ;
                    g_first_need_lp = false ;
                    PrettyPrint( walk, new_line ) ;
                    if ( g_pretty_str.size() > 0 ) {
                      result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
                      return true ;
                    } // if
                    
                    else {
                      if ( g_ErrorMessage == "\0" ) 
                        g_ErrorMessage = "ERROR (cdr with incorrect argument type) : nil" ;
                      return false ;
                    } // else
                  } // else if ()

                  else {
                    walk = walk -> left ;
                    l++ ;
                  } // else
                } // while ()
              } // else if()

              else if ( walk -> left == NULL && walk -> right == NULL && walk -> right_str != "\0" ) {
                walk -> left_str = walk -> right_str ;
                walk -> right_str = "\0" ;
                g_pretty_str = "\0" ;
                g_space = 0 ;
                bool new_line = false ;
                g_first_need_lp = false ;
                PrettyPrint( walk, new_line ) ;
                if ( g_pretty_str.size() > 0 ) {
                  result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
                  return true ;
                } // if
                
                else {
                  if ( g_ErrorMessage == "\0" ) 
                    g_ErrorMessage = "ERROR (cdr with incorrect argument type) : nil" ;
                  return false ;
                } // else
              } // else if ()
              
              bool new_line = false ;
              g_pretty_str = "( " ;
              g_space = 2 ;
              g_first_need_lp = true ;
              PrettyPrint( walk, new_line ) ;
              // cout << "g_pretty_str::: " << g_pretty_str << endl ;
              if ( g_pretty_str.size() > 0 ) {
                result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
                return true ;
              } // if
              
              else {
                if ( g_ErrorMessage == "\0" ) 
                  g_ErrorMessage = "ERROR (cdr with incorrect argument type) : nil" ;
                return false ;
              } // else
            } // if()

            else {
              if ( g_ErrorMessage == "\0" ) 
                g_ErrorMessage = "ERROR (cdr with incorrect argument type) : " + Error_print( temp ) ;
              return false ;
            } // else 
          } // else 
        } // else if

        else {
          if ( walk -> right != NULL && walk -> right_str == "\0" ) {
            if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : cdr" ;
          } // if

          return false ;
        } // else
      } // else
    } // if() 
  } // else 

  return false ;
} // Eval_cdr()

bool Eval_cons( Tree_node *walk, string &result, Tree_node *root ) {
  int n = 0 ;
  Tree_node *node1 = NULL ;
  Tree_node *node2 = NULL ;
  Tree_node *pre = walk ;
  string value ;
  while ( walk != NULL ) {
    if ( n < 2 ) {
      if ( node1 == NULL ) node1 = walk ;
      else node2 = walk ; 
      n++ ;
    } // if

    else n++ ;
    pre = walk ;
    walk = walk -> right ;
  } // while

  Tree_node *root_tempp = new Tree_node ;
  Init( root_tempp ) ;

  if ( pre -> right_str != "\0" && pre -> right_str != "nil" ) {
    if ( g_ErrorMessage == "\0" ) {
      bool new_line = false ;
      PrettyPrint( root, new_line ) ;
      g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
      g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
    } // if

    return false ;
  } // if

  else if ( n != 2 ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : cons" ;
    return false ;
  } // if

  else {

    n = 1 ;
    while ( n <= 2 ) {
      int index = 0 ;
      bool ss = true ;
      string s = "\0" ;
      Tree_node *node ;
      if ( n == 1 ) {
        s = node1 -> left_str ;
        node = node1 ;
        node -> right = NULL ;
      } // if

      else {
        s = node2 -> left_str ;
        node = node2 ;
        node -> right = NULL ;
      } // else

      string temp = "\0" ;

      if ( InList( node -> left_str, index, value ) || IsFunction( node -> left_str ) ) {
        if ( InList( node -> left_str, index, value ) ) s = value ;
        ss = false ;
      } // if

      else if ( node -> left != NULL ) {
        node = node -> left ;
        ss = true ;
      } // else if

      if ( IsDigit( s ) || IsFloat( s ) || ( s[0] == '\"' && s[s.size()-1] == '\"' ) || 
           s == "#t" || s == "nil" || s == "#f" || s == "()" || IsPrimitive_function( s ) ) {
        if ( s == "nil" || s == "#f" || s == "()" ) {
          if ( n == 2 ) s = "\0" ;
          else s = "nil" ;
        } // if

        if ( n == 1 ) root_tempp -> left_str = s ;
        else root_tempp -> right_str = s ;
        n++ ;
      } // if

      else {
        Tree_node *temp_walk = node ;
        while ( temp_walk -> left != NULL ) temp_walk = temp_walk -> left ;

        if ( temp_walk -> left_str == "#<procedure clean-environment>" || 
             temp_walk -> left_str == "#<procedure define>" || 
             temp_walk -> left_str == "#<procedure exit>" ||
             ( InList( temp_walk -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( temp_walk -> left_str, index, value ) ) now = value ;
          else now = temp_walk -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()
          
          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else {
          if ( EvalSExp( node, temp, ss ) ) {
            if ( IsDigit( temp ) || IsFloat( temp ) || ( temp[0] == '\"' && temp[temp.size()-1] == '\"' ) || 
                 temp == "#t" || temp == "nil" || temp == "#f" || temp == "()" ) {
              if ( temp == "nil" || temp == "#f" || temp == "()" ) {
                if ( n == 2 ) temp = "\0" ;
                else temp = "nil" ;
              } // if

              if ( n == 1 ) root_tempp -> left_str = temp ;
              else root_tempp -> right_str = temp ;
            } // if 

            else {
              Tree_node *temp_tree = new Tree_node ;
              Init( temp_tree ) ;
              g_tokenList.clear() ;
              string str = "\0" ;
              for ( int i = 0 ; i < temp.size() ; i++ ) {
                if ( ( temp[i] == ' ' || temp[i] == '(' || temp[i] == ')' || temp[i] == '\"' ||
                       ( i+1 < temp.size() && temp[i-1] == ' ' && temp[i] == '.' && temp[i+1] == ' ' ) || 
                       temp[i] == '\n' ) && str != "#<procedure" ) {
                  if ( str != "\0" ) {
                    g_tokenList.push_back( str ) ;
                    str = "\0" ;
                  } // if

                  if ( temp[i] == '\"' ) {
                    str = str + temp[i] ;
                    i++ ;
                    while ( temp[i] != '\"' ) {
                      str = str + temp[i] ;

                      if ( temp[i] == '\\' ) {
                        if ( i + 1 < temp.size() ) {
                          i++ ;
                          str = str + temp[i] ;
                        } // if()
                      } // if()

                      i++ ;
                    } // while

                    str = str + temp[i] ;
                  } // if

                  else if ( temp[i] != ' ' && temp[i] != '\n' ) {
                    str = temp[i] ;
                    g_tokenList.push_back( str ) ;
                    str = "\0" ;
                  } // else if
                } // if

                else str = str + temp[i] ;
              } // for

              if ( str != "\0" ) g_tokenList.push_back( str ) ;

              BuildTree( temp_tree ) ;
              if ( n == 1 ) {
                if ( temp[0] == '(' && temp[temp.size()-1] == ')' ) root_tempp -> left = temp_tree ;
                else root_tempp -> left_str = temp_tree -> left_str ;
              } // if ()

              else {
                if ( temp[0] == '(' && temp[temp.size()-1] == ')' ) root_tempp -> right = temp_tree ;
                else root_tempp -> right_str = temp_tree -> left_str ; 
              } // else 
            } // else

            n++ ;
          } // if

          else {
            return false ;
          } // else
        } // else
      } // else 
    } // while

    bool new_line = false ;
    g_pretty_str = "( " ;
    g_space = 2 ;
    g_first_need_lp = true ;
    PrettyPrint( root_tempp, new_line ) ;
    if ( g_pretty_str.size() > 0 ) result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
    else result = "nil" ;
    return true ;
  } // else
} // Eval_cons()

bool Eval_quote( Tree_node *walk, string &result ) {
  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : quote" ;
    return false ;
  } // if

  else {
    g_pretty_str = "\0" ;
    g_space = 0 ;
    g_first_need_lp = true ;
    bool new_line = false ;
    PrettyPrint( walk, new_line ) ;
    result = g_pretty_str.substr( 0, g_pretty_str.size()-3 ) ;
    return true ;
  } // else
} // Eval_quote()

bool Eval_cmp( Tree_node *walk, string &result ) {
  Tree_node *root = walk ;
  string op = walk -> left_str ;
  string temp = "\0" ;
  int num = 0 ;
  float n1 = 0.0, n2 = 0.0 ;

  Tree_node *temp_walk = walk ;
  if ( walk -> right != NULL ) temp_walk = temp_walk -> right ;

  int para_num = 1;
  while ( temp_walk -> right != NULL ) {
    para_num++ ;
    temp_walk = temp_walk -> right ;
  } // while

  if ( para_num < 2 ) {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( op ) ;
    return false ;
  } // if ()

  while ( walk -> right != NULL ) {
    walk = walk -> right ;
    temp = "\0" ;
    num++ ;
    if ( walk -> left != NULL ) {
      Tree_node *tw = walk ;
      int index ;
      string value = "\0" ;
      while ( tw -> left != NULL ) tw = tw -> left ;
      if ( tw -> left_str == "#<procedure clean-environment>" || tw -> left_str == "#<procedure define>" || 
           tw -> left_str == "#<procedure exit>" || 
           ( InList( tw -> left_str, index, value ) && 
             ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
               value == "#<procedure exit>" ) ) ) {
        string now = "\0" ;
        if ( InList( tw -> left_str, index, value ) ) now = value ;
        else now = tw -> left_str ;

        if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
          g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
        } // if()

        else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
          g_ErrorMessage = "ERROR (level of DEFINE)" ;
        } // else if()

        else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
          g_ErrorMessage = "ERROR (level of EXIT)" ;
        } // else if

        return false ;
      } // if()

      else if ( EvalSExp( walk -> left, temp, true ) ) {
        if ( !IsDigit( temp ) && !IsFloat( temp ) ) {
          if ( g_ErrorMessage == "\0" ) {
            if ( IsPrimitive_function( temp ) )
              g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " + temp ;
            else {
              g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " ;
              g_ErrorMessage = g_ErrorMessage + Error_print( temp ) ;
            } // else 
          } // if ()

          return false ;
        } // if

        if ( num == 1 ) n1 = atof( temp.c_str() ) ;

        else {
          if ( num >= 2 ) {
            if ( num > 2 ) n1 = n2 ;
            n2 = atof( temp.c_str() ) ;
          } // if

          if ( ( op == "#<procedure >>" ) && n1 > n2 && result != "nil" ) result = "#t" ;
          else if ( ( op == "#<procedure >=>" ) && n1 >= n2 && result != "nil" ) 
            result = "#t" ;
          else if ( ( op == "#<procedure <>" ) && n1 < n2 && result != "nil" ) result = "#t" ;
          else if ( ( op == "#<procedure <=>" ) && n1 <= n2 && result != "nil" ) 
            result = "#t" ;
          else if ( ( op == "#<procedure =>" ) && n1 == n2 && result != "nil" ) result = "#t" ;
          else result = "nil" ;
        } // else
      } // else if

      else return false ;
    } // if

    else if ( walk -> left_str != "\0" ) {
      int index = 0 ;
      string n = "\0" ;
      string value ;

      if ( IsDigit( walk -> left_str ) || IsFloat( walk -> left_str ) ) n = walk -> left_str ;

      else if ( InList( walk -> left_str, index, value ) ) {
        n = value ;
        if ( !IsDigit( n ) && !IsFloat( n ) ) {
          if ( g_ErrorMessage == "\0" ) {
            if ( IsPrimitive_function( n ) )
              g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " + n ;
            else 
              g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " + 
                               Error_print( n ) ;
          } // if ()

          return false ;
        } // if
      } // else if

      else {
        if ( ( walk -> left_str[0] == '\"' && walk -> left_str[walk -> left_str.size()-1] == '\"' ) || 
             walk -> left_str == "#t" || walk -> left_str == "nil" || IsFunction( walk -> left_str ) ) {
          if ( g_ErrorMessage == "\0" && IsFunction( walk -> left_str ) ) {
            g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " + 
                             walk -> left_str ;
          } // if ()

          else g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " + 
                                Error_print( walk -> left_str ) ;
        } // if

        else {
          if ( g_ErrorMessage == "\0" ) 
            g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
        } // else

        return false ;
      } // else

      if ( num == 1 ) n1 = atof( n.c_str() ) ;

      else {
        if ( num >= 2 ) {
          if ( num > 2 ) n1 = n2 ;
          n2 = atof( n.c_str() ) ;
        } // if

        if ( ( op == "#<procedure >>" ) && n1 > n2 && result != "nil" ) result = "#t" ;
        else if ( ( op == "#<procedure >=>" ) && n1 >= n2 && result != "nil" ) result = "#t" ;
        else if ( ( op == "#<procedure <>" ) && n1 < n2 && result != "nil" ) result = "#t" ;
        else if ( ( op == "#<procedure <=>" ) && n1 <= n2 && result != "nil" ) result = "#t" ;
        else if ( ( op == "#<procedure =>" ) && n1 == n2 && result != "nil" ) result = "#t" ;
        else result = "nil" ;
      } // else
    } // else if
  } // while

  if ( num < 2 ) {
    if ( g_ErrorMessage == "\0" ) {
      if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        bool new_line = false ;
        PrettyPrint( root, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
      } // if

      else g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( op ) ;
    } // if

    return false ;
  } // if
  
  else if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
    if ( g_ErrorMessage == "\0" ) {
      bool new_line = false ;
      PrettyPrint( root, new_line ) ;
      g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
      g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
    } // if

    return false ;
  } // else if

  return true ;
} // Eval_cmp()

bool Eval_string( Tree_node *walk, string &result ) {
  Tree_node *root = walk ;
  string op = walk -> left_str ;
  int num = 0 ;
  string str1 = "\0", str2 = "\0" ;

  Tree_node *temp_walk = walk ;
  if ( walk -> right != NULL ) temp_walk = temp_walk -> right ;

  int para_num = 1;
  while ( temp_walk -> right != NULL ) {
    para_num++ ;
    temp_walk = temp_walk -> right ;
  } // while

  if ( para_num < 2 ) {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( op ) ;
    return false ;
  } // if ()

  while ( walk -> right != NULL ) {
    walk = walk -> right ;
    num++ ;
    if ( walk -> left != NULL ) {
      string temp = "\0" ;
      Tree_node *tw = walk ;
      while ( tw -> left != NULL ) tw = tw -> left ;
      int index ;
      string value = "\0" ;
      if ( tw -> left_str == "#<procedure clean-environment>" || tw -> left_str == "#<procedure define>" || 
           tw -> left_str == "#<procedure exit>" || 
           ( InList( tw -> left_str, index, value ) && 
             ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
               value == "#<procedure exit>" ) ) ) {
        string now = "\0" ;
        if ( InList( tw -> left_str, index, value ) ) now = value ;
        else now = tw -> left_str ;

        if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
          g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
        } // if()

        else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
          g_ErrorMessage = "ERROR (level of DEFINE)" ;
        } // else if()
        
        else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
          g_ErrorMessage = "ERROR (level of EXIT)" ;
        } // else if

        return false ;
      } // if()

      else if ( EvalSExp( walk -> left, temp, true ) ) {
        if ( temp[0] != '\"' || temp[temp.size()-1] != '\"' ) {
          if ( g_ErrorMessage == "\0" ) {
            if ( IsPrimitive_function( temp ) )
              g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " + temp ;
            else {
              g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " ; 
              g_ErrorMessage = g_ErrorMessage + Error_print( temp ) ;
            } // else
          } // if ()

          return false ;
        } // if

        if ( num == 1 ) str1 = temp ;

        else {
          if ( num >= 2 ) {
            if ( num > 2 ) str1 = str2 ;
            str2 = temp ;
          } // if 
          
          if ( ( op == "string>?" || op == "#<procedure string>?>" ) && 
               str1 > str2 && result != "nil" ) result = "#t" ;
          else if ( ( op == "string<?" || op == "#<procedure string<?>" ) && 
                    str1 < str2 && result != "nil" ) result = "#t" ;
          else if ( ( op == "string=?" || op == "#<procedure string=?>" ) && 
                    str1 == str2 && result != "nil" ) result = "#t" ;
          else if ( op == "string-append" || op == "#<procedure string-append>" ) {
            if ( result == "\0" ) 
              result = result + str1.substr( 0, str1.size()-1 ) + str2.substr( 1, str2.size()-1 ) ;
            else result = result.substr( 0, result.size()-1 ) + str2.substr( 1, str2.size()-1 ) ; 
          } // else if

          else result = "nil" ;
        } // else
      } // else if

      else return false ;
    } // if

    else if ( walk -> left_str != "\0" ) {
      int index = 0 ;
      string str = "\0" ;
      string value ;

      if ( walk -> left_str[0] == '\"' && walk -> left_str[walk -> left_str.size()-1] == '\"' ) 
        str = walk -> left_str ;

      else if ( InList( walk -> left_str, index, value ) ) {
        str = value ;
        if ( str[0] != '\"' || str[str.size()-1] != '\"' ) {
          if ( g_ErrorMessage == "\0" )
            g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " + str ;
          return false ;
        } // if
      } // else if

      else {
        if ( IsDigit( walk -> left_str ) || IsFloat( walk -> left_str ) || walk -> left_str == "#t" || 
             walk -> left_str == "nil" || IsFunction( walk -> left_str ) ) {
          if ( g_ErrorMessage == "\0" && IsFunction( walk -> left_str ) ) 
            g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " + 
                             walk -> left_str ;

          else if ( g_ErrorMessage == "\0" )
            g_ErrorMessage = "ERROR (" + Error_print( op ) + " with incorrect argument type) : " + 
                             Error_print( walk -> left_str ) ;
        } // if

        else {
          if ( g_ErrorMessage == "\0" ) 
            g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
        } // else

        return false ;
      } // else

      if ( num == 1 ) str1 = str ;

      else {
        if ( num >= 2 ) {
          if ( num > 2 ) str1 = str2 ;
          str2 = str ;
        } // if
        
        if ( ( op == "string>?" || op == "#<procedure string>?>" ) && 
             str1 > str2 && result != "nil" ) result = "#t" ;
        else if ( ( op == "string<?" || op == "#<procedure string<?>" ) && 
                  str1 < str2 && result != "nil" ) result = "#t" ;
        else if ( ( op == "string=?" || op == "#<procedure string=?>" ) && 
                  str1 == str2 && result != "nil" ) result = "#t" ;
        else if ( op == "string-append" || op == "#<procedure string-append>" ) {
          if ( result == "\0" ) 
            result = result + str1.substr( 0, str1.size()-1 ) + str2.substr( 1, str2.size()-1 ) ;
          else result = result.substr( 0, result.size()-1 ) + str2.substr( 1, str2.size()-1 ) ; 
        } // else if

        else result = "nil" ;
      } // else
    } // else if
  } // while

  if ( num < 2 ) {
    if ( g_ErrorMessage == "\0" ) {
      if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        bool new_line = false ;
        PrettyPrint( root, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
      } // if

      else g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( op ) ;
    } // if

    return false ;
  } // if
  
  else if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
    if ( g_ErrorMessage == "\0" ) {
      bool new_line = false ;
      PrettyPrint( root, new_line ) ;
      g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
      g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
    } // if

    return false ;
  } // else if
  
  return true ;
} // Eval_string()

bool Eval_list( Tree_node *walk, string &result, Tree_node *root ) {
  int n = 0 ;
  int ii = 0 ;
  Tree_node *root_tempp = new Tree_node ;
  Init( root_tempp ) ;
  Tree_node *node = root_tempp ;
  string temp = "\0" ;
  string value ;

  while ( walk -> right != NULL ) {
    bool ss = true ;
    walk = walk -> right ;
    Tree_node *tt = walk ;
    if ( walk -> right_str == "\0" || walk -> right_str == "nil" ) {
      if ( n > 0 ) {
        node -> right = new Tree_node ;
        Init( node -> right ) ;
        node = node -> right ;
      } // if

      string s = "\0" ;
      if ( walk -> left_str != "\0" ) {
        s = walk -> left_str ;
        if ( InList( s, ii, value ) || IsFunction( s ) ) {
          if ( InList( s, ii, value ) ) s = value ;
          ss = false ;
        } // if
      } // if

      else if ( walk -> left != NULL ) {
        tt = walk -> left ;
        ss = true ;
      } // else if

      if ( IsDigit( s ) || IsFloat( s ) || ( s[0] == '\"' && s[s.size()-1] == '\"' ) || 
           s == "#t" || s == "nil" || s == "#f" || s == "()" || IsPrimitive_function( s ) ) {
        if ( s == "nil" || s == "#f" || s == "()" ) s = "nil" ;
        
        node -> left_str = s ;
      } // if

      else {
        Tree_node *temp_walk = tt ;
        while ( temp_walk -> left != NULL ) temp_walk = temp_walk -> left ;
        int index ;
        if ( temp_walk -> left_str == "#<procedure clean-environment>" || 
             temp_walk -> left_str == "#<procedure define>" || 
             temp_walk -> left_str == "#<procedure exit>" ||
             ( InList( temp_walk -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( temp_walk -> left_str, index, value ) ) now = value ;
          else now = temp_walk -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()
            
          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( EvalSExp( tt, temp, ss ) ) {
          if ( IsDigit( temp ) || IsFloat( temp ) || ( temp[0] == '\"' && temp[temp.size()-1] == '\"' ) || 
               temp == "#t" || temp == "nil" || temp == "#f" || temp == "()" ) {
            if ( temp == "nil" || temp == "#f" || temp == "()" ) temp = "nil" ;
            
            node -> left_str = temp ;
          } // if 

          else {
            Tree_node *temp_tree = new Tree_node ;
            Init( temp_tree ) ;
            g_tokenList.clear() ;
            string str = "\0" ;
            for ( int i = 0 ; i < temp.size() ; i++ ) {
              if ( ( temp[i] == ' ' || temp[i] == '(' || temp[i] == ')' || temp[i] == '\"' ||
                     ( i+1 < temp.size() && temp[i-1] == ' ' && temp[i] == '.' && temp[i+1] == ' ' ) || 
                     temp[i] == '\n' ) && str != "#<procedure" ) {
                if ( str != "\0" ) {
                  g_tokenList.push_back( str ) ;
                  str = "\0" ;
                } // if

                if ( temp[i] == '\"' ) {
                  str = str + temp[i] ;
                  i++ ;
                  while ( temp[i] != '\"' ) {
                    str = str + temp[i] ;

                    if ( temp[i] == '\\' ) {
                      if ( i + 1 < temp.size() ) {
                        i++ ;
                        str = str + temp[i] ;
                      } // if()
                    } // if()

                    i++ ;
                  } // while

                  str = str + temp[i] ;
                } // if

                else if ( temp[i] != ' ' && temp[i] != '\n' ) {
                  str = temp[i] ;
                  g_tokenList.push_back( str ) ;
                  str = "\0" ;
                } // else if
              } // if

              else str = str + temp[i] ;
            } // for

            if ( str != "\0" ) g_tokenList.push_back( str ) ;

            BuildTree( temp_tree ) ;
            if ( temp[0] == '(' && temp[temp.size()-1] == ')' ) node -> left = temp_tree ;
            else node -> left_str = temp_tree -> left_str ;
          } // else
        } // else if

        else return false ;
      } // else
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        bool new_line = false ;
        PrettyPrint( root, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
      } // if

      return false ;
    } // else
    
    n++ ;
  } // while

  if ( n == 0 ) {
    if ( walk -> right_str == "\0" || walk -> right_str == "nil" ) {
      result = "nil" ;
      return true ;
    } // if
    
    else {
      if ( g_ErrorMessage == "\0" ) {
        bool new_line = false ;
        PrettyPrint( root, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
      } // if
      
      return false ;
    } // else
  } // if

  else {
    bool new_line = false ;
    g_pretty_str = "( " ;
    g_space = 2 ;
    g_first_need_lp = true ;
    PrettyPrint( root_tempp, new_line ) ;
    if ( g_pretty_str.size() > 0 ) result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
    else result = "nil" ;
    return true ;
  } // else
} // Eval_list()

bool Eval_clear_define( Tree_node *walk, string &result ) {
  if ( walk -> right == NULL && walk -> right_str == "\0" ) {
    g_define_list.clear() ;
    g_lambda_define_list.clear() ;
    result = "environment cleaned" ;
    return true ;
  } // if

  else {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : clean-environment" ;
    return false ;
  } // else
} // Eval_clear_define()

bool Eval_compute( string str, Tree_node *walk, string &result, Tree_node *root ) {
  vector <string> num ;
  string temp, value ;
  int index ;

  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( str ) ;
    return false ;
  } // if()

  Tree_node *temp_walk = walk ;
  int para_num = 1;
  while ( temp_walk -> right != NULL ) {
    para_num++ ;
    temp_walk = temp_walk -> right ;
  } // while

  if ( para_num < 2 ) {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( str ) ;
    return false ;
  } // if ()

  while ( walk != NULL ) {
    if ( walk -> left_str != "\0" || walk -> left != NULL ) {
      if ( walk -> left_str != "\0" && 
           ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) ) {
        
        if ( IsDigit( walk -> left_str ) || IsFloat( walk -> left_str ) ) num.push_back( walk -> left_str ) ;

        else if ( InList( walk -> left_str, index, value ) ) {
          if ( IsDigit( value ) || IsFloat( value ) ) num.push_back( value ) ;

          else {
            if ( g_ErrorMessage == "\0" ) {
              if ( IsPrimitive_function( value ) ) 
                g_ErrorMessage = "ERROR (" + Error_print( str ) + " with incorrect argument type) : " + 
                                 value ;

              else {
                g_ErrorMessage = "ERROR (" + Error_print( str ) + " with incorrect argument type) : " ; 
                g_ErrorMessage = g_ErrorMessage + Error_print( value ) ;
              } // else
            } // if ()

            return false ;
          } // else 
        } // else if()

        else {
          if ( walk -> left_str == "#t" || walk -> left_str == "nil" || IsFunction( walk -> left_str ) ||
               ( walk -> left_str[0] == '\"' && walk -> left_str[walk -> left_str.size()-1] == '\"' ) ) {
            if ( g_ErrorMessage == "\0" && IsFunction( walk -> left_str ) ) 
              g_ErrorMessage = "ERROR (" + Error_print( str ) + " with incorrect argument type) : " + 
                               walk -> left_str ;

            else if ( g_ErrorMessage == "\0" ) 
              g_ErrorMessage = "ERROR (" + Error_print( str ) + " with incorrect argument type) : " + 
                               Error_print( walk -> left_str ) ;
          } // if

          else {
            if ( g_ErrorMessage == "\0" ) 
              g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
          } // else

          return false ;
        } // else
      } // if()

      else if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        if ( g_ErrorMessage == "\0" ) {
          bool new_line = false ;
          PrettyPrint( root, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        } // if 

        return false ;
      } // else if()

      else if ( walk -> left != NULL ) {
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;

        if ( tw -> left_str == "#<procedure clean-environment>" || 
             tw -> left_str == "#<procedure define>" || 
             tw -> left_str == "#<procedure exit>" ||
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( EvalSExp( walk -> left, temp, true ) ) {
          if ( IsDigit( temp ) || IsFloat( temp ) ) num.push_back( temp ) ;

          else {
            if ( g_ErrorMessage == "\0" ) {
              if ( IsPrimitive_function( temp ) )
                g_ErrorMessage = "ERROR (" + Error_print( str ) + " with incorrect argument type) : " + 
                                 temp ;

              else 
                g_ErrorMessage = "ERROR (" + Error_print( str ) + " with incorrect argument type) : " + 
                                 Error_print( temp ) ;
            } // if 

            return false ;
          } // else ()
        } // else if()

        else {
          // if ( temp == "no return value" ) {
          //   g_ErrorMessage = "ERROR (unbound parameter) : " + g_pretty_str ;
          //   g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
          // } // if

          return false ;
        } // else
      } // else if()
    } // if()

    walk = walk -> right ;
    temp = "\0" ;
  } // while()

  if ( num.size() < 2 ) {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( str ) ;
    return false ;
  } // if()

  else {
    bool have_float = false ;

    for ( int i = 0 ; i < num.size() ; i++ ) {
      if ( IsFloat( num[i] ) ) have_float = true ;
    } // for()

    float total = 0 ;

    if ( str == "#<procedure +>" ) {
      for ( int i = 0 ; i < num.size() ; i++ ) {
        total = total + atof( num[i].c_str() ) ;
      } // for()
    } // if()

    else if ( str == "#<procedure ->" ) {
      total = atof( num[0].c_str() ) ;
      for ( int i = 1 ; i < num.size() ; i++ ) {
        total = total - atof( num[i].c_str() ) ;
      } // for()
    } // else if()

    else if ( str == "#<procedure *>" ) {
      total = atof( num[0].c_str() ) ;
      for ( int i = 1 ; i < num.size() ; i++ ) {
        total = total * atof( num[i].c_str() ) ;
      } // for()
    } // else if()

    else if ( str == "#<procedure />" ) {
      total = atof( num[0].c_str() ) ;
      for ( int i = 1 ; i < num.size() ; i++ ) {
        if ( atof( num[i].c_str() ) != -0 || atof( num[i].c_str() ) != 0 || atof( num[i].c_str() ) != +0 ) 
          total = total / atof( num[i].c_str() ) ;
        else {
          if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (division by zero) : " + Error_print( str ) ;
          return false ;
        } // else 
      } // for()
    } // else if()

    if ( have_float == false ) result = Int2str( Float2Int( total ) ) ;
    else result = Float2str( total ) ; 
    return true ;
  } // else()
} // Eval_compute()

bool Eval_logic_not( Tree_node *root, Tree_node *walk, string &result ) {
  string temp ;

  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : not" ;
    return false ;
  } // if()

  else {
    if ( walk -> right == NULL && walk -> left == NULL && 
         ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) && 
         EvalSExp( walk, temp, false ) ) {
      if ( temp == "nil" ) result = "#t" ;
      else result = "nil" ;
      return true ;
    } // if()

    else if ( walk -> right == NULL && walk -> right_str != "\0" && walk -> right_str != "nil" ) {
      if ( g_ErrorMessage == "\0" ) {
        bool new_line = false ;
        PrettyPrint( root, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
      } // if

      return false ;
    } // else if()

    else {
      Tree_node *tw = walk ;
      while ( tw -> left != NULL ) tw = tw -> left ;
      int index ;
      string value_temp = "\0" ;
      temp = "\0" ;

      if ( tw -> left_str == "#<procedure clean-environment>" || tw -> left_str == "#<procedure define>" || 
           tw -> left_str == "#<procedure exit>" ||
           ( InList( tw -> left_str, index, value_temp ) && 
             ( value_temp == "#<procedure clean-environment>" || value_temp == "#<procedure define>" || 
               value_temp == "#<procedure exit>" ) ) ) {
        string now = "\0" ;
        if ( InList( tw -> left_str, index, value_temp ) ) now = value_temp ;
        else now = tw -> left_str ;

        if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
          g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
        } // if()

        else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
          g_ErrorMessage = "ERROR (level of DEFINE)" ;
        } // else if()

        else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
          g_ErrorMessage = "ERROR (level of EXIT)" ;
        } // else if

        return false ;
      } // if()

      else if ( walk -> right == NULL && walk -> left != NULL && EvalSExp( walk -> left, temp, true ) ) {
        if ( temp == "nil" ) result = "#t" ;
        else result = "nil" ;
        return true ;
      } // else if

      else return false ;
    } // else
  } // else 
} // Eval_logic_not()

bool Eval_logic_or( string str, Tree_node *root, Tree_node *walk, string &result ) {
  string temp, value ;
  int index ;

  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( str ) ;
    return false ;
  } // if()

  Tree_node *temp_walk = walk ;
  int num = 1;
  while ( temp_walk -> right != NULL ) {
    num++ ;
    temp_walk = temp_walk -> right ;
  } // while

  if ( num < 2 ) {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( str ) ;
    return false ;
  } // if ()

  while ( walk != NULL ) {
    if ( walk -> left_str != "\0" || walk -> left != NULL ) {
      if ( ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) && 
           walk -> left_str != "\0" ) {
        if ( IsSymbol( walk -> left_str ) == true ) {
          if ( InList( walk -> left_str, index, value ) ) {
            if ( value != "nil" ) {
              result = value ;
              return true ;
            } // if ()
          } // if ()

          else if ( !InList( walk -> left_str, index, value ) && !IsFunction( walk -> left_str ) &&
                    walk -> left_str != "#t" &&  walk -> left_str != "nil" ) {
            if ( g_ErrorMessage == "\0" ) {
              g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
            } // if()

            return false ;
          } // else if

          else {
            if ( value != "nil" ) {
              result = walk -> left_str ;
              return true ;
            } // if ()
          } // else 
        } // if()

        else {
          if ( walk -> left_str != "nil" ) {
            result = walk -> left_str ;
            return true ;
          } // if ()
        } // else 
      } // if()
    
      else if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        if ( g_ErrorMessage == "\0" ) {
          bool new_line = false ;
          PrettyPrint( root, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        } // if

        return false ;
      } // else if()

      else if ( walk -> left != NULL ) {
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;
        if ( tw -> left_str == "#<procedure clean-environment>" || 
             tw -> left_str == "#<procedure define>" || 
             tw -> left_str == "#<procedure exit>" ||
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( EvalSExp( walk -> left, temp, true ) ) {
          if ( temp != "nil" ) {
            result = temp ;
            return true ;
          } // if()
        } // else if ()

        else return false ;
      } // else if()
    } // if()

    walk = walk -> right ;
    temp = "\0" ;
  } // while()

  result = "nil" ;
  return true ;
} // Eval_logic_or()

bool Eval_logic_and( string str, Tree_node *root, Tree_node *walk, string &result ) {
  string temp, value ;
  int index ;

  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( str ) ;
    return false ;
  } // if()

  Tree_node *temp_walk = walk ;
  int num = 1;
  while ( temp_walk -> right != NULL ) {
    num++ ;
    temp_walk = temp_walk -> right ;
  } // while

  if ( num < 2 ) {
    if ( g_ErrorMessage == "\0" ) 
      g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( str ) ;
    return false ;
  } // if ()

  while ( walk != NULL ) {
    if ( walk -> left_str != "\0" || walk -> left != NULL ) {
      if ( ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) && 
           walk -> left_str != "\0" ) {
        if ( IsSymbol( walk -> left_str ) == true ) {
          if ( InList( walk -> left_str, index, value ) ) {
            if ( value == "nil" ) {
              result = "nil" ;
              return true ;
            } // if()

            else result = value ;
          } // if()

          else if ( !InList( walk -> left_str, index, value ) && !IsFunction( walk -> left_str ) &&
                    walk -> left_str != "#t" &&  walk -> left_str != "nil" ) {
            if ( g_ErrorMessage == "\0" ) {
              g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
            } // if()

            return false ;
          } // else if

          else {
            if ( walk -> left_str == "nil" ) {
              result = "nil" ;
              return true ;
            } // if()

            else result = walk -> left_str ;
          } // else
        } // if()

        else {
          if ( walk -> left_str == "nil" ) {
            result = "nil" ;
            return true ;
          } // if()

          else result = walk -> left_str ;
        } // else
      } // if()
    
      else if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        if ( g_ErrorMessage == "\0" ) {
          bool new_line = false ;
          PrettyPrint( root, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        } // if

        return false ;
      } // else if()

      else if ( walk -> left != NULL ) {
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;
        if ( tw -> left_str == "#<procedure clean-environment>" || 
             tw -> left_str == "#<procedure define>" || 
             tw -> left_str == "#<procedure exit>" ||
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( EvalSExp( walk -> left, temp, true ) ) {
          if ( temp == "nil" ) {
            result = "nil" ;
            return true ;
          } // if()

          else result = temp ;
        } // else if()

        else {
          // if ( temp == "no return value" ) {
          //   g_ErrorMessage = "ERROR (unbound condition) : " + g_pretty_str ;
          //   g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
          // } // if
          
          return false ;
        } // else 
      } // else if()
    } // if()

    walk = walk -> right ;
    temp = "\0" ;
  } // while()

  return true ;
} // Eval_logic_and()

bool Eval_equal( Tree_node *root, Tree_node *walk, string &result ) {
  vector <string> num ;
  string temp, value ;
  int index ;

  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : equal?" ;
    return false ;
  } // if()

  Tree_node *temp_walk = walk ;
  int para_num = 1;
  while ( temp_walk -> right != NULL ) {
    para_num++ ;
    temp_walk = temp_walk -> right ;
  } // while

  if ( para_num != 2 ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : equal?" ;
    return false ;
  } // if ()

  while ( walk != NULL && num.size() < 2 ) {
    if ( walk -> left_str != "\0" || walk -> left != NULL ) {
      if ( ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) && 
           walk -> left == NULL ) {
        if ( IsSymbol( walk -> left_str ) == true ) {
          if ( InList( walk -> left_str, index, value ) ) num.push_back( value ) ;

          else if ( !InList( walk -> left_str, index, value ) && !IsFunction( walk -> left_str ) &&
                    walk -> left_str != "#t" &&  walk -> left_str != "nil" ) {
            if ( g_ErrorMessage == "\0" ) {
              g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
            } // if()

            return false ;
          } // else if

          else num.push_back( walk -> left_str ) ;
        } // if()

        else {
          num.push_back( walk -> left_str ) ;
        } // else 
      } // if()
    
      else if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        if ( g_ErrorMessage == "\0" ) {
          bool new_line = false ;
          PrettyPrint( root, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        } // if()

        return false ;
      } // else if()

      else if ( walk -> left != NULL ) {
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;

        if ( tw -> left_str == "#<procedure clean-environment>" || 
             tw -> left_str == "#<procedure define>" || 
             tw -> left_str == "#<procedure exit>" ||
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) )  ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( EvalSExp( walk -> left, temp, true ) ) num.push_back( temp ) ;
        else return false ;
      } // else if()
    } // if()

    walk = walk -> right ;
    temp = "\0" ;
  } // while()

  if ( num[0] == num[1] ) result = "#t" ;
  else result = "nil" ;
  return true ;
} // Eval_equal()

bool Eval_eqv( Tree_node *root, Tree_node *walk, string &result ) {
  vector <string> num ;
  string temp, value ;
  int index ;
  bool one = false ;
  bool two = false ;
  int index1 ;
  int index2 ;

  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : eqv?" ;
    return false ;
  } // if()

  Tree_node *temp_walk = walk ;
  int para_num = 1;
  while ( temp_walk -> right != NULL ) {
    para_num++ ;
    temp_walk = temp_walk -> right ;
  } // while

  if ( para_num != 2 ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : eqv?" ;
    return false ;
  } // if ()

  while ( walk != NULL && num.size() < 2 ) {
    if ( walk -> left_str != "\0" || walk -> left != NULL ) {
      if ( ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) && 
           walk -> left_str != "\0" ) {
        if ( IsSymbol( walk -> left_str ) == true ) {
          if ( InList( walk -> left_str, index, value ) ) { 
            num.push_back( value ) ;
            if ( num.size() == 1 ) {
              one = true ;
              index1 = g_define_list[index].index ;
            } // if 

            else if ( num.size() == 2 ) {
              two = true ;
              index2 = g_define_list[index].index ;
            } // else if
          } // if()

          else if ( !InList( walk -> left_str, index, value ) && !IsFunction( walk -> left_str ) &&
                    walk -> left_str != "#t" && walk -> left_str != "nil" ) {
            if ( g_ErrorMessage == "\0" ) {
              g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
            } // if()

            return false ;
          } // else if

          else num.push_back( walk -> left_str ) ;
        } // if()
 
        else {
          num.push_back( walk -> left_str ) ;
        } // else 
      } // if()
    
      else if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        if ( g_ErrorMessage == "\0" ) {
          bool new_line = false ;
          PrettyPrint( root, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        } // if()

        return false ;
      } // else if()

      else if ( walk -> left != NULL ) {
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;

        if ( tw -> left_str == "#<procedure clean-environment>" || tw -> left_str == "#<procedure define>" ||
             tw -> left_str == "#<procedure exit>" ||
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) )  ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( EvalSExp( walk -> left, temp, true ) ) {
          if ( g_index != -100 ) {
            num.push_back( temp ) ;

            if ( num.size() == 1 ) {
              one = true ;
              index1 = g_index ;
            } // if 

            else if ( num.size() == 2 ) {
              two = true ;
              index2 = g_index ;
            } // else if

          } // if 

          else num.push_back( temp ) ;
        } // else if()

        else return false ;
      } // else if()

      else {
        // if ( temp == "no return value" ) {
        //   // g_ErrorMessage = "ERROR (unbound parameter) : " + g_pretty_str ;
        //   // g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        //   return false ;
        // } // if

        if ( g_ErrorMessage != "\0" ) return false ;
      } // else 
    } // if()

    walk = walk -> right ;
    g_index = -100 ;
    temp = "\0" ; 
  } // while()

  if ( num[0] == num[1] ) {
    if ( ! ( ( num[0][0] == '\"' ) && ( num[0][num[0].size()-1] == '\"' ) ) && 
         ( IsDigit( num[0] ) || IsFloat( num[0] ) || num[0] == "#t" || num[0] == "nil" || 
           IsFunction( num[0] ) || IsSymbol( num[0] ) ) ) { 
      result = "#t" ;
    } // if()

    else if ( ! ( ( num[0][0] == '\"' ) && ( num[0][num[0].size()-1] == '\"' ) ) && 
              one == true && two == true ) {
      if ( g_local == false && 
           ( g_define_list[index1].index == index2 || g_define_list[index2].index == index1 || 
             index1 == index2 || 
             ( g_define_list[index1].index != -99 && g_define_list[index2].index != -99 &&
               g_define_list[index1].index == g_define_list[index2].index ) ) ) {
        result = "#t" ;
      } // if()

      else result = "nil" ;
    } // if()

    else {
      result = "nil" ;
    } // else 
  } // if()

  else result = "nil" ;
  return true ;
} // Eval_eqv()

bool Eval_sequencing( Tree_node *walk, string &result ) {
  vector <string> num ;
  string temp, value ;
  int index ;

  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : begin" ;
    return false ;
  } // if ()

  else {
    while ( walk != NULL ) {
      if ( walk -> right_str == "\0" && walk -> left == NULL &&
           ( walk -> left_str == "nil" ||  walk -> left_str == "#t" || 
             !IsPrimitive_function( walk -> left_str ) ) ) {
        
        if ( IsSymbol( walk -> left_str ) == true ) { 
          if ( InList( walk -> left_str, index, value ) ) num.push_back( value ) ;

          else {
            if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
            return false ;
          } // else
        } // if()

        else num.push_back( walk -> left_str ) ;
      } // if()

      else if ( walk -> right_str == "\0" && walk -> left == NULL && IsFunction( walk -> left_str ) ) 
        num.push_back( walk -> left_str ) ; 

      else if ( walk -> left != NULL ) {
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;
        // cout << "tw: " << tw -> left_str << endl ;

        if ( tw -> left_str == "#<procedure clean-environment>" || tw -> left_str == "#<procedure define>" ||
             tw -> left_str == "#<procedure exit>" ||
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( EvalSExp( walk -> left, temp, true ) ) {
          // cout << "begin temp: " << temp << endl ;
          num.push_back( temp ) ;
        } // else if ()

        else {
          // cout << "temp = " << temp << endl ;
          if ( temp == "no return value" ) num.push_back( temp ) ;
          // else return false ;
          else if ( g_ErrorMessage != "\0" ) return false ;
        } // else

        // before: else return false ; after : if ( temp == "no return value" ) num.push_back( temp ) ;
      } // else if()

      else {
        if ( g_ErrorMessage != "\0" ) return false ;
      } // else

      walk = walk -> right ;
      temp = "\0" ;
    } // while()

    if ( num.size() != 0 ) {
      result = num[num.size()-1] ;
      if ( result == "no return value" ) {
        // cout << "rrrrrrrrr= " << result << endl ;
        return false ;
      } // if
      // cout << "result: " << result << endl ;
      // cout << "b ans = " << result << endl ;
      g_ErrorMessage = "\0" ;
      // cout << "?????????????" << endl ;
      return true ;
    } // if()

    else {
      return false ;
    } // else 
  } // else 
} // Eval_sequencing()

bool Eval_if( Tree_node *root, Tree_node *walk, string &result ) {
  vector <string> num ;
  string temp, value ;
  int index ;
  bool come_in = false ;

  if ( walk == NULL ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : if" ;
    return false ;
  } // if()

  Tree_node *temp_walk = walk ;
  int para_num = 1;
  while ( temp_walk -> right != NULL ) {
    para_num++ ;
    temp_walk = temp_walk -> right ;
  } // while

  if ( para_num != 2 && para_num != 3 ) {
    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : if" ;
    return false ;
  } // if ()

  while ( walk != NULL ) {
    if ( walk -> left_str != "\0" || walk -> left != NULL ) {
      if ( ( walk -> right_str == "\0" || ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) && 
           walk -> left_str != "\0" ) {
        if ( IsSymbol( walk -> left_str ) && InList( walk -> left_str, index, value ) ) { 
          num.push_back( value ) ;
        } // if()

        else {
          if ( !IsSymbol( walk -> left_str ) || IsPrimitive_function( walk -> left_str ) ) {
            num.push_back( walk -> left_str ) ;
          } // if ()

          else {
            g_ErrorMessage = "ERROR (unbound symbol) : " + walk -> left_str ;
            num.push_back( g_ErrorMessage ) ;

            if ( num.size() == 1 ) return false ;

            g_ErrorMessage = "\0" ;
            g_pretty_str = "( " ;
            g_space = 2 ;
            g_first_need_lp = true ;
          } // else 
        } // else 
      } // if()
    
      else if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        bool new_line = false ;
        g_pretty_str = "( " ;
        g_space = 2 ;
        g_first_need_lp = true ;
        PrettyPrint( root, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        num.push_back( g_ErrorMessage ) ;
        if ( num.size() == 1 ) return false ;

        g_ErrorMessage = "\0" ;
        g_pretty_str = "( " ;
        g_space = 2 ;
        g_first_need_lp = true ;
      } // else if()

      else if ( walk -> left != NULL ) {
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;

        if ( tw -> left_str == "#<procedure clean-environment>" || tw -> left_str == "#<procedure define>" ||
             tw -> left_str == "#<procedure exit>" ||
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
            num.push_back( g_ErrorMessage ) ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
            num.push_back( g_ErrorMessage ) ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
            num.push_back( g_ErrorMessage ) ;
          } // else if

          if ( num.size() == 1 ) {
            // cout << "@@@@@@@@@@@@" << endl ;
            return false ;
          } // if () 

          g_ErrorMessage = "\0" ;
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
        } // if()

        else if ( ( num.size() == 0 || num.size() == 1 || 
                    ( num.size() == 2 && ( num[0] == "nil" ) ) ) && EvalSExp( walk -> left, temp, true ) ) {
          num.push_back( temp ) ;
        } // else if()

        else {
          num.push_back( g_ErrorMessage ) ;

          if ( num.size() == 1 && g_ErrorMessage != "\0" ) {
            return false ;
          } // if ()

          if ( num.size() == 3 && ( num[0] == "nil" ) && g_ErrorMessage != "\0" ) {
            return false ;
          } // if ()

          g_ErrorMessage = "\0" ;
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
        } // else()
      } // else if()

      else {
        num.push_back( g_ErrorMessage ) ;
        
        if ( num.size() == 1 && g_ErrorMessage != "\0" ) return false ;

        if ( num.size() == 3 && ( num[0] == "nil" ) && g_ErrorMessage != "\0" ) {
          return false ;
        } // if ()
      } // else()
    } // if ()

    walk = walk -> right ;
    temp = "\0" ;
  } // while()

  if ( num.size() == 3 && ( num[0] == "nil" ) ) {
    result = num[2] ;
  } // if ()

  else if ( num.size() == 2 && ( num[0] == "nil" ) ) {
    bool new_line = false ; 
    g_pretty_str = "( " ;
    g_space = 2 ;
    g_first_need_lp = true ;
    PrettyPrint( root, new_line ) ;
    g_ErrorMessage = "ERROR (no return value) : " + Error_print( g_pretty_str )  ;
    g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
    return false ;
  } // else if()

  else result = num[1] ;
  return true ;
} // Eval_if()

bool Eval_cond( Tree_node *walk, string &result, Tree_node *root ) {
  string ans = "\0" ;
  int n = 0 ; // ???????????????????????????????????????????????????????
  bool temp_else = false ; // ??????????????????????????????????????????????????else
  bool no_value_error = false ;
  bool hasError = false ; // ???????????????????????at????r
  bool hasAns = false ;

  if ( walk == NULL ) {
    bool new_line = false ;
    g_pretty_str = "( " ;
    g_space = 2 ;
    PrettyPrint( root, new_line ) ;
    g_ErrorMessage = "ERROR (COND format) : " + Error_print( g_pretty_str ) ;
    g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
    return false ;
  } // if

  else {
    // cout << "hhh" << endl ;
    while ( walk != NULL ) {
      string temp = "\0" ;
      string ans_temp = "\0" ;
      string value ;
      Tree_node *pre = walk ;
      bool ss = true ;
      walk = walk -> left ;
      // root = walk ;

      if ( ans != "\0" && g_ErrorMessage == "\0" ) hasAns = true ;

      if ( pre -> right_str != "\0" || walk == NULL ) {
        bool new_line = false ;
        g_pretty_str = "( " ;
        g_space = 2 ;
        PrettyPrint( root, new_line ) ;
        if ( pre -> right_str != "\0" && pre -> right_str != "nil" ) 
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
        else g_ErrorMessage = "ERROR (COND format) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
        return false ;
      } // if
      
      else if ( walk -> left_str != "\0" ) {
        string str = walk -> left_str ;
        int index = 0 ;
        bool hasQuote = false ;
        // cout << "??????????" << endl ;
        // cout << "s= "<<IsPrimitive_function( str )<<endl;
        if ( InList( str, index, value ) ) {
          if ( pre -> right == NULL && str == "else" ) str = "else" ;
          else str = value ;
        } // if
        
        else if ( str == "#<procedure quote>" ) {
          hasQuote = true ;
          string quote_str = "\0" ;
          bool level_error = false ;
          string level_str ;
          if ( Eval_quote( walk -> right, quote_str ) ) {
            // cout << "q=" <<quote_str << endl ;
            Tree_node *temp_tree = new Tree_node ;
            Init( temp_tree ) ;
            walk = temp_tree ;
            g_tokenList.clear() ;
            string str = "\0" ;
            string temp = quote_str ;
            bool quote = false ;

            // cout<<"pp= "<<g_pretty_str<<endl;
            for ( int i = 0 ; i < temp.size() ; i++ ) {
              if ( ( temp[i] == ' ' || temp[i] == '(' || temp[i] == ')' || temp[i] == '\"' ||
                     ( i+1 < temp.size() && temp[i-1] == ' ' && temp[i] == '.' && temp[i+1] == ' ' ) || 
                     temp[i] == '\n' ) && str != "#<procedure" ) {
                if ( str != "\0" ) {
                  if ( !quote && IsPrimitive_function_PRO( str ) ) {
                    if ( str == "exit" || str == "define" || str == "clean-environment" ) {
                      level_error = true ;
                      level_str = str ;
                    } // if

                    else if ( str == "quote" ) quote = true ;

                    str = "#<procedure " + str + ">" ;
                  } // if

                  g_tokenList.push_back( str ) ;
                  str = "\0" ;
                } // if

                if ( temp[i] == '\"' ) {
                  str = str + temp[i] ;
                  i++ ;
                  while ( temp[i] != '\"' ) {
                    str = str + temp[i] ;

                    if ( temp[i] == '\\' ) {
                      if ( i + 1 < temp.size() ) {
                        i++ ;
                        str = str + temp[i] ;
                      } // if()
                    } // if()

                    i++ ;
                  } // while

                  str = str + temp[i] ;
                } // if

                else if ( temp[i] != ' ' && temp[i] != '\n' ) {
                  str = temp[i] ;
                  g_tokenList.push_back( str ) ;
                  str = "\0" ;
                } // else if
              } // if

              else str = str + temp[i] ;
            } // for

            if ( str != "\0" ) g_tokenList.push_back( str ) ;
            BuildTree( temp_tree ) ;

            if ( ans == "\0" && ! ( quote_str[0] == '(' && quote_str[quote_str.size()-1] == ')' ) && 
                 temp_tree -> right == NULL && !level_error ) {
              // cout << "???"<<endl ;
              str = temp_tree -> left_str ;
              int index_temp = 0 ;
              string value_temp ;
              if ( IsDigit( str ) || IsFloat( str ) || ( str[0] == '\"' && str[str.size()-1] == '\"' ) || 
                   str == "#t" || str == "nil" ) 
                ans_temp = str ;
              else if ( IsPrimitive_function_PRO( str ) ) ans_temp = "#<procedure " + str + ">" ;

              else if ( InList( str, index_temp, value_temp ) ) ans_temp = value_temp ;

              else {
                if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (unbound symbol) : " + str ;
                if ( temp == "no return value" ) no_value_error = true ;
                else no_value_error = false ;
                ans_temp = "\0" ;
              } // else 
            } // if

            else if ( ans == "\0" && level_error ) {
              if ( level_str == "exit" ) g_ErrorMessage = "ERROR (level of EXIT)" ;
              else if ( level_str == "define" ) g_ErrorMessage = "ERROR (level of DEFINE)" ;
              else g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
              return false ;
            } // else if

            else if ( ans == "\0" && EvalSExp( temp_tree, temp, true ) ) {
              // cout << "!!!!!!!!!" << endl ;
              ans_temp = temp ;
              if ( no_value_error ) {
                no_value_error = false ;
                g_ErrorMessage = "\0" ;
              } // if
            } // else if

            else {
              // cout << "?????????" << endl ;
              if ( temp == "no return value" ) no_value_error = true ;
              else no_value_error = false ;
              ans_temp = "\0" ;
            } // else 
          } // if

          // cout << "quote_str= " << quote_str << endl;
        } // else if

        else if ( !IsDigit( str ) && !IsFloat( str ) && str != "#t" && str != "#f" && str != "nil" && 
                  str != "()" && ! ( str[0] == '\"' && str[str.size()-1] == '\"' ) && 
                  !IsPrimitive_function( str ) ) {
          if ( ( str == "else" && ! InList( str, index, value ) && pre -> right != NULL ) 
               || str != "else" ) {
            if ( g_ErrorMessage == "\0" ) {
              g_ErrorMessage = "ERROR (unbound symbol) : " + str ;
              // stat_error = true ;
            } // if
          } // if
        } // else if

        // cout << "s1= "<<str<<endl;
        if ( !hasQuote ) {
          if ( str != "#f" && str != "nil" && str != "()" && str != "0" ) {
            if ( str == "else" ) {
              temp_else = true ;
            } // if
            
            int in = 0 ;
            // root = walk ;
            // begin()
            while ( walk != NULL ) {
              // Tree_node *pre2 ;
              if ( walk -> right_str == "\0" || walk -> right_str == "nil" ) {
                walk = walk -> right ;
                if ( walk != NULL ) {
                  in++ ;
                  string s1 = walk -> left_str ;
                  // cout << "s1= " << s1 << endl ;
                  int i1 = 0 ;
                  if ( s1 != "\0" && 
                       ( ( IsDigit( s1 ) || IsFloat( s1 ) || 
                           ( s1[0] == '\"' && s1[s1.size()-1] == '\"' ) ) || 
                         s1 == "#t" || s1 == "#f" || s1 == "nil" || s1 == "()" || 
                         IsFunction( s1, value ) || InList( s1, i1, value ) ) ) {
                    if ( IsFunction( s1, value ) || InList( s1, i1, value ) ) s1 = value ;
                    ans_temp = s1 ;
                    // cout << "????????????????????????\n" ;
                    if ( no_value_error ) {
                      no_value_error = false ;
                      g_ErrorMessage = "\0" ;
                    } // if
                  } // if

                  else if ( s1 != "\0" && !InList( s1, i1, value ) ) {
                    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (unbound symbol) : " + s1 ;
                    if ( temp == "no return value" ) no_value_error = true ;
                    else no_value_error = false ;
                    ans_temp = "\0" ;
                  } // else if

                  else {
                    Tree_node *tw = walk ;
                    while ( tw -> left != NULL ) tw = tw -> left ;
                    if ( ( tw -> left_str == "#<procedure clean-environment>" || 
                           tw -> left_str == "#<procedure define>" ||
                           tw -> left_str == "#<procedure exit>" ||
                           ( InList( tw -> left_str, index, value ) && 
                             ( value == "#<procedure clean-environment>" || 
                               value == "#<procedure define>" || 
                               value == "#<procedure exit>" ) ) ) && ans == "\0" ) {
                      string now = "\0" ;
                      if ( InList( tw -> left_str, index, value ) ) now = value ;
                      else now = tw -> left_str ;

                      if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
                        g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
                      } // if()

                      else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
                        g_ErrorMessage = "ERROR (level of DEFINE)" ;
                      } // else if()

                      else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
                        g_ErrorMessage = "ERROR (level of EXIT)" ;
                      } // else if

                      return false ;
                    } // if()

                    else if ( walk -> left != NULL && ans == "\0" && EvalSExp( walk -> left, temp, true ) ) {
                      ans_temp = temp ;
                      if ( no_value_error ) {
                        no_value_error = false ;
                        g_ErrorMessage = "\0" ;
                      } // if
                    } // else if

                    else {
                      if ( temp == "no return value" ) no_value_error = true ;
                      else no_value_error = false ;
                      ans_temp = "\0" ;
                    } // else
                    // else return false ;
                  } // else 
                } // if

                else if ( in == 0 ) {
                  bool new_line = false ;
                  g_pretty_str = "( " ;
                  g_space = 2 ;
                  PrettyPrint( root, new_line ) ;
                  g_ErrorMessage = "ERROR (COND format) : " + Error_print( g_pretty_str ) ;
                  g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
                  return false ;
                } // else if
              } // if

              else {
                bool new_line = false ;
                g_pretty_str = "( " ;
                g_space = 2 ;
                g_first_need_lp = true ;
                PrettyPrint( pre -> left, new_line ) ;
                g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
                g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
                return false ;
              } // else
            } // while

            // if ( ans_temp != "\0" ) cout << "fffffffff = " << ans_temp << endl ;
          } // if

          else {
            if ( walk -> right == NULL || walk -> right_str != "\0" ) {
              bool new_line = false ;
              g_pretty_str = "( " ;
              g_space = 2 ;
              PrettyPrint( root, new_line ) ;
              if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) 
                g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
              else g_ErrorMessage = "ERROR (COND format) : " + Error_print( g_pretty_str ) ;
              g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
              return false ;
            } // if
          } // else
        } // if
      } // if

      else {
        // cout << "??????????" << endl ;
        Tree_node *tw = walk ;
        while ( tw -> left != NULL ) tw = tw -> left ;
        int index ;
        if ( tw -> left_str == "#<procedure clean-environment>" || 
             tw -> left_str == "#<procedure define>" ||
             tw -> left_str == "#<procedure exit>" ||
             ( InList( tw -> left_str, index, value ) && 
               ( value == "#<procedure clean-environment>" || value == "#<procedure define>" || 
                 value == "#<procedure exit>" ) ) ) {
          string now = "\0" ;
          if ( InList( tw -> left_str, index, value ) ) now = value ;
          else now = tw -> left_str ;

          if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
            g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
          } // if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
            g_ErrorMessage = "ERROR (level of DEFINE)" ;
          } // else if()

          else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
            g_ErrorMessage = "ERROR (level of EXIT)" ;
          } // else if

          return false ;
        } // if()

        else if ( walk -> left != NULL && EvalSExp( walk -> left, temp, true ) ) {
          if ( temp != "#f" && temp != "nil" && temp != "()" && temp != "0" ) {
            int in = 0 ;
            while ( walk != NULL ) {
              if ( walk -> right_str == "\0" || walk -> right_str == "nil" ) {
                walk = walk -> right ;
                if ( walk != NULL ) {
                  in++ ;
                  string s1 = walk -> left_str ;
                  int i1 = 0 ;
                  // cout << "s1= " << s1 << endl; 
                  if ( s1 != "\0" && 
                       ( ( IsDigit( s1 ) || IsFloat( s1 ) || 
                           ( s1[0] == '\"' && s1[s1.size()-1] == '\"' ) ) || 
                         s1 == "#t" || s1 == "nil" || IsFunction( s1, value ) || 
                         InList( s1, i1, value ) ) ) {
                    if ( IsFunction( s1, value ) || InList( s1, i1, value ) ) s1 = value ;
                    ans_temp = s1 ;
                    if ( no_value_error ) {
                      no_value_error = false ;
                      g_ErrorMessage = "\0" ;
                    } // if
                  } // if

                  else if ( s1 != "\0" && !InList( s1, i1, value ) ) {
                    if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (unbound symbol) : " + s1 ;
                    if ( temp == "no return value" ) no_value_error = true ;
                    else no_value_error = false ;
                    ans_temp = "\0" ;
                  } // else if

                  else {
                    Tree_node *tw = walk ;
                    while ( tw -> left != NULL ) tw = tw -> left ;
                    if ( ( tw -> left_str == "#<procedure clean-environment>" || 
                           tw -> left_str == "#<procedure define>" ||
                           tw -> left_str == "#<procedure exit>" ||
                           ( InList( tw -> left_str, index, value ) && 
                             ( value == "#<procedure clean-environment>" || 
                               value == "#<procedure define>" || 
                               value == "#<procedure exit>" ) ) ) && ans == "\0" ) {
                      string now = "\0" ;
                      if ( InList( tw -> left_str, index, value ) ) now = value ;
                      else now = tw -> left_str ;

                      if ( g_ErrorMessage == "\0" && now == "#<procedure clean-environment>" ) {
                        g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
                      } // if()

                      else if ( g_ErrorMessage == "\0" && now == "#<procedure define>" ) {
                        g_ErrorMessage = "ERROR (level of DEFINE)" ;
                      } // else if()

                      else if ( g_ErrorMessage == "\0" && now == "#<procedure exit>" ) {
                        g_ErrorMessage = "ERROR (level of EXIT)" ;
                      } // else if

                      return false ;
                    } // if()

                    else if ( walk -> left != NULL && ans == "\0" && EvalSExp( walk -> left, temp, true ) ) {
                      ans_temp = temp ;
                      if ( no_value_error ) {
                        no_value_error = false ;
                        g_ErrorMessage = "\0" ;
                      } // if
                      // cout << "555555" << endl ;
                      // cout << "rr= " << result << endl ;
                    } // else if
                      
                    
                    else {
                      if ( temp == "no return value" ) no_value_error = true ;
                      else no_value_error = false ;
                      // cout << "333333" << endl ;
                      // cout << "rr= " << temp << endl ;
                      // ans = "\0" ;
                      ans_temp = "\0" ;
                    } // else 
                    // else return false ;
                  } // else
                } // if

                else if ( in == 0 ) {
                  bool new_line = false ;
                  g_pretty_str = "( " ;
                  g_space = 2 ;
                  PrettyPrint( root, new_line ) ;
                  g_ErrorMessage = "ERROR (COND format) : " + Error_print( g_pretty_str ) ;
                  g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
                  return false ;
                } // else if
              } // if

              else {
                bool new_line = false ;
                g_pretty_str = "( " ;
                g_space = 2 ;
                PrettyPrint( pre -> left, new_line ) ;
                g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
                g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
                return false ;
              } // else
            } // while
          } // if

          else {
            if ( walk -> right == NULL || walk -> right_str != "\0" ) {
              bool new_line = false ;
              g_pretty_str = "( " ;
              g_space = 2 ;
              PrettyPrint( root, new_line ) ;
              if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) 
                g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
              else g_ErrorMessage = "ERROR (COND format) : " + g_pretty_str ;
              return false ;
            } // if
          } // else
        } // else if

        else {
          if ( temp == "no return value" ) no_value_error = true ;
          else no_value_error = false ;
          // cout << "22222" << endl ;
          // ans = "\0" ;
          ans_temp = "\0" ;
        } // else 
        // else return false ;
      } // else

      if ( ans == "\0" && ans_temp != "\0" ) {
        ans = ans_temp ; 
        if ( no_value_error ) {
          no_value_error = false ;
          g_ErrorMessage = "\0" ;
        } // if
          // cout << "this ans = " << ans_temp << endl ;
      } // if
      // ans = ans_temp ;
      walk = pre -> right ;
      n++ ;
    } // while
  } // else

  // hasAns???????????ns????????????or????????????????????
  if ( ans != "\0" && g_ErrorMessage != "\0" && !hasAns ) return false ;

  else if ( ans == "\0" ) {
    if ( no_value_error || g_ErrorMessage == "\0" ) {
      bool new_line = false ;
      g_pretty_str = "( " ;
      g_space = 2 ;
      PrettyPrint( root, new_line ) ;
      g_ErrorMessage = "ERROR (no return value) : " + Error_print( g_pretty_str ) ;
      g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
      result = "no return value" ;
    } // if

    return false ;
  } // else if

  else {
    result = ans ;
    return true ;
  } // else
} // Eval_cond()

string Print_Result( string str ) {
  // cout << str << endl ;
  string return_result = "\0" ;
  string temp = "\0" ;
  for ( int i = 0 ; i < str.size() ; i++ ) {
    if ( ( str[i] == ' ' || str[i] == '\n' ) && temp != "#<procedure" ) {
      if ( temp != "\0" ) {
        if ( IsFloat( temp ) ) temp = Float( Float2str( round( atof( temp.c_str() ) * 1000 ) / 1000.0 ) ) ;
        return_result = return_result + temp ;
        temp = "\0" ;
      } // if

      return_result = return_result + str[i] ;
    } // if

    else if ( str[i] == '\"' ) {
      temp = temp + str[i] ;
      i++ ;
      while ( str[i] != '\"' ) {
        if ( str[i] == '\\' && i + 1 < str.size() ) {
          i++ ;
          if ( str[i] == 'n' ) temp = temp + "\n" ;
          else if ( str[i] == 't' ) temp = temp + "\t" ;
          else if ( str[i] == '\"' ) temp = temp + "\"" ;
          else if ( str[i] == '\\' ) temp = temp + "\\" ;
          else temp = temp + str[i-1] + str[i] ;
        } // if

        else {
          temp = temp + str[i] ;
        } // else

        i++ ;
      } // while

      temp = temp + str[i] ;
      return_result = return_result + temp ;
      temp = "\0" ;
    } // else if

    else temp = temp + str[i] ;
  } // for

  if ( temp != "\0" ) return_result = return_result + temp ;
  return return_result ;
} // Print_Result()

bool EvalSExp( Tree_node *walk, string &result, bool single ) {
  g_change = true ;
  Tree_node *walk_temp = walk ;
  bool temp_Recursive = g_hasRecursive ;
  while ( walk_temp -> right != NULL ) {
    walk_temp = walk_temp -> right ;
  } // while

  if ( walk_temp -> right_str != "\0" && walk_temp -> right_str != "nil" ) {
    g_pretty_str = "( " ;
    g_space = 2 ;
    g_first_need_lp = true ;
    bool l = false ;
    PrettyPrint( walk, l ) ;
    g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
    g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
    return false ;
  } // if

  if ( walk -> left != NULL ) {
    g_hasRecursive = true ;
    bool yn = EvalSExp( walk -> left, walk -> left_str, true ) ;
    // cout << "dd= " << yn << "   ," <<  walk -> left_str << endl ;
    if ( !yn ) return false ;
    else walk -> left = NULL ;
    g_index = -100 ;
  } // if

  int index = 0 ;
  string value ;
  // cout << "w= " << walk -> left_str << endl ;

  if ( g_exit ) {
    // if ( temp_Recursive ) {
    g_ErrorMessage = "ERROR (level of EXIT)" ;
    g_exit = false ;
    return false ;
    // } // if
  } // if

  if ( single == false && ( IsFunction( walk -> left_str ) || InList( walk -> left_str, index, value ) ) ) {
    result = walk -> left_str ;

    if ( InList( walk -> left_str, index, value ) ) {
      result = value ;
      if ( g_Isfrom == "G" && index < g_define_list.size() ) g_change = g_define_list[index].change ;
      // if ( g_change == true ) result = Print_Result( result ) ;
    } // if()
   
    return true ;
  } // if()

  else if ( walk -> left_str == "#<procedure car>" ) {
    if ( walk -> right != NULL && Eval_car( walk -> right, result, walk ) ) {
      // if ( ( result == "exit" || result == "define" || result == "clean-environment" ) && g_change ) {
      //   result = "#<procedure " + result + ">" ;
      // } // if
      // cout << "r= " << result << endl ;

      return true ;
    } // if

    else {
      // cout << "hhhhhhhhhhhhhhhheeeeeeeeeeeeee" << endl ;
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : car" ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure cdr>" ) {
    // cout << "CCCCCCCCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDDDRRRRRRRRRRRRRRRRRRRRRR!!!!!!!!" << endl ;
    if ( walk -> right != NULL && Eval_cdr( walk -> right, result, walk ) ) {
      // if ( ( result == "exit" || result == "define" || result == "clean-environment" ) && g_change ) {
      //   result = "#<procedure " + result + ">" ;
      // } // if

      return true ;
    } // if ()

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : cdr" ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if()

  else if ( walk -> left_str == "#<procedure cons>" ) {
    // cout << "CCCCCCCCCOOOOOOOOOOOOOONNNNNNNNNNNNNNSSSSSSSSSSSSSSS!!!!!!!!!!!!!!!!!" << endl ;
    if ( walk -> right != NULL && Eval_cons( walk -> right, result, walk ) ) {
      // g_index = -100 ;
      // cout << "result: " << result << endl ;
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : cons" ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure string?>" || walk -> left_str == "#<procedure null?>" ||
            walk -> left_str == "#<procedure pair?>" || walk -> left_str == "#<procedure symbol?>" ||
            walk -> left_str == "#<procedure integer?>" || walk -> left_str == "#<procedure real?>" || 
            walk -> left_str == "#<procedure number?>" || walk -> left_str == "#<procedure list?>" ||
            walk -> left_str == "#<procedure atom?>" || walk -> left_str == "#<procedure boolean?>" ) {
    if ( walk -> right != NULL && Eval_judge( walk -> left_str, walk -> right, result, walk ) ) {
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) 
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( walk -> left_str ) ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure quote>" ) {
    if ( walk -> right != NULL && Eval_quote( walk -> right, result ) ) {
      g_change = false ;
      return true ;
    } // if ()

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : quote" ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure >>" || walk -> left_str == "#<procedure <=>" ||
            walk -> left_str == "#<procedure >=>" || walk -> left_str == "#<procedure =>" || 
            walk -> left_str == "#<procedure <>" ) {
    if ( Eval_cmp( walk, result ) ) {
      // g_index = -100 ;
      return true ;
    } // if

    else return false ;
  } // else if

  else if ( walk -> left_str == "#<procedure string-append>" || 
            walk -> left_str == "#<procedure string>?>" || 
            walk -> left_str == "#<procedure string<?>" || walk -> left_str == "#<procedure string=?>" ) {
    if ( Eval_string( walk, result ) ) {
      if ( walk -> left_str == "string-append" ) {
        Tree_node *newTree = new Tree_node ;
        Init( newTree ) ;
        newTree -> left_str = result ;
        g_pretty_str = "\0" ;
        g_space = 0 ;
        g_first_need_lp = false ;
        bool n = false ;
        PrettyPrint( newTree, n ) ;
        if ( g_pretty_str.size() > 0 ) result = g_pretty_str.erase( g_pretty_str.size() - 1 ) ;
        else result = "\0" ;
      } // if
   
      // g_index = -100 ;
      return true ;
    } // if

    else return false ;
  } // else if

  else if ( walk -> left_str == "#<procedure list>" ) {
    if ( Eval_list( walk, result, walk ) ) {
      // g_index = -100 ;
      return true ;
    } // if

    else return false ;
  } // else if

  else if ( walk -> left_str == "#<procedure define>" ) {
    if ( g_hasRecursive && temp_Recursive ) {
      g_ErrorMessage = "ERROR (level of DEFINE)" ;
      return false ;
    } // if

    if ( !g_change ) {
      g_ErrorMessage = "ERROR (attempt to apply non-function) : " + walk -> left_str ;
      return false ;
    } // if

    else if ( walk -> right != NULL && Eval_define( walk -> right, result, walk ) ) {
      result = result + " defined" ;
      g_change = false ;
      // g_index = -100 ;
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        bool new_line = false ;
        PrettyPrint( walk, new_line ) ;
        g_ErrorMessage = "ERROR (DEFINE format) : " + Error_print( g_pretty_str ) ;
        g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure clean-environment>" ) {
    if ( g_hasRecursive && temp_Recursive ) {
      g_ErrorMessage = "ERROR (level of CLEAN-ENVIRONMENT)" ;
      return false ;
    } // if

    if ( !g_change ) {
      g_ErrorMessage = "ERROR (attempt to apply non-function) : " + walk -> left_str ;
      return false ;
    } // if

    else if ( Eval_clear_define( walk, result ) ) return true ;
    return false ;
  } // else if

  else if ( walk -> left_str == "#<procedure +>" || walk -> left_str == "#<procedure ->" || 
            walk -> left_str == "#<procedure *>" || walk -> left_str == "#<procedure />" ) {
    if ( walk -> right != NULL && Eval_compute( walk -> left_str, walk -> right, result, walk ) ) {
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) 
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( walk -> left_str ) ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure not>" ) {
    if ( walk -> right != NULL && Eval_logic_not( walk, walk -> right, result ) ) {
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) 
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( walk -> left_str ) ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure and>" ) {
    if ( walk -> right != NULL && Eval_logic_and( walk -> left_str, walk, walk -> right, result ) ) {
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) 
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print( walk -> left_str ) ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure or>" ) {
    if ( walk -> right != NULL && Eval_logic_or( walk -> left_str, walk, walk -> right, result ) ) {
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) 
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " +  Error_print( walk -> left_str ) ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure equal?>" ) {
    if ( walk -> right != NULL && Eval_equal( walk, walk -> right, result ) ) {
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) 
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " +  Error_print( walk -> left_str ) ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " +  Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure eqv?>" ) {
    if ( walk -> right != NULL && Eval_eqv( walk, walk -> right, result ) ) {
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) 
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " +  Error_print( walk -> left_str ) ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure begin>" ) {
    if ( walk -> right != NULL && Eval_sequencing( walk -> right, result ) ) {
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) 
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print(  walk -> left_str ) ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + Error_print( g_pretty_str ) ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if

      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure if>" ) {
    if ( walk -> right != NULL && Eval_if( walk, walk -> right, result ) ) {
      // cout << "ifT ans = " << result << endl ;
      return true ;
    } // if

    else {
      if ( g_ErrorMessage == "\0" ) {
        if ( walk -> right_str == "\0" ) 
          g_ErrorMessage = "ERROR (incorrect number of arguments) : " + Error_print(  walk -> left_str ) ;
        else {
          g_pretty_str = "( " ;
          g_space = 2 ;
          g_first_need_lp = true ;
          bool new_line = false ;
          PrettyPrint( walk, new_line ) ;
          g_ErrorMessage = "ERROR (non-list) : " + g_pretty_str ;
          g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
        } // else
      } // if
      // cout << "ifF ans = " << result << endl ;
      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure cond>" ) {
    if ( Eval_cond( walk -> right, result, walk ) ) {
      return true ;
    } // if

    else {
      return false ;
    } // else 
  } // else if

  else if ( walk -> left_str == "#<procedure exit>" ) {
    // if ( g_hasRecursive ) {
    //   g_ErrorMessage = "ERROR (level of EXIT)" ;
    //   return false ;
    // } // else if

    if ( walk -> right != NULL ) {
      if ( g_ErrorMessage == "\0" ) g_ErrorMessage = "ERROR (incorrect number of arguments) : exit" ;
      return false ;
    } // if ()

    else if ( !g_change ) {
      g_ErrorMessage = "ERROR (attempt to apply non-function) : " + walk -> left_str ;
      return false ;
    } // else if

    else {
      if ( walk -> right_str != "\0" ) {
        if ( walk -> right_str == "nil" || walk -> right_str == "()" || walk -> right_str == "#f" ) {
          g_exit = true ;
          return true ;
        } // if ()
        
        else {
          if ( g_ErrorMessage == "\0" ) {    
            bool newline = false ;
            PrettyPrint( walk, newline ) ;
            g_ErrorMessage = "ERROR (non-list) : " + g_pretty_str ;
            g_ErrorMessage.erase( g_ErrorMessage.size() - 1 ) ;
          } // if ()

          return false ;
        } // else 
      } // if()

      g_exit = true ;
      return true ;
    } // else ()

    g_exit = true ;
    return true ;
  } // else if()

  else if ( InList( walk -> left_str, index, value ) ) {
    if ( !single ) {
      result = value ;
      return true ;
    } // if

    else {
      walk -> left_str = g_define_list[index].result ;
      if ( g_change && IsPrimitive_function( walk -> left_str ) ) {
        if ( walk != NULL && EvalSExp( walk, result, single ) ) return true ;
        else return false ;
      } // if

      else {
        string str = "\0" ;
        if ( g_ErrorMessage == "\0" ) {
          if ( g_change ) str = g_define_list[index].result ;
          else str = g_define_list[index].symbol ;
          g_ErrorMessage = "ERROR (attempt to apply non-function) : " + Print_Result( str ) ;
        } // if

        return false ;
      } // else
    } // else



  } // else if()

  else if ( ( IsDigit( walk -> left_str ) || IsFloat( walk -> left_str ) || walk -> left_str == "#t" || 
              walk -> left_str == "#f" || walk -> left_str == "nil" || walk -> left_str == "." || 
              ( walk -> left_str[0] == '\"' && walk -> left_str[walk -> left_str.size()-1] == '\"' ) ||
              ( ! ( IsDigit( walk -> left_str ) || IsFloat( walk -> left_str ) || 
                    walk -> left_str == "#t" || walk -> left_str == "#f" || walk -> left_str == "nil" || 
                    walk -> left_str == "." || 
                    ( walk -> left_str[0] == '\"' && 
                      walk -> left_str[walk -> left_str.size()-1] == '\"' ) ) && 
                InList( walk -> left_str, index, value ) ) ) && 
            ( ( walk -> right == NULL && walk -> left == NULL && single == false &&
                ( walk -> right_str == "\0" || 
                  ( walk -> right_str != "\0" && walk -> right_str == "nil" ) ) )
              || ( InList( walk -> left_str, index, value ) && single == true ) ) ) { 
      
    if ( ! ( IsDigit( walk -> left_str ) || IsFloat( walk -> left_str ) || walk -> left_str == "#t" || 
             walk -> left_str == "#f" || 
             walk -> left_str == "nil" || walk -> left_str == "." || 
             ( walk -> left_str[0] == '\"' && walk -> left_str[walk -> left_str.size()-1] == '\"' ) ) && 
         InList( walk -> left_str, index, value ) ) {
      result = value ;
    }  // if() 

    else result = walk -> left_str ;
    return true ;
  } // else if

  else {  
    if ( g_ErrorMessage == "\0" ) {
      string sss = walk -> left_str ;
      if ( walk -> right_str != "\0" && walk -> right_str != "nil" ) {
        bool new_line = false ;
        g_pretty_str = "( " ;
        g_space = 2 ;
        g_first_need_lp = true ;
        PrettyPrint( walk, new_line ) ;
        g_ErrorMessage = "ERROR (non-list) : " + g_pretty_str ;
        g_ErrorMessage = g_ErrorMessage.erase( g_ErrorMessage.size()-1 ) ;
      } // if

      else if ( IsSymbol( walk -> left_str ) && ( sss[0] != '(' && sss[sss.size()-1] != ')' ) && 
                !IsPrimitive_function_PRO( walk -> left_str ) && g_change == true ) 
        g_ErrorMessage = "ERROR (unbound symbol) : " + Print_Result( walk -> left_str ) ;

      else {
        if ( single ) 
          g_ErrorMessage = "ERROR (attempt to apply non-function) : " + Print_Result( walk -> left_str ) ;
      } // else
    } // if

    return false ;
  } // else
} // EvalSExp()

void Go_all( Tree_node *walk ) {
  if ( walk -> left_str != "\0" && IsPrimitive_function_PRO( walk -> left_str ) ) {
    walk -> left_str = "#<procedure " + walk -> left_str + ">" ;
    // cout << "!!! " << walk -> left_str << endl ;
    if ( walk -> left_str == "#<procedure quote>" ) {
      return ;
    } // if ()
  } // if
  
  if ( walk -> left != NULL ) {
    Tree_node *temp = walk -> left ;
    Go_all( temp ) ;
  } // if
  
  if ( walk -> right != NULL ) {
    Tree_node *temp = walk -> right ;
    Go_all( temp ) ;
  } // if 
  
  if ( walk -> right_str != "\0" && IsPrimitive_function_PRO( walk -> right_str ) ) {
    walk -> left_str = "#<procedure " + walk -> right_str + ">" ;
  } // if
} // Go_all()

int main() {  
  char ch = getchar() ;
  string temp = "\0" ;
  while ( ch != '\n' ) {
    temp = temp + ch ;
    ch = getchar() ; 
  } // while
  
  bool correct = false ;
  cout << "Welcome to OurScheme!" << endl ; 
  string token = GetToken() ;
  g_line = 1 ;
  
  while ( !g_end_of_file && token != "\0" && !g_exit ) {
    if ( S_exp( token ) ) {
      cout << "\n> " ; 

      Tree_node * root = new Tree_node ;
      Init( root ) ;
      BuildTree( root ) ;

      string result = "" ;
      bool new_line = false, ss = false ;
      PrettyPrint( root, new_line ) ;

      if ( g_pretty_str[0] == '(' ) {
        ss = true ;
        g_pretty_str = "( " ;
        g_space = 2 ;
      } // if

      else g_pretty_str = "\0" ;
      
      Go_all( root ) ;
      if ( EvalSExp( root, result, ss ) ) {
        if ( !g_exit ) cout << Print_Result( result ) << endl ;
        else cout << Print_Result( result ) ;
      } // if
      
      else {
        cout << g_ErrorMessage << endl ;
      } // else

      correct = true ; 
    } // if

    else if ( !g_end_of_file ) {
      cout << "\n> " << g_ErrorMessage << endl ;
      if ( g_end_of_line == false ) {
        char ch = getchar() ;
        while ( ch != '\n' && ch != EOF ) ch = getchar() ;
        if ( ch == EOF ) g_end_of_file = true ;
      } // if()

      correct = false ;
    } // else if
    
    g_tokenList.clear() ;
    g_column = 0 ;
    g_token_column = 0 ;
    g_ErrorMessage = "\0" ;
    g_pretty_str = "\0" ;
    g_end_of_line = false ;
    g_first_need_lp = false ;
    g_space = 0 ;
    g_change = true ;
    g_index = -100 ;
    g_local = false ;
    g_now_function = "\0" ;
    g_function_call_number = 0 ;
    g_let_func_call_number = 0 ;
    g_Isfrom = "\0" ;
    g_temp_vector.clear() ; 
    g_hasRecursive = false ;

    if ( !g_end_of_file && correct == true && !g_exit ) {
      if ( !g_now_state ) {
        g_column = 1 ;
        g_token_column = 1 ;
      } // if

      g_line = 1 ;
      token = GetToken() ;
   
      if ( g_line > 1 ) g_line-- ;
   
    } // if()
    
    else if ( !g_end_of_file && correct == false && !g_exit ) {
      g_line = 1 ;
      g_temp_token = "\0" ;
      token = GetToken() ;
    } // else if
  } // while()
  
  if ( g_end_of_file ) {
    if ( token != "\0" && S_exp( token ) ) {
      cout << "\n> " ; 

      Tree_node * root = new Tree_node ;
      Init( root ) ;
      BuildTree( root ) ;

      string result = "" ;
      bool new_line = false, ss = false ;
      PrettyPrint( root, new_line ) ;

      if ( g_pretty_str[0] == '(' ) {
        ss = true ;
        g_pretty_str = "( " ;
        g_space = 2 ;
      } // if

      else g_pretty_str = "\0" ;
      
      Go_all( root ) ;
      if ( EvalSExp( root, result, ss ) ) {
        if ( !g_exit ) cout << Print_Result( result ) << endl ;
        else cout << Print_Result( result ) ;
      } // if
      
      else {
        cout << g_ErrorMessage << endl ;
      } // else
    } // if ()

    cout << "\n> ERROR (no more input) : END-OF-FILE encountered" ;
  } // if ()

  cout << endl << "Thanks for using OurScheme!" ;
  // int a ;
  // cin >> a ;
} // main()