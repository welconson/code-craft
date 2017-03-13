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

#define EMPTY 0
#define FALSE 0
#define TRUE 1

struct node;		//节点的结构体
struct des_node;	//消费节点的结构体
struct edge;		//边的结构体
typedef struct node NODE;
typedef struct des_node DES_NODE;
typedef struct edge EDGE;
typedef struct node * PNODE;
typedef struct des_node * PDES_NODE;
typedef struct edge * PEDGE;

/*关于NODE的函数*/
PNODE initNODE();
/*关于EDGE的函数*/
PEDGE initEDGE(int num , int capacity , int unit_price);
/*关于DES_NOD的函数*/
PDES_NODE initDES_NODE(int node_num , int demand);

/*该函数用来处理将内存中的数据存储为设计好的结构体*/
int data_handle(char * topo[], PNODE NODElist[] ,
		PDES_NODE DES_NODElist[] , int *NODE_SIZE ,
		int *DES_NODE_SIZE , int *SERVER_PRICE);




#endif /* CDN_UTIL_H_ */
