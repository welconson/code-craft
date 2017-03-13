/*
 * util.c
 *
 *  Created on: Mar 11, 2017
 *      Author: welcome
 */
#include "util.h"

typedef struct node
{
	int node_num;
	PEDGE edgelist;
}NODE;

typedef struct des_node
{
	int node_num;
	int demand;
	int traffic;
}DES_NODE;

typedef struct edge
{
	int node_num;
	int capacity;
	int traffic;
	int unit_price;
	PEDGE nextP;
}EDGE;

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

int data_handle(char * topo[], PNODE NODElist[] ,
		PDES_NODE DES_NODElist[] , int *NODE_SIZE ,
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
	NODElist = (PNODE*)malloc(nodenum*sizeof(PNODE));
	if(NULL == NODElist)
		return FALSE;
	for(int i = 0 ; i < nodenum ; i++)
		NODElist[i] = initNODE();
	/*获取边数*/
	cntcolumn++;
	while(line[cntcolumn] != ' ')
		cntcolumn++;
	/*获取消费节点数*/
	cntcolumn++;
	nodenum = 0;
	while(line[cntcolumn] != '\r')
		nodenum = line[cntcolumn++] - '0' + nodenum*10;
	/*为消费节点分配动态数组空间*/
	*DES_NODE_SIZE = nodenum;
	DES_NODElist = (PDES_NODE*)malloc(nodenum*sizeof(PDES_NODE));
	if(NULL == DES_NODElist)
		return FALSE;
	/*		获取服务器单价		*/
	cntline = cntline + 2;
	line = topo[cntline];
	cntcolumn = 0;
	int server_price = 0;
	while(line[cntcolumn] != '\r')
		server_price = line[cntcolumn++] - '0' + server_price * 10;
	*SERVER_PRICE = server_price;
	/*		获取网络节点之间的边集		*/
	cntline = cntline + 2;
	PEDGE pe = NULL;
	line = topo[cntline];
	while(line[0] != '\r')
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
		while(line[cntcolumn] != '\r')
			unit_price = line[cntcolumn++] - '0' + unit_price * 10;
		/*			将获取的边加入对应点的边集			*/
		pe = NODElist[5]->edgelist;
		while(pe != NULL)
			pe = pe->nextP;
		pe = initEDGE(node_num2 , capacity, unit_price);

		pe = NODElist[node_num2]->edgelist;
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
		while(line[cntcolumn] != '\r')
			demand = line[cntcolumn++] - '0' + demand * 10;
		DES_NODElist[des_node_num] = initDES_NODE(node_num , demand);
		cntline++;
	}
	return TRUE;
}

