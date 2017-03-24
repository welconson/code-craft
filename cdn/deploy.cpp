#include "deploy.h"


//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	clock_t start,end;
	start = clock();
	PNODE* NODElist = NULL;					//网络节点表
	PDES_NODE* DES_NODElist = NULL;			//消费节点表
	int SERVER_PRICE = EMPTY;				//服务器成本
	int NODE_SIZE = EMPTY;					//网络节点数量
	int DES_NODE_SIZE = EMPTY;				//消费节点数量
	int EDGE_SIZE = EMPTY;
	int cost = INFINITY_CAPACITY;							//成本
	int server_num = 0;						//服务器数量
	int *server_position = NULL;	//对应序号的服务器的网络节点号
	int demand = EMPTY;
	/*处理数据*/
	if(!data_handle(topo , &NODElist , &DES_NODElist ,
			&NODE_SIZE , &EDGE_SIZE , &DES_NODE_SIZE , &SERVER_PRICE))
		return;
	/*初始化工作*/
	PNODE destination = NODElist[DESTINATION];
	PNODE source = NODElist[SOURCE];
	int *NODE_TO_DEStable = (int *)malloc(sizeof(int) * NODE_SIZE);
	int *NODE_TO_SERtable = (int *)malloc(sizeof(int) * NODE_SIZE);
	/*添加超级汇点,计算总需求量，建立网络节点到消费节点的映射表*/
	initlize(NODElist , DES_NODElist , DES_NODE_SIZE , NODE_SIZE ,
			destination , NODE_TO_DEStable , &demand);
	PSOLUTION nowbest = initSOLUTION( 0 , NOTSET);
	PSOLUTION now = nowbest;
	nowbest->server_num = DES_NODE_SIZE;
	for(int i = 0 ; i < DES_NODE_SIZE ; i++)
	{
		nowbest->servertable[i] = DES_NODElist[i]->node_num;
		NODE_TO_SERtable[DES_NODElist[i]->node_num] = i;
	}
	nowbest->servertable[DES_NODE_SIZE] = NOTSET;

	/*记录产生解决方案次数*/
	int solve = 0;
	int solve_pointer = 0;
	/*初始化解决方案数组集*/
	PSOLUTION * solutionset = (PSOLUTION*)malloc(sizeof(PSOLUTION) * SOLU_SIZE);
	while(now != NULL && now->degree < MAX_NODE_DEGREE && (int)((end - start)/CLK_TCK) < 88 )
	{
		/*给定服务器的数量和位置，将边集的残余流量重置,加入超级源点的边集,将超级汇点的边集的残余流量重置*/
		PPATH * to_DES = initDESPATH(DES_NODE_SIZE , DES_NODElist);
		PPATH * to_SOU = initSOUPATH(server_num);
		server_position = now->servertable;
		server_num = now->server_num;
//		for(int i = 0 ; i < server_num ; i++);
//			printf("%d |",server_position[i]);
//		printf("\n");
		/*根据当前解决方案产生后续解决方案*/
		if(solve - solve_pointer < SOLU_BUFFER)
		{
			for(int i = 0 ; i < NODE_SIZE ; i++)
			{
				if(NODElist[i]->try_attri == 0)
				{
					NODElist[i]->try_attri = NODElist[i]->edge_num * TRY_ATTRI;
				}
			}
		}
		solu_generater(NODE_SIZE , now , server_position , NODElist,
				&solve , EDGE_SIZE , solutionset);
//		printf("passs\n");
		int thecost = server_num * SERVER_PRICE;
		reset(NODElist , NODE_SIZE , server_num , server_position , source);
		/*给定服务器完成最小费用流*/
		if(remain_traffic(DES_NODE_SIZE , NODE_SIZE , server_num , demand , &thecost,server_position ,
				source , to_DES , to_SOU, DES_NODElist , NODElist , NODE_TO_DEStable , NODE_TO_SERtable)
				&& thecost < cost)
		{
			cost = thecost;
			nowbest = now;
			now->pathlist = to_DES;
//			printf("%d\n", thecost);
//			if(cost == 2042)
//			{
//				break;
//			}
			/*之前指定服务器时的数据清理，路径保留*/
			clean(source ,server_num , server_position);
			nowbest = now;
		}
		else
		{
			/*之前指定服务器时的数据清理,含无用路径清理*/
			clean(source, server_num , server_position ,DES_NODE_SIZE , to_DES);
			free(now->servertable);
			free(now);
		}
		solutionset[solve_pointer%SOLU_SIZE] = NULL;
		/*指定下一个解决方案*/
		now = solutionset[(++solve_pointer)%SOLU_SIZE];
		end = clock();
//		printf("%d %d |" , solve_pointer ,solve);
	}
	// 需要输出的内容
	printf("%d %d %d\n%d\n", solve_pointer , solve , SOLU_SIZE, cost);
	for(int i = 0 ; i < NODE_SIZE ; i++)
	{
		printf("%d %d|",NODElist[i]->try_attri ,NODElist[i]->edge_num);
	}
	PPATH *to_DES = nowbest->pathlist;
	char * topo_file = getresult(to_DES , DES_NODE_SIZE);
	end = clock();
	printf("time=%f\n",(double)(end - start)/1000000);
//	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);
}
