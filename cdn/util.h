/*
 * util.h
 *
 *  Created on: Mar 11, 2017
 *      Author: welcome
 */

#ifndef CDN_UTIL_H_
#define CDN_UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deploy.h"
#include "fibonacci.h"



struct node;		//节点的结构体
struct des_node;	//消费节点的结构体
struct edge;		//边的结构体
typedef struct node NODE;
typedef struct des_node DES_NODE;
typedef struct edge EDGE;
typedef struct path PATH;
typedef struct node * PNODE;
typedef struct des_node * PDES_NODE;
typedef struct edge * PEDGE;
typedef struct path * PPATH;
typedef struct solution SOLUTION;
typedef struct solution * PSOLUTION;
/*网络节点*/
struct node
{
	int node_num;
	int edge_num;
	int try_attri;
	PEDGE edgelist;
};
/*消费节点*/
struct des_node
{
	int node_num;
	int demand;
	int traffic;
	int cost;
	PEDGE edge;
};
/*边*/
struct edge
{
	int node_num;
	int capacity;
	int traffic;
	int unit_price;
	int remain;
	PEDGE reverse;
	PEDGE nextP;
};
/*路径*/
struct path
{
	int *path;
	int traffic;
	PPATH nextp;
};
/*解决方案*/
struct solution
{
	int server_num;
	int *servertable;
	int degree;
	int frontnode;
	PPATH *pathlist;
};
extern PNODE* NODElist;					//网络节点表
extern PDES_NODE* DES_NODElist;			//消费节点表
extern int NODE_SIZE;					//网络节点数量
extern int DES_NODE_SIZE;				//消费节点数量
extern int *server_position;			//对应序号的服务器的网络节点号
extern int SERVER_PRICE;				//服务器成本
extern int EDGE_SIZE;					//边集数量
extern int *alloc_table;	//对应消费节点挂载的服务器的网络节点号
extern int *NODE_TO_DEStable;	//直连网络节点到对应消费节点的映射
extern int *distance_between_des[];//直连网络节点间的最小距离
extern PNODE destination;				//超级汇点
extern PNODE source;					//超级源点
extern int demand;						//总费用
extern int bestcost;					//最小费用
/*关于NODE的函数*/
PNODE initNODE();
/*关于EDGE的函数*/
PEDGE initEDGE(int num , int capacity , int unit_price);
/*关于DES_NOD的函数*/
PDES_NODE initDES_NODE(int node_num , int demand);
/*关于PATH的函数*/
PPATH initPATH(int start);
/*初始化通往每个消费点的路径*/
PPATH * initDESPATH(int DES_NODE_SIZE ,PDES_NODE *DES_NODElist);
/*初始化每个服务节点的路径指针*/
PPATH * initSOUPATH(int server_num);
/*初始化解决方案*/
PSOLUTION initSOLUTION(int degree , int frontnode);
/*将结果格式化成字符串*/
char * getresult(PPATH * to_DES , int DES_NODE_SIZE);
/*该函数用来处理将内存中的数据存储为设计好的结构体*/
int data_handle(char * topo[], PNODE **NODElist ,
		PDES_NODE **DES_NODElist , int *NODE_SIZE ,int *EDGE_SIZE,
		int *DES_NODE_SIZE ,  int *SERVER_PRICE);

void dijkstra(int *front, int *traffic);
/*适用于探索合并消费节点的寻路*/
void dijkstra_merge(int source,int *distance ,int *front);
/*完成增流链的补充*/
int remain_traffic(int server_num , int demand , int bestcost,int *cost, int * server_position ,
		PNODE source ,PPATH * to_DES);
/*初始化工作*/
void initlize(PNODE *NODElist , PDES_NODE *DES_NODElist , int DES_NODE_SIZE ,int NODE_SIZE,
		PNODE destination , int *NODE_TO_DEStable , int *demand);
/*给定服务器的数量和位置,将边集的残余流量重置,加入超级源点的边集 , 将超级汇点的边集的残余流量重置*/
void reset(PNODE *NODElist, int NODE_SIZE, int server_num , int *server_position , PNODE source);
/*之前指定服务器时的数据清理，路径保留*/
void clean(PNODE source);
/*之前指定服务器时的数据清理,含无用路径清理*/
void clean(PNODE source, int DES_NODE_SIZE , PPATH *to_DES);
/*解决方案产生器*/
void solu_generater(int NODE_SIZE, PSOLUTION now, int *server_position, PNODE *NODElist,
		int *solve, int EDGE_SIZE , PSOLUTION *solutionset);
/*管理消费节点的合并*/
void merge(PSOLUTION so);
#endif /* CDN_UTIL_H_ */
