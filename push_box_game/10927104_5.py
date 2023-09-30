
# 演算法分析機測
# 學號:10927104/10927109
# 姓名:張郁琪/陳宥蓁
# 中原大學資訊工程系

import time
from queue import Queue

r, c = 0, 0  # r: 迷宮的列數 c: 迷宮的行數

maze = [[]]  # 迷宮

d = [ [-1, 1, 0 ,0], [0 ,0, -1,1] ] #四個方位 # 西 # 東 # 南 # 北 

push = ['N', 'S', 'W', 'E'] #人推動箱子的方向
move = ['n', 's', 'w', 'e'] #人移動的方向

tmp = "" # 暫存移動路徑 #

class P:
    def __init__(self, x, y, px, py, ans):
        self.x = x     # 箱子位置
        self.y = y     # 箱子位置
        self.px = px   # 人的位置
        self.py = py   # 人的位置
        self.ans = ans # 記錄路徑

def valid(x, y):
    if ( x >= 0 and x < r and y >= 0 and y < c ) :
        if maze[x][y] != '#' : return True 
        else : return False 
        
    else : return False 


# 找人 #
def bfs2(temp, px, py):
    global tmp
    src = P(temp.x, temp.y, temp.px, temp.py, temp.ans)
    src.ans = ""
    tmp = ""
    q = Queue()
    q.put(src)
    v = [[False] * c for i in range(r)]
    
    while q.qsize():
        now = q.get()
        
        #人抵達了他該在的位置
        if now.px == px and now.py == py:
            tmp = now.ans
            return True
        
        #人往四個方位走
        for i in range(4):
        
            #人想前往的位置
            nxt = P( now.x, now.y, now.px, now.py, now.ans )
            nxt.px = now.px + d[0][i]
            nxt.py = now.py + d[1][i]
            
            #這個位置不能超過地圖的範圍
            if not valid(nxt.px, nxt.py): continue
            
            #人在走的時候不能經過箱子
            if nxt.px == src.x and nxt.py == src.y: continue
            
            #這個位置走過就不能再探訪
            if v[nxt.px][nxt.py]: continue
            
            #探訪過了
            v[nxt.px][nxt.py] = True
            
            #將人移動的位置記錄下來
            nxt.ans = now.ans + move[i]
            
            q.put(nxt)
            
    return False

# 找箱子 #
def bfs1():
    global tmp, maze
    st = P(0, 0, 0, 0, "")
    
    #紀錄人、箱子的位置
    for i in range(r):
        for j in range(c):
            if maze[i][j] == 'B':
                st.x = i
                st.y = j
                maze[i][j] = '.'
                
            elif maze[i][j] == 'S':
                st.px = i
                st.py = j
                maze[i][j] = '.'
       
    q = Queue()
    q.put(st)
    v = [[[False] * 4 for i in range(c)] for j in range(r)]
    
    ans = "Impossible."
    
    # 紀錄箱子與人"動"的步數
    boxCnt = float('inf') 
    manCnt = float('inf')
    
    while q.qsize():
    
        now = q.get()
        
        # 到達終點了 !!! #
        if maze[now.x][now.y] == 'T':
            nowBoxCnt = 0 
            nowManCnt = 0
            
            for i in range(len(now.ans)):
                if now.ans[i] >= 'A' and now.ans[i] <= 'Z': nowBoxCnt += 1
                else: nowManCnt += 1
                
            if nowBoxCnt < boxCnt or (nowBoxCnt == boxCnt and nowManCnt < manCnt):
                ans = now.ans
                boxCnt = nowBoxCnt
                manCnt = nowManCnt
                
            continue
        
        #往四個方位走
        for i in range(4):
        
            #箱子想前往的位置
            nxt = P( now.x, now.y, now.px, now.py, now.ans )
            nxt.x = now.x + d[0][i]
            nxt.y = now.y + d[1][i]
            
            #這個位置不能超過地圖的範圍
            if not valid(nxt.x, nxt.y): continue
            
            #這個位置走過就不能再探訪
            if v[nxt.x][nxt.y][i] : continue

            #人在推箱子以前必須能夠抵達箱子相反方向的位置(ex: 箱子往西推人就必須在箱子的東方)
            px = now.x - d[0][i]
            py = now.y - d[1][i]
            
            #判斷人能不能到箱子旁邊的位置
            if ( not bfs2(now, px, py) ): continue
            
            #探訪過了
            v[nxt.x][nxt.y][i] = True 
            
            #將箱子移動後的位置記錄下來
            nxt.ans = now.ans + tmp 
            nxt.ans = nxt.ans + push[i]
            
            #更新人的位置
            nxt.px = now.x 
            nxt.py = now.y
            
            q.put(nxt)
            
    return ans

def main():
    global r, c, maze, tmp
    maze_num = 0 # 紀錄總共有幾個迷宮
    
    print( "請輸入迷宮的列數與行數: ", end = "" )
    temp = input().split()
    r = int(temp[0]) #列數
    c = int(temp[1]) #行數
    
    while ( r != 0 and c != 0 ):   
        if r <= 20 and c <= 20 :
            maze = [[] for i in range(r)] 
            tmp = ""
            for i in range( r ) :
                print( "請輸入每一列的迷宮: ", end = '' )
                r_str = input()
                for j in range( len(r_str) ) : maze[i].append( r_str[j] )
                
            start_time = time.time() #開始時間
            maze_num = maze_num + 1 
            output = "Maze #" + str( maze_num )
            print( output )
            print( bfs1() )
        
            total_time = time.time() - start_time #總花費時間
            print( "總共花費", total_time, "秒\n" ) 
            
        else : print( "迷宮的列數與行數皆不能超過20！" )
        
        print( "請輸入迷宮的列數與行數: ", end = "" )
        temp = input().split()
        r = int(temp[0]) #列數
        c = int(temp[1]) #行數
    
    print( "程式結束" )
if __name__ == '__main__':
    main()