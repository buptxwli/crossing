#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int num,row;
const int maxnum=100;
const int maxrow=100;
const int maxtime=maxnum*(2+maxrow);
bool mark[maxrow];
bool mark_go[maxnum];
bool mark_release[maxnum];
int time;
int waiting_time[maxnum];
int at_zone[maxnum];
int at_zone_size;
int token;
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
int step_total[maxnum];
int step_now[maxnum];
void debug_path(int t)
{
	for(int i=0;i<num;i++)
	{
	    printf("car[%d]:from<%d,%d>to<%d,%d>\n",i,startpoint[i].x,startpoint[i].y,endpoint[i].x,endpoint[i].y);
		for(int m=0;m<t;m++)
		{
			printf("(%d,%d)->",now[i][m].x,now[i][m].y);
		}
		printf("(%d,%d)\n",now[i][t].x,now[i][t].y);
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
            buffer[now[i][m].x][now[i][m].y] = i+'0';
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
void calc_shortest()
{
    for(int i=0;i<num;i++)
    {
        step_total[i]=2+abs(endpoint[i].x-startpoint[i].x)+1; /*abs() is needed and add 1 is also needed,(0,0)>(1,2),(0,0),(0,1),(1,1),(1,2),4step*/
        for(int j=0;j<step_total[i];j++)
        {
            if(j==0)
            {
                shortest_path[i][j]=startpoint[i];
                continue;
            }
            else if(j==step_total[i]-1)      /*I have added else, is that correct*/
            {
                shortest_path[i][j]=endpoint[i];
                continue;
            }
            else if(startpoint[i].x<endpoint[i].x)   //down
            {
                shortest_path[i][j].x=startpoint[i].x+j-1;
                shortest_path[i][j].y=1;
                continue;
            }
            else if(startpoint[i].x>endpoint[i].x)  //up
            {
                shortest_path[i][j].x=startpoint[i].x-j+1;
                shortest_path[i][j].y=1;
                continue;
            }
            else if(startpoint[i].x==endpoint[i].x)
            {
                shortest_path[i][j].x=endpoint[i].x;
                shortest_path[i][j].y=1;
                continue;
            }
        }
    }
    return;
}
void calc_at_crossing_zone(int t)
{
    at_zone_size=0;
    for(int i=0;i<num;i++)
    {
        if(now[i][t]==endpoint[i])    /*I think it is not needed */
            continue;
        if(now[i][t].y==0 || now[i][t].y==2)
        {
            at_zone[at_zone_size++]=i;
            mark_go[i]=false;
        }
    }
    return;
}
void calc_waitingtime(int t)
{
    bool flag;
    if(at_zone_size==0)
    {
        memset(waiting_time,0,sizeof(waiting_time));
        return;
    }
    if(at_zone_size==1)
    {
        memset(waiting_time,0,sizeof(waiting_time));
        waiting_time[at_zone[0]]=t;
        return;
    }
    for(int i=0;i<num;i++)
    {
        for(int j=0;j<at_zone_size;j++)
        {
            flag=false;
            if(i==at_zone[j])
            {
                flag=true;
                break;
            }
        }
        if(flag)                      /*I change the logic, is that correct*/
            waiting_time[i]=t;
        else
            waiting_time[i]=0;
    }
    return;
}
bool cmp(int x,int y)
{
    return waiting_time[at_zone[x]]>waiting_time[at_zone[y]];    /* if cmp return true,they don't need to change */
}
void range_by_waiting(int t)
{
    if(at_zone_size==1 || at_zone_size==0)
        return;
    sort(at_zone,at_zone+at_zone_size,cmp);
    return;
}
bool judge_get (int x)
{
    for(int i=1;i<step_total[x]-1;i++)
    {
        if(mark[shortest_path[x][i].x]==true)
            return false;
    }
    return true;
}
int token_release ()
{
    for(int i=0;i<at_zone_size;i++)
    {
        bool flag=judge_get(at_zone[i]);
        if(flag==true)
            return at_zone[i];
    }
    return -1;
}
bool judge_end(int t)
{
	for(int j=0;j<num;j++)
	{
		if(now[j][t]!=endpoint[j])   //this always confuses  me ,I will check it through practice
            return false;
	}
	return true;
}
void end_release(int t)
{
    for(int i=0;i<num;i++)
    {
        if(mark_release[i]==true)
            continue;
        if(now[i][t]==endpoint[i] && startpoint[i]!=endpoint[i])
        {
            for(int j=1;j<step_total[i]-1;j++)
                mark[shortest_path[i][j].x]=false;
            mark_release[i]=true;
        }
    }
    return;
}
void crossing_token(int t)
{
    end_release(t);
    bool flag_end=judge_end(t);
    if(flag_end==false)
    {
        calc_at_crossing_zone(t);
        if(at_zone_size==0)
        {
            for(int i=0;i<num;i++)
            {
                if(now[i][time]==endpoint[i])
                    now[i][time+1]=endpoint[i];
                else
                {
                    now[i][time+1]=shortest_path[i][step_now[i]+1];
                    step_now[i]++;
                }
            }
        }
        else
        {
            calc_waitingtime(t);
            range_by_waiting(t);
            token=token_release();
            if(token!=-1)
            {
                for(int i=1;i<step_total[token]-1;i++)
                    mark[shortest_path[token][i].x]=true;
                mark_go[token]=true;
            }
            for(int i=0;i<num;i++)     /*for this part,if token is -1,means no car can go ,you just need to make other car who are in the crossing to go
                                         (mark_go[i]==true) */
            {
                if(mark_go[i]==true)
                {
                    if(now[i][t]==endpoint[i])
                        now[i][t+1]=endpoint[i];
                    else
                    {
                        now[i][t+1]=shortest_path[i][step_now[i]+1];
                        step_now[i]++;
                    }
                }
                else
                {
                    now[i][t+1]=now[i][t];
                }
            }
        }
        time++;
        crossing_token(time);
    }
    return;
}
int main()
{
    freopen("input.txt","r",stdin);
    memset(mark,false,sizeof(mark));
    memset(mark_go,false,sizeof(mark_go));
    memset(mark_release,false,sizeof(mark_release));
    memset(step_now,0,sizeof(step_now));
    memset(step_total,0,sizeof(step_total));
	while (scanf("%d %d",&row,&num)!=EOF)
	{
		for(int i=0;i<num;i++)
        {
            scanf("%d%d%d%d",&startpoint[i].x,&startpoint[i].y,&endpoint[i].x,&endpoint[i].y);
            now[i][0]=startpoint[i];
        }
        calc_shortest();
		crossing_token(0);
		printf("time:%d\n",time);
        debug_path(time);
        debug_plot(time);
	}
	return 0;
}
