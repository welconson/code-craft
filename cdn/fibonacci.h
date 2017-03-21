/*
 * fibonacci.h
 *
 *  Created on: Mar 17, 2017
 *      Author: welcome
 */

#ifndef CDN_FIBONACCI_H_
#define CDN_FIBONACCI_H_
#include <stdio.h>
#include <stdlib.h>
#include "util.h"


typedef struct heap HEAP;
typedef struct heap * PHEAP;
typedef struct tree TREE;
typedef struct tree * PTREE;
#define MAX_DEGREE 10
struct heap
{
	int n;
	PTREE min;
};

struct tree
{
	int key;
	int degree;
	int mark;
	int num;
	PTREE child;
	PTREE parent;
	PTREE left;
	PTREE right;
};

PHEAP get_heap();
void heap_insert(PHEAP h , PTREE x);
void heap_link(PHEAP h , PTREE y , PTREE x);
void consolidate(PHEAP h);
PTREE heap_extract_min(PHEAP h);
void cut(PHEAP h, PTREE x, PTREE y);
void cascading_cut(PHEAP h, PTREE y);
void heap_decrease(PHEAP h , PTREE x, int k);

#endif /* CDN_FIBONACCI_H_ */
