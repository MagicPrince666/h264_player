#ifndef CLIST_H
#define CLIST_H

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <iostream>

typedef  int ElemType;

typedef struct Node
{
    ElemType data;
    struct Node *next;
}Node, *PNode;

typedef struct List
{
    PNode head;
    PNode tail;
    int size;
}List,*PList;

bool InitList(PList list);
void Create_t(PList list,ElemType x);             //尾插
void Create_h(PList list,ElemType x);             //头插
void del_back(PList list);                        //尾删
void del_front(PList list);                       //头删
void sortList(PList list);                        //排序
void insert_val(PList list,ElemType x);           //按值插
PNode find(PList list,ElemType x);
void del_val(PList list,ElemType x);
void modify(PList list,ElemType x1,ElemType x2);
void clear(PList list);
void destroy(PList list);
void reserve(PList list);
int length(PList list);
void menu();
void showList(PList list);
void show_tail(PList list);
ElemType next(PList list,ElemType x);
ElemType prio(PList list,ElemType x);
PNode prev(PList list,PNode p);

#endif