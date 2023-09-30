import os
import queue

dataList = []
fileName = ""
Gantt_chart = []

def FCFS() :
    start_time = 0
    end_time = 0
    time_list = []
    global Gantt_chart

    for i in range( len( dataList ) ) :
        if start_time < dataList[i][2] :
            for j in range( dataList[i][2] - start_time ) : Gantt_chart.append( -1 ) # -1 代表CPU閒置
            start_time = dataList[i][2]

        start_time = start_time + dataList[i][1]
        for y in range( dataList[i][1] ) : Gantt_chart.append( dataList[i][0] )

        end_time = start_time
        temp = [dataList[i][0], end_time, dataList[i][2], dataList[i][1]] # ID # end_time # Arrival Time # CPU Burst 
        time_list.append( temp )

    time_list.sort( key = lambda x:x[0] )
    return time_list

def RR( time ) :
    global Gantt_chart
    start_time = 0
    end_time = 0
    time_list = []
    ready_queue = queue.Queue()
    time_slice = time
    i = 0 

    while ( i < len( dataList ) ) or ready_queue.qsize() != 0 :
        if i < len( dataList )  and start_time < dataList[i][2] and ready_queue.empty() :
            for jj in range( dataList[i][2] - start_time ) : Gantt_chart.append( -1 ) # -1 代表CPU閒置
            start_time = dataList[i][2]

        for j in range( i , len( dataList ) ) :
            if ( dataList[j][2] > start_time ) : 
                i = j
                break 

            else : 
                # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 、 # 剩餘時間
                temp = [dataList[j][0], dataList[j][1], dataList[j][2], dataList[j][3], dataList[j][1]]
                ready_queue.put( temp ) # 將到達的process放入ready_queue中
                i = i + i

        if ( ready_queue.qsize() != 0 ) :
            cmp = ready_queue.get()

            if ( cmp[4] <= time_slice ) :
                start_time = start_time + cmp[4]
                for y in range( cmp[4] ) : Gantt_chart.append( cmp[0] )
                end_time = start_time
                temp = [cmp[0], end_time, cmp[2], cmp[1]] # ID # end_time # Arrival Time # CPU Burst
                time_list.append( temp )

            else :
                start_time = start_time + time_slice 
                for y in range( time_slice ) : Gantt_chart.append( cmp[0] )
                temp = [cmp[0], cmp[1], cmp[2], cmp[3], cmp[4] - time_slice] # ID # CPU Burst # Arrival Time # Priority # 剩餘時間

                for k in range( i , len( dataList ) ) :
                    if ( dataList[k][2] > start_time ) : 
                        i = k 
                        break 

                    else : 
                        # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 、 # 剩餘時間
                        temp1 = [dataList[k][0], dataList[k][1], dataList[k][2], dataList[k][3], dataList[k][1]]
                        ready_queue.put( temp1 ) # 將到達的process放入ready_queue中
                        i = i + 1

                ready_queue.put( temp )
            
    time_list.sort( key = lambda x:x[0] )
    return time_list

def SJF() :
    global Gantt_chart
    start_time = 0
    end_time = 0
    time_list = []
    ready_list = []
    ready_queue = queue.Queue()
    i = 0 

    while ( i < len( dataList ) ) or ready_queue.qsize() != 0 :
        if i < len( dataList ) and start_time < dataList[i][2] and ready_queue.empty() :
            for jj in range( dataList[i][2] - start_time ) : Gantt_chart.append( -1 ) # -1 代表CPU閒置
            start_time = dataList[i][2]

        for j in range( i, len( dataList ) ) :
            if ( dataList[j][2] > start_time ) : 
                i = j
                break 

            else : 
                # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 
                temp = [dataList[j][0], dataList[j][1], dataList[j][2], dataList[j][3]]
                ready_queue.put( temp ) # 將到達的process放入ready_queue
                i = i + 1

        if ( ready_queue.qsize() != 0 ) :
            # 先依照CPU_Brust排序
            ready_list = []
            while ready_queue.qsize() != 0 : ready_list.append( ready_queue.get() )
            ready_list.sort( key = lambda x:x[1] )
            for ii in range( len( ready_list ) ) : ready_queue.put( ready_list[ii] )
            ready_list.clear()
            # 取出queue第一個
            cmp = ready_queue.get()

            start_time = start_time + cmp[1]
            for y in range( cmp[1] ) : Gantt_chart.append( cmp[0] )

            end_time = start_time
            temp = [cmp[0], end_time, cmp[2], cmp[1]] # ID # end_time # Arrival Time # CPU Burst
            time_list.append( temp )
            
    time_list.sort( key = lambda x:x[0] )
    return time_list

def SRTF() :
    global Gantt_chart
    start_time = 0
    end_time = 0
    time_list = []
    ready_list = []
    ready_queue = queue.Queue()
    i = 0 

    while ( i < len( dataList ) ) or ready_queue.qsize() != 0 :
        if i < len( dataList ) and start_time < dataList[i][2] and ready_queue.empty() :
            for jj in range( dataList[i][2] - start_time ) : Gantt_chart.append( -1 ) # -1 代表CPU閒置
            start_time = dataList[i][2]

        for j in range( i, len( dataList ) ) :
            if ( dataList[j][2] > start_time ) : 
                i = j
                break 

            else : 
                # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 、# 剩餘 CPU Burst
                temp = [dataList[j][0], dataList[j][1], dataList[j][2], dataList[j][3], dataList[j][1]]
                ready_queue.put( temp ) # 將到達的process放入ready_queue
                i = i + 1

        if ( ready_queue.qsize() != 0 ) :
            # 先依照CPU_Brust排序
            ready_list = []
            while ready_queue.qsize() != 0 : ready_list.append( ready_queue.get() )
            ready_list.sort( key = lambda x:x[4] )
            for ii in range( len( ready_list ) ) : ready_queue.put( ready_list[ii] )
            ready_list.clear()
            # 取出queue第一個
            cmp = ready_queue.get()

            if ( i < len( dataList ) ) :
                if ( cmp[4] - 1 > 0 ) :
                    start_time = start_time + 1
                    Gantt_chart.append( cmp[0] )
                    # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 、# 剩餘 CPU Burst
                    temp = [cmp[0], cmp[1], cmp[2], cmp[3], cmp[4] - 1]
                    ready_queue.put( temp ) # 將到達的process放入ready_queue

                else :
                    start_time = start_time + 1
                    Gantt_chart.append( cmp[0] )
                    end_time = start_time
                    temp = [cmp[0], end_time, cmp[2], cmp[1]] # ID # end_time # Arrival Time # CPU Burst
                    time_list.append( temp )

            else :
                start_time = start_time + cmp[4]
                for y in range( cmp[4] ) : Gantt_chart.append( cmp[0] )
                end_time = start_time
                temp = [cmp[0], end_time, cmp[2], cmp[1]] # ID # end_time # Arrival Time # CPU Burst
                time_list.append( temp )
            
    time_list.sort( key = lambda x:x[0] )
    return time_list

def HRRN() :
    global Gantt_chart
    start_time = 0
    end_time = 0
    time_list = []
    ready_list = []
    ready_queue = queue.Queue()
    i = 0 

    while ( i < len( dataList ) ) or ready_queue.qsize() != 0 :
        if i < len( dataList ) and start_time < dataList[i][2] and ready_queue.empty() :
            for jj in range( dataList[i][2] - start_time ) : Gantt_chart.append( -1 ) # -1 代表CPU閒置
            start_time = dataList[i][2]

        for j in range( i, len( dataList ) ) :
            if ( dataList[j][2] > start_time ) : 
                i = j
                break 

            else : 
                # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 
                temp = [dataList[j][0], dataList[j][1], dataList[j][2], dataList[j][3]]
                temp.append( ( start_time - dataList[j][2] + dataList[j][1] ) / dataList[j][1] ) # ( waiting time + cpu burst time ) / cpu burst time
                ready_queue.put( temp ) # 將到達的process放入ready_queue
                i = i + 1

        if ( ready_queue.qsize() != 0 ) :
            # 先依照CPU_Brust排序
            ready_list = []
            temp_ready_list = []
            temp_ready_list1 = []
            while ready_queue.qsize() != 0 : temp_ready_list.append( ready_queue.get() )
            index = len( temp_ready_list ) - 1
            while index >= 0 :
                temp_ready_list1.append( temp_ready_list[index] )
                index = index - 1

            temp_ready_list1.sort( key = lambda x:x[4] )
            index = len( temp_ready_list1 ) - 1
            while index >= 0 :
                ready_list.append( temp_ready_list1[index] )
                index = index - 1

            for ii in range( len( ready_list ) ) : ready_queue.put( ready_list[ii] )
            ready_list.clear()
            # 取出queue第一個
            cmp = ready_queue.get()

            start_time = start_time + cmp[1]
            for y in range( cmp[1] ) : Gantt_chart.append( cmp[0] )
            end_time = start_time
            temp = [cmp[0], end_time, cmp[2], cmp[1]] # ID # end_time # Arrival Time # CPU Burst
            time_list.append( temp )

            temp_list = []
            while ready_queue.qsize() != 0 :
                temp = ready_queue.get() 
                temp[4] = ( start_time - temp[2] + temp[1] ) / temp[1]
                temp_list.append( temp )

            for x in range( len(temp_list) ) : ready_queue.put( temp_list[x] )
            
    time_list.sort( key = lambda x:x[0] )
    return time_list

def PPRR( time ) :
    global Gantt_chart
    start_time = 0
    end_time = 0
    time_list = []
    ready_queue = queue.Queue()
    time_slice = time
    i = 0 

    while ( i < len( dataList ) ) or ready_queue.qsize() != 0 :
        if i < len( dataList ) and start_time < dataList[i][2] and ready_queue.empty() :
            for jj in range( dataList[i][2] - start_time ) : Gantt_chart.append( -1 ) # -1 代表CPU閒置
            start_time = dataList[i][2]

        for j in range( i , len( dataList ) ) :
            if ( dataList[j][2] > start_time ) : 
                i = j
                break 

            else : 
                # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 、 # 剩餘時間
                temp = [dataList[j][0], dataList[j][1], dataList[j][2], dataList[j][3], dataList[j][1]]
                ready_queue.put( temp ) # 將到達的process放入ready_queue中
                i = i + i

        if ( ready_queue.qsize() != 0 ) :
            # 先依照 Priority 排序
            ready_list = []
            while ready_queue.qsize() != 0 : ready_list.append( ready_queue.get() )
            ready_list.sort( key = lambda x:x[3] )
            for ii in range( len( ready_list ) ) : ready_queue.put( ready_list[ii] )
            ready_list.clear()
            # 取出queue第一個
            cmp = ready_queue.get()

            if i < len( dataList ) :
                if ( cmp[4] - time_slice > 0 ) :
                    tt = 1
                    preemptive = False
                    while tt <= time_slice :
                        start_time = start_time + 1 
                        Gantt_chart.append( cmp[0] )

                        for k in range( i , len( dataList ) ) :
                            if ( dataList[k][2] > start_time ) : 
                                i = k 
                                break 

                            elif ( dataList[k][2] <= start_time and dataList[k][3] < cmp[3] ) :
                                # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 、 # 剩餘時間
                                temp1 = [dataList[k][0], dataList[k][1], dataList[k][2], dataList[k][3], dataList[k][1]]
                                ready_queue.put( temp1 ) # 將到達的process放入ready_queue中
                                preemptive = True
                                i = k + 1

                            elif ( dataList[k][2] <= start_time ) :
                                # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 、 # 剩餘時間
                                temp1 = [dataList[k][0], dataList[k][1], dataList[k][2], dataList[k][3], dataList[k][1]]
                                ready_queue.put( temp1 ) # 將到達的process放入ready_queue中
                                i = k + 1

                        if preemptive == True : break
                        tt = tt + 1
                    
                    temp = [cmp[0], cmp[1], cmp[2], cmp[3]] # ID # CPU Burst # Arrival Time # Priority
                    if ( preemptive == True ) : temp.append( cmp[4] - 1 * tt )
                    else : temp.append( cmp[4] - time_slice )
                    ready_queue.put( temp )

                else :
                    tt = 1
                    preemptive = False
                    while tt <= cmp[4] :
                        start_time = start_time + 1 
                        Gantt_chart.append( cmp[0] )

                        for k in range( i , len( dataList ) ) :
                            if ( dataList[k][2] > start_time ) : 
                                i = k 
                                break 

                            elif ( dataList[k][2] <= start_time and dataList[k][3] < cmp[3] ) :
                                # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 、 # 剩餘時間
                                temp1 = [dataList[k][0], dataList[k][1], dataList[k][2], dataList[k][3], dataList[k][1]]
                                ready_queue.put( temp1 ) # 將到達的process放入ready_queue中
                                preemptive = True
                                i = k + 1

                            elif ( dataList[k][2] <= start_time ) :
                                # ID 、 # CPU Burst 、 # Arrival Time 、 # Priority 、 # 剩餘時間
                                temp1 = [dataList[k][0], dataList[k][1], dataList[k][2], dataList[k][3], dataList[k][1]]
                                ready_queue.put( temp1 ) # 將到達的process放入ready_queue中
                                i = k + 1

                        if preemptive == True : break
                        tt = tt + 1
                    
                    if ( ( preemptive == True ) and ( cmp[4] - tt > 0 ) ) : 
                        temp = [cmp[0], cmp[1], cmp[2], cmp[3], cmp[4] - tt] # ID # CPU Burst # Arrival Time # Priority # 剩餘時間
                        ready_queue.put( temp )

                    else :
                        end_time = start_time
                        temp = [cmp[0], end_time, cmp[2], cmp[1]] # ID # end_time # Arrival Time # CPU Burst
                        time_list.append( temp )

            else :
                if ( cmp[4] <= time_slice ) :
                    start_time = start_time + cmp[4]
                    for y in range( cmp[4] ) : Gantt_chart.append( cmp[0] )
                    end_time = start_time
                    temp = [cmp[0], end_time, cmp[2], cmp[1]] # ID # end_time # Arrival Time # CPU Burst
                    time_list.append( temp )

                else :
                    start_time = start_time + time_slice 
                    for y in range( time_slice ) : Gantt_chart.append( cmp[0] )
                    # ID # CPU Burst # Arrival Time # Priority # 剩餘時間
                    temp = [cmp[0], cmp[1], cmp[2], cmp[3], cmp[4] - time_slice] 
                    ready_queue.put( temp )

    time_list.sort( key = lambda x:x[0] )
    return time_list

def turnaround_time( list ) :
    return_list = []

    for i in range( len( list ) ) :
        # ID  # turnaround_time = finish time - arrival time  # CPU Burst
        temp = [list[i][0], list[i][1] - list[i][2], list[i][3]]
        return_list.append( temp )

    return return_list

def waiting_time( list ) :
    return_list = []

    for i in range( len( list ) ) :
        # ID  # waiting_time = turnaround_time - CPU Burst
        temp = [list[i][0], list[i][1] - list[i][2]]
        return_list.append( temp )

    return return_list

def str_split( temp_str ) :
    return_list = []
    temp = ""

    for i in range( len( temp_str ) ) :
        if ( temp_str[i] != '' and temp_str[i] != ' ' and temp_str[i] != '\t' and temp_str[i] != '\n' ) :
            temp = temp + temp_str[i]
        
        else :
            if temp != "" :
                return_list.append( int( temp ) )
                temp = ""
    
    if temp != "" : return_list.append( int( temp ) )
    return return_list

def arrival_time_sort() :
    global dataList
    dataList.sort( key = lambda x:x[0] )
    dataList.sort( key = lambda x:x[2] )

def change_NTOC( list ) :
    change_list = []

    for i in range( len( list ) ) :
        if ( list[i] == -1 ) : change_list.append( '-' )
        elif ( list[i] == 10 ) : change_list.append( 'A' )
        elif ( list[i] == 11 ) : change_list.append( 'B' )
        elif ( list[i] == 12 ) : change_list.append( 'C' )
        elif ( list[i] == 13 ) : change_list.append( 'D' )
        elif ( list[i] == 14 ) : change_list.append( 'E' )
        elif ( list[i] == 15 ) : change_list.append( 'F' )
        elif ( list[i] == 16 ) : change_list.append( 'G' )
        elif ( list[i] == 17 ) : change_list.append( 'H' )
        elif ( list[i] == 18 ) : change_list.append( 'I' )
        elif ( list[i] == 19 ) : change_list.append( 'J' )
        elif ( list[i] == 20 ) : change_list.append( 'K' )
        elif ( list[i] == 21 ) : change_list.append( 'L' )
        elif ( list[i] == 22 ) : change_list.append( 'M' )
        elif ( list[i] == 23 ) : change_list.append( 'N' )
        elif ( list[i] == 24 ) : change_list.append( 'O' )
        elif ( list[i] == 25 ) : change_list.append( 'P' )
        elif ( list[i] == 26 ) : change_list.append( 'Q' )
        elif ( list[i] == 27 ) : change_list.append( 'R' )
        elif ( list[i] == 28 ) : change_list.append( 'S' )
        elif ( list[i] == 29 ) : change_list.append( 'T' )
        elif ( list[i] == 30 ) : change_list.append( 'U' )
        elif ( list[i] == 31 ) : change_list.append( 'V' )
        elif ( list[i] == 32 ) : change_list.append( 'W' )
        elif ( list[i] == 33 ) : change_list.append( 'X' )
        elif ( list[i] == 34 ) : change_list.append( 'Y' )
        elif ( list[i] == 35 ) : change_list.append( 'Z' )
        else : change_list.append( str( list[i] ) )

    return change_list

def write_file( ID_list, turnaround_time_list, waiting_time_list, method ) :
    file = open( "out_" + fileName + ".txt", 'w' ) 
    
    if method == 1 : file.write( "FCFS\n==        FCFS==\n" )
    elif method == 2 : file.write( "RR\n==          RR==\n" )
    elif method == 3 : file.write( "SJF\n==         SJF==\n" )
    elif method == 4 : file.write( "SRTF\n==        SRTF==\n" )
    elif method == 5 : file.write( "HRRN\n==        HRRN==\n" )
    elif method == 6 : file.write( "Priority RR\n==        PPRR==\n" )

    change_ID_list = change_NTOC( ID_list )

    for i in range( len(change_ID_list) ) : file.write( change_ID_list[i] )
    file.write( "\n" )
    for i in range( len( ID_list ) ) : file.write( "=" )
    file.write( "\n\n" )

    if method == 1 : file.write( "Waiting Time\nID\tFCFS\n" )
    elif method == 2 : file.write( "Waiting Time\nID\tRR\n" )
    elif method == 3 : file.write( "Waiting Time\nID\tSJF\n" )
    elif method == 4 : file.write( "Waiting Time\nID\tSRTF\n" )
    elif method == 5 : file.write( "Waiting Time\nID\tHRRN\n" )
    elif method == 6 : file.write( "Waiting Time\nID\tPPRR\n" )

    for i in range( len( ID_list ) ) : file.write( "=" )
    file.write( "\n" )

    for i in range( len( waiting_time_list ) ) : file.write( str(waiting_time_list[i][0]) + "\t" + str(waiting_time_list[i][1]) + "\n" )
    for i in range( len( ID_list ) ) : file.write( "=" )
    file.write( "\n\n" )

    if method == 1 : file.write( "Turnaround Time\nID\tFCFS\n" )
    elif method == 2 : file.write( "Turnaround Time\nID\tRR\n" )
    elif method == 3 : file.write( "Turnaround Time\nID\tSJF\n" )
    elif method == 4 : file.write( "Turnaround Time\nID\tSRTF\n" )
    elif method == 5 : file.write( "Turnaround Time\nID\tHRRN\n" )
    elif method == 6 : file.write( "Turnaround Time\nID\tPPRR\n" )

    for i in range( len( ID_list ) ) : file.write( "=" )
    file.write( "\n" )

    for i in range( len( turnaround_time_list ) ) : file.write( str(turnaround_time_list[i][0]) + "\t" + str(turnaround_time_list[i][1]) + "\n" )
    for i in range( len( ID_list ) ) : file.write( "=" )
    file.write( "\n\n" )
    file.close()

def write_file_all_method( time_slice ) :

    file = open( "out_" + fileName + ".txt", 'w' ) 

    file.write( "all\n" + "==        FCFS==\n" )
    FCFS_time_list = FCFS()
    FCFS_turnaround_time_list = turnaround_time( FCFS_time_list )
    FCFS_waiting_time_list = waiting_time( FCFS_turnaround_time_list )
    change_ID_list = change_NTOC( Gantt_chart )
    for i in range( len(change_ID_list) ) : file.write( change_ID_list[i] )
    Gantt_chart.clear()

    file.write( "\n" + "==          RR==\n" )
    RR_time_list = RR( time_slice )
    RR_turnaround_time_list = turnaround_time( RR_time_list )
    RR_waiting_time_list = waiting_time( RR_turnaround_time_list )
    change_ID_list = change_NTOC( Gantt_chart )
    for i in range( len(change_ID_list) ) : file.write( change_ID_list[i] )
    Gantt_chart.clear()

    file.write( "\n" + "==         SJF==\n" )
    SJF_time_list = SJF()
    SJF_turnaround_time_list = turnaround_time( SJF_time_list )
    SJF_waiting_time_list = waiting_time( SJF_turnaround_time_list )
    change_ID_list = change_NTOC( Gantt_chart )
    for i in range( len(change_ID_list) ) : file.write( change_ID_list[i] )
    Gantt_chart.clear()

    file.write(  "\n" + "==        SRTF==\n" )
    SRTF_time_list = SRTF()
    SRTF_turnaround_time_list = turnaround_time( SRTF_time_list )
    SRTF_waiting_time_list = waiting_time( SRTF_turnaround_time_list )
    change_ID_list = change_NTOC( Gantt_chart )
    for i in range( len(change_ID_list) ) : file.write( change_ID_list[i] )
    Gantt_chart.clear()

    file.write( "\n" + "==        HRRN==\n" )
    HRRN_time_list = HRRN()
    HRRN_turnaround_time_list = turnaround_time( HRRN_time_list )
    HRRN_waiting_time_list = waiting_time( HRRN_turnaround_time_list )
    change_ID_list = change_NTOC( Gantt_chart )
    for i in range( len(change_ID_list) ) : file.write( change_ID_list[i] )
    Gantt_chart.clear()

    file.write( "\n" + "==        PPRR==\n" )
    PPRR_time_list = PPRR( time_slice )
    PPRR_turnaround_time_list = turnaround_time( PPRR_time_list )
    PPRR_waiting_time_list = waiting_time( PPRR_turnaround_time_list )
    change_ID_list = change_NTOC( Gantt_chart )
    for i in range( len(change_ID_list) ) : file.write( change_ID_list[i] )
    Gantt_chart.clear()

    file.write( "\n" + "===========================================================\n\n" )
    file.write( "Waiting Time\n" + "ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n" )
    file.write( "===========================================================\n" )
    for i in range( len( FCFS_waiting_time_list ) ) :
        file.write( str(FCFS_waiting_time_list[i][0]) + "\t" + str(FCFS_waiting_time_list[i][1]) + "\t" + str(RR_waiting_time_list[i][1]) + "\t" + 
                    str(SJF_waiting_time_list[i][1]) + "\t" + str(SRTF_waiting_time_list[i][1]) + "\t" + str(HRRN_waiting_time_list[i][1]) + "\t" + 
                    str(PPRR_waiting_time_list[i][1]) + "\n")
    file.write( "===========================================================\n\n" )
    file.write( "Turnaround Time\n" + "ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n" )
    file.write( "===========================================================\n" )
    for i in range( len( FCFS_turnaround_time_list ) ) :
        file.write( str(FCFS_turnaround_time_list[i][0]) + "\t" + str(FCFS_turnaround_time_list[i][1]) + "\t" + str(RR_turnaround_time_list[i][1]) + "\t" + 
                    str(SJF_turnaround_time_list[i][1]) + "\t" + str(SRTF_turnaround_time_list[i][1]) + "\t" + str(HRRN_turnaround_time_list[i][1]) + "\t" + 
                    str(PPRR_turnaround_time_list[i][1]) + "\n")
    file.write( "===========================================================\n\n" )
    file.close()

def main() :
    run = True
    global dataList  # 存輸入的資料
    global fileName
    global Gantt_chart

    while ( run == True ) :
        Gantt_chart.clear()
        dataList.clear()
        fileName = ""
        print( 'Please enter File Name(eg. input1、input2): ' )
        fileName = input()
        method = 0 
        time_slice = 0
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
                        temp_list = str_split( file_line )
                        method = int( temp_list[0] )
                        time_slice = int( temp_list[1] )

                    elif now_line_num == 2 :
                        file_line = ""
                    
                    else :
                        temp_list = str_split( file_line )
                        if( len( temp_list ) != 0 ) : dataList.append( temp_list )

            file.close()

            if method == 0 :
                print( '程式結束!!!' )
                run = False 

            elif method == 1 :
                arrival_time_sort()
                time_list = FCFS() 
                turnaround_time_list = turnaround_time( time_list )
                waiting_time_list = waiting_time( turnaround_time_list )
                write_file( Gantt_chart, turnaround_time_list, waiting_time_list, method )

            elif method == 2 :
                arrival_time_sort()
                time_list = RR( time_slice )
                turnaround_time_list = turnaround_time( time_list )
                waiting_time_list = waiting_time( turnaround_time_list )
                write_file( Gantt_chart, turnaround_time_list, waiting_time_list, method )
            
            elif method == 3 :
                arrival_time_sort()
                time_list = SJF()
                turnaround_time_list = turnaround_time( time_list )
                waiting_time_list = waiting_time( turnaround_time_list )
                write_file( Gantt_chart, turnaround_time_list, waiting_time_list, method )

            elif method == 4 :
                arrival_time_sort()
                time_list = SRTF()
                turnaround_time_list = turnaround_time( time_list )
                waiting_time_list = waiting_time( turnaround_time_list )
                write_file( Gantt_chart, turnaround_time_list, waiting_time_list, method )

            elif method == 5 :
                arrival_time_sort()
                time_list = HRRN()
                turnaround_time_list = turnaround_time( time_list )
                waiting_time_list = waiting_time( turnaround_time_list )
                write_file( Gantt_chart, turnaround_time_list, waiting_time_list, method )
            
            elif method == 6 :
                arrival_time_sort()
                time_list = PPRR( time_slice )
                turnaround_time_list = turnaround_time( time_list )
                waiting_time_list = waiting_time( turnaround_time_list )
                write_file( Gantt_chart, turnaround_time_list, waiting_time_list, method )
            
            elif method == 7 :
                arrival_time_sort()
                write_file_all_method( time_slice )

            else :
                print( '沒有這個排程法!!!' )
            
        else : print( '開檔失敗!!!' )

if __name__ == '__main__':
    main()