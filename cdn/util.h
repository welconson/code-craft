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
#include "fibonacci.h"

#define EMPTY 0
#define FALSE 0
#define TRUE 1
#define MAX_PATH 11
#define PATH_END -1
#define MAX_LINE_LEN 55000
#define MAX_BIT		5
#define UNREACH		1000000

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

struct node
{
	int node_num;
	PEDGE edgelist;
};

struct des_node
{
	int node_num;
	int demand;
	int traffic;
};

struct edge
{
	int node_num;
	int capacity;
	int traffic;
	int unit_price;
	PEDGE nextP;
};

struct path
{
	int *path;
	int traffic;
	int cost;
	PPATH nextp;
};

/*关于NODE的函数*/
PNODE initNODE();
/*关于EDGE的函数*/
PEDGE initEDGE(int num , int capacity , int unit_price);
/*关于DES_NOD的函数*/
PDES_NODE initDES_NODE(int node_num , int demand);
/*关于PATH的函数*/
PPATH initPATH();
/*初始化通往每个消费点的路径*/
PPATH * initDESPATH(int DES_NODE_NUM , int SERVER_PRICE);
/*将结果格式化成字符串*/
char * getresult(PPATH * to_DES , int DES_NODE_SIZE);
/*该函数用来处理将内存中的数据存储为设计好的结构体*/
int data_handle(char * topo[], PNODE **NODElist ,
		PDES_NODE **DES_NODElist , int *NODE_SIZE ,
		int *DES_NODE_SIZE , int *SERVER_PRICE);

void dijkstra(PNODE * NODElist , int NODE_SIZE , int source);


#endif /* CDN_UTIL_H_ */
