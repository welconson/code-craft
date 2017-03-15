#include "deploy.h"


//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	PNODE* NODElist = NULL;
	PDES_NODE* DES_NODElist = NULL;
	int SERVER_PRICE = EMPTY;
	int NODE_SIZE = EMPTY;
	int DES_NODE_SIZE = EMPTY;
	int cost = 0;
	int server_num = 0;
	if(!data_handle(topo , &NODElist , &DES_NODElist ,
			&NODE_SIZE , &DES_NODE_SIZE , &SERVER_PRICE))
		return;
	PPATH * to_DES = initDESPATH(DES_NODE_SIZE , SERVER_PRICE);	//服务器搭建在邻接节点
	server_num = DES_NODE_SIZE;
	for(int i = 0 ; i < DES_NODE_SIZE ; i++)
	{
		to_DES[i]->traffic = DES_NODElist[i]->demand;		//服务器搭建在邻接节点，流量直接满足需求
		to_DES[i]->path[0] = DES_NODElist[i]->node_num;		//记录传输路线
		cost = cost + to_DES[i]->cost;						//合计初始的服务器费用
//		printf("path[%d]:%d\n", i , DES_NODElist[i]->node_num);		//test
	}
	// 需要输出的内容
	char * topo_file = getresult(to_DES , DES_NODE_SIZE);
//	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);

}
