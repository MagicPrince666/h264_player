#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ker_list.h"

struct node
{
	char * name;
	struct list_head list;
};

void init_list(struct list_head **p)
{
	*p = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(*p);
}

void insert(char* str, struct list_head *head)
{
	struct node  *_new;
    int len = strlen(str);
	_new = (struct node *)malloc(sizeof(struct node));
	_new->name = (char *)malloc(len);
    memcpy(_new->name,str,len);
	list_add_tail(&_new->list, head);
}

void show(struct list_head *head)
{
	struct list_head *pos;
	struct node *tmp;
	list_for_each(pos, head)
	{
		tmp = list_entry(pos, struct node, list);
		printf("%s ",tmp->name);
	}
}

void _delete(struct list_head *head,struct list_head *nod)
{
    struct node  *_del;
    struct list_head *pos;
	for (pos = (head)->prev; pos != (head); pos = pos->prev)
	{
		if(pos == nod)break;
	}
	_del = list_entry(pos, struct node, list);
	free(_del->name);
    list_del(pos);
	// list_for_each(pos, head)
	// {
	// 	_del = list_entry(pos, struct node, list);
	// 	free(_del->name);
    //     list_del(pos);
	// }
}

int main(void)
{
	int i,n;
	struct list_head *head;
    char str[100];
	
	init_list(&head);
	printf("input a int number\n");
	scanf("%d",&n);
	for (i = 1; i <= n; i++)
	{
        sprintf(str,"name%d",i);
		insert(str, head);	
	}
	show(head);
	printf("\n");	

    _delete(head,head);
	printf("\n");

    show(head);
    printf("\n");
	return 0;
}