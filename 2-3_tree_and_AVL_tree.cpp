 /* 資訊二甲 10927106 陳鈺翔  10927109 陳宥蓁 */ 
#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>

using namespace std ;
		
struct Graduate_information {
	int element = 0 ;		// idx，唯一序號 
	string schoolname ;		// 學校名稱 
	string departmentname ;	// 科系名稱 
	string type ;		// 日夜別
	string degree ;		// 等級別
	int number_of_students ;	// 學生數 	 
} ; // end Graduate_information

class SchoolList {
	private:		
		vector <Graduate_information> database ;	
		string fileID ;
		void reset() {
		  database.clear() ;
	      fileID.clear() ;
		} // reset()
		
	public:
		vector<Graduate_information> get_school_data() { return database ; } ;
		SchoolList() { reset() ; } 			// constructor	
					
		bool readfile() { // 讀檔 
		  fstream inFile ;
		  string  fileName ;
		  reset() ;
			
		  cout << endl << "Input a file number ([0]Quit): " ;
		  cin >> fileID ;
		  fileName = "input" + fileID + ".txt " ;
			
		  if ( fileID.compare( "0" ) == 0 ) return false ;
		  
		  inFile.open( fileName.c_str(), fstream::in ) ;
		  if ( !inFile.is_open() ) {
			cout << endl << "###" << fileName << "does not exist!###" << endl ;
			return false ;	
		  } // end if
			
		  else {
			char cstr[255] ;
			int fNo,pre,pos ;
			int idx = 1 ;
			inFile.getline( cstr, 255, '\n' ) ;
			inFile.getline( cstr, 255, '\n' ) ;
			inFile.getline( cstr, 255, '\n' ) ;
				
			while ( inFile.getline( cstr, 255, '\n' )  ) {
  			  Graduate_information oneR ;
			  string buf,cut ;
					
			 /* buf : 暫存讀入的一整行內容
			    pos : tab的位置
			    pre :  每一串文字的第一個位置 
			    cut :  擷取的字元                
			    fNo :  目前擷取第幾項資料內容    */ 
					   
			  fNo = 0 ;
			  pre = 0 ;
			  buf.assign( cstr ) ;
			  do {
				pos = buf.find_first_of( '\t', pre ) ;		// 從pre起找'\t'的位置 
				cut = buf.substr( pre, pos - pre ) ;		// 擷取pre開始 pos-pre個字元 
				switch ( ++fNo ) {
				  case 2 : oneR.schoolname = cut ;		// 學校名稱 
					  break ;
				  case 4 : oneR.departmentname = cut ;		// 科系名稱 
					  break ;
				  case 5 : oneR.type = cut ;			// 日夜別 
					  break ;
				  case 6 : oneR.degree = cut ;			// 等級別 
					  break ;
				  case 7 : oneR.number_of_students = get_number( cut ) ;	// 學生數 
					  break ;	
				  default: break ;
				} // end switch
						
				pre = ++pos ;
			  } while ( ( pos > 0 ) && ( fNo <= 11 ) ) ;
					
			  oneR.element = idx ;
			  database.push_back( oneR ) ;
			  idx++ ;			
			} // end while
				
			inFile.close() ;		
		  } // end else
				
		  if ( ! database.size() ) {
			cout << endl << "Get nothing from the file" << fileName << "! ###" << endl ;
			return false ;
		  } // if
		  else return true ;
		} // end readfile

		int get_number( string cstr ) { // string -> int
		  int i = 0 ;
	      int number = 0 ;
	      while( cstr[i] != '\0' ) {
		    if ( cstr[i] <= '9' && cstr[i] >= '0' ) {
			  number = number * 10 ;
			  number = number + ( cstr[i] - '0' ) ;		
		    } // end if
		    i++ ;
	      } // end while
	      
	      return number ;
	    } // get_number()
} ; // end SchoolList

struct node_data {
  string key ;
  vector <int> num ;
};

struct two_three_node {
  vector < node_data > data ; 
  two_three_node *left ;
  two_three_node *middle ;           
  two_three_node *right ;
};

class Two_three_tree {
  private:
    two_three_node *root = NULL ;			 // 2-3 tree root 
    two_three_node *tempp ; 				// 存分裂節點中最大的學校名稱資料 且tempp左中右子樹是有接好的 
    node_data mid ; 				// 存分裂節點中間的(要往上搬的)資料 
    bool spilt ;					 // 是否分裂 
    
  public:
    void build_two_three_tree( vector<Graduate_information> data ) {
      for( int i = 0 ; i < data.size() ; i++ ) {
        // 先找找看要加入的學校名稱是否已經存在，已經存在 find = true，不存在 find = false  
        bool find = search( root, data[i].element, data[i].schoolname ) ; 
	    if( find == false ) {
	      two_three_node *pre = root ;
	      insert( root, data[i].element, pre, data[i].schoolname );
	    } // if() 
      } // for()
    } // build_two_three_tree()
	
	void insert( two_three_node *&now, int index, two_three_node *pre, string str ) {
	  if( now == NULL ) { // empty tree
        now = new two_three_node ;
        node_data a ;
        a.key = str ;
        a.num.push_back( index ) ;
        now -> data.push_back( a ) ;
		now -> left = NULL ;
		now -> middle = NULL ;
		now -> right = NULL ;
	  } // if()	
		
	  else {
	    if( now -> left == NULL && now -> middle == NULL && now -> right == NULL ) {  // now是葉節點
          if( now -> data.size() == 1 ) {  // now是 2-node ( one key )
            if( str.compare( now -> data[0].key ) < 0 ) { // 欲加入的學校名稱 < 目前節點的 key學校名稱 
              now -> data.push_back( now -> data[0] ) ;
		      now -> data[0].num.clear() ;
              now -> data[0].key = str ;
              now -> data[0].num.push_back( index ) ;
              spilt = false ; //不用分裂 
            } // if 

            else { // 欲加入的學校名稱 > 目前節點的 key學校名稱 
              node_data a ;
              a.key = str ;
              a.num.push_back( index ) ;
              now -> data.push_back( a ) ;
              spilt = false ; //不用分裂 
		    } // else if()
          } // if()

          else {
            split_three_node( now, index, str ) ;
			if( spilt == true && now == pre ) split_on_root( now ) ;
		  } // else
        } // if()

        else { // now不是葉節點                         
          if( now -> data.size() == 1 ) { // now是 2-node ( one key )
            if( str.compare( now -> data[0].key ) < 0 ) { // 欲加入的學校名稱 < 目前節點的 key學校名稱  
		      insert( now -> left, index, now, str ) ; //往左走 
		    
              if( spilt == true ) { //分裂過 
                now -> data.push_back( now -> data[0] ) ;
                now -> data[0].num.clear() ;
                swap( now -> data[0], mid ) ;
			    now -> right = now -> middle ;
			    now -> middle = tempp ;
			    mid.key = "" ;
	            mid.num.clear() ;
	            tempp = NULL ;
			    spilt = false ; // 不用分裂了 
              } // if()
            } // if()
  
            else { // 欲加入的學校名稱 > 目前節點的 key學校名稱 
              insert( now -> middle, index, now, str ) ; //往中間走  

              if( spilt == true ) { //分裂過  
                now -> data.push_back( mid ) ;
                now -> right = tempp ;
                mid.key = "" ;
	            mid.num.clear() ;
	            tempp = NULL ;
			    spilt = false ; // 不用分裂了 
              } // if()
            } // else ()
          } // if()

          else {  // now是 3 node ( two key )
            if( str.compare( now -> data[0].key ) < 0 ) { // 欲加入的學校名稱 < 目前節點第一筆資料的 key學校名稱
		      insert( now -> left, index, now, str ) ; //往左走
		      if( spilt == true ) { //分裂過
                vector <int> d ;
                swap( d, mid.num ) ;
                string temp_midValue = mid.key ;
 
		        two_three_node *temp = new two_three_node ; // temp 是存目前欲分裂節點中key最大的資料節點
		        temp -> data.push_back( now -> data[1] ) ;
		        now -> data.pop_back() ;
		      
		        swap( mid, now -> data[0] ) ;
		        now -> data[0].num.clear() ;
		        now -> data[0].key = temp_midValue ;
		        swap( now -> data[0].num, d ) ;
		        
		        temp -> left = now -> middle ;
		        temp -> middle = now -> right ;
		        temp -> right = NULL ;
		        now -> middle = tempp ;
		        now -> right = NULL ;
			    tempp = temp ;
			    temp = NULL ;
		      } // if()
            } // if()
          
            else if( str.compare( now -> data[1].key ) < 0 ) {  // 欲加入的學校名稱 < 目前節點第二筆的 key學校名稱 
		      insert( now -> middle, index, now, str ) ; //往中間走
              if( spilt == true ) { //分裂過
                two_three_node *temp = new two_three_node ; // temp 是存目前欲分裂節點中key最大的資料節點  
	            temp -> data.push_back( now -> data[1] ) ;
		        now -> data.pop_back() ;
	            temp -> left = tempp ;
	            temp -> middle = now -> right ;
	            temp -> right = NULL ;
	            now -> right = NULL ;
	            tempp = temp ;
			    temp = NULL ;
		      } // if()
	        } // else if()
	      
            else {   // 欲加入的學校名稱 > 目前節點第二筆的 key學校名稱  
		      insert( now -> right, index, now, str ) ; //往右走 
		      if( spilt == true ) { //分裂過
		        two_three_node *temp = new two_three_node ; // temp 是存目前欲分裂節點中key最大的資料節點   
		        temp -> data.push_back( mid ) ;
		        mid.num.clear() ;
		        swap( mid, now -> data[1] ) ;
		        now -> data.pop_back() ;
		        temp -> left = now -> right ;
		        temp -> middle = tempp ;
		        temp -> right = NULL ;
		        now -> right = NULL ;
			    tempp = temp ;
			    temp = NULL ;
		      } // if()
            } // else 
          } // else() 
    
	      if( spilt == true && now == pre ) split_on_root( now ) ;
        } // else
      } // else 
    } // insert()

    void split_on_root( two_three_node *&now ) {
      two_three_node *temp = new two_three_node ;
	  temp -> data.push_back( mid ) ;
	  temp -> left = now ;
	  temp -> middle = tempp ;
      temp -> right = NULL ;
      now = temp ;
	          
	  temp = NULL ;
	  tempp = NULL ;
      mid.key = "" ;
	  mid.num.clear() ;
      spilt = false ; 
	} // split_on_root()

  	void split_three_node( two_three_node *current, int index, string str ) { 
	  tempp = new two_three_node ;
	  
      if ( str.compare( current -> data[0].key ) < 0 ) {
      	swap( mid, current -> data[0] ) ; 
        current -> data[0].num.clear() ;
        
        tempp -> data.push_back( current -> data[1] ) ;
        current -> data.pop_back() ;
          
        current -> data[0].key = str ;
        current -> data[0].num.push_back( index ) ;
      } // if()

      else if ( str.compare( current -> data[1].key ) < 0 ) {
        mid.key = str ;
        mid.num.push_back( index ) ;
        
        tempp -> data.push_back( current -> data[1] ) ;
        current -> data.pop_back() ;
      } // else if()

      else {
        swap( mid, current -> data[1] ) ;
        current -> data.pop_back() ;
        
        node_data a ;
        a.key = str ;
        a.num.push_back( index ) ;
        tempp -> data.push_back( a ) ;
      } // else 

      tempp -> left = NULL ;
	  tempp -> middle = NULL ;
	  tempp -> right = NULL ;
	  spilt = true ;
    } // split_three_node()

    bool search( two_three_node *now, int index, string str ) {
      if( now != NULL ) {
        if( now -> data.size() == 1 ) {   //目前指向節點是 2-node ( one key )
          if( str.compare( now -> data[0].key ) == 0 ) {
          	now -> data[0].num.push_back( index ) ;
          	return true ;
		  } // if()
		  
          else if( str.compare( now -> data[0].key ) < 0 ) return search( now -> left , index, str );   
          else return search( now -> middle, index, str );
        } // if () 

        else {   //目前指向節點是 3-node ( two key )
          if( str.compare( now -> data[0].key ) == 0 ) {
          	now -> data[0].num.push_back( index ) ;
          	return true ;
		  } // if()
		  
          else if( str.compare( now -> data[0].key ) < 0 ) return search( now -> left , index, str ) ;
          else if( str.compare( now -> data[1].key ) == 0 ) {
          	now -> data[1].num.push_back( index ) ;
          	return true ;
		  } // else if()
		  
		  else if( str.compare( now -> data[1].key ) < 0 ) return search( now -> middle, index, str ) ;
		  else return search( now -> right, index, str );
        } // else 
      } // if()
      
      else return false ;
    } // search()
	
	int Height( two_three_node *root ) {  // 樹高 
	  if( root == NULL ) return 0 ;
      else return Height( root -> left ) +1 ;
	} // Height()

	void display( vector<Graduate_information> datas ) { 
	  cout << "Tree height = " <<  Height( root ) << endl ;
	  
	  int i = 0 , j = 0 , count = 1 ;
      if( root -> data.size() == 2 ) {
      	if( root -> data[0].num[i] < root -> data[1].num[j] ) {
      	  for( int i = 0 ; i < root -> data[0].num.size() ; i++, count++ ) cout << count << ": [" << root -> data[0].num[i] << "] " << root -> data[0].key << ", " << datas[root -> data[0].num[i]-1].departmentname << ", " << datas[root -> data[0].num[i]-1].type << ", " << datas[root -> data[0].num[i]-1].degree << ", " << datas[root -> data[0].num[i]-1].number_of_students << endl ;
	      for( int j = 0 ; j < root -> data[1].num.size() ; j++, count++ ) cout << count << ": [" << root -> data[1].num[j] << "] " << root -> data[1].key << ", " << datas[root -> data[1].num[j]-1].departmentname << ", " << datas[root -> data[1].num[j]-1].type << ", " << datas[root -> data[1].num[j]-1].degree << ", " << datas[root -> data[1].num[j]-1].number_of_students << endl ;
		} // if()
		
		else {
		  for( int j = 0 ; j < root -> data[1].num.size() ; j++, count++ ) cout << count << ": [" << root -> data[1].num[j] << "] " << root -> data[1].key << ", " << datas[root -> data[1].num[j]-1].departmentname << ", " << datas[root -> data[1].num[j]-1].type << ", " << datas[root -> data[1].num[j]-1].degree << ", " << datas[root -> data[1].num[j]-1].number_of_students << endl ;
		  for( int i = 0 ; i < root -> data[0].num.size() ; i++, count++ ) cout << count << ": [" << root -> data[0].num[i] << "] " << root -> data[0].key << ", " << datas[root -> data[0].num[i]-1].departmentname << ", " << datas[root -> data[0].num[i]-1].type << ", " << datas[root -> data[0].num[i]-1].degree << ", " << datas[root -> data[0].num[i]-1].number_of_students << endl ;
		} // else 
	  } // if()
      
      else for( int i = 0 ; i < root -> data[0].num.size() ; i++, count++ ) cout << count << ": [" << root -> data[0].num[i] << "] " << root -> data[0].key << ", " << datas[root -> data[0].num[i]-1].departmentname << ", " << datas[root -> data[0].num[i]-1].type << ", " << datas[root -> data[0].num[i]-1].degree << ", " << datas[root -> data[0].num[i]-1].number_of_students << endl ;
	} // display()
};

struct Avltree_node {
	vector <int> idx ;			// 每筆的序號 
	string key ;			// key，也就是科系名稱 
	Avltree_node* left ;
	Avltree_node* right ;
} ; // end Twothreetree_node

class AVLTree {
	private:
	  Avltree_node* root ;	//樹根 
	
	public:
	  AVLTree() { root = NULL ; }		// constructor 
	  Avltree_node* get_root() { return root ; }
		
	  void eliminate( Avltree_node* &node ) {
		if( node !=  NULL ){
			eliminate( node -> left ) ;
			eliminate( node -> right ) ;
			delete node ;
		} //  end if
	  } // end eliminate
		
	  void buildavltree( vector<Graduate_information> data ) {
		for ( int i = 0 ; i < data.size() ; i++ ) root = insert( data[i], root ) ;
	  } // end buildavltree
		
	  Avltree_node* newnode( Graduate_information x ) {
		Avltree_node* node = new Avltree_node ;	
		node -> left = NULL ;
		node -> right = NULL ;
		node -> idx.push_back( x.element ) ;		// idx
		node -> key = x.departmentname ;		// 科系名稱
		return node ;
	  } // end newnode 
		
	  Avltree_node* insert( Graduate_information x, Avltree_node* node ) {
		//    1.先找目前資料在 樹中位置 
	
		// *********目前節點為NULL*************
		if ( node == NULL ) 
			return newnode( x ) ;	// 回傳此處節點，當遞迴回來時，讓原先指向NULL的節點可以指向新的節點		

		// ********** 資料的科系名稱 > 目前節點的科系名稱 ******** 
		else if ( x.departmentname.compare( node -> key ) > 0  ) 
			node -> right = insert( x, node -> right ) ;	// 繼續往右走，遞迴回來時，如果有旋轉，原先node -> right並不是我們要的節點，
								                            // 所以要讓 node -> right指向旋轉過後新的節點 
		
		// ********** 資料的科系名稱 < 目前節點的科系名稱 ******** 
		else if ( x.departmentname.compare( node -> key ) < 0 )
			node -> left = insert( x, node -> left ) ;	// 繼續往左走，遞迴回來時，如果有旋轉，原先node -> left並不是我們要的節點， 
								                        // 所以要讓 node -> left指向旋轉過後新的節點
								                        
		// ********** 資料的科系名稱 == 目前節點的科系名稱 ******** 
		else if ( x.departmentname.compare( node -> key ) == 0 ) node -> idx.push_back( x.element ) ;	// idx
		
		//    2.計算平衡係數，是否要平衡
		int BF = height( node -> left ) - height( node -> right ) ;	 
	
		//    3.平衡 
		if ( BF == 2 ) {						
		  BF = height( node -> left -> left ) - height( node -> left -> right ) ;
		
		  if ( BF == 1 || BF == 0 ) 		// LL
				return rightrotate( node ) ;

		  else {					// LR
			node -> left = leftrotate( node -> left ) ;		
			return rightrotate( node ) ;
		  } // end else
		} // end if
	 
		else if ( BF == -2 ) {			
		  BF = height( node -> right -> left ) - height( node -> right -> right ) ;
							
	 	  if ( BF == -1 || BF == 0 )		
			return leftrotate( node ) ;		// RR
			
		  else {						
			node -> right = rightrotate( node -> right ) ;	// RL
			return leftrotate( node ) ;				
		  } // end else		
		} // end else if
		
		return node ;	// 沒有節點需要做改變 	
	  } // end insert
	
		/* 左(右)旋 寫法 
	   	1.將右(左) 子樹的左(右) 子節點 用一個指標暫存他的位置 
	   	2.將右(左) 子樹的左(右) 子節點連接到 目前位置的節點 
	  	3.最後把當前節點的右(左)節點 指向一開始暫存右(左) 子樹的左(右) 子節點的位置 
	  	4.回傳節點讓父節點跟回傳的節點接起來  */ 
		Avltree_node* leftrotate( Avltree_node* node ) {
			Avltree_node* subright = node -> right ;		
			Avltree_node* temp = subright -> left ;		
	 
			subright -> left = node ;			
			node -> right = temp ; 
	
			return subright ;	
		} // end leftrotate
		
		Avltree_node* rightrotate( Avltree_node* node ) {
			Avltree_node* subleft = node -> left ;		 
			Avltree_node* temp = subleft -> right ;		
	 
			subleft -> right = node ;			
			node -> left = temp ; 
	
			return subleft ;	 
		} // end rightrotate
					
		int height( Avltree_node* node ) {
	      int left, right ;
	      if ( node == NULL ) return 0 ;
	      else {
		    left = height( node -> left ) ;	
		    right = height( node -> right ) ;
	      } // end else
		
	      return ( max( left, right ) + 1 ) ; 
        } // end height
		
		void display( vector<Graduate_information> data ) {
			cout << "Tree height: " << height( root ) << endl ;
			for( int i = 0 ; i < root -> idx.size() ; i++ ) {
				cout << i+1 << ": [" << data[ root -> idx[i] - 1 ].element << "] " << data[ root -> idx[i] - 1 ].schoolname << ", " << root -> key <<
				", " << data[ root -> idx[i] - 1 ].type << ", " << data[ root -> idx[i] - 1 ].degree << ", " << data[ root -> idx[i] - 1 ].number_of_students<< endl ; 
			} // end for
		} // end display
	
		~AVLTree() { eliminate( root ) ; }	  // destructor 
} ; // end AVLTree

void start() {
  cout << endl << "*** Search Tree Utilities ***" ;
  cout << endl << "* 0. Quit                   *" ;
  cout << endl << "* 1. Build a 2-3tree	    *" ;
  cout << endl << "* 2. Build a AVLtree	    *" ;
  cout << endl << "*****************************" << endl ;
  cout << "Please input command(0, 1, 2): "  ;			
} // start()

int react() {     // determond command 
  start() ;	      // 初始畫面 	
  string s ;
  cin >> s ;
  
  int length = s.length() ;
  if ( length == 1 && s[0] == '0' ) return 0 ;
  else if ( length == 1 && s[0] == '1' ) return 1 ;
  else if ( length == 1 && s[0] == '2' ) return 2 ;
  else if ( length == 1 && s[0] == '3' ) return 3 ;
  else return -1;	
} // React()

void mission1( SchoolList x ) {
  Two_three_tree two_three_tree ;
  two_three_tree.build_two_three_tree( x.get_school_data() ) ;
  two_three_tree.display( x.get_school_data() ) ;
} // end mission1

void mission2( SchoolList x ) {
	AVLTree y ;	
	y.buildavltree( x.get_school_data() ) ;
	y.display( x.get_school_data() ) ;
} // emd mission2

int main() {
	while (1) {
		int option = react() ;
		
		if ( option == 0 ) return 0 ;
				
		else if ( option == 1 )  {
			SchoolList x ;			
			bool readfilesuccessfully = x.readfile() ;	// 讀檔 		 

			while ( ( option == 0 || option == 1 || option == 2 ) && readfilesuccessfully == true ) {
				if ( option == 0 ) return 0 ;
				else if ( option == 1 ) mission1( x ) ;
				else if ( option == 2 ) mission2( x ) ;	
					
				option = react() ;
				if ( option != 0 && option != 1 && option != 2 ) cout << "\ncommand does not exist" << endl ;
				else if ( option == 1 ) readfilesuccessfully = x.readfile() ; // 讀檔 	
			} // end while
		} // end else if
		
		else if ( option == 2 ) cout << endl << "### Choose 1 first. ###" << endl ;
      	else cout << "\nPlease enter the command again\n" ;
 	} // end while
} // end main
