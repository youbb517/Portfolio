import os

reference_string = ""
fileName = ""
page_frames = 0
page_frame_list = []
page_falut = 0
page_replace = 0

def FIFO() :
    global page_frame_list, page_falut, page_replace
    in_list = False
    ans = []
    page_falut_list = []

    for i in range( len( reference_string ) ) :
        if ( len( page_frame_list ) < page_frames ) :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] : 
                    in_list = True 
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 0 ]
               page_frame_list.append( temp )
               page_falut = page_falut + 1
        
        else :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] : 
                    in_list = True 
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 0 ]
               page_frame_list.pop(0)
               page_frame_list.append( temp )
               page_falut = page_falut + 1
               page_replace = page_replace + 1
        
        ans_temp = []
        for y in range( len( page_frame_list ) ) : ans_temp.append( page_frame_list[y][0] )
        ans_temp.reverse()
        ans.append( ans_temp )
        if in_list == False : page_falut_list.append( "F" )
        else : ans_temp = page_falut_list.append( "" )

        in_list = False

    return ans, page_falut_list

def LRU() :
    global page_frame_list, page_falut, page_replace
    in_list = False
    ans = []
    page_falut_list = []

    for i in range( len( reference_string ) ) :
        if ( len( page_frame_list ) < page_frames ) :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] : 
                    in_list = True 
                    temp = [ reference_string[i], 0 ]
                    page_frame_list.pop(j)
                    page_frame_list.append( temp )
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 0 ]
               page_frame_list.append( temp )
               page_falut = page_falut + 1
        
        else :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] : 
                    in_list = True 
                    temp = [ reference_string[i], 0 ]
                    page_frame_list.pop(j)
                    page_frame_list.append( temp )
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 0 ]
               page_frame_list.pop(0)
               page_frame_list.append( temp )
               page_falut = page_falut + 1
               page_replace = page_replace + 1
        
        ans_temp = []
        for y in range( len( page_frame_list ) ) : ans_temp.append( page_frame_list[y][0] )
        ans_temp.reverse()
        ans.append( ans_temp )
        if in_list == False : page_falut_list.append( "F" ) 
        else : ans_temp = page_falut_list.append( "" )

        in_list = False

    return ans, page_falut_list

def LFU() :
    global page_frame_list, page_falut, page_replace
    in_list = False
    ans = []
    page_falut_list = []

    for i in range( len( reference_string ) ) :
        if ( len( page_frame_list ) < page_frames ) :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] : 
                    page_frame_list[j][1] = page_frame_list[j][1] + 1
                    in_list = True 
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 1 ]
               page_frame_list.append( temp )
               page_falut = page_falut + 1
        
        else :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] : 
                    page_frame_list[j][1] = page_frame_list[j][1] + 1
                    in_list = True 
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 1 ]
               index = pop_count_min( page_frame_list )
               page_frame_list.pop(index)
               page_frame_list.append( temp )
               page_falut = page_falut + 1
               page_replace = page_replace + 1
        
        ans_temp = []
        for y in range( len( page_frame_list ) ) : ans_temp.append( page_frame_list[y][0] )
        ans_temp.reverse()
        ans.append( ans_temp )
        if in_list == False : page_falut_list.append( "F" ) 
        else : ans_temp = page_falut_list.append( "" ) 

        in_list = False

    return ans, page_falut_list

def MFU() :
    global page_frame_list, page_falut, page_replace
    in_list = False
    ans = []
    page_falut_list = []

    for i in range( len( reference_string ) ) :
        if ( len( page_frame_list ) < page_frames ) :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] : 
                    page_frame_list[j][1] = page_frame_list[j][1] + 1
                    in_list = True 
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 1 ]
               page_frame_list.append( temp )
               page_falut = page_falut + 1
        
        else :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] : 
                    page_frame_list[j][1] = page_frame_list[j][1] + 1
                    in_list = True 
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 1 ]
               index = pop_count_max( page_frame_list )
               page_frame_list.pop(index)
               page_frame_list.append( temp )
               page_falut = page_falut + 1
               page_replace = page_replace + 1
        
        ans_temp = []
        for y in range( len( page_frame_list ) ) : ans_temp.append( page_frame_list[y][0] )
        ans_temp.reverse()
        ans.append( ans_temp )
        if in_list == False : page_falut_list.append( "F" ) 
        else : ans_temp = page_falut_list.append( "" ) 

        in_list = False

    return ans, page_falut_list

def LFU_LRU() :
    global page_frame_list, page_falut, page_replace
    in_list = False
    ans = []
    page_falut_list = []

    for i in range( len( reference_string ) ) :
        if ( len( page_frame_list ) < page_frames ) :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] : 
                    in_list = True 
                    temp = [ page_frame_list[j][0], page_frame_list[j][1] + 1 ]
                    page_frame_list.pop(j)
                    page_frame_list.append( temp )
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 1 ]
               page_frame_list.append( temp )
               page_falut = page_falut + 1
        
        else :
            for j in range( len( page_frame_list ) ) :
                if page_frame_list[j][0] == reference_string[i] :
                    in_list = True
                    temp = [ page_frame_list[j][0], page_frame_list[j][1] + 1 ]
                    page_frame_list.pop(j)
                    page_frame_list.append( temp )
                    break
            
            if in_list == False :
               temp = [ reference_string[i], 1 ]
               index = pop_count_min( page_frame_list )
               page_frame_list.pop(index)
               page_frame_list.append( temp )
               page_falut = page_falut + 1
               page_replace = page_replace + 1
        
        ans_temp = []
        for y in range( len( page_frame_list ) ) : ans_temp.append( page_frame_list[y][0] )
        ans_temp.reverse()
        ans.append( ans_temp )
        if in_list == False : page_falut_list.append( "F" ) 
        else : ans_temp = page_falut_list.append( "" ) 

        in_list = False

    return ans, page_falut_list

def pop_count_min( page_frame_list ) :
    index = 0 
    count = page_frame_list[0][1]
    for i in range( len( page_frame_list ) ) :
        if ( count > page_frame_list[i][1] ) : 
            index = i 
            count = page_frame_list[i][1]

    return index

def pop_count_max( page_frame_list ) :
    index = 0 
    count = page_frame_list[0][1]
    for i in range( len( page_frame_list ) ) :
        if ( count < page_frame_list[i][1] ) : 
            index = i 
            count = page_frame_list[i][1]

    return index

def write_file( file, ans, page_falut_list, method ) :
    if method == 1 : file.write( "--------------FIFO-----------------------\n" )
    elif method == 2 : file.write( "--------------LRU-----------------------\n" ) 
    elif method == 3 : file.write( "--------------Least Frequently Used Page Replacement-----------------------\n" ) 
    elif method == 4 : file.write( "--------------Most Frequently Used Page Replacement -----------------------\n" ) 
    elif method == 5 : file.write( "--------------Least Frequently Used LRU Page Replacement-----------------------\n" ) 

    for i in range( len( ans ) ) :
       if ( i < len( reference_string ) ) : file.write( reference_string[i] + "\t" )
       for j in range( len( ans[i] ) ) : file.write( ans[i][j] )
       if ( ( i < len( page_falut_list ) ) and ( page_falut_list[i] == "F" ) ) : file.write( "\t" + page_falut_list[i] )
       file.write( "\n" )

    file.write( "Page Fault = " + str(page_falut) + "  Page Replaces = " + str(page_replace) + "  Page Frames = " + str(page_frames) + "\n" )

def Init() : 
    global page_frame_list, page_falut, page_replace
    page_frame_list.clear()
    page_falut = 0 
    page_replace = 0

def main() :
    run = True
    global reference_string  # 存輸入的reference string
    global fileName
    global page_frames 
    global page_frame_list
    global page_falut
    global page_replace

    while ( run == True ) :
        Init()
        reference_string = ""
        fileName = ""
        print( 'Please enter File Name(eg. input1、input2): ' )
        fileName = input()
        method = 0 
        now_line_num = 0
        eof = False

        # 判斷檢查檔案是否存在
        if( os.path.isfile( fileName + ".txt" ) ) :
            file = open( fileName + ".txt", 'r' )

            while not eof :
                file_line = file.readline()
                now_line_num = now_line_num + 1
                temp_list = []

                if not file_line : eof = True 

                else :
                    if now_line_num == 1 :
                        temp_list = file_line.split() 
                        method = int( temp_list[0] )
                        page_frames = int( temp_list[1] )

                    elif now_line_num == 2 : 
                       temp_list = file_line.split() 
                       reference_string = temp_list[0]
                       eof = True

            file.close()

            if method == 0 :
                print( '程式結束!!!' )
                run = False 

            elif method == 1 :
              file = open( "out_" + fileName + ".txt", 'w' ) 
              ans, page_falut_list = FIFO()
              write_file( file, ans, page_falut_list, method )
              file.close()

            elif method == 2 :
              file = open( "out_" + fileName + ".txt", 'w' ) 
              ans, page_falut_list = LRU()
              write_file( file, ans, page_falut_list, method )
              file.close()
            
            elif method == 3 :
              file = open( "out_" + fileName + ".txt", 'w' ) 
              ans, page_falut_list = LFU()
              write_file( file, ans, page_falut_list, method )
              file.close()

            elif method == 4 :
              file = open( "out_" + fileName + ".txt", 'w' ) 
              ans, page_falut_list = MFU()
              write_file( file, ans, page_falut_list, method )
              file.close()

            elif method == 5 :
              file = open( "out_" + fileName + ".txt", 'w' ) 
              ans, page_falut_list = LFU_LRU()
              write_file( file, ans, page_falut_list, method )
              file.close()
            
            elif method == 6 :
              file = open( "out_" + fileName + ".txt", 'w' ) 
              ans, page_falut_list = FIFO()
              write_file( file, ans, page_falut_list, 1 )
              file.write( "\n" )
              Init()
              
              ans, page_falut_list = LRU()
              write_file( file, ans, page_falut_list, 2 )
              file.write( "\n" )
              Init()

              ans, page_falut_list = LFU()
              write_file( file, ans, page_falut_list, 3 )
              file.write( "\n" )
              Init()

              ans, page_falut_list = MFU()
              write_file( file, ans, page_falut_list, 4 )
              file.write( "\n" )
              Init()

              ans, page_falut_list = LFU_LRU()
              write_file( file, ans, page_falut_list, 5 )
              file.close()

            else : print( '沒有這個頁置換法!!!' )
        else : print( '開檔失敗!!!' )

if __name__ == '__main__':
    main()