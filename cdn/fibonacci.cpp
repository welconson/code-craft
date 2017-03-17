/*
 * fibonacci.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: welcome
 */

#include "fibonacci.h"

PHEAP get_heap()
{
	PHEAP h = (PHEAP)malloc(sizeof(HEAP));
	if(NULL == h)
		return NULL;
	h->min = NULL;
	h->n = 0;
	return h;
}
/*插入堆节点*/
void heap_insert(PHEAP h , PTREE x)
{
	x->degree = 0;
	x->parent = NULL;
	x->child = NULL;
	x->mark = FALSE;
	if( NULL == h->min )
	{
		x->left = x;
		x->right = x;
		h->min = x;
	}
	else
	{
		PTREE tmp =h->min->left;
		tmp->right = x;
		x->left = tmp;
		x->right = h->min;
		h->min->left = x;
		if(x->key < h->min->key)
			h->min = x;
	}
	h->n++;
}
/*合并两个根节点至x节点*/
void heap_link(PHEAP h , PTREE y , PTREE x)
{
	y->left->right = y->right;
	y->right->left = y->left;
	PTREE child = x->child;
	if(NULL == child)
	{
		x->child = y;
		y->left = y;
		y->right = y;
	}
	else
	{
		child->left->right = y;
		y->left = child->left;
		child->left = y;
		y->right = child;
	}
	x->degree++;
	y->parent = x;
	y->mark = FALSE;
}
/*将堆的根节点合并为互不相同的度的根节点*/
void consolidate(PHEAP h)
{
	PTREE a[MAX_DEGREE];
	for(int i = 0 ; i < MAX_DEGREE ; i++)
		a[i] = NULL;
	PTREE x = h->min;
	PTREE last_tag = h->min->right;
	PTREE last = NULL;
	int degree = 0;
	while(last != last_tag)
	{
		PTREE next = x->left;
		last = x;
		degree = x->degree;
		while (NULL != a[degree])
		{
			PTREE y = a[degree];
			if (x->key > y->key)
			{
				PTREE tmp = x;
				x = y;
				y = tmp;
			}
			heap_link(h, y, x);
			a[degree] = NULL;
			degree++;
		}
		a[degree] = x;
		x = next;
	}
	h->min = NULL;
	for( int i = 0 ; i <MAX_DEGREE ; i++)
	{
		if(NULL != a[i])
		{
			if(NULL == h->min)
				h->min = a[i];
			else
				if(h->min->key > a[i]->key)
					h->min = a[i];
		}
	}
}

/*抽取最小节点*/
TREE heap_extract_min(PHEAP h)		//此函数输出为结构体
{
	TREE z = {};
	h->n--;
	if (NULL != h->min)
	{
		z.num = h->min->num;
		z.key = h->min->key;
		z.child = h->min->child;
		z.left = h->min->left;
		z.right = h->min->right;
		PTREE child = h->min->child;
		if (NULL != child)
		{
			PTREE x = child;
			do
			{
				x->parent = NULL;
				x = x->left;
			} while (x != child);
			if (h->min != z.right)
			{
			child = x->right;
			z.left->right = child;
			child->left = z.left;
			z.right->left = x;
			x->right = z.right;
			}
			else
			{
				z.left = x;
				z.right = x->right;
			}
		}
		else
		{
			z.left->right = z.right;
			z.right->left = z.left;
		}
		free(h->min);
		if (z.right == h->min)
			h->min = NULL;
		else
		{
			h->min = z.right;
			consolidate(h);
		}
	}
	return z;
}
/*切断父节点y和字节点x,并将x加入到根节点集中*/
void cut(PHEAP h, PTREE x, PTREE y)
{
	if(x->right != x->left)		//判断是否只有一个子节点x
	{
		if(x == y->child)
		{
			/*只有含有多个字节点，且字节点指向x*/
			y->child = x->right;		//修改字节点指向
		}
		x->left->right = x->right;	//将x从字节点链中删除
		x->right->left = x->left;
		int degree = 0;
		PTREE tmp = y->child;
		do
		{
			if(degree < tmp->degree)
				degree = tmp->degree;
			tmp = tmp->left;
		}while(tmp != y->child);
		if(degree > x->degree)
			return;
		y->degree = degree + 1;
	}
	else
	{
		/*只有一个子节点x*/
		y->child =NULL;
		y->degree = 0;
	}
	x->parent = NULL;
	x->left = h->min->left;
	h->min->left->right = x;
	x->right = h->min;
	h->min->left = x;
}
/*级联切断*/
void cascading_cut(PHEAP h, PTREE y)
{
	PTREE z = y->parent;
	if(NULL != z)
		if(y->mark == FALSE)
			y->mark =TRUE;
		else
		{
			cut(h , y ,z);
			cascading_cut(h , z);
		}
}
/*关键字减值*/
void heap_decrease(PHEAP h , PTREE x, int k)
{
	if(k > h->min->key)
		return;
	x->key = k;
	PTREE y = x->parent;
	if(NULL != y)
	{
		cut(h , x ,y);
		cascading_cut(h , y);
	}
	if(x->key < h->min->key)
		h->min = x;
}
