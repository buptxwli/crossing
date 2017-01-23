/*
6 7
#######
X>X>X>X
X>Y>X>Y
Y<Y<Y<Y
Y<X<Y<X
#######
3
0 0 5 0
0 2 5 4
0 2 5 4
*/
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstdio>
using namespace std;
const int maxnum=100;
const int maxrow=100;
const int maxline=50;
const int maxcross=100;
const int maxtime=maxnum*maxrow*maxline;
bool mark_point[maxrow][maxline];
int min_time;      //only for shortest
int row,line,num;  // whole situation
char in[100][100];   //graph
struct P
{
	int x,y;
	bool operator ==(const P &A) const
	{
	    return (x==A.x && y==A.y);
	}
	bool operator !=(const P &A) const
	{
	    return (x!=A.x || y!=A.y);
	}
};
P startpoint[maxnum];  //whole situation
P endpoint[maxnum];   //whole situation
P shortest_path[maxnum][maxtime];
P now[maxnum][maxtime];
void graph_input()
{
	for(int i=0;i<row;i++)
	{
		getchar();
		for(int j=0;j<line;j++)
		{
			scanf("%c",&in[i][j]);
		}
	}
	return;
}
void debug_path(int t)
{
	for(int i=0;i<num;i++)
	{
	    printf("car[%d]:from<%d,%d>to<%d,%d>\n",i,startpoint[i].x,startpoint[i].y,endpoint[i].x,endpoint[i].y);
		for(int m=0;m<t;m++)
		{
			printf("(%d,%d)->",shortest_path[i][m].x,shortest_path[i][m].y);
		}
		printf("(%d,%d)\n",shortest_path[i][t].x,shortest_path[i][t].y);
	}
	return;
}
void debug_plot(int t)
{
    char tmp[100][100];
    for(int m=0;m<t+1;m++)
    {
        for (int i=0;i<row;i++)
		{
			for(int j=0;j<line;j++)
                tmp[i][j]=in[i][j];
        }
        for (int i=0;i<num;i++)
		{
            tmp[shortest_path[i][m].x][shortest_path[i][m].y] = i+'0';
        }
        for (int i=0;i<row;i++)
		{
            for (int j=0;j<line;j++)
                printf("%c", tmp[i][j]);
			printf("\n");
        }
        printf("\n");
    }
    printf("\n");
    return;
}
void debug_dfs(int t)
{
    char buffer[100][100];
        for (int i=0;i<row;i++)
		{
			for(int j=0;j<line;j++)
                buffer[i][j]=in[i][j];
        }
        for (int i=0;i<num;i++)
		{
            buffer[now[i][t].x][now[i][t].y] = i+'0';
        }
        for (int i=0;i<row;i++)
		{
            for (int j=0;j<line;j++)
                printf("%c", buffer[i][j]);
			printf("\n");
        }
    printf("\n");
    return;
}
bool judge_deadlock(int t)
{
    for(int k=0;k<num;k++)
    {
        if(now[k][t+1]!=now[k][t])
            return false;
    }
    return true;
}
bool judge_end(int t)
{
	for(int j=0;j<num;j++)
	{
		if(now[j][t+1]!=endpoint[j])
            return false;
	}
	return true;
}
bool judge_exchange(int t)
{
	for(int p=0;p<num-1;p++)
	{
		for(int q=p+1;q<num;q++)
		{
			if(now[p][t+1]==now[q][t] && now[p][t]==now[q][t+1] && now[p][t]!=now[p][t+1] && now[q][t]!=now[q][t+1])
				return true;
		}
	}
	return false;
}
bool judge_cycle(int t)
{
    for(int p=0;p<num;p++)
    {
        if(now[p][t]!=now[p][t+1])
        {
            for(int q=0;q<t;q++)
            {
                if(now[p][t+1]==now[p][q])
                    return true;
            }
        }
    }
    return false;
}
void dfs(int carnum,int time)
{
    if(time+1>=min_time)
        return;
    if(carnum==num)
    {
        //debug_dfs(time);
        bool flag1=judge_deadlock(time);
        if(flag1)
            return;
        bool flag2=judge_cycle(time);
        if(flag2)
            return;
        bool flag3=judge_exchange(time);
        if(flag3)
            return;
		bool flag4=judge_end(time);
		if(flag4)
        {
            if (time+1<min_time)
            {
                min_time = time+1;
                for(int j=0;j<num;j++)
                {
                    for(int k=0;k<=min_time;k++)
                        shortest_path[j][k]=now[j][k];
                }
            }
            else
                return;
		}
		else
			dfs(0,time+1);
        return;
    }
    int i=carnum;
    if(now[i][time]!=endpoint[i])
    {
        if(in[now[i][time].x][now[i][time].y]=='#')
        {
            if(startpoint[i].x<endpoint[i].x && mark_point[now[i][time].x+1][now[i][time].y]==false)  //down
            {
                now[i][time+1].x=now[i][time].x+1;
                now[i][time+1].y=now[i][time].y;
                mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                dfs(carnum+1,time);
                mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                now[i][time+1]=now[i][time];
            }
            if(startpoint[i].x>endpoint[i].x && mark_point[now[i][time].x-1][now[i][time].y]==false)  //up
            {
                now[i][time+1].x=now[i][time].x-1;
                now[i][time+1].y=now[i][time].y;
                mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                dfs(carnum+1,time);
                mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                now[i][time+1]=now[i][time];
            }
        }
        if(in[now[i][time].x][now[i][time].y]=='>' )
        {
            if(now[i][time].x-1==endpoint[i].x && now[i][time].y==endpoint[i].y)  //up to end
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x-1;
                now[i][time+1].y=now[i][time].y;
                dfs(carnum+1,time);
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if( mark_point[now[i][time].x][now[i][time].y+1]==false)  //right
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x;
                now[i][time+1].y=now[i][time].y+1;
                mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                dfs(carnum+1,time);
                mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
        }
        if(in[now[i][time].x][now[i][time].y]=='<' )
        {
            if(now[i][time].x+1==endpoint[i].x &&now[i][time].y==endpoint[i].y) //down to end
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x+1;
                now[i][time+1].y=now[i][time].y;
                dfs(carnum+1,time);
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if(mark_point[now[i][time].x][now[i][time].y-1]==false) //left
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x;
                now[i][time+1].y=now[i][time].y-1;
                mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                dfs(carnum+1,time);
                mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
        }
        if(in[now[i][time].x][now[i][time].y]=='X')
        {
            if(in[now[i][time].x+1][now[i][time].y]=='X' && startpoint[i].x<endpoint[i].x && mark_point[now[i][time].x+1][now[i][time].y]==false) //down
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x+1;
                now[i][time+1].y=now[i][time].y;
                mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                dfs(carnum+1,time);
                mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if(in[now[i][time].x-1][now[i][time].y]=='X' && startpoint[i].x>endpoint[i].x && mark_point[now[i][time].x-1][now[i][time].y]==false)//up
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x-1;
                now[i][time+1].y=now[i][time].y;
                mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                dfs(carnum+1,time);
                mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if(now[i][time].x+1==endpoint[i].x &&now[i][time].y==endpoint[i].y)//down to end
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x+1;
                now[i][time+1].y=now[i][time].y;
                dfs(carnum+1,time);
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if(now[i][time].x-1==endpoint[i].x &&now[i][time].y==endpoint[i].y)//up to end
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x-1;
                now[i][time+1].y=now[i][time].y;
                dfs(carnum+1,time);
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if(now[i][time].y+1<line)
            {
                if( in[now[i][time].x][now[i][time].y+1]=='>' &&mark_point[now[i][time].x][now[i][time].y+1]==false) //right
                {
                    mark_point[now[i][time].x][now[i][time].y]=false;
                    now[i][time+1].x=now[i][time].x;
                    now[i][time+1].y=now[i][time].y+1;
                    mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                    dfs(carnum+1,time);
                    mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                    now[i][time+1]=now[i][time];
                    mark_point[now[i][time].x][now[i][time].y]=true;
                }
            }
            if(now[i][time].y-1>=0)
            {
                if( in[now[i][time].x][now[i][time].y-1]=='<' &&mark_point[now[i][time].x][now[i][time].y-1]==false) //left
                {
                    mark_point[now[i][time].x][now[i][time].y]=false;
                    now[i][time+1].x=now[i][time].x;
                    now[i][time+1].y=now[i][time].y-1;
                    mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                    dfs(carnum+1,time);
                    mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                    now[i][time+1]=now[i][time];
                    mark_point[now[i][time].x][now[i][time].y]=true;
                }
            }
        }
        if(in[now[i][time].x][now[i][time].y]=='Y')
        {
            if(in[now[i][time].x+1][now[i][time].y]=='Y' && startpoint[i].x<endpoint[i].x && mark_point[now[i][time].x+1][now[i][time].y]==false) //down
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x+1;
                now[i][time+1].y=now[i][time].y;
                mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                dfs(carnum+1,time);
                mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if(in[now[i][time].x-1][now[i][time].y]=='Y' && startpoint[i].x>endpoint[i].x && mark_point[now[i][time].x-1][now[i][time].y]==false)//up
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x-1;
                now[i][time+1].y=now[i][time].y;
                mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                dfs(carnum+1,time);
                mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if(now[i][time].x+1==endpoint[i].x &&now[i][time].y==endpoint[i].y)//down to end
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x+1;
                now[i][time+1].y=now[i][time].y;
                dfs(carnum+1,time);
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if(now[i][time].x-1==endpoint[i].x &&now[i][time].y==endpoint[i].y)//up to end
            {
                mark_point[now[i][time].x][now[i][time].y]=false;
                now[i][time+1].x=now[i][time].x-1;
                now[i][time+1].y=now[i][time].y;
                dfs(carnum+1,time);
                now[i][time+1]=now[i][time];
                mark_point[now[i][time].x][now[i][time].y]=true;
            }
            if(now[i][time].y+1<line)
            {
                if( in[now[i][time].x][now[i][time].y+1]=='>' &&mark_point[now[i][time].x][now[i][time].y+1]==false) //right
                {
                    mark_point[now[i][time].x][now[i][time].y]=false;
                    now[i][time+1].x=now[i][time].x;
                    now[i][time+1].y=now[i][time].y+1;
                    mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                    dfs(carnum+1,time);
                    mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                    now[i][time+1]=now[i][time];
                    mark_point[now[i][time].x][now[i][time].y]=true;
                }
            }
            if(now[i][time].y-1>=0)
            {
                if( in[now[i][time].x][now[i][time].y-1]=='<' &&mark_point[now[i][time].x][now[i][time].y-1]==false) //left
                {
                    mark_point[now[i][time].x][now[i][time].y]=false;
                    now[i][time+1].x=now[i][time].x;
                    now[i][time+1].y=now[i][time].y-1;
                    mark_point[now[i][time+1].x][now[i][time+1].y]=true;
                    dfs(carnum+1,time);
                    mark_point[now[i][time+1].x][now[i][time+1].y]=false;
                    now[i][time+1]=now[i][time];
                    mark_point[now[i][time].x][now[i][time].y]=true;
                }
            }
        }

    }
    if (now[i][time]==endpoint[i] || in[now[i][time].x][now[i][time].y]!='X')
    {
        now[i][time+1]=now[i][time];
        dfs(carnum+1, time);
    }
    return;
}
int main()
{
    freopen("input.txt","r",stdin);
    while (scanf("%d%d",&row,&line)!=EOF)
    {
        graph_input();
        scanf("%d",&num);
        for(int i=0;i<num;i++)
        {
            scanf("%d%d%d%d",&startpoint[i].x,&startpoint[i].y,&endpoint[i].x,&endpoint[i].y);
            now[i][0]=startpoint[i];
        }
        memset(mark_point,false,sizeof(mark_point));
        min_time=row*line*num;
        dfs(0,0);
		printf("time:%d\n",min_time);
		debug_plot(min_time);
		debug_path(min_time);
    }
    return 0;
}
