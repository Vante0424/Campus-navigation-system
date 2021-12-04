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
	char ID[20];		  	//�ʺ�
	char Passwd[20];  		//����
	struct User *next;
};

typedef struct 				//����ص���Ϣ�Ľṹ��
{
	int No;    				//У԰�ص����
	char Name[20];    		//У԰�ص���
	char Info[200];  	//�ص�����
} StateData;

typedef struct 				//�ڽӾ���
{
	int arcs[MAXVEX][MAXVEX];  		 //�߼�
	StateData vex[MAXVEX];      	  //���㼯
	int Vexnum;              //������Ŀ
	int Arcnum;              //����Ŀ
} AdjMatrix;

typedef struct Node
{
	int Date;  				//����Ԫ�ص�ֵ���洢�ص����
	struct Node *next;
} QueueNode;

typedef struct
{
	QueueNode *front; 			//ͷָ��
	QueueNode *fear; 			//βָ��
} LinkQueue;


//�������� 
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
	printf("\t\t\t\t||         ***��ӭʹ�������ǻ�У԰����***        ||\n");
	printf("\t\t\t\t||                                               ||\n");
	printf("\t\t\t\t|| * * * * * * * * * * * * * * * * * * * * * * * ||\n\n");
	for(i = 1; i <= G->Vexnum; i++)
	{
		printf("\t\t\t\t%d.%-8s",i,G->vex[i].Name);
		count++;
		if(count == 2)			//һ������ 
		{
			printf("\n");
			count = 0;
		}
	}
	printf("\n\n");
}


//�ڽӾ���,��������ͼ
int CreateG( AdjMatrix *G )
{
	int i,j,weight,m,n;
	
	//��"·����Ϣ.txt"�ļ��ж�ȡУ԰ͼ�ľ�����Ŀ��·����Ŀ
	FILE *fp1;
	fp1=fopen("Road.txt","r");
	fscanf(fp1, "%d %d", &G->Vexnum, &G->Arcnum);

	//��ʼ���ڽӾ���
	for(i = 1; i <= G->Vexnum; i++)
		for(j = 1; j<= G->Vexnum; j++)
		{
			G->arcs[i][j] = INFINITY;
		}

	//��ȡ"·����Ϣ.txt"�ļ���������ż����룬����ֵ���ڽӾ���
	while(fscanf(fp1,"%d %d %d",&i,&j,&weight) != EOF)
	{
		G->arcs[i][j] = weight;			//���Խ��߶Գ� 
		G->arcs[j][i] = weight;
	}
	fclose(fp1);

	//��"�ص����.txt"�ļ��ж�ȡУ԰ͼ�еľ�����������
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


//��ѯ���м�·��
void DfsSearch( AdjMatrix *G )
{
	int start,end;
	system("cls");			//���� 
	printf("\n\n");
	Print(G);
	do
	{
		printf("\t\t\t\t\t������������:");
		scanf("%d",&start);
	}while( start<=0 || start>G->Vexnum );

	do
	{
		printf("\t\t\t\t\t�������յ����:");
		scanf("%d",&end);
	}while( start<=0 || start>G->Vexnum );

	printf("\n\t\t\t\t\t���ص�������·�����Ϊ:\n\n");

	int m = 2;

	memset(Stack,0,INFINITY);
	memset(Visited,0,MAXVEX);

	Stack[1] = start;
	Visited[start] = 1;

	DFS(G, m, start, end);

	printf("\n\t\t\t\t\t�����������...");

	memset(Stack,0,INFINITY);
	memset(Visited,0,MAXVEX);

	count = 0;
	getch();
}


//�鿴��ͼ����
void Info( AdjMatrix *G )
{
	int i,j,choice;
	system("cls");
	printf("\n\n");
	Print(G);

	do
	{
		printf("\t\t\t\t\t������Ҫ��ѯ�ĵص����(��0����):");
		scanf("%d",&choice);
		if(choice == 0)
			break;
		if(choice < 0 || choice > G->Vexnum)
			continue;
		printf("\n\t\t\t\t\t--->%s:%s\n",G->vex[choice].Name,G->vex[choice].Info);
		printf("\n\t\t\t\t\t�õص��·�����Ϊ:\n");
		for(i = 1; i <= G->Vexnum; i++)
		{
			if(i == choice) 		//���������иþ��������һ�еĸ���Ԫ��
			{
				for(j = 1; j <= G->Vexnum; j++)
				{
					if(G->arcs[i][j] != INFINITY)			 //�л����
					{
						printf("\t\t\t\t\t%-8s --- %8s: %3d m\n", G->vex[i].Name,G->vex[j].Name,G->arcs[i][j]);
					}
				}
			}
		}
		printf("\n\n");
	}while(1);

	printf("\n\t\t\t\t\t�����������...");
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


//��ѯ��С����������Ѳ������� 
void MinTree(AdjMatrix *G)
{
	int i,count=0;
	int start;
	system("cls");
	printf("\n\n");
	Print(G);

	do
	{
		printf("\t\t\t\t\t������������:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);
	
	for(i = 1; i <= G->Vexnum; i++)
	{
		if(G->arcs[start][i] != INFINITY)
			count++;
	}

	if(count == 0)
	{
		printf("\n\t\t\t\t\t�����������...");
		getch();
	}
	else
	{
		printf("\n");
		Prim(G, start);
	}
}


//Ѱ����һ���ٽӵ�
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
				w = i;  			//�����ǰ�ڵ�ĵ�һ���ڽӵ㣨�����ţ�
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
						w = NextAdj(G, w, v);	//w����ĵõ�һ���ڽӵ㣬v�����w��һ���ڽӵ�(�����һ���ڽӵ�����)
					}
					break;
				}
			}
		}
	}
}



//�����ת����
void BfsSearch( AdjMatrix *G )
{
	int start, end;
	system("cls");
	printf("\n\n");
	Print(G);
	
	do
	{
		printf("\t\t\t\t\t������������:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);

	do
	{
		printf("\t\t\t\t\t�������յ����:");
		scanf("%d",&end);
	}while(start<=0 || start>G->Vexnum);

	if(G->arcs[start][end] != INFINITY)
		printf("\n\t\t\t\t\t��%s��%s����Ҫ��ת\n",G->vex[start].Name,G->vex[end].Name);
	
	if(G->arcs[start][end] == INFINITY)
	{
		printf("\n\t\t\t\t\t��%s��%s��ת���������ٵ�·��Ϊ:\n\n",G->vex[start].Name,G->vex[end].Name);
		printf("\t\t\t");
		BFS(G, start, end);
		printf("%s\n",G->vex[end].Name);
	}
	
	printf("\n\t\t\t\t\t�����������...");
	getch();
}


//DijkstraѰ�����·�� 
void Dijkstra(AdjMatrix *G, int start, int end, int dist[], int path[][MAXVEX])
{
	int mindist, i, j, k, t = 1;

	for(i = 1; i <= G->Vexnum; i++)
	{
		dist[i] = G->arcs[start][i];  		//��dist�����ʼ��
		if(G->arcs[start][i] != INFINITY)
			path[i][1] = start;  			//����û����ڣ���path[i][1]ΪԴ��
	}
	
	path[start][0] = 1; 					//start���뵽S��

	for(i = 2; i <= G->Vexnum; i++) 		//Ѱ�Ҹ������·��
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
		path[k][0] = 1;   					//�ҵ����·�������õ���뵽S������

		for(j = 1; j <= G->Vexnum; j++) 	//�޸�·��
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
	printf("\n\t\t\t\t    %s--->%s�����·��Ϊ: ��%s",G->vex[start].Name,G->vex[end].Name,G->vex[start].Name);
	for(j = 2; path[i][j] != 0; j++)
	{
		printf("**%s",G->vex[path[i][j]].Name);
	}
	
	printf("**%s, ����Ϊ%d m\n",G->vex[end].Name,dist[i]);
	printf("\n\t\t\t\t\t�����������...");
	
	getch();
}


//����Dijkstra�㷨��ô����ر굽���յ�ر�����·��
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
		printf("\t\t\t\t\t������������:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);
	
	do
	{
		printf("\t\t\t\t\t�������յ����:");
		scanf("%d",&end);
	}while(end<=0 || end>G->Vexnum);
	
	Dijkstra(G,start,end,dist,path);
}



//Prim�㷨 
void Prim(AdjMatrix *G, int start)
{
	struct
	{
		int adjvex;
		int lowcost;
	}closedge[MAXVEX];
	
	int i, e, k, m, min;
	closedge[start].lowcost = 0;

	for(i = 1; i <= G->Vexnum; i++)			//�Գ��˳���������deep���ж����ʼ����Ӧ��closedge����
	{
		if(i != start)
		{
			closedge[i].adjvex = start;
			closedge[i].lowcost = G->arcs[start][i];
		}
	}

	for(e = 1; e <= G->Vexnum-1; e++)  		//����ѡ�е�n-1�����������ı�
	{
		//ѡ��Ȩֵ��С�ı�
		min = INFINITY;
		for(k = 1; k <= G->Vexnum; k++)
		{
			if(closedge[k].lowcost != 0 && closedge[k].lowcost < min)
			{
				m = k;
				min = closedge[k].lowcost;
			}
		}
		
		printf("\t\t\t\t\t��%s---%s:%d m\n", G->vex[closedge[m].adjvex].Name,G->vex[m].Name,closedge[m].lowcost);
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
	
	printf("\n\t\t\t\t\t�����������...");
	getch();
}


//�����µص�
void AddState(AdjMatrix *G)
{
	int i,j;
	int x,y;
	system("cls");
	printf("\n\n");
	Print(G);
	
	printf("\t\t\t\t\t���������ӵĵص�����:");
	scanf("%s",G->vex[++G->Vexnum].Name);
	
	G->vex[G->Vexnum].No = G->Vexnum;
	printf("\t\t\t\t\t������õص�ļ��:");
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

	printf("\n\t\t\t\t\t�ص���ӳɹ���^_^\n");
	printf("\n\t\t\t\t\t�������������һ��...");

	getch();
}


//��ȡadmin.txt�ļ�
struct User *Read()
{
	struct User *phead,*pnew,*pend;  	//��������
	
	FILE *fp;
	fp=fopen("Admin.txt","rt");
	phead=NULL; 						//ͷָ��ָ���

	if(fp!=NULL)
	{
		while(!feof(fp))
		{
			pnew=(struct User *)malloc(sizeof(struct User));	//���½�㿪�ٿռ�
			pnew->next=NULL;

			//���һ�в�����������,����ֹѭ��
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

	fclose(fp); //�ر��ļ�
	return phead; //����ͷָ��
}


//�����·��
void AddRoad(AdjMatrix *G)
{
	int i,j;
	int start,end,weight;
	system("cls");
	printf("\n\n");
	Print(G);
	
	do
	{
		printf("\t\t\t\t\t����������·�ߵ�������:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);

	do
	{
		printf("\t\t\t\t\t����������·�ߵ��յ����:");
		scanf("%d",&end);
	}while(end<=0 || end>G->Vexnum);

	printf("\t\t\t\t\t����������·�ߵĳ���:");
	scanf("%d",&weight);

	if(G->arcs[start][end]== INFINITY ) 		//�����ǰû���������������
	{
		G->arcs[start][end] = weight;
		G->arcs[end][start] = weight;  			 //��������Ԫ�ظ�ֵ�����Խ��߶Գƣ�ͼ����+1
		G->Arcnum ++;

		//���½������·��֮���ͼ����Ϣд��"·����Ϣ.txt"�ļ���
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

		printf("\n\t\t\t\t\t·����ӳɹ�\n");
	}
	else
	{
		printf("\n\t\t\t\t\t·���Ѿ�����\n");
	}
	printf("\n\t\t\t\t\t�����������...");
	
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
        printf("\t\t\t\t\t������ɾ���ĵص����:");
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
    	
		printf("\n\t\t\t\t\t�õص�ɾ���ɹ���\n");
    	printf("\n\t\t\t\t\t�������������һ��...");
    	getch();
	}
	else
	{
		printf("\n\t\t\t\t\t�õص����δɾ����·�ߣ�����ɾ��·�ߣ�\n");
    	printf("\n\t\t\t\t\t�������������һ��...");
    	getch();
	}
}


//������·��
void RoadDel(AdjMatrix *G)
{
	int i,j;
	int start, end;
	system("cls");
	printf("\n\n");
	Print(G);

	do
	{
		printf("\t\t\t\t\t�����볷��·�ߵ���㽨��:");
		scanf("%d",&start);
	}while(start<=0 || start>G->Vexnum);

	do
	{
		printf("\t\t\t\t\t�����볷��·�ߵ��յ㽨��:");
		scanf("%d",&end);
	}while(end<=0 || end>G->Vexnum);

	if(G->arcs[start][end] != INFINITY ) 		 //�����ǰ��������
	{
		G->arcs[start][end] = INFINITY;
		G->arcs[end][start] = INFINITY;  		//�򽫾���Ԫ�ظ�ֵΪ�����ʾû����������
		G->Arcnum --;     						//����ͼ�ı���-1
		
		////���½�������֮���ͼ����Ϣд���ļ���
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
		printf("\n\t\t\t\t\t����·�߳����ɹ�\n");
	}
	else
	{
		printf("\n\t\t\t\t\t��·�߲�����\n");
	}
	printf("\n\t\t\t\t\t�����������...");
	getch();

}


//�οͲ˵�
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
		printf("\t\t\t\t||         ~~~��ӭʹ�������ʵ��ѧ�ǻ�԰����~~~        ||\n");
		printf("\t\t\t\t||                                                     ||\n");
		printf("\t\t\t\t||                  1. �鿴�ص���Ϣ                    ||\n");
		printf("\t\t\t\t||                  2. ��ѯ��·��                    ||\n");
		printf("\t\t\t\t||                  3. ��ѯ��ת����·��                ||\n");
		printf("\t\t\t\t||                  4. ��ѯ����·��                    ||\n");
		printf("\t\t\t\t||                  5. ��Ѳ�������                    ||\n");
		printf("\t\t\t\t||                  0. �˳�                            ||\n");
		printf("\t\t\t\t||                                                     ||\n");
		printf("\t\t\t\t|| * * * * * * * * * * * * * * * * * * * * * * * * * * ||\n");
		printf("\n\n");

		do
		{
			printf("\t\t\t\t��ѡ����Ҫ���еĲ���:");
			scanf("%d",&choice);
		}while( choice<0 || choice>5 );

		getchar();
		printf("\t\t\t\t�������������һ��...");
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


//����Ա�˵�
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
		printf("\t\t\t\t||         ~~~��ӭʹ�������ʵ��ѧ�ǻ�԰����~~~         ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||                   1. �鿴�ص���Ϣ                    ||\n");
		printf("\t\t\t\t||                   2. ��ѯ��·��                    ||\n");
		printf("\t\t\t\t||                   3. ��ӵص�                        ||\n");
		printf("\t\t\t\t||                   4. ���·��                        ||\n");
		printf("\t\t\t\t||                   5. ɾ���ص�                        ||\n");
		printf("\t\t\t\t||                   6. ɾ��·��                        ||\n");
		printf("\t\t\t\t||                   7. ��ѯ��ת����·��                ||\n");
		printf("\t\t\t\t||                   8. ��ѯ����·��                    ||\n");
		printf("\t\t\t\t||                   9. ��ѯ��Ѳ�������                ||\n");
		printf("\t\t\t\t||                   0. �˳�                            ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||* * * * * * * * * * * * * * * * * * * * * * * * * * * ||\n");
		printf("\n\n");

		do
		{
			printf("\t\t\t\t��ѡ����Ҫ���еĲ���:");
			scanf("%d",&choice);
		}while( choice<0 || choice>9 );

		getchar();
		printf("\t\t\t\t�������������һ��...");
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


//����Ա��¼
void AdminLog()
{
	system("cls");

	printf("\n\n");
	printf("\t\t\t\t||* * * * * * * * * * * * * * * * * * * * * * * ||\n");
	printf("\t\t\t\t||                                              ||\n");
	printf("\t\t\t\t||                  ����Ա��¼                  ||\n");
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
		printf("\n\t\t\t\t\t�ʺ�:");
		scanf("%s",id);

		getchar();  //���ջس�

		printf("\t\t\t\t\t����:");
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
				printf("\n\t\t\t\t\t��¼�ɹ�!��������������˵�...");
				getch();
				Admin();
				break;
			}
			t=t->next;
		}
		if(t==NULL)
		{
			printf("\n\t\t\t\t\t�û�������������,����������!\n\n");
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
		printf("\t\t\t\t||            Hi����ã�����СO��                       ||\n");
		printf("\t\t\t\t||       ~~~ ��ӭʹ�������ʵ��ѧ�ǻ�У԰���� ~~~       ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||                    1. �����ο�                       ||\n");
		printf("\t\t\t\t||                    2. ���ǹ���Ա                     ||\n");
		printf("\t\t\t\t||                    0. �˳�ϵͳ                       ||\n");
		printf("\t\t\t\t||                                                      ||\n");
		printf("\t\t\t\t||* * * * * * * * * * * * * * * * * * * * * * * * * * * ||\n");
		printf("\n\n");

		do
		{
			printf("\t\t\t\t��ѡ���������(����0~2):");
			scanf("%d",&choice);
		}while( choice<0 || choice>2 );
		
		getchar();
		
		printf("\t\t\t\t�������������һ��...");
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
