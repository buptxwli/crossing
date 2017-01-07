#include <cstring>
#include <cstdio>
#include <vector>
using namespace std;
int num,row;
bool mark[26];
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
P startpoint[26];
P endpoint[26];
P now[26][50];
void debug_plot(int t) {
    char buffer[100][3];
    for (int i=0;i<row;i++) {
        buffer[i][0] = '>';
        buffer[i][1] = 'X';
        buffer[i][2] = '>';
    }
    for(int m=0;m<t+1;m++)
    {
        for (int i=0;i<row;i++) {
        buffer[i][0] = '>';
        buffer[i][1] = 'X';
        buffer[i][2] = '>';
        }
        for (int i=0;i<num;i++) {
            buffer[now[i][m].x][now[i][m].y] = i+'0';
        }
        for (int i=0;i<row;i++) {
            for (int j=0;j<3;j++) {
                printf("%c", buffer[i][j]);
            }
        printf("\n");
        }
        printf("\n");
    }
    printf("\n");
    return;
}
void dfs(int carnum,int time)
{
    bool flag1=true;
	if (time>min_time)
        return;
    if(time>=2)
    {
        for(int k=0;k<num;k++)
        {
            if(now[k][time-1]!=now[k][time])
            {
                flag1=false;
                break;
            }
        }
        if(flag1)
            return;
    }
	bool flag=true;
	if(carnum==num)
	{
		for(int j=0;j<num;j++)
		{
			if(now[j][time]!=endpoint[j])
            {
                flag=false;
                break;
            }
		}
		if(flag && time<min_time)
		{
		    min_time = time;
			printf("time:%d\n",time);
			debug_plot(time);
            return;
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
	}
	return 0;
}
