#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"
#include "util.h"
#include "fibonacci.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NODE_SIZE 1002
#define MAX_DES_SIZE	500
#define MAX_DEMAND	5000
#define UNLINKED	-1
#define EMPTY 0
#define FALSE 0
#define TRUE 1
#define MAX_PATH 30
#define PATH_END -1
#define MAX_LINE_LEN 55000
#define MAX_BIT		8
#define UNREACH		1<<20
#define NOTSET		-1
#define INFINITY_CAPACITY	1<<30
#define NO_DISTANCE	0
#define DESTINATION NODE_SIZE + 1
#define SOURCE		NODE_SIZE
#define SOLU_SIZE	((EDGE_SIZE * NODE_SIZE)<<1)
#define TRY_ATTRI	10
#define SOLU_BUFFER	(EDGE_SIZE * NODE_SIZE)
#define MAX_NODE_DEGREE	10
#define CLK_TCK		1000000
#define USABLE		1
#define UNUSABLE	0

void deploy_server(char * graph[MAX_EDGE_NUM], int edge_num, char * filename);

#endif
