import os
import time
from datetime import datetime
from datetime import timezone, timedelta
import multiprocessing
import threading
import queue

dataList = [] # 放存入資料

def BubbleSort( list, method, q ) :
    n = len( list )
    for i in range( n - 1 ) :
        for j in range( n - i - 1 ) :
            if list[j] > list[j+1] :
                temp = list[j]
                list[j] = list[j+1]
                list[j+1] = temp
    
    if method == 2 or method == 3 or method == 4 :
        q.put( list )

def Merge( leftList, rightList, q ) :
    temp_list = []

    leftIndex = 0 
    rightIndex = 0

    while ( leftIndex < len( leftList ) and rightIndex < len( rightList ) ) :
        if ( leftList[leftIndex] <= rightList[rightIndex] ) :
            temp_list.append( leftList[leftIndex] )
            leftIndex = leftIndex + 1

        else :
            temp_list.append( rightList[rightIndex] )
            rightIndex = rightIndex + 1
    
    while ( ( leftIndex < len( leftList ) ) and ( rightIndex >= len( rightList ) ) ) :
        temp_list.append( leftList[leftIndex] )
        leftIndex = leftIndex + 1

    while ( ( leftIndex >= len( leftList ) ) and ( rightIndex < len( rightList ) ) ) :
        temp_list.append( rightList[rightIndex] )
        rightIndex = rightIndex + 1

    q.put( temp_list )

def OutFile( list, outputFileName, CPU_time ):
    file = open( outputFileName, 'w' ) 
    file.write( "Sort : \n" )
    for i in range( len( list ) ): file.write( str( list[i] ) + "\n" ) 

    # datetime.timezone 負責時區的轉換 
    # datetime.timedelta 增加或減少日期或時間
    # (時區為 UTC+8) 
    
    utc = timezone.utc
    utc_time = datetime.utcnow().replace( tzinfo = utc )
    time_Taiwan = utc_time.astimezone( timezone( timedelta( hours = 8 ) ) )

    file.write("CPU Time : ")
    file.write(str(CPU_time))
    file.write( "\n" )
    file.write("Output Time : " )
    file.write(str(time_Taiwan)) 
    file.close()

# 將讀入資料進行切割
def splitList( k ) :
    return_list = []
    isDivisible = True
    
    n = len( dataList ) // k 
    if ( len( dataList ) % k != 0 ) : isDivisible = False 

    index = 0
    for i in range( 1, k + 1 ) :
        if ( i == k and isDivisible == False ) : return_list.append( dataList[ index : len( dataList ) ] )
        else : return_list.append( dataList[ index : i * n ] )
        index = i * n 

    return return_list

def method2( k, split_List, q ) :
    for i in range( k ) : BubbleSort( split_List[i], 2, q )
    while q.qsize() != 1 : Merge( q.get(), q.get(), q )

def main() :

    global dataList  # 存輸入的資料
    run = True

    while ( run == True ) :
        dataList.clear()
        print( '請輸入檔案名稱: ' )
        fileName = input()

        # 判斷檢查檔案是否存在
        if( os.path.isfile( fileName + ".txt" ) ) :
            file = open( fileName + ".txt", 'r' )
            dataList = list( map ( int, file.read().split() ) ) # 讀檔案的資料
            file.close()

            print( '請輸入要切成幾分:(正整數) ' )
            k = int( input() )
            while k <= 0 :
                print( '不符合要求，請再次輸入要切成幾分:(正整數) ' )
                k = int( input() )

            print( '請輸入方法編號:(方法1，方法2，方法3，方法4) ' )
            method = int( input() )
            while method <= 0 or method > 4 :
                print( '不符合4種中的一種，請再次輸入方法編號:(方法1，方法2，方法3，方法4) ' )
                method = int( input() )
            
            if method == 0 :
                print( '程式結束!!!' )
                run = False 

            elif method == 1 :
                start = time.time()
                q = queue.Queue
                BubbleSort( dataList, method, q )
                end = time.time()
                OutFile( dataList, fileName + "_output" + str(1) + ".txt", end - start )

            elif method == 2 :
                split_List = splitList( k )

                # Manager 提供 processes 之間的共享資料 
                # 管理共享信息的管理者(Manager)
                # queue.Queue()的方法不適合用於 process 間的通信

                a = multiprocessing.Manager()
                q = a.Queue()
                start = time.time()
                p = multiprocessing.Process( target = method2, args = ( k, split_List, q ) )
                p.start()
                p.join()
                end = time.time()
                OutFile( q.get(), fileName + "_output" + str(2) + ".txt", end - start )
            
            elif method == 3 :
                split_List = splitList( k )
                a = multiprocessing.Manager()
                q = a.Queue()
                start = time.time()

                # 建立 k 個 process
                process_list = []
                for i in range( k ) :
                    process_list.append( multiprocessing.Process( target = BubbleSort, args = ( split_List[i], method, q ) ) )
                    process_list[i].start()

                # 等待 process 結束
                for i in range( k ):
                    process_list[i].join()

                process_list.clear()

                process_list2 = []
                have_process_num = 0

                while have_process_num != ( k - 1 ) and q.qsize != 1 :
                        
                    for i in range( q.qsize() // 2 ) :
                        p = multiprocessing.Process( target = Merge, args = ( q.get(), q.get(), q ) )
                        p.start()
                        process_list2.append( p )
                        have_process_num = have_process_num + 1

                    for i in range( len( process_list2 ) ) :
                        process_list2[i].join()
                        
                    process_list2.clear()
                    
                end = time.time()
                OutFile( q.get(), fileName + "_output" + str(3) + ".txt", end - start )

            elif method == 4 :
                split_List = splitList( k )
                q = queue.Queue()
                start = time.time()

                # 建立 k 個 thread
                thread_list = []
                for i in range( k ) :
                    thread_list.append( threading.Thread( target = BubbleSort, args = ( split_List[i], method, q ) ) )
                    thread_list[i].start()

                # 等待 thread 結束
                for i in range( k ):
                    thread_list[i].join()

                thread_list.clear()

                thread_list2 = []
                have_thread_num = 0

                while have_thread_num != ( k - 1 ) and q.qsize != 1 :
                        
                    for i in range( q.qsize() // 2 ) :
                        t = threading.Thread( target = Merge, args = ( q.get(), q.get(), q ) )
                        t.start()
                        thread_list2.append( t )
                        have_thread_num = have_thread_num + 1

                    for i in range( len( thread_list2 ) ) :
                        thread_list2[i].join()
                        
                    thread_list2.clear()
                    
                end = time.time()
                OutFile( q.get(), fileName + "_output" + str(4) + ".txt", end- start )

            else :
                print( '沒有這個方法!!!' )

        else : print( '開檔失敗!!!' )
if __name__ == '__main__':
    main()