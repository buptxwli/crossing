#include <cstring>
#include <cstdio>
#include <vector>
using namespace std;
int num,row;
const int maxnum=26;
const int maxrow=26;
const int maxtime=maxnum*(2+maxrow);
bool mark[maxrow];
int min_time;
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
P startpoint[maxnum];
P endpoint[maxnum];
P now[maxnum][maxtime];
P shortest_path[maxnum][maxtime];
void debug_path(int t)
{
	for(int i=0;i<num;i++)
	{
	    printf("car[%d]:",i);
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
    char buffer[100][3];
    for(int m=0;m<t+1;m++)
    {
        for (int i=0;i<row;i++)
		{
			buffer[i][0] = '>';
			buffer[i][1] = 'X';
			buffer[i][2] = '>';
        }
        for (int i=0;i<num;i++)
		{
            buffer[shortest_path[i][m].x][shortest_path[i][m].y] = i+'0';
        }
        for (int i=0;i<row;i++)
		{
            for (int j=0;j<3;j++)
                printf("%c", buffer[i][j]);
			printf("\n");
        }
        printf("\n");
    }
    printf("\n");
    return;
}
bool judge_deadlock(int t)
{
    for(int k=0;k<num;k++)
    {
        if(now[k][t-1]!=now[k][t])
            return false;
    }
    return true;
}
bool judge_end(int t)
{
	for(int j=0;j<num;j++)
	{
		if(now[j][t]!=endpoint[j])
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
			if(now[p][t+1]==now[q][t] && now[p][t]==now[q][t+1])
				return true;
		}
	}
	return false;
}
void dfs(int carnum,int time)
{
	if (time>min_time)
        return;
	if(time>2)
	{
		bool flag1=judge_deadlock(time);
        if(flag1)
            return;
	}
	bool flag2=judge_exchange(time);
	if(flag2)
		return;
	if(carnum==num)
	{
		bool flag3=judge_end(time);
		if(flag3 && time<min_time)
		{
		    min_time = time;
			for(int j=0;j<num;j++)
            {
                for(int k=0;k<time;k++)
                    shortest_path[j][k]=now[j][k];
            }
		}
		else
			dfs(0,time+1);
        return;
	}
	int i=carnum;
    if(now[i][time].y==0 && mark[now[i][time].x]==false &&now[i][time]!=endpoint[i])
    {
        now[i][time+1].y=1;
        now[i][time+1].x=now[i][time].x;
        mark[now[i][time+1].x]=true;
        dfs(carnum+1,time);
        now[i][time+1].y=0;
        mark[now[i][time+1].x]=false;
		now[i][time+1].x=now[i][time].x;
    }
    if(now[i][time].y==1 && now[i][time].x==endpoint[i].x &&now[i][time]!=endpoint[i])
    {
        now[i][time+1].y=endpoint[i].y;
        now[i][time+1].x=now[i][time].x;
        mark[now[i][time+1].x]=false;
        dfs(carnum+1,time);
        now[i][time+1].y=1;
        mark[now[i][time+1].x]=true;
		now[i][time+1].x=now[i][time].x;
    }
    if(now[i][time].y==1 && now[i][time].x!=endpoint[i].x && mark[now[i][time].x-1]==false && startpoint[i].x>endpoint[i].y &&now[i][time]!=endpoint[i])  //up
    {
        mark[now[i][time].x]=false;
		now[i][time+1].y=now[i][time].y;
        now[i][time+1].x=now[i][time].x-1;
        mark[now[i][time+1].x]=true;
        dfs(carnum+1,time);
        mark[now[i][time+1].x]=false;
        now[i][time+1].x=now[i][time+1].x+1;
        mark[now[i][time].x]=true;
    }
    if(now[i][time].y==1 && now[i][time].x!=endpoint[i].x && mark[now[i][time].x+1]==false && startpoint[i].x<endpoint[i].y&&now[i][time]!=endpoint[i])
    {
        mark[now[i][time].x]=false;
		now[i][time+1].y=now[i][time].y;
        now[i][time+1].x=now[i][time].x+1;
        mark[now[i][time+1].x]=true;
        dfs(carnum+1,time);
        mark[now[i][time+1].x]=false;
        now[i][time+1].x=now[i][time+1].x-1;
        mark[now[i][time].x]=true;
    }
    if(now[i][time].y==2 && mark[now[i][time].x]==false&&now[i][time]!=endpoint[i])
    {
        now[i][time+1].y=1;
        now[i][time+1].x=now[i][time].x;
        mark[now[i][time].x]=true;
        dfs(carnum+1,time);
        now[i][time+1].y=2;
        mark[now[i][time].x]=false;
    }
    now[i][time+1]=now[i][time];
    dfs(carnum+1, time);
    return;
}
int main()
{
    freopen("input.txt","r",stdin);
    //freopen("output.txt","w",stdout);
    memset(mark,false,sizeof(mark));
	while (scanf("%d %d",&row,&num)!=EOF)
	{
		for(int i=0;i<num;i++)
        {
            scanf("%d%d%d%d",&startpoint[i].x,&startpoint[i].y,&endpoint[i].x,&endpoint[i].y);
            now[i][0]=startpoint[i];
        }
        min_time =num*(2+row);
		dfs(0,0);
		printf("time:%d\n",min_time);
		debug_plot(min_time);
		debug_path(min_time);
	}
	return 0;
}
