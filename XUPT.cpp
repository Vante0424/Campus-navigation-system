#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <windows.h>
#define MAXVEX 50
#define INFINITY 32768
#define MAXL 20
#define MAXC 66

int Visited[INFINITY];
int Stack[INFINITY];
int count;

struct User
{
	char ID[20];		  	//帐号
	char Passwd[20];  		//密码
	struct User *next;
};

typedef struct 				//保存地点信息的结构体
{
	int No;    				//校园地点序号
	char Name[20];    		//校园地点名
	char Info[200];  	//地点描述
} StateData;

typedef struct 				//邻接矩阵
{
	int arcs[MAXVEX][MAXVEX];  		 //边集
	StateData vex[MAXVEX];      	  //顶点集
	int Vexnum;              //顶点数目
	int Arcnum;              //边数目
} AdjMatrix;

typedef struct Node
{
	int Date;  				//队列元素的值，存储地点序号
	struct Node *next;
} QueueNode;

typedef struct
{
	QueueNode *front; 			//头指针
	QueueNode *fear; 			//尾指针
} LinkQueue;


//函数声明 
int IsEmpty(LinkQueue *Q);
int InitQueue(LinkQueue *Q);
int EnQueue(LinkQueue *Q, int x);
int DeQueue(LinkQueue *Q, int *x);
void Print(AdjMatrix *G);
int CreateG(AdjMatrix *G);
void DfsSearch(AdjMatrix *G);
void Info(AdjMatrix *G);
void DFS(AdjMatrix *G, int m, int i, int end);
void MinTree(AdjMatrix *G);
int NextAdj(AdjMatrix *G, int w, int v);
void BFS(AdjMatrix *G, int start, int end);
void BfsSearch(AdjMatrix *G);
void Dijkstra(AdjMatrix *G, int start, int end, int dist[], int path[][MAXVEX]);
void Short(AdjMatrix *G);
void Prim(AdjMatrix *G, int start);
void AddState(AdjMatrix *G);
struct User *Read();
void AddRoad(AdjMatrix *G);
void RoadDel(AdjMatrix *G);
int User();
int Admin();
void AdminLog();
void StateDel(AdjMatrix *G);


int IsEmpty(LinkQueue *Q)
{
	if(Q->front == Q->fear)
		return 1;
	else
		return 0;
}


int InitQueue(LinkQueue *Q)
{
	Q->front = (QueueNode *)malloc(sizeof(QueueNode));
	if(Q->front != NULL)
	{
		Q->fear = Q->front;
		Q->front->next = NULL;
		return 1;
	}
	else
	{
		return 0;
	}
}


int EnQueue(LinkQueue *Q, int x)
{
	QueueNode *Newnode;
	Newnode = (QueueNode *)malloc(sizeof(QueueNode));
	if(Newnode != NULL)
	{
		Newnode->Date = x;
		Newnode->next = NULL;
		Q->fear->next = Newnode;
		Q->fear = Newnode;
		return 1;
	}
	else
	{
		return 0;
	}
}


int DeQueue(LinkQueue *Q, int *x)
{
	QueueNode *p;
	if(Q->front == Q->fear)
		return 1;
	p = Q->front->next;
	Q->front->next = p->next;
	if(Q->fear == p)
		Q->fear = Q->front;
	*x = p->Date;
	free(p);
	return 1;
}


void Print( AdjMatrix *G )
{
	int i,count = 0;
	printf("\t\t\t\t|| * * * * * * * * * * * * * * * * * * * * * * * ||\n");
	printf("\t\t\t\t||                                               ||\n");
	printf("\t\t\t\t||         ***欢迎使用西邮智慧校园导航***        ||\n");
	printf("\t\t\t\t||                                               ||\n");
	printf("\t\t\t\t|| * * * * * * * * * * * * * * * * * * * * * * * ||\n\n");
	for(i = 1; i <= G->Vexnum; i++)
	{
		printf("\t\t\t\t%d.%-8s",i,G->vex[i].Name);
		count++;
		if(count == 2)			//一行两个 
		{
			printf("\n");
			count = 0;
		}
	}
	printf("\n\n");
}


//邻接矩阵,创建无向图
int CreateG( AdjMatrix *G )
{
	int i,j,weight,m,n;
	
	//从"路线信息.txt"文件中读取校园图的景点数目和路线数目
	FILE *fp1;
	fp1=fopen("Road.txt","r");
	fscanf(fp1, "%d %d", &G->Vexnum, &G->Arcnum);

	//初始化邻接矩阵
	for(i = 1; i <= G->Vexnum; i++)
		for(j = 1; j<= G->Vexnum; j++)
		{
			G->arcs[i][j] = INFINITY;
		}

	//读取"路线信息.txt"文件中两点序号及距离，并赋值给邻接矩阵
	while(fscanf(fp1,"%d %d %d",&i,&j,&weight) != EOF)
	{
		G->arcs[i][j] = weight;			//主对角线对称 
		G->arcs[j][i] = weight;
	}
	fclose(fp1);

	//从"地点介绍.txt"文件中读取校园图中的景点名及描述
	FILE *fp2;
	fp2 = fopen("State.txt","rt");

	for(i = 1; i <= G->Vexnum; i++)
	{
		G->vex[i].No = i;
		fscanf(fp2, "%s %s", G->vex[i].Name,G->vex[i].Info);
	}
	fclose(fp2);
	return 1;
}


//查询所有简单路线
void DfsSearch( AdjMatrix *G )
{
	int start,end;
	system("cls");			//清屏 
	printf("\n\n");
	Print(G);
	do
	{
		printf("\t\t\t\t\t请输入起点序号:");
		scanf("%d",&start);
	}while( start<=0 || start>G->Vexnum );

	do
	{
		printf("\t\t\t\t\t请输入终点序号:");
		scanf("%d",&end);
	}while( start<=0 || start>G->Vexnum );

	printf("\n\t\t\t\t\t两地点间的所有路线情况为:\n\n");

	int m = 2;

	memset(Stack,0,INFINITY);
	memset(Visited,0,MAXVEX);

	Stack[1] = start;
	Visited[start] = 1;

	DFS(G, m, start, end);

	printf("\n\t\t\t\t\t按任意键返回...");

	memset(Stack,0,INFINITY);
	memset(Visited,0,MAXVEX);

	count = 0;
	getch();
}


//查看地图介绍
void Info( AdjMatrix *G )
{
	int i,j,choice;
	system("cls");
	printf("\n\n");
	Print(G);

	do
	{
		printf("\t\t\t\t\t请输入要查询的地点序号(按0返回):");
		scanf("%d",&choice);
		if(choice == 0)
			break;
		if(choice < 0 || choice > G->Vexnum)
			continue;
		printf("\n\t\t\t\t\t--->%s:%s\n",G->vex[choice].Name,G->vex[choice].Info);
		printf("\n\t\t\t\t\t该地点的路线情况为:\n");
		for(i = 1; i <= G->Vexnum; i++)
		{
			if(i == choice) 		//遍历矩阵中该景点序号这一行的各点元素
			{
				for(j = 1; j <= G->Vexnum; j++)
				{
					if(G->arcs[i][j] != INFINITY)			 //有弧输出
					{
						printf("\t\t\t\t\t%-8s --- %8s: %3d m\n", G->vex[i].Name,G->vex[j].Name,G->arcs[i][j]);
					}
				}
			}
		}
		printf("\n\n");
	}while(1);

	printf("\n\t\t\t\t\t按任意键返回...");
	getch();
}


void DFS(AdjMatrix *G, int m, int i, int end)
{
	int j,k;
	for(j = 1; j <= G->Vexnum; j++)
	{
		if(G->arcs[i][j] != INFINITY && Visited[j] == 0)
		{
			Visited[j] = 1;
			
			if(j == end)
			{
				count++;
				printf("*%d.",count);
				
				for(k = 1; k < m; k++)
				{
					printf("\t\t%s->", G->vex[Stack[k]].Name);
				}
				printf("%s\n", G->vex[end].Name);
				Visited[j] = 0;
			}
			else
			{
				Stack[m] = j;
				m++;
				DFS(G, m, j, end);
				m--;
				Visited[j] = 0;
			}
		}
	}
}


//查询最小生成树，最佳布网方案 
void MinTree(AdjMatrix *G)
{
	int i,count=0;
	int start;
	system("cls");
	printf("\n\n");
	Print(G);

	do
	{
		printf("\t\t\t\t\t请输入起点序号:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);
	
	for(i = 1; i <= G->Vexnum; i++)
	{
		if(G->arcs[start][i] != INFINITY)
			count++;
	}

	if(count == 0)
	{
		printf("\n\t\t\t\t\t按任意键返回...");
		getch();
	}
	else
	{
		printf("\n");
		Prim(G, start);
	}
}


//寻找下一个临接点
int NextAdj(AdjMatrix *G, int w, int v)
{
	int i, j;
	for(i = w+1; i < G->Vexnum; i++)
	{
		if(G->arcs[v][i] != INFINITY)
		{
			j = i;
			return j;
		}
	}
	return -1;
}

void BFS(AdjMatrix *G, int start, int end)
{
	int vis[INFINITY];
	int i, num;
	int w, v;

	LinkQueue *Q;

	Q=(LinkQueue*)malloc(sizeof(LinkQueue));

	if(start == end)
		return;

	memset(vis, 0, INFINITY);
	vis[start] = 1;

	InitQueue(Q);
	EnQueue(Q, start);
	while(Q->front != Q->fear)
	{
		DeQueue(Q, &v);
		num = v;
		
		for(i = 1; i <= G->Vexnum; i++)
		{
			if(G->arcs[num][i] != INFINITY)
			{
				w = i;  			//求出当前节点的第一个邻接点（求出序号）
				while(w != -1)
				{
					if(vis[w] == 0)
					{
						if(w == end)
						{
							BFS(G, start, num);
							printf("%s->",G->vex[num].Name);
							return;
						}
						vis[w] = 1;
						EnQueue(Q, w);
						w = NextAdj(G, w, v);	//w是求的得第一个邻接点，v是相对w下一个邻接点(求出下一个邻接点的序号)
					}
					break;
				}
			}
		}
	}
}



//最佳中转方案
void BfsSearch( AdjMatrix *G )
{
	int start, end;
	system("cls");
	printf("\n\n");
	Print(G);
	
	do
	{
		printf("\t\t\t\t\t请输入起点序号:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);

	do
	{
		printf("\t\t\t\t\t请输入终点序号:");
		scanf("%d",&end);
	}while(start<=0 || start>G->Vexnum);

	if(G->arcs[start][end] != INFINITY)
		printf("\n\t\t\t\t\t从%s到%s不需要中转\n",G->vex[start].Name,G->vex[end].Name);
	
	if(G->arcs[start][end] == INFINITY)
	{
		printf("\n\t\t\t\t\t从%s到%s中转次数数最少的路径为:\n\n",G->vex[start].Name,G->vex[end].Name);
		printf("\t\t\t");
		BFS(G, start, end);
		printf("%s\n",G->vex[end].Name);
	}
	
	printf("\n\t\t\t\t\t按任意键返回...");
	getch();
}


//Dijkstra寻找最短路径 
void Dijkstra(AdjMatrix *G, int start, int end, int dist[], int path[][MAXVEX])
{
	int mindist, i, j, k, t = 1;

	for(i = 1; i <= G->Vexnum; i++)
	{
		dist[i] = G->arcs[start][i];  		//对dist数组初始化
		if(G->arcs[start][i] != INFINITY)
			path[i][1] = start;  			//如果该弧存在，则path[i][1]为源点
	}
	
	path[start][0] = 1; 					//start加入到S中

	for(i = 2; i <= G->Vexnum; i++) 		//寻找各条最短路径
	{
		mindist = INFINITY;
		for(j = 1; j <= G->Vexnum; j++)
			if(!path[j][0] && dist[j] < mindist)
			{
				k = j;
				mindist = dist[j];
			}
		
		if(mindist == INFINITY)
			return ;
		path[k][0] = 1;   					//找到最短路径，将该点加入到S集合中

		for(j = 1; j <= G->Vexnum; j++) 	//修改路径
		{
			if(!path[j][0] && G->arcs[k][j] < INFINITY && dist[k]+G->arcs[k][j] < dist[j])
			{
				dist[j] = dist[k] + G->arcs[k][j];
				t = 1;
				
				while(path[k][t] != 0)
				{
					path[j][t] = path[k][t];
					t++;
				}

				path[j][t] = k;
				path[j][t+1] = 0;
			}
		}
	}
	
	for(i = 1; i <= G->Vexnum; i++)
		if(i == end)
			break;
	printf("\n\t\t\t\t    %s--->%s的最短路线为: 从%s",G->vex[start].Name,G->vex[end].Name,G->vex[start].Name);
	for(j = 2; path[i][j] != 0; j++)
	{
		printf("**%s",G->vex[path[i][j]].Name);
	}
	
	printf("**%s, 距离为%d m\n",G->vex[end].Name,dist[i]);
	printf("\n\t\t\t\t\t按任意键返回...");
	
	getch();
}


//运用Dijkstra算法求得从起点地标到各终点地标的最短路线
void Short(AdjMatrix *G)
{
	int start, end;

	int dist[MAXVEX];
	int path[MAXVEX][MAXVEX] = {{0}};
	system("cls");
	printf("\n\n");

	Print(G);
	
	do
	{
		printf("\t\t\t\t\t请输入起点序号:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);
	
	do
	{
		printf("\t\t\t\t\t请输入终点序号:");
		scanf("%d",&end);
	}while(end<=0 || end>G->Vexnum);
	
	Dijkstra(G,start,end,dist,path);
}



//Prim算法 
void Prim(AdjMatrix *G, int start)
{
	struct
	{
		int adjvex;
		int lowcost;
	}closedge[MAXVEX];
	
	int i, e, k, m, min;
	closedge[start].lowcost = 0;

	for(i = 1; i <= G->Vexnum; i++)			//对除了出发点以外deep所有顶点初始化对应的closedge数组
	{
		if(i != start)
		{
			closedge[i].adjvex = start;
			closedge[i].lowcost = G->arcs[start][i];
		}
	}

	for(e = 1; e <= G->Vexnum-1; e++)  		//控制选中的n-1条符合条件的边
	{
		//选择权值最小的边
		min = INFINITY;
		for(k = 1; k <= G->Vexnum; k++)
		{
			if(closedge[k].lowcost != 0 && closedge[k].lowcost < min)
			{
				m = k;
				min = closedge[k].lowcost;
			}
		}
		
		printf("\t\t\t\t\t从%s---%s:%d m\n", G->vex[closedge[m].adjvex].Name,G->vex[m].Name,closedge[m].lowcost);
		closedge[m].lowcost = 0;
		
		for(i = 1; i <= G->Vexnum; i++)
		{
			if(i != m && G->arcs[m][i] < closedge[i].lowcost)
			{
				closedge[i].lowcost = G->arcs[m][i];
				closedge[i].adjvex = m;
			}
		}
	}
	
	printf("\n\t\t\t\t\t按任意键返回...");
	getch();
}


//增加新地点
void AddState(AdjMatrix *G)
{
	int i,j;
	int x,y;
	system("cls");
	printf("\n\n");
	Print(G);
	
	printf("\t\t\t\t\t请输入增加的地点名称:");
	scanf("%s",G->vex[++G->Vexnum].Name);
	
	G->vex[G->Vexnum].No = G->Vexnum;
	printf("\t\t\t\t\t请输入该地点的简介:");
	scanf("%s",G->vex[G->Vexnum].Info);

	for(i = 1; i <= G->Vexnum; i++)
	{
		G->arcs[G->Vexnum][i] = INFINITY;
		G->arcs[i][G->Vexnum] = INFINITY;
	}

	FILE *fp;
	fp=fopen("road.txt","wt");
	fprintf(fp,"%d %d\n",G->Vexnum,G->Arcnum);

	for(i = 1; i <= G->Vexnum; i++)
		for(j = i; j <= G->Vexnum; j++)
		{
			if(G->arcs[i][j] != INFINITY)
			{
				fprintf(fp,"%d %d %d\n",i,j,G->arcs[i][j]);
			}
		}
	fclose(fp);

	FILE *fp2;
	fp2=fopen("State.txt","at");
	fprintf(fp2,"\n%s %s", G->vex[G->Vexnum].Name, G->vex[G->Vexnum].Info);
	fclose(fp2);

	printf("\n\t\t\t\t\t地点添加成功！^_^\n");
	printf("\n\t\t\t\t\t按任意键进入下一步...");

	getch();
}


//读取admin.txt文件
struct User *Read()
{
	struct User *phead,*pnew,*pend;  	//创建链表
	
	FILE *fp;
	fp=fopen("Admin.txt","rt");
	phead=NULL; 						//头指针指向空

	if(fp!=NULL)
	{
		while(!feof(fp))
		{
			pnew=(struct User *)malloc(sizeof(struct User));	//给新结点开辟空间
			pnew->next=NULL;

			//如果一行不是两个数据,则终止循环
			if((fscanf(fp,"\n%s %s",pnew->ID,pnew->Passwd))!=2)
				break;
			if(phead==NULL)
			{
				phead=pnew;
			}
			else
			{
				pend->next=pnew;
			}
			pend=pnew;
		}
	}

	fclose(fp); //关闭文件
	return phead; //返回头指针
}


//添加新路线
void AddRoad(AdjMatrix *G)
{
	int i,j;
	int start,end,weight;
	system("cls");
	printf("\n\n");
	Print(G);
	
	do
	{
		printf("\t\t\t\t\t请输入增加路线的起点序号:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);

	do
	{
		printf("\t\t\t\t\t请输入增加路线的终点序号:");
		scanf("%d",&end);
	}while(end<=0 || end>G->Vexnum);

	printf("\t\t\t\t\t请输入增加路线的长度:");
	scanf("%d",&weight);

	if(G->arcs[start][end]== INFINITY ) 		//如果以前没有这条弧，则添加
	{
		G->arcs[start][end] = weight;
		G->arcs[end][start] = weight;  			 //并将矩阵元素赋值，主对角线对称，图边数+1
		G->Arcnum ++;

		//重新将添加新路径之后的图的信息写入"路线信息.txt"文件中
		FILE *fp;
		fp=fopen("Road.txt","wt");
		fprintf(fp,"%d %d\n",G->Vexnum,G->Arcnum);

		for(i = 1; i <= G->Vexnum; i++)
			for(j = i; j <= G->Vexnum; j++)
			{
				if(G->arcs[i][j] != INFINITY)
				{
					fprintf(fp,"%d %d %d\n",i,j,G->arcs[i][j]);
				}
			}
		fclose(fp);

		printf("\n\t\t\t\t\t路线添加成功\n");
	}
	else
	{
		printf("\n\t\t\t\t\t路线已经存在\n");
	}
	printf("\n\t\t\t\t\t按任意键返回...");
	
	getch();
}


void StateDel(AdjMatrix *G)
{
    int i,j,k;
    int x,y;
    int No;
    system("cls");
    printf("\n\n");
    Print(G);
    
	do
	{
        printf("\t\t\t\t\t请输入删除的地点序号:");
        scanf("%d",&No);
    }while(No<=0 || No>G->Vexnum);
    
	if(G->Arcnum==0)
    {
    	G->Vexnum--;

    	FILE *fp;
    	fp=fopen("Road.txt","wt");
    	fprintf(fp,"%d %d\n",G->Vexnum,G->Arcnum);
    	
		for(i = 1; i <= G->Vexnum; i++)
    	    for(j = i; j <= G->Vexnum; j++)
			{
     	       if(G->arcs[i][j] != INFINITY)
				{
                fprintf(fp,"%d %d %d\n",i,j,G->arcs[i][j]);
            	}
        	}
   		fclose(fp);

    	FILE *fp2;
    	fp2=fopen("State.txt","wt");
    	for(i = 1; i <= G->Vexnum+1; i++)
		{
    		if(i!=No)
    	    fprintf(fp2,"%s %s\n", G->vex[i].Name, G->vex[i].Info);
    	}
    	fclose(fp2);
    	CreateG(G);
    	
		printf("\n\t\t\t\t\t该地点删除成功！\n");
    	printf("\n\t\t\t\t\t按任意键进入下一步...");
    	getch();
	}
	else
	{
		printf("\n\t\t\t\t\t该地点存在未删除的路线，请先删除路线！\n");
    	printf("\n\t\t\t\t\t按任意键进入下一步...");
    	getch();
	}
}


//撤销旧路线
void RoadDel(AdjMatrix *G)
{
	int i,j;
	int start, end;
	system("cls");
	printf("\n\n");
	Print(G);

	do
	{
		printf("\t\t\t\t\t请输入撤销路线的起点建筑:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);

	do
	{
		printf("\t\t\t\t\t请输入撤销路线的终点建筑:");
		scanf("%d",&end);
	}while(end<=0 || end>G->Vexnum);

	if(G->arcs[start][end] != INFINITY ) 		 //如果以前有这条弧
	{
		G->arcs[start][end] = INFINITY;
		G->arcs[end][start] = INFINITY;  		//则将矩阵元素赋值为无穷，表示没有这条弧了
		G->Arcnum --;     						//并将图的边数-1
		
		////重新将撤销弧之后的图的信息写入文件中
		FILE *fp;
		fp=fopen("Road.txt","wt");
		fprintf(fp,"%d %d",G->Vexnum,G->Arcnum);
		fputc('\n',fp);
		
		for(i = 1; i <= G->Vexnum; i++)
			for(j = i; j <= G->Vexnum; j++)
			{
				if(G->arcs[i][j] != INFINITY)
				{
					fprintf(fp,"%d %d %d",i,j,G->arcs[i][j]);
					fputc('\n',fp);
				}
			}
		fclose(fp);
		printf("\n\t\t\t\t\t这条路线撤销成功\n");
	}
	else
	{
		printf("\n\t\t\t\t\t该路线不存在\n");
	}
	printf("\n\t\t\t\t\t按任意键返回...");
	getch();

}


//游客菜单
int User()
{
	AdjMatrix G;
	int choice;
	CreateG(&G);

	do
	{
		system("cls");
		printf("\n\n");
		printf("\t\t\t\t|| * * * * * * * * * * * * * * * * * * * * * * * * * * ||\n");
		printf("\t\t\t\t||                                                     ||\n");
		printf("\t\t\t\t||         ~~~欢迎使用西安邮电大学智慧园导航~~~        ||\n");
		printf("\t\t\t\t||                                                     ||\n");
		printf("\t\t\t\t||                  1. 查看地点信息                    ||\n");
		printf("\t\t\t\t||                  2. 查询简单路线                    ||\n");
		printf("\t\t\t\t||                  3. 查询中转最少路线                ||\n");
		printf("\t\t\t\t||                  4. 查询最优路线                    ||\n");
		printf("\t\t\t\t||                  5. 最佳布网方案                    ||\n");
		printf("\t\t\t\t||                  0. 退出                            ||\n");
		printf("\t\t\t\t||                                                     ||\n");
		printf("\t\t\t\t|| * * * * * * * * * * * * * * * * * * * * * * * * * * ||\n");
		printf("\n\n");

		do
		{
			printf("\t\t\t\t请选择您要进行的操作:");
			scanf("%d",&choice);
		}while( choice<0 || choice>5 );

		getchar();
		printf("\t\t\t\t按任意键进入下一步...");
		getch();
		printf("\n");

		switch(choice)
		{
			case 1:
				Info(&G);
				break;
			case 2:
				DfsSearch(&G);
				break;
			case 3:
				BfsSearch(&G);
				break;
			case 4:
				Short(&G);
				break;
			case 5:
				MinTree(&G);
				break;
			case 0:
				return 0;
		}
	}while(1);
}


//管理员菜单
int Admin()
{
	AdjMatrix G;
	int choice;
	CreateG(&G);
	
	do
	{
		system("cls");
		printf("\n\n");

		printf("\t\t\t\t||* * * * * * * * * * * * * * * * * * * * * * * * * * * ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||         ~~~欢迎使用西安邮电大学智慧园导航~~~         ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||                   1. 查看地点信息                    ||\n");
		printf("\t\t\t\t||                   2. 查询简单路线                    ||\n");
		printf("\t\t\t\t||                   3. 添加地点                        ||\n");
		printf("\t\t\t\t||                   4. 添加路线                        ||\n");
		printf("\t\t\t\t||                   5. 删除地点                        ||\n");
		printf("\t\t\t\t||                   6. 删除路线                        ||\n");
		printf("\t\t\t\t||                   7. 查询中转最少路线                ||\n");
		printf("\t\t\t\t||                   8. 查询最优路线                    ||\n");
		printf("\t\t\t\t||                   9. 查询最佳布网方案                ||\n");
		printf("\t\t\t\t||                   0. 退出                            ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||* * * * * * * * * * * * * * * * * * * * * * * * * * * ||\n");
		printf("\n\n");

		do
		{
			printf("\t\t\t\t请选择您要进行的操作:");
			scanf("%d",&choice);
		}while( choice<0 || choice>9 );

		getchar();
		printf("\t\t\t\t按任意键进入下一步...");
		getch();
		printf("\n");
		switch(choice)
		{
			case 1:
				Info(&G);
				break;
			case 2:
				DfsSearch(&G);
				break;
			case 3:
				AddState(&G);
				break;
			case 4:
				AddRoad(&G);
				break;
			case 5:
				StateDel(&G);
				break;
			case 6:
				RoadDel(&G);
				break;
			case 7:
				BfsSearch(&G);
				break;
			case 8:
			 	Short(&G);
			 	break;
			case 9:
				MinTree(&G);
				break;
			case 0 :
				exit(0);
		}
	}while(1);
}


//管理员登录
void AdminLog()
{
	system("cls");

	printf("\n\n");
	printf("\t\t\t\t||* * * * * * * * * * * * * * * * * * * * * * * ||\n");
	printf("\t\t\t\t||                                              ||\n");
	printf("\t\t\t\t||                  管理员登录                  ||\n");
	printf("\t\t\t\t||                                              ||\n");
	printf("\t\t\t\t||* * * * * * * * * * * * * * * * * * * * * * * ||\n");
	printf("\n\n");

	int i,n;
	char ch;

	for(i=0; i<3; i++)
	{
		struct User *t;
		t=Read();
		char id[20]= {0};
		char passwd[20]= {0};
		printf("\n\t\t\t\t\tLog In...");
		printf("\n\t\t\t\t\t帐号:");
		scanf("%s",id);

		getchar();  //吸收回车

		printf("\t\t\t\t\t密码:");
		for(n=0;; n++)
		{
			passwd[n]=getch();
			switch(passwd[n])
			{
				case '\b':
					passwd[--n]='\0';
					n--;
					printf("\b \b");
					break;
				
				case '\r':
					passwd[n]='\0';
					putchar('\n');
					break;
				
				default :
					putchar(' ');
					break;
			}
			if(passwd[n]=='\0')
				break;
		}
		while(t!=NULL)
		{
			if(strcmp(id,t->ID)==0&&strcmp(passwd,t->Passwd)==0)
			{
				printf("\n\t\t\t\t\t登录成功!按任意键进入主菜单...");
				getch();
				Admin();
				break;
			}
			t=t->next;
		}
		if(t==NULL)
		{
			printf("\n\t\t\t\t\t用户名或密码有误,请重新输入!\n\n");
		}
	}
}

int main()
{
	int choice;
	do
	{
		system("cls");

		printf("\n\n");
		printf("\t\t\t\t||* * * * * * * * * * * * * * * * * * * * * * * * * * * ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||            Hi，你好！我是小O！                       ||\n");
		printf("\t\t\t\t||       ~~~ 欢迎使用西安邮电大学智慧校园导航 ~~~       ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||                    1. 我是游客                       ||\n");
		printf("\t\t\t\t||                    2. 我是管理员                     ||\n");
		printf("\t\t\t\t||                    0. 退出系统                       ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||* * * * * * * * * * * * * * * * * * * * * * * * * * * ||\n");
		printf("\n\n");

		do
		{
			printf("\t\t\t\t请选择您的身份(输入0~2):");
			scanf("%d",&choice);
		}while( choice<0 || choice>2 );
		
		getchar();
		
		printf("\t\t\t\t按任意键进入下一步...");
		getch();
		printf("\n");
		switch(choice)
		{
			case 1:
				User();
				break;
			case 2:
				AdminLog();
				break;
			case 0:
				return 0;
		}
	} while(1);
	return 0;
}
