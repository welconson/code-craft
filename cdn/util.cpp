/*
 * util.c
 *
 *  Created on: Mar 11, 2017
 *      Author: welcome
 */
#include "util.h"
#include <string.h>

/*关于NODE的函数*/
PNODE initNODE()
{
	PNODE pn = (PNODE)malloc(sizeof(NODE));
	if(NULL == pn)
		return NULL;
	pn->edgelist = NULL;
	pn->node_num = EMPTY;
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
	return pd;
}
/*关于PATH的函数*/
PPATH initPATH()
{
	PPATH pp = (PPATH)malloc(sizeof(PATH));
	if(NULL == pp)
		return NULL;
	pp->path = (int *)malloc(sizeof(int) * MAX_PATH);
	if(NULL == pp->path)
		return NULL;
	for(int i = 0 ; i < MAX_PATH ; i++)
		pp->path[i] = PATH_END;
	pp->traffic = 0;
	pp->nextp = NULL;
	pp->cost = 0;
	return pp;
}
/*初始化通往每个消费点的路径*/
PPATH * initDESPATH(int DES_NODE_NUM ,int SERVER_PRICE)
{
	PPATH * ppath = (PPATH *)malloc(sizeof(PPATH) * DES_NODE_NUM);
	if(NULL == ppath)
		return NULL;
	for(int i = 0 ; i < DES_NODE_NUM ; i++ )
	{
		ppath[i] = initPATH();
		if(NULL == ppath[i])
			return NULL;
		ppath[i]->cost = SERVER_PRICE;
	}
	return ppath;
}
/*将数据以邻接链表的形式动态存储起来*/
int data_handle(char * topo[], PNODE **NODElist ,
		PDES_NODE **DES_NODElist , int *NODE_SIZE ,
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
	PNODE *nodelist = (PNODE*)malloc(nodenum*sizeof(PNODE));
	*NODElist = nodelist;
	if(NULL == nodelist)
		return FALSE;
	for(int i = 0 ; i < nodenum ; i++)
		nodelist[i] = initNODE();
	/*获取边数*/
	cntcolumn++;
	while(line[cntcolumn] != ' ')
		cntcolumn++;
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
	PEDGE pe = NULL;
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
		pe = nodelist[node_num1]->edgelist;
		while(pe != NULL)
			pe = pe->nextP;
		pe = initEDGE(node_num2 , capacity, unit_price);

		pe = nodelist[node_num2]->edgelist;
		while(pe != NULL)
			pe = pe->nextP;
		pe = initEDGE(node_num1 , capacity, unit_price);

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
			for(int j = 0 ; pathlist->path[j] != PATH_END ; j++)		//获取每个路径
			{
				int cnt = MAX_BIT - 1;
				j = 0;
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
			/*			获取消费节点的标号			*/
			int cnt = MAX_BIT - 1;
			char word[MAX_BIT];
			int node_num = i;
			do
			{
				int m = node_num % 10;
				word[--cnt] = m + '0';
				node_num = node_num / 10;
			}while(node_num > 0);
			while(word[cnt] != '\0')
				line[cntcolumn++] = word[cnt++];
			line[cntcolumn++] = ' ';
			/*			获取路径的流量			*/
			cnt = MAX_BIT - 1;
			for(int i = 0 ; i < MAX_BIT ; i++)
				word[i] = '\0';
			node_num = pathlist->traffic;
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
			cntpath++;
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

void merge_server()
{
}

void dijkstra()
{

}
