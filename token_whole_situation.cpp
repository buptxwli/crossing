#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstdio>
using namespace std;
const int maxnum=100;
const int maxrow=100;
const int maxline=100;
const int maxcross=100;
const int maxtime=maxnum*maxrow*maxline;
bool graph_mark[1000];
bool mark_point[maxrow][maxline];
bool mark_go[maxnum];
bool mark_release[maxnum][maxcross];
bool mark_cross[maxrow][maxline];   //new
int time;
int row,line,num;  // whole situation
int type;
int crossnum;
int step_total[maxnum];
int step_now[maxnum];
int at_zone[maxnum][maxcross];
int at_zone_size[maxcross];
int waiting_time[maxnum][maxline];
int token[maxcross];
int change_total[maxnum];
char in[100][100];   //graph
int graph_pre[1000];
int graph_dis[1000];
int graph_turn[1000];
int graph_size;
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
P shortest_path[maxnum][maxtime]; //whole situation
P change_point[maxnum][maxrow];  //off crossing zone
P point[1000]; //graph
P now[maxnum][maxtime];
struct E    //graph only
{
    int nextx,nexty,cost;
	E(int _nextx=0,int _nexty=0,int _cost=0):nextx(_nextx),nexty(_nexty),cost(_cost){}
};
vector <E> edge[1000];  //graph
struct node  //graph only
{
	int name,tmpdis;
	int turn;
	node(int _name=0,int _tmpdis=0,int _turn=0):name(_name),tmpdis(_tmpdis),turn(_turn){}
	bool operator <(const node &r)const
	{
        if(tmpdis!=r.tmpdis)
            return tmpdis>r.tmpdis;
        else
            return turn>r.turn;
	}
};
struct C
{
    int x,y;
    int number;
};
int cross_y[maxline];
C cross[maxrow*maxline];  //new
priority_queue<node>que; //graph
void graph_input()
{
    graph_size=0;
	for(int i=0;i<row;i++)
	{
		getchar();
		for(int j=0;j<line;j++)
		{
			scanf("%c",&in[i][j]);
			point[graph_size].x=i;
			point[graph_size].y=j;
			graph_size++;
            if(in[i][j]=='Y')           //1.22
                type=1;
		}
	}
	return;
}
void graph_build()
{
    for(int i=0;i<graph_size;i++)
		edge[i].clear();
    for(int i=0;i<graph_size;i++)
    {
        if(point[i].x==0)      //first line only down
        {
            edge[i].push_back(E(point[i].x+1,point[i].y,1));
        }
        if(point[i].x==row-1)  //last line only up
        {
            edge[i].push_back(E(point[i].x-1,point[i].y,1));
        }
        if (in[point[i].x][point[i].y]=='>')  //turn left
        {
            edge[i].push_back(E(point[i].x,point[i].y+1,1));
        }
        if(in[point[i].x][point[i].y]=='<')   //turn right
        {
            edge[i].push_back(E(point[i].x,point[i].y-1,1));
        }
        if(point[i].x==1)
        {
            edge[i].push_back(E(point[i].x-1,point[i].y,1));
        }
        if(point[i].x==row-2)
        {
            edge[i].push_back(E(point[i].x+1,point[i].y,1));
        }
        if(in[point[i].x][point[i].y]=='X')
        {
            if(point[i].x!=line-1 && in[point[i].x][point[i].y+1]=='>')
            {
                edge[i].push_back(E(point[i].x,point[i].y+1,1));
            }
            if(point[i].x!=0 && in[point[i].x][point[i].y-1]=='<')
            {
                edge[i].push_back(E(point[i].x,point[i].y-1,1));
            }
            if(in[point[i].x+1][point[i].y]=='X')
            {
                edge[i].push_back(E(point[i].x+1,point[i].y,1));
            }
            if (in[point[i].x-1][point[i].y]=='X')
            {
                edge[i].push_back(E(point[i].x-1,point[i].y,1));
            }
        }
        if(in[point[i].x][point[i].y]=='Y')
        {
            if(point[i].x!=line-1 && in[point[i].x][point[i].y+1]=='>')
            {
                edge[i].push_back(E(point[i].x,point[i].y+1,1));
            }
            if(point[i].x!=0 && in[point[i].x][point[i].y-1]=='<')
            {
                edge[i].push_back(E(point[i].x,point[i].y-1,1));
            }
            if(in[point[i].x+1][point[i].y]=='Y')
            {
                edge[i].push_back(E(point[i].x+1,point[i].y,1));
            }
            if (in[point[i].x-1][point[i].y]=='Y')
            {
                edge[i].push_back(E(point[i].x-1,point[i].y,1));
            }
        }
    }
}
void graph_inital()
{
    memset(graph_mark,false,sizeof(graph_mark));
    memset(graph_pre,-1,sizeof(graph_pre));
    for(int i=0;i<graph_size;i++) graph_dis[i]=123456789;
    while(!que.empty()) que.pop();
}
void find_shortest(int i)
{
    graph_dis[startpoint[i].x*line+startpoint[i].y]=0;
    que.push(node(startpoint[i].x*line+startpoint[i].y,0));
    node tmp;
    while(!que.empty())
    {
        tmp=que.top();
        que.pop();
        //printf("pop:(%d,%d),d=%d,t=%d\n", tmp.name/line, tmp.name%line, tmp.tmpdis, tmp.turn);
        int u=tmp.name;
        if(graph_mark[u]) continue;
        graph_mark[u]=true;
        int now_x = u/line;
        int now_y = u%line;
        int now_turn = tmp.turn;
        int last_x = graph_pre[u]/line;
        int last_y = graph_pre[u]%line;
        for(int j=0;j<edge[u].size();j++)
        {
            int next_x=edge[u][j].nextx;
            int next_y=edge[u][j].nexty;
            int next_name = next_x*line+next_y;
        	int next_turn = now_turn;
        	if (now_x!=startpoint[i].x || now_y!=startpoint[i].y)
	        	if (now_x-last_x!=next_x-now_x || now_y-last_y!=next_y-now_y)
	        		next_turn++;
			int c=edge[u][j].cost;

            if((!graph_mark[next_name]&&graph_dis[next_name]>graph_dis[u]+c) ||
			(graph_dis[next_name]==graph_dis[u]+c && graph_turn[next_name]>next_turn))
            {
                graph_dis[next_name]=graph_dis[u]+c;
                graph_pre[next_name]=u;
                graph_turn[next_name] = next_turn;
                node next_node;
                next_node.name = next_x*line+next_y;
            	next_node.tmpdis = graph_dis[next_name];
				next_node.turn = next_turn;
                que.push(next_node);
            }
        }
    }
    if(graph_dis[endpoint[i].x*line+endpoint[i].y]==123456789)
        step_total[i]=-1;
    else
    {
        step_total[i]=graph_dis[endpoint[i].x*line+endpoint[i].y]+1;
        int tmppre=graph_pre[endpoint[i].x*line+endpoint[i].y];
        shortest_path[i][step_total[i]-1]=endpoint[i];
        int j=1;
        while(tmppre>=0)
        {
            int prex=tmppre/line;
            int prey=tmppre%line;
            shortest_path[i][step_total[i]-1-j].x=prex;
            shortest_path[i][step_total[i]-1-j].y=prey;
            tmppre=graph_pre[tmppre];
            j++;
        }
    }
    return;
}
void debug_graph(int x)
{
    if(step_total[x]==-1)
    {
        printf("can't arrive");
        return;
    }
    printf("step=%d:\n",step_total[x]-1);
    for(int i=0;i<step_total[x]-1;i++)
    {
        printf("(%d,%d)->",shortest_path[x][i].x,shortest_path[x][i].y);
    }
    printf("(%d,%d)\n",shortest_path[x][step_total[x]-1].x,shortest_path[x][step_total[x]-1].y);
    return;
}
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
            tmp[now[i][m].x][now[i][m].y] = i+'0';
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
void calc_small_crossnum()  //new
{
    for(int i=1;i<row-1;i++)
    {
        for(int j=0;j<line;j++)
        {
            if(in[i][j]!='X' && in[i][j]!='Y')
                continue;
            if(mark_cross[i][j]==true)
                continue;
            else
            {
                mark_cross[i][j]=true;
                cross[crossnum].x=i;
                cross[crossnum].y=j;
                if(in[i+1][j]==in[i][j])
                {
                    mark_cross[i+1][j]=true;
                    cross[crossnum].number=2;
                }
                else
                    cross[crossnum].number=1;
                crossnum++;
            }
        }
    }
    return;
}
void calc_big_crossnum()
{
    for(int i=0;i<line;i++)
    {
        if(in[1][i]=='X')
        {
            cross_y[crossnum++]=i;
        }
    }
    return;
}
void calc_at_small_crossing_zone(int t,int z)  //new
{
    at_zone_size[z]=0;
    for(int i=0;i<num;i++)
    {
        if(now[i][t]==endpoint[i])
            continue;
        else if(in[now[i][t].x][now[i][t].y]=='X' ||in[now[i][t].x][now[i][t].y]=='Y')
            continue;
        else if(cross[z].number==1)
        {
            if(shortest_path[i][step_now[i]+1].x==cross[z].x &&shortest_path[i][step_now[i]+1].y==cross[z].y)
            {
                at_zone[at_zone_size[z]++][z]=i;
                mark_go[i]=false;
            }
            else
                continue;
        }
        else if(cross[z].number==2)
        {
            if(shortest_path[i][step_now[i]+1].y==cross[z].y && (shortest_path[i][step_now[i]+1].x==cross[z].x ||shortest_path[i][step_now[i]+1].x==cross[z].x+1))
            {
                at_zone[at_zone_size[z]++][z]=i;
                mark_go[i]=false;
            }
            else
                continue;
        }
    }
    return;
}
void calc_at_big_crossing_zone(int t,int z)
{
    at_zone_size[z]=0;
    for(int i=0;i<num;i++)
    {
        if(now[i][t]==endpoint[i])
            continue;
        else if(now[i][t].y==cross_y[z] && (now[i][t].x==0 || now[i][t].x==row-1))
        {
            at_zone[at_zone_size[z]++][z]=i;
            mark_go[i]=false;
        }
        else if((in[now[i][t].x][now[i][t].y]=='>' ||in[now[i][t].x][now[i][t].y]=='<') && shortest_path[i][step_now[i]+1].y==cross_y[z])
        {
            at_zone[at_zone_size[z]++][z]=i;
            mark_go[i]=false;
        }
    }
    return;
}
void calc_waitingtime(int t,int z)
{
    bool flag;
    if(at_zone_size[z]==0)
    {
        for(int i=0;i<num;i++)
            waiting_time[i][z]=0;
        return;
    }
    else if(at_zone_size[z]==1)
    {
        for(int i=0;i<num;i++)
        {
            if(i==at_zone[0][z])
                waiting_time[i][z]++;
            else
                waiting_time[i][z]=0;
        }
        return;
    }
    for(int i=0;i<num;i++)
    {
        flag=false;
        for(int j=0;j<at_zone_size[z];j++)
        {
            if(i==at_zone[j][z])
            {
                flag=true;
                break;
            }
        }
        if(flag)
            waiting_time[i][z]++;
        else
            waiting_time[i][z]=0;
    }
    return;
}
void range_by_waiting(int t,int z)
{
    if(at_zone_size[z]==1 || at_zone_size[z]==0)
        return;
    for(int i=0;i<at_zone_size[z];i++)
    {
        for(int j=0;j<at_zone_size[z]-1-i;j++)
        {
            if(waiting_time[at_zone[j][z]][z]<waiting_time[at_zone[j+1][z]][z])
            {
                int tmp=at_zone[j][z];
                at_zone[j][z]=at_zone[j+1][z];
                at_zone[j+1][z]=tmp;
            }
        }
    }
    return;
}
void calc_small_change_point()  //new
{
    for(int i=0;i<num;i++)
    {
        for(int j=1;j<step_total[i];j++)
        {
            if((in[shortest_path[i][j].x][shortest_path[i][j].y]=='>'||in[shortest_path[i][j].x][shortest_path[i][j].y]=='<' ) &&(in[shortest_path[i][j-1].x][shortest_path[i][j-1].y]=='X' ||in[shortest_path[i][j-1].x][shortest_path[i][j-1].y]=='Y'))
            {
                change_point[i][change_total[i]++]=shortest_path[i][j];
            }
        }
    }
    return;
}
void calc_big_change_point()
{
    for(int i=0;i<num;i++)
    {
        for(int j=1;j<step_total[i];j++)
        {
            if((in[shortest_path[i][j].x][shortest_path[i][j].y]=='>'||in[shortest_path[i][j].x][shortest_path[i][j].y]=='<' ) &&in[shortest_path[i][j-1].x][shortest_path[i][j-1].y]=='X')
            {
                change_point[i][change_total[i]++]=shortest_path[i][j];
            }
        }
    }
    return;
}
bool judge_small_get (int x)   //new
{
   for(int i=step_now[x]+1;i<step_total[x]-1;i++)
   {
       if(in[shortest_path[x][i].x][shortest_path[x][i].y]=='X' || in[shortest_path[x][i].x][shortest_path[x][i].y]=='Y')
       {
           if(mark_point[shortest_path[x][i].x][shortest_path[x][i].y]==true)
               return false;
       }
       else
       {
           for(int j=0;j<change_total[x];j++)
           {
               if(shortest_path[x][i]==change_point[x][j])
               {
                   if(mark_point[change_point[x][j].x][change_point[x][j].y]==true)
                       return false;
                   else
                       return true;
               }
           }
       }
   }
    return true;
}
bool judge_big_get (int x)
{
    if(step_now[x]==0)
    {
        if(change_total[x]==0)
        {
            for(int i=1;i<step_total[x]-1;i++)
            {
                if(mark_point[shortest_path[x][i].x][shortest_path[x][i].y]==true)
                    return false;
            }
            return true;
        }
        else
        {
            for(int i=step_now[x]+1;i<step_total[x]-1;i++)
            {
                if(shortest_path[x][i]!=change_point[x][0])
                {
                    if(mark_point[shortest_path[x][i].x][shortest_path[x][i].y]==true)
                        return false;
                }
                else
                    break;
            }
            if(mark_point[change_point[x][0].x][change_point[x][0].y]==true)
                return false;
            return true;
        }
    }
    else
    {
       for(int i=step_now[x]+1;i<step_total[x]-1;i++)
       {
           if(in[shortest_path[x][i].x][shortest_path[x][i].y]=='X')
           {
               if(mark_point[shortest_path[x][i].x][shortest_path[x][i].y]==true)
                   return false;
           }
           else
           {
               for(int j=0;j<change_total[x];j++)
               {
                   if(shortest_path[x][i]==change_point[x][j])
                   {
                       if(mark_point[change_point[x][j].x][change_point[x][j].y]==true)
                           return false;
                       else
                           return true;
                   }
               }
           }
       }
    }
    return true;
}
int token_release (int k)   //1.22
{
    bool flag;
    for(int i=0;i<at_zone_size[k];i++)
    {
        if(type==1)
            flag=judge_small_get(at_zone[i][k]);
        else
            flag=judge_big_get(at_zone[i][k]);
        if(flag==true)
            return at_zone[i][k];
    }
    return -1;
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
void end_small_release(int t)   //off_crossing_zone  new
{
    for(int i=0;i<num;i++)
    {
        for(int k=0;k<crossnum;k++)
        {
            if(mark_release[i][k]==true)
                continue;
            else if(now[i][t-1]==now[i][t])
                continue;
            else if((in[now[i][t-1].x][now[i][t-1].y]=='X' && in[now[i][t].x][now[i][t].y]!='X')||(in[now[i][t-1].x][now[i][t-1].y]=='Y' || in[now[i][t].x][now[i][t].y]!='Y'))
            {
                if(cross[k].number==1)
                {
                    if(now[i][t-1].x!=cross[k].x || now[i][t-1].y!=cross[k].y)
                        continue;
                    else if(now[i][t].x!=cross[k].x || now[i][t].y!=cross[k].y)
                    {
                        for(int p=step_now[i]-1;p>0;p--)
                        {
                            if(shortest_path[i][p].y==now[i][t-1].y)
                                mark_point[shortest_path[i][p].x][shortest_path[i][p].y]=false;
                        }
                        mark_release[i][k]=true;
                    }
                }
                else if(cross[k].number==2)
                {
                    if(now[i][t-1].y!=cross[k].y)
                        continue;
                    else if (now[i][t-1].x!=cross[k].x && now[i][t-1].x!=cross[k].x+1)
                        continue;
                    else if (now[i][t].y!=cross[k].y || (now[i][t].x!=cross[k].x && now[i][t].x!=cross[k].x+1))
                    {
                        for(int p=step_now[i]-1;p>0;p--)
                        {
                            if(shortest_path[i][p].y==now[i][t-1].y)
                                mark_point[shortest_path[i][p].x][shortest_path[i][p].y]=false;
                        }
                        mark_release[i][k]=true;
                    }
                }
            }
        }
    }
    return;
}
void end_big_release(int t)   //off_crossing_zone
{
    for(int i=0;i<num;i++)
    {
        for(int k=0;k<crossnum;k++)
        {
            if(mark_release[i][k]==true)
                continue;
            else if(in[now[i][t-1].x][now[i][t-1].y]=='X' &&in[now[i][t].x][now[i][t].y]!='X' && now[i][t-1].y==cross_y[k])
            {
                for(int p=step_now[i]-1;p>0;p--)
                {
                    if(shortest_path[i][p].y==now[i][t-1].y)
                        mark_point[shortest_path[i][p].x][shortest_path[i][p].y]=false;
                }
                mark_release[i][k]=true;
            }

        }
    }
    return;
}
void go_release(int t) //new
{
    for(int i=0;i<num;i++)
    {
        if(now[i][t]==now[i][t-1])
            continue;
        else if(in[now[i][t-1].x][now[i][t-1].y]=='X')
            continue;
        else if(type==1 && in[now[i][t-1].x][now[i][t-1].y]=='Y')
            continue;
        else if(in[now[i][t-1].x][now[i][t-1].y]=='#')
            continue;
        else
        {
            mark_point[now[i][t-1].x][now[i][t-1].y]=false;
        }
    }
    return;
}
void judge_small_go(int t)  //new
{
    memset(mark_go,false,sizeof(mark_go));
    for(int i=0;i<num;i++)
    {
        if((in[now[i][t].x][now[i][t].y]=='>' || in[now[i][t].x][now[i][t].y]=='<')&&(in[shortest_path[i][step_now[i]+1].x][shortest_path[i][step_now[i]+1].y]=='X' ||in[shortest_path[i][step_now[i]+1].x][shortest_path[i][step_now[i]+1].y]=='Y'))
            continue;
        if(in[now[i][t].x][now[i][t].y]=='X' || in[now[i][t].x][now[i][t].y]=='Y')
            mark_go[i]=true;
        else if((in[now[i][t].x][now[i][t].y]=='>' || in[now[i][t].x][now[i][t].y]=='<')&&mark_point[shortest_path[i][step_now[i]+1].x][shortest_path[i][step_now[i]+1].y]==false)
            mark_go[i]=true;
    }
}
void judge_big_go(int t)  //new
{
    memset(mark_go,false,sizeof(mark_go));
    for(int i=0;i<num;i++)
    {
        if((in[now[i][t].x][now[i][t].y]=='>' || in[now[i][t].x][now[i][t].y]=='<')&&in[shortest_path[i][step_now[i]+1].x][shortest_path[i][step_now[i]+1].y]=='X')
            continue;
        if(in[now[i][t].x][now[i][t].y]=='X')
            mark_go[i]=true;
        else if((in[now[i][t].x][now[i][t].y]=='>' || in[now[i][t].x][now[i][t].y]=='<')&&mark_point[shortest_path[i][step_now[i]+1].x][shortest_path[i][step_now[i]+1].y]==false)
            mark_go[i]=true;
    }
}
void crossing_token(int t)  //new
{
    if(t!=0)
    {
        if(type==1)
            end_small_release(t);
        else
            end_big_release(t);
        go_release(t);
    }
    if(type==1)
        judge_small_go(t);
    else
        judge_big_go(t);
    bool flag_end=judge_end(t);
    if(flag_end==false)
    {
        for(int k=0;k<crossnum;k++)
        {
            if(type==1)
                calc_at_small_crossing_zone(t,k);
            else
                calc_at_big_crossing_zone(t,k);
            if(at_zone_size[k]==0)
            {
                continue;
            }
            else
            {
                calc_waitingtime(t,k);
                range_by_waiting(t,k);
                token[k]=token_release(k);
                if(token[k]!=-1 && type==1)
                {
                    for(int i=step_now[token[k]]+1;i<step_total[token[k]]-1;i++)
                    {
                        if(shortest_path[token[k]][i].y==cross[k].y)     //here
                            mark_point[shortest_path[token[k]][i].x][shortest_path[token[k]][i].y]=true;
                        else if(shortest_path[token[k]][i].y!=cross[k].y && shortest_path[token[k]][i-1].y==cross[k].y )
                        {
                            mark_point[shortest_path[token[k]][i].x][shortest_path[token[k]][i].y]=true;
                            break;
                        }
                    }
                    mark_go[token[k]]=true;
                }
                else if(token[k]!=-1 && type==0)
                {
                    for(int i=step_now[token[k]]+1;i<step_total[token[k]]-1;i++)
                    {
                        if(shortest_path[token[k]][i].y==cross_y[k])
                            mark_point[shortest_path[token[k]][i].x][shortest_path[token[k]][i].y]=true;
                        else if(shortest_path[token[k]][i].y!=cross_y[k] && shortest_path[token[k]][i-1].y==cross_y[k] )
                        {
                            mark_point[shortest_path[token[k]][i].x][shortest_path[token[k]][i].y]=true;
                            break;
                        }
                    }
                    mark_go[token[k]]=true;
                }
            }
        }
        for(int i=0;i<num;i++)
        {
            if(mark_go[i]==true)
            {
                if(now[i][t]==endpoint[i])
                    now[i][t+1]=endpoint[i];
                else
                {
                    now[i][t+1]=shortest_path[i][step_now[i]+1];
                    step_now[i]++;
                    mark_point[now[i][t+1].x][now[i][t+1].y]=true;
                }
            }
            else
            {
                now[i][t+1]=now[i][t];
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
    while (scanf("%d%d",&row,&line)!=EOF)
    {
        graph_input();
        graph_build();
        scanf("%d",&num);
        for(int i=0;i<num;i++)
        {
            graph_inital();
            scanf("%d%d%d%d",&startpoint[i].x,&startpoint[i].y,&endpoint[i].x,&endpoint[i].y);
            now[i][0]=startpoint[i];
            printf("car[%d]\n",i);
            find_shortest(i);
            //debug_graph(i);
            if(step_total[i]==-1)
                return 0;
        }
        memset(mark_cross,false,sizeof(mark_cross));
        memset(mark_point,false,sizeof(mark_point));
        memset(mark_go,false,sizeof(mark_go));
        memset(mark_release,false,sizeof(mark_release));
        memset(step_now,0,sizeof(step_now));
        time=0;
        if(type==1)
        {
            calc_small_crossnum();
            calc_small_change_point();
        }
        else
        {
            calc_big_crossnum();
            calc_big_change_point();
        }
		crossing_token(0);
		printf("time:%d\n",time);
        debug_path(time);
        debug_plot(time);
    }
    return 0;
}
