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
	if (NULL == h)
		return NULL;
	h->min = NULL;
	h->n = 0;
	return h;
}
/*插入堆节点*/
void heap_insert(PHEAP h, PTREE x)
{
	x->degree = 0;
	x->parent = NULL;
	x->child = NULL;
	x->mark = FALSE;
	if (NULL == h->min)
	{
		x->left = x;
		x->right = x;
		h->min = x;
	}
	else
	{
		PTREE tmp = h->min->left;
		tmp->right = x;
		x->left = tmp;
		x->right = h->min;
		h->min->left = x;
		if (x->key < h->min->key)
			h->min = x;
	}
	h->n++;
}
/*合并两个根节点至x节点*/
void heap_link(PTREE y, PTREE x)
{
	y->left->right = y->right;
	y->right->left = y->left;
	PTREE child = x->child;
	if (NULL == child)
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
	for (int i = 0; i < MAX_DEGREE; i++)
		a[i] = NULL;
	PTREE x = h->min;
	PTREE last_tag = h->min->right;
	PTREE last = NULL;
	int degree = 0;
	while (last != last_tag)
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
			heap_link(y, x);
			a[degree] = NULL;
			degree++;
		}
		a[degree] = x;
		x = next;
	}
	h->min = NULL;
	for (int i = 0; i <MAX_DEGREE; i++)
	{
		if (NULL != a[i])
		{
			if (NULL == h->min)
				h->min = a[i];
			else
				if (h->min->key > a[i]->key)
					h->min = a[i];
		}
	}
}

/*抽取最小节点*/
PTREE heap_extract_min(PHEAP h)		//此函数输出为结构体
{
	PTREE z = h->min;
	h->n--;
	if (h->n > 0)
	{
		PTREE child = z->child;
		if (NULL != child)
		{
			/*该节点具有子节点*/
			PTREE x = child;
			do
			{
				x->parent = NULL;
				x = x->left;
			} while (x != child);
			if (z != z->right)
			{
				/*具有多个根节点*/
				child = x->right;
				z->left->right = child;
				child->left = z->left;
				z->right->left = x;
				x->right = z->right;
			}
			/*只有一个根节点h->min*/
			/*子节点的表不需要改变*/
			h->min = child;
		}
		else
		{
			/*h->min没有子节点*/
			if (z == z->right)
				/*只有一个根节点h->min*/
				h->min = NULL;
			else
			{
				/*多个根节点*/
				z->left->right = z->right;
				z->right->left = z->left;
				h->min = z->right;
			}
		}
		if(NULL != h->min)
			consolidate(h);
	}
	return z;
}
/*切断父节点y和字节点x,并将x加入到根节点集中*/
void cut(PHEAP h, PTREE x, PTREE y)
{
	if (x != x->left)		//判断是否只有一个子节点x
	{
		if (x == y->child)
		{
			/*只有含有多个子节点，且字节点指向x*/
			y->child = x->right;		//修改子节点指向
		}
		x->left->right = x->right;	//将x从子节点链中删除
		x->right->left = x->left;
		int degree = 0;
		/*得到除了x节点外，y的子节点中的最大度数*/
		PTREE tmp = y->child;
		do
		{
			if (degree < tmp->degree)
				degree = tmp->degree;
			tmp = tmp->left;
		} while (tmp != y->child);
		if (x->degree > degree)
			y->degree = degree + 1;
	}
	else
	{
		/*只有一个子节点x*/
		y->child = NULL;
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
	if (NULL != z)
	{
		if (y->mark == FALSE)
			y->mark = TRUE;
		else
		{
			cut(h, y, z);
			cascading_cut(h, z);
		}
	}
}
/*关键字减值*/
void heap_decrease(PHEAP h, PTREE x, int k)
{
	if (k > x->key)
		return;
	x->key = k;
	PTREE y = x->parent;
	if (NULL != y && x->key < y->key)
	{
		cut(h, x, y);
		cascading_cut(h, y);
	}
	if (x->key < h->min->key)
		h->min = x;
}

void heap_insert_max(PHEAP h, PTREE x)
{
	x->degree = 0;
	x->parent = NULL;
	x->child = NULL;
	x->mark = FALSE;
	if (NULL == h->min)
	{
		x->left = x;
		x->right = x;
		h->min = x;
	}
	else
	{
		PTREE tmp = h->min->left;
		tmp->right = x;
		x->left = tmp;
		x->right = h->min;
		h->min->left = x;
		if (x->key > h->min->key)
			h->min = x;
	}
	h->n++;
}
void heap_link_max(PTREE y, PTREE x)
{
	y->left->right = y->right;
	y->right->left = y->left;
	PTREE child = x->child;
	if (NULL == child)
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
void consolidate_max(PHEAP h)
{
	PTREE a[MAX_DEGREE];
	for (int i = 0; i < MAX_DEGREE; i++)
		a[i] = NULL;
	PTREE x = h->min;
	PTREE last_tag = h->min->right;
	PTREE last = NULL;
	int degree = 0;
	while (last != last_tag)
	{
		PTREE next = x->left;
		last = x;
		degree = x->degree;
		while (NULL != a[degree])
		{
			PTREE y = a[degree];
			if (x->key < y->key)
			{
				PTREE tmp = x;
				x = y;
				y = tmp;
			}
			heap_link_max(y, x);
			a[degree] = NULL;
			degree++;
		}
		a[degree] = x;
		x = next;
	}
	h->min = NULL;
	for (int i = 0; i <MAX_DEGREE; i++)
	{
		if (NULL != a[i])
		{
			if (NULL == h->min)
				h->min = a[i];
			else
				if (h->min->key < a[i]->key)
					h->min = a[i];
		}
	}
}
PTREE heap_extract_max(PHEAP h)		//此函数输出为结构体
{
	PTREE z = h->min;
	h->n--;
	if (h->n > 0)
	{
		PTREE child = z->child;
		if (NULL != child)
		{
			/*该节点具有子节点*/
			PTREE x = child;
			do
			{
				x->parent = NULL;
				x = x->left;
			} while (x != child);
			if (z != z->right)
			{
				/*具有多个根节点*/
				child = x->right;
				z->left->right = child;
				child->left = z->left;
				z->right->left = x;
				x->right = z->right;
			}
			/*只有一个根节点h->min*/
			/*子节点的表不需要改变*/
			h->min = child;
		}
		else
		{
			/*h->min没有子节点*/
			if (z == z->right)
				/*只有一个根节点h->min*/
				h->min = NULL;
			else
			{
				/*多个根节点*/
				z->left->right = z->right;
				z->right->left = z->left;
				h->min = z->right;
			}
		}
		if(NULL != h->min)
			consolidate_max(h);
	}
	return z;
}
void cut_max(PHEAP h, PTREE x, PTREE y)
{
	if (x != x->left)		//判断是否只有一个子节点x
	{
		if (x == y->child)
		{
			/*只有含有多个子节点，且字节点指向x*/
			y->child = x->right;		//修改子节点指向
		}
		x->left->right = x->right;	//将x从子节点链中删除
		x->right->left = x->left;
		int degree = 0;
		/*得到除了x节点外，y的子节点中的最大度数*/
		PTREE tmp = y->child;
		do
		{
			if (degree < tmp->degree)
				degree = tmp->degree;
			tmp = tmp->left;
		} while (tmp != y->child);
		if (x->degree > degree)
			y->degree = degree + 1;
	}
	else
	{
		/*只有一个子节点x*/
		y->child = NULL;
		y->degree = 0;
	}
	x->parent = NULL;
	x->left = h->min->left;
	h->min->left->right = x;
	x->right = h->min;
	h->min->left = x;
}
void cascading_cut_max(PHEAP h, PTREE y)
{
	PTREE z = y->parent;
	if (NULL != z)
	{
		if (y->mark == FALSE)
			y->mark = TRUE;
		else
		{
			cut_max(h, y, z);
			cascading_cut_max(h, z);
		}
	}
}
void heap_increase(PHEAP h, PTREE x, int k)
{
	if (k < x->key)
		return;
	x->key = k;
	PTREE y = x->parent;
	if (NULL != y && x->key > y->key)
	{
		cut_max(h, x, y);
		cascading_cut_max(h, y);
	}
	if (x->key > h->min->key)
		h->min = x;
}
