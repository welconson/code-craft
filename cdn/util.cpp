/*
 * util.c
 *
 *  Created on: Mar 11, 2017
 *      Author: welcome
 */
#include "util.h"

/*关于NODE的函数*/
PNODE initNODE()
{
	PNODE pn = (PNODE)malloc(sizeof(NODE));
	if(NULL == pn)
		return NULL;
	pn->edgelist = NULL;
	pn->node_num = EMPTY;
	pn->edge_num = EMPTY;
	pn->try_attri = EMPTY;
	return pn;
}

/*关于EDGE的函数*/
PEDGE initEDGE(int num , int capacity , int unit_price)
{
	PEDGE pe = (PEDGE)malloc(sizeof(EDGE));
	if(NULL == pe)
		return NULL;
	pe->node_num = num;
	pe->capacity = capacity;
	pe->remain = capacity;
	pe->unit_price = unit_price;
	pe->traffic = EMPTY;
	pe->nextP = NULL;
	return pe;
}
/*关于DES_NODE的函数*/
PDES_NODE initDES_NODE(int node_num , int demand)
{
	PDES_NODE pd = (PDES_NODE)malloc(sizeof(DES_NODE));
	if(NULL == pd)
		return NULL;
	pd->traffic = 0;
	pd->demand = demand;
	pd->node_num = node_num;
	pd->cost = 0;
	pd->edge = NULL;
	return pd;
}
/*关于PATH的函数*/
PPATH initPATH(int start)
{
	PPATH pp = (PPATH)malloc(sizeof(PATH));
	if(NULL == pp)
		return NULL;
	pp->path = (int *)malloc(sizeof(int) * MAX_PATH);
	if(NULL == pp->path)
		return NULL;
	pp->traffic = 0;
	pp->nextp = NULL;
	pp->path[0] = start;
	return pp;
}
/*初始化每个消费节点的路径指针*/
PPATH * initDESPATH(int DES_NODE_NUM ,PDES_NODE *DES_NODElist)
{
	PPATH * ppath = (PPATH *)malloc(sizeof(PPATH) * DES_NODE_NUM);
	if(NULL == ppath)
		return NULL;
	for(int i = 0 ; i < DES_NODE_NUM ; i++ )
	{
		ppath[i] = initPATH(i);
		if(NULL == ppath[i])
			return NULL;
		ppath[i]->traffic = 0;
	}
	return ppath;
}
/*初始化每个服务节点的路径指针*/
PPATH * initSOUPATH(int server_num)
{
	PPATH * ppath = (PPATH*)malloc(sizeof(PPATH) * server_num);
	if(NULL == ppath)
		return NULL;
	return ppath;
}
/*初始化解决方案*/
PSOLUTION initSOLUTION(int degree , int frontnode)
{
	PSOLUTION s = (PSOLUTION)malloc(sizeof(SOLUTION));
	if( NULL == s )
		return NULL;
	s->server_num = 0;
	s->degree = degree;
	s->frontnode = frontnode;
	s->servertable = (int*)malloc(sizeof(int)*MAX_DES_SIZE) ;
	s->pathlist = NULL;
	return s;
}
/*将数据以邻接链表的形式动态存储起来*/
int data_handle(char * topo[], PNODE **NODElist ,
		PDES_NODE **DES_NODElist , int *NODE_SIZE ,int *EDGE_SIZE,
		int *DES_NODE_SIZE , int *SERVER_PRICE)
{
	/*获取网络节点的数量和消费节点的数量*/
	int cntline = 0;
	int cntcolumn = 0;
	int nodenum = 0;
	char *line = topo[0];
	/*获取网络节点数*/
	while(line[cntcolumn] != ' ')
		nodenum = line[cntcolumn++] - '0' + nodenum*10;
	/*为网络节点分配动态数组空间*/
	*NODE_SIZE = nodenum;
	/*多出两个节点分别是超接源点和超级汇点*/
	PNODE *nodelist = (PNODE*)malloc((nodenum + 2)*sizeof(PNODE));
	*NODElist = nodelist;
	if(NULL == nodelist)
		return FALSE;
	for(int i = 0 ; i < nodenum + 2 ; i++)
		nodelist[i] = initNODE();
	/*获取边数*/
	cntcolumn++;
	int edge_num = 0;
	while(line[cntcolumn] != ' ')
		edge_num = line[cntcolumn++] - '0' + edge_num*10;
	*EDGE_SIZE = edge_num;
	/*获取消费节点数*/
	cntcolumn++;
	nodenum = 0;
	while(line[cntcolumn] != '\r' && line[cntcolumn] != '\n')
		nodenum = line[cntcolumn++] - '0' + nodenum*10;
	/*为消费节点分配动态数组空间*/
	*DES_NODE_SIZE = nodenum;
	PDES_NODE *des_nodelist = (PDES_NODE*)malloc(nodenum*sizeof(PDES_NODE));
	*DES_NODElist = des_nodelist;
	if(NULL == des_nodelist)
		return FALSE;
	/*		获取服务器单价		*/
	cntline = cntline + 2;
	line = topo[cntline];
	cntcolumn = 0;
	int server_price = 0;
	while(line[cntcolumn] != '\r' && line[cntcolumn] != '\n')
		server_price = line[cntcolumn++] - '0' + server_price * 10;
	*SERVER_PRICE = server_price;
	/*		获取网络节点之间的边集		*/
	cntline = cntline + 2;
	PNODE pnode = NULL;
	line = topo[cntline];
	while(line[0] != '\r' && line[0] != '\n')
	{
		cntcolumn = 0;
		int node_num1 = 0;
		int node_num2 = 0;
		int capacity = 0;
		int unit_price = 0;
		/*				获取起始点			*/
		while(line[cntcolumn] != ' ')
			node_num1 = line[cntcolumn++] - '0' + node_num1 * 10;
		/*				获取到达点			*/
		cntcolumn++;
		while(line[cntcolumn] != ' ')
			node_num2 = line[cntcolumn++] - '0' + node_num2 * 10;
		/*				获取容量				*/
		cntcolumn++;
		while(line[cntcolumn] != ' ')
			capacity = line[cntcolumn++] - '0' + capacity * 10;
		/*				获取单价				*/
		cntcolumn++;
		while(line[cntcolumn] != '\r' && line[cntcolumn] != '\n')
			unit_price = line[cntcolumn++] - '0' + unit_price * 10;
		/*			将获取的边加入对应点的边集			*/
		pnode = nodelist[node_num1];
		nodelist[node_num1]->edge_num++;
		if( pnode->edgelist != NULL)
		{
			/*		每次将新生成的一条边插入边集的头位置		*/
			PEDGE tmp = pnode->edgelist;
			pnode->edgelist = initEDGE(node_num2 , capacity, unit_price);
			pnode->edgelist->nextP = tmp;
		}
		else
			pnode->edgelist = initEDGE(node_num2 , capacity, unit_price);

		pnode = nodelist[node_num2];
		nodelist[node_num2]->edge_num++;
		if( pnode->edgelist != NULL)
		{
			/*		每次将新生成的一条边插入边集的头位置		*/
			PEDGE tmp = pnode->edgelist;
			pnode->edgelist = initEDGE(node_num1 , capacity, unit_price);
			pnode->edgelist->nextP = tmp;
		}
		else
			pnode->edgelist = initEDGE(node_num1 , capacity, unit_price);
		/*将互为反向边的两边互连*/
		nodelist[node_num1]->edgelist->reverse = nodelist[node_num2]->edgelist;
		nodelist[node_num2]->edgelist->reverse = nodelist[node_num1]->edgelist;
		cntline++;
		line = topo[cntline];
	}
	/*获取消费节点与网络节点边集*/
	cntline++;
	while(topo[cntline] != NULL)
	{
		line = topo[cntline];
		cntcolumn = 0;
		int des_node_num = 0;
		int node_num = 0;
		int demand = 0;
		/*获取消费节点号*/
		while(line[cntcolumn] != ' ')
			des_node_num = line[cntcolumn++] - '0' + des_node_num * 10;
		/*获取网络节点号*/
		cntcolumn++;
		while(line[cntcolumn] != ' ')
			node_num = line[cntcolumn++] - '0' + node_num * 10;
		/*获取消费需求*/
		cntcolumn++;
		while(line[cntcolumn] != '\r' && line[cntcolumn] != '\n')
			demand = line[cntcolumn++] - '0' + demand * 10;
		des_nodelist[des_node_num] = initDES_NODE(node_num , demand);
		cntline++;
	}
	return TRUE;
}

char * getresult(PPATH * to_DES , int DES_NODE_SIZE)
{
	char *result = (char *)malloc(sizeof(char) * MAX_LINE_LEN);;
	char result_path[MAX_LINE_LEN];
	memset(result_path , 0 , sizeof(result_path));
	int cntpath = 0;
	for(int i = 0 ; i < DES_NODE_SIZE ; i++)		//遍历所有消费节点
	{
		PPATH pathlist = to_DES[i];			//用来记录每个消费节点的路径集头节点
		while(pathlist != NULL)			//遍历每个消费节点的路径集
		{
			char line[MAX_LINE_LEN];									//每个路径都要重新写行
			int cntcolumn = 0;
			int j = 0 ;
			for(; pathlist->path[j] != PATH_END ; j++);	//获取每个路径
			for(j-- ; j > -1 ; j--)
			{
				int cnt = MAX_BIT - 1;
				char word[MAX_BIT];
				int node_num = pathlist->path[j];
				do
				{
					int m = node_num % 10;
					word[--cnt] = m + '0';
					node_num = node_num / 10;
				}while(node_num > 0);
				while(word[cnt] != '\0')
					line[cntcolumn++] = word[cnt++];
				line[cntcolumn++] = ' ';
			}
			/*			获取路径的流量			*/
			int cnt = MAX_BIT - 1;
			char word[MAX_BIT];
			for(int i = 0 ; i < MAX_BIT ; i++)
				word[i] = '\0';
			int node_num = pathlist->traffic;
			do
			{
				int m = node_num % 10;
				word[--cnt] = m + '0';
				node_num = node_num / 10;
			}while(node_num > 0);
			while(word[cnt] != '\0')
				line[cntcolumn++] = word[cnt++];
			line[cntcolumn++] = '\n';
			line[cntcolumn] = '\0';				//行尾添加字符串终止符号
			cntpath++;							//行数计数
			pathlist = pathlist->nextp;
			strcat(result_path , line);				//将此条路径信息加入结果集
		}
	}
	/*			获取路径的数量			*/
	int cntcolumn = 0;
	int cnt = MAX_BIT - 1;
	char word[MAX_BIT];
	memset(word , 0 , sizeof(word));
	do
	{
		int m = cntpath % 10;
		word[--cnt] = m + '0';
		cntpath = cntpath / 10;
	}while(cntpath > 0);
	while(word[cnt] != '\0')
		result[cntcolumn++] = word[cnt++];
	result[cntcolumn++] = '\n';
	result[cntcolumn++] = '\n';
	result[cntcolumn] = '\0';
	strcat(result,result_path);
	int i = 0;
	for(; result[i] != '\0'; i++);
	result[--i] = '\0';
	return result;
}


/*适用于服务器向消费节点寻路*/
void dijkstra(PNODE * NODElist , int NODE_SIZE , PNODE source,
		int *distance, int *front , int *traffic)
{
	PHEAP h = get_heap();
	PTREE *TREElist = (PTREE*)malloc(sizeof(PTREE) * (NODE_SIZE + 2));
	for(int i = 0; i < NODE_SIZE + 2; i++)
	{
		TREElist[i] = (PTREE)malloc(sizeof(TREE));
		TREElist[i]->num = i;
		TREElist[i]->key = UNREACH;
		distance[i] = UNREACH;
		front[i] = UNLINKED;
		traffic[i] = EMPTY;
		heap_insert(h , TREElist[i]);
	}
	/*设置源节点*/
	heap_decrease(h , TREElist[NODE_SIZE] , 0);
	traffic[NODE_SIZE] = INFINITY_CAPACITY;
	while(h->n > 0)
	{
		PTREE t = heap_extract_min(h);
		int node_num = t->num;
		for(PEDGE elist = NODElist[node_num]->edgelist;
				elist != NULL ; elist = elist->nextP)
		{
			int reach_num = elist->node_num;
			int newkey = t->key + elist->unit_price;
			if(TREElist[reach_num]->key > newkey && elist->remain > 0)
			{
				heap_decrease(h , TREElist[reach_num] ,
				newkey);
				front[reach_num] = node_num;
				distance[reach_num] = newkey;
				traffic[reach_num] = traffic[node_num] > elist->remain ?
						elist->remain : traffic[node_num];
			}
		}
	}
	for(int i = 0 ; i < NODE_SIZE + 2; i++)
	{
//		printf("%d:",i);
//		printf("%d  ",distance[i]);
//		printf("%d  %d |",front[i] , traffic[i]);
		free(TREElist[i]);
	}
	free(TREElist);
	free(h);
//	for(int i = 0 ; i < NODE_SIZE ; i++)
//	{
//		printf("%dto %d:%d\t\t", source,i ,TREElist[i]->key);
//	}
//	printf("\n");
}

int remain_traffic(int DES_NODE_SIZE , int NODE_SIZE ,int server_num ,
		int demand , int *cost,int * server_position ,
		PNODE source , PPATH * to_DES , PPATH * to_SOU ,PDES_NODE *DES_NODElist,
		PNODE * NODElist ,int * NODE_TO_DEStable , int *NODE_TO_SERtable)
{
	while(demand > 0)
	{
		int *distance = (int *)malloc(sizeof(int) * MAX_NODE_SIZE);
		int *fronttable = (int *)malloc(sizeof(int) * MAX_NODE_SIZE);
		int *traffic = (int *)malloc(sizeof(int) * MAX_NODE_SIZE);
		dijkstra(NODElist , NODE_SIZE, source, distance, fronttable ,traffic);
		if(fronttable[DESTINATION] != UNLINKED)
		{
			/*超级汇点有前驱点，则证明路线是连通的*/
			int thetraffic = traffic[DESTINATION];
			demand -= thetraffic;
			/*超级汇点的前驱节点是该消费节点，则证明该消费节点的流量还没满足要求*/
			PPATH pathlist = to_DES[NODE_TO_DEStable[fronttable[DESTINATION]]];
			/*判断头节点路径是否使用*/
			if(pathlist->traffic != 0)
			{
				/*为该消费节点创建新的路径*/
				PPATH tmp = initPATH(NODE_TO_DEStable[fronttable[DESTINATION]]);
				tmp->nextp = pathlist->nextp;
				pathlist->nextp = tmp;
				pathlist = tmp;
			}
			int path_end = 1;
			for(int front = fronttable[DESTINATION] , behind = DESTINATION ;
					front != NODE_SIZE; behind = front , front = fronttable[front])
			{
				PEDGE edge= NODElist[behind]->edgelist;
				for( ; edge->node_num != front; )
					edge=edge->nextP;
				/*我们将超级源点到超级汇点的路线视为正向边，前驱得到的边则为逆向边*/
				edge->reverse->remain -= thetraffic;
				edge->remain = edge->reverse->capacity - edge->reverse->remain;
				pathlist->path[path_end++] = front;
			}
			*cost += thetraffic * distance[DESTINATION];
			pathlist->traffic = thetraffic;
			pathlist->path[path_end] = PATH_END;
//			int server = pathlist->path[path_end-1];
//			if(to_SOU[server] != NULL)
//			{
//				PPATH tmp = to_SOU[server]->nextp;
//				to_SOU[server]->nextp = pathlist;
//
//			}
//			else
//				to_SOU[server]->nextp = pathlist;
			free(distance);
			free(fronttable);
			free(traffic);
		}
		else
			{
			free(distance);
			free(fronttable);
			free(traffic);
			return FALSE;
			}
	}
	return TRUE;
}
/*初始化工作*/
void initlize(PNODE *NODElist , PDES_NODE *DES_NODElist , int DES_NODE_SIZE ,int NODE_SIZE,
		PNODE destination , int *NODE_TO_DEStable , int *demand)
{
	/*为每个汇点建立通往超级汇点的双向路径*/
	for(int i = 0 ; i < DES_NODE_SIZE ; i++)
	{
		PEDGE pe = initEDGE(DESTINATION , DES_NODElist[i]->demand , NO_DISTANCE);
		pe->nextP = NODElist[DES_NODElist[i]->node_num]->edgelist;
		NODElist[DES_NODElist[i]->node_num]->edgelist = pe;
		PEDGE pe0 = initEDGE(DES_NODElist[i]->node_num , DES_NODElist[i]->demand , NO_DISTANCE);
		pe0->nextP = destination->edgelist;
		destination->edgelist = pe0;
		pe->reverse = pe0;
		pe0->reverse = pe;
		NODE_TO_DEStable[DES_NODElist[i]->node_num] = i;
		*demand += DES_NODElist[i]->demand;
	}
	/*为每个网络节点设置设为服务器的尝试次数*/
	for(int i = 0 ; i < NODE_SIZE ; i++)
		NODElist[i]->try_attri =NODElist[i]->edge_num * TRY_ATTRI;
	/*将网络节点的总流量和边集数量建堆*/
	PHEAP edgenum_heap = get_heap();
	PHEAP nodecap_heap = get_heap();
	for(int i = 0 ; i < NODE_SIZE ; i++)
	{
		PEDGE p = NODElist[i]->edgelist;
		int cap = 0;
		int cnt = 0;
		while( p != NULL)
		{
			cap += p->capacity;
			p = p->nextP;
			cnt++;
		}
		PTREE t = (PTREE)malloc(sizeof(TREE));
		PTREE t0 = (PTREE)malloc(sizeof(TREE));
		t->num = i;
		t->key = cnt;
		t0->num = i;
		t0->key = cap;
		heap_insert_max(edgenum_heap , t);
		heap_insert_max(nodecap_heap , t0);
//		printf("%d:%d %d\t|", i,cnt,cap);
	}
}
/*给定服务器的数量和位置，将边集的残余流量重置,加入超级源点的边集,将超级汇点的边集的残余流量重置*/
void reset(PNODE *NODElist, int NODE_SIZE, int server_num , int *server_position ,PNODE source)
{
	/*将边集的残余流量重置*/
	for(int i = 0 ; i < NODE_SIZE ; i++)
	{
		PEDGE p = NODElist[i]->edgelist;
		while( p != NULL)
		{
			p->remain = p->capacity;
			p = p->nextP;
		}
	}
	/*加入超级源点的边集*/
	for(int i = 0 ; i < server_num ; i++)
	{
		PEDGE pe = initEDGE(server_position[i], INFINITY_CAPACITY , NO_DISTANCE);
		pe->nextP = source->edgelist;
		source->edgelist = pe;
	}
	/*将超级汇点的边集的残余流量重置*/
	PEDGE pe = NODElist[DESTINATION]->edgelist;
	while( pe != NULL)
	{
		pe->remain = pe->capacity;
		pe->reverse->remain = pe->capacity;
		pe = pe->nextP;
	}
}
/*之前指定服务器时的数据清理，路径保留*/
void clean(PNODE source , int server_num , int *server_position)
{
	/*释放当前超级源点的边集*/
	PEDGE pe = source->edgelist;
	PEDGE next;
	source->edgelist = NULL;
	while(pe != NULL)
	{
		next = pe->nextP;
		free(pe);
		pe = next;
	}
	for(int i = 0 ; i < server_num ; i++)
		server_position[i] = NOTSET;
}
/*之前指定服务器时的数据清理,含无用路径清理*/
void clean(PNODE source ,  int server_num , int *server_position ,int DES_NODE_SIZE , PPATH *to_DES)
{
	/*释放当前服务器的边集*/
	clean(source , server_num , server_position);
	/*释放当前服务器下的路径*/
	for(int i = 0 ; i < DES_NODE_SIZE ; i++)
	{
		PPATH p = to_DES[i];
		PPATH next;
		while(p != NULL)
		{
			next = p->nextp;
			free(p);
			p = next;
		}
	}
}
/*解决方案产生器*/
void solu_generater(int NODE_SIZE, PSOLUTION now, int *server_position, PNODE *NODElist,
		int *solve, int EDGE_SIZE , PSOLUTION *solutionset)
{
	int merge_table[NODE_SIZE + 2] = {0};
	int fronttable[NODE_SIZE] = {0};
	int server_num = now->server_num;
	merge_table[SOURCE] = UNREACH;
	merge_table[DESTINATION] = UNREACH;
	for(int m = 0 ; m < server_num ; m++)
		merge_table[server_position[m]]++;
	for(int i = 0 ; i < server_num ; i++)
	{
		for(PEDGE pe = NODElist[server_position[i]]->edgelist ; pe != NULL ; pe = pe->nextP)
		{
			int node = pe->node_num;
			if(node == now->frontnode )
				continue;
			if(merge_table[node] == 0)
			{
				if(NODElist[node]->try_attri > 0)
				{
					NODElist[node]->try_attri--;
					merge_table[node]++;
					fronttable[node] = i;
					int solve_num = (*solve) % SOLU_SIZE;
					solutionset[solve_num] = initSOLUTION(now->degree + 1 , server_position[i]);
					for(int j = 0 ; j < server_num ; j++)
						solutionset[solve_num]->servertable[j] = server_position[j];
					solutionset[solve_num]->servertable[i] = node;
					solutionset[solve_num]->server_num = server_num;
					(*solve)++;
				}
			}
			else if(merge_table[node] == 1)
			{
				if(NODElist[i]->try_attri > 0)
				{
					NODElist[i]->try_attri--;
					int solve_num = (*solve) % SOLU_SIZE;
					solutionset[solve_num] = initSOLUTION(now->degree + 1 , server_position[i]);
					for(int j = 0 ,  m = 0; m < server_num ;m++)
					{
						if( m == node )
							continue;
						solutionset[solve_num]->servertable[j++] = server_position[m];
					}
					solutionset[solve_num]->servertable[server_num -1] = NOTSET;
					solutionset[solve_num]->server_num = server_num - 1;
					(*solve)++;
				}

				if(NODElist[node]->try_attri > 0)
				{
					NODElist[node]->try_attri--;
					int solve_num = (*solve) % SOLU_SIZE;
					solutionset[solve_num] = initSOLUTION(now->degree + 1 , server_position[i]);
					for(int j = 0 ,  m = 0; m < server_num ;m++)
					{
						if( m == i )
							continue;
						solutionset[solve_num]->servertable[j++] = server_position[m];
					}
					solutionset[solve_num]->servertable[server_num -1] = NOTSET;
					solutionset[solve_num]->server_num = server_num - 1;
					(*solve)++;
				}
			}
		}
	}
}
