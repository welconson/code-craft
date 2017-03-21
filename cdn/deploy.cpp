#include "deploy.h"


//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	PNODE* NODElist = NULL;					//网络节点表
	PDES_NODE* DES_NODElist = NULL;			//消费节点表
	int SERVER_PRICE = EMPTY;				//服务器成本
	int NODE_SIZE = EMPTY;					//网络节点数量
	int DES_NODE_SIZE = EMPTY;				//消费节点数量
	int cost = INFINITY_CAPACITY;							//成本
	int server_num = 0;						//服务器数量
	int server_position[MAX_DES_SIZE] = {};	//对应序号的服务器的标号
	/*处理数据*/
	if(!data_handle(topo , &NODElist , &DES_NODElist ,
			&NODE_SIZE , &DES_NODE_SIZE , &SERVER_PRICE))
		return;
	/*初始化消费节点到邻接的网络节点的路径*/
	PPATH * to_DES = initDESPATH(DES_NODE_SIZE , DES_NODElist);
	/*添加超级汇点*/
	PNODE destination = NODElist[DESTINATION];
	int demand = EMPTY;
	int *NODE_TO_DEStable = (int *)malloc(sizeof(int) * NODE_SIZE);
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
		demand += DES_NODElist[i]->demand;
	}
//	/*暂时设置三个网络节点测试*/
//	server_position[0] = 0;
//	server_position[1] = 1;
//	server_position[2] = 15;
//	server_position[3] = 38;
//	server_num = 4;
//	cost += SERVER_PRICE * server_num;
	/*将初始服务器都设为空*/
	for(int i = 0 ; i < MAX_DES_SIZE ; i++)
		server_position[i] = NOTSET;
	/*暴搜*/
	for(int n = 2 ; n < DES_NODE_SIZE + 1; n++)
	{
		server_num = n;
		for(int i = 0 ; i < n; i++)
			server_position[i] = i;
		while(server_position[0] != NODE_SIZE - n + 1)
		{
			int thecost = SERVER_PRICE * server_num;
			PPATH * to_DES = initDESPATH(DES_NODE_SIZE , DES_NODElist);
			for(int i = n - 1 ; i > 0 ; i--)
				if(server_position[i] == NODE_SIZE - (n - i))
				{
					server_position[i] = server_position[i - 1] + 1;
					server_position[i - 1]++;
				}
			if(server_position[0] == NODE_SIZE - n + 1)
				break;
			for(int i = 0 ; i < NODE_SIZE ; i++)
			{
				PEDGE p = NODElist[i]->edgelist;
				while( p != NULL)
				{
					p->remain = p->capacity;
					p = p->nextP;
				}
			}
			PNODE source = NODElist[SOURCE];
			for(int i = 0 ; i < server_num ; i++)
			{
				PEDGE pe = initEDGE(server_position[i], INFINITY_CAPACITY , NO_DISTANCE);
				pe->nextP = source->edgelist;
				source->edgelist = pe;
			}
			PEDGE pe = NODElist[DESTINATION]->edgelist;
			while( pe != NULL)
			{
				pe->remain = pe->capacity;
				pe->reverse->remain = pe->capacity;
				pe = pe->nextP;
			}
			if(remain_traffic(DES_NODE_SIZE , NODE_SIZE , server_num , demand , &thecost,server_position ,
					source , to_DES , DES_NODElist , NODElist , NODE_TO_DEStable) && thecost < cost)
			{
				cost = thecost;
				printf("%d\n", cost);
				printf("%d\n" , n);
			}
			server_position[n-1]++;
			pe = source->edgelist;
			PEDGE next;
			source->edgelist = NULL;
			while(pe != NULL)
			{
				next = pe->nextP;
				free(pe);
				pe = next;
			}
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
	}
//	/*添加超级源点*/
//	PNODE source = NODElist[SOURCE];
//	/*为每个源点建立通往超级源点的路径*/
//	for(int i = 0 ; i < server_num ; i++)
//	{
//		PEDGE pe = initEDGE(server_position[i], INFINITY_CAPACITY , NO_DISTANCE);
//		pe->nextP = source->edgelist;
//		source->edgelist = pe;
//	}
//	/*对每个消费节点未满足的流量进行补充*/
//	if(remain_traffic(DES_NODE_SIZE , NODE_SIZE , server_num , demand , &cost,server_position ,
//			source , to_DES , DES_NODElist , NODElist , NODE_TO_DEStable))
//			printf("there is a way\n");
//	else printf("no way\n");
//	printf("%d\n",cost);
	// 需要输出的内容
	char * topo_file = getresult(to_DES , DES_NODE_SIZE);
//	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);
}
