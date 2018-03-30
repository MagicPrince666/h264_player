#include"clist.h"

void menu()           //提供选项的菜单函数
{
    printf("***************************************************************\n");
    printf("* [0] quit_system [1] Create_t   [2] Create_h    [3] showList *\n");
    printf("* [4] del_back    [5] del_front  [6] insert_val  [7] show_tail*\n");
    printf("* [8] find        [9] del_val    [10] sortList   [11] modify  *\n");
    printf("* [12]clear       [13]destroy    [14] resver     [15]length   *\n");
    printf("* [16] next       [17]prio                                    *\n");
    printf("***************************************************************\n");
}

bool InitList(PList list)
{
    list->head = (PNode)malloc(sizeof(Node)); //初始化一个头结点
    assert(list->head != NULL);               //断言，表达式为真，接着往下执行

    list->head->next = list->head;            //初始化head和tail指针，使其都指向头节点
    list->tail = list->head;

    list->size = 0;                           //长度初始化为0
    return true;
}

void Create_t(PList list,ElemType x)     //尾插法
{
    PNode s = (PNode)malloc(sizeof(Node));
    assert(s != NULL);
    s->data = x;                         //填充数据域
    list->tail->next = s;                //tail指向最后一个节点，把新建立的节点链接到链表的最后
    s->next = list->head;                //单循环链表，新节点的next指向头结点           
    list->tail = s;                      //改变尾指针的指向
    list->size++;
} 

void showList(PList list)            //链表显示函数
{
    if(1>list->size)
    {
        printf("--链表为空\n");
        return ;
    }
    PNode p = list->head->next;
    while(list->head != p)           //逐个遍历链表
    {
        printf("%d ",p->data);
        p=p->next;
    }
    printf("\n");
}

void Create_h(PList list,ElemType x)  //头插法
{
    PNode s = (PNode)malloc(sizeof(Node));
    assert(s != NULL);
    s->data = x;                             //填充数据域
    s->next = list->head->next;              //新节点指向第一个节点           
    list->head->next = s;                    //头节点的next指向新节点s
    if(list->size == 0)                      //如果是第一次头插，需改变尾指针和尾节点的next指向，之后都不改变
    {
        list->tail = s;
        list->tail->next = list->head;        
    }
    list->size++;        //插入一个，长度加1
}

void del_back(PList list)    //尾删
{
    if(0==list->size)
        return;
    PNode p = list->head;
    while(list->head != p->next->next)      //找到倒数第二个节点
    {
        p = p->next;
    }                      
    p->next = list->head;                   //把最后一个节点分离
    free(list->tail);                       //释放最后一个节点
    list->tail = p;                         //尾指针指向原来的倒数第二个节点（现在倒数第一）
    printf("--尾节点已删除\n");
    list->size--;
}

void del_front(PList list)   //头删
{
    if(0==list->size)
        return;
    else
    {
        PNode p = list->head->next;
        if(1==list->size) 
        {                                    //只有一个节点，若删去，需改变尾指针
            list->tail = list->head;
            list->head->next = list->head;
        }
        else
        {                
            list->head->next = p->next;      //头指针指向第二个节点    
        }
        free(p);                             //释放第一个节点
    }                            
    printf("--头节点已删除\n");
    list->size--;
}


void show_tail(PList list)     //为测试尾指针是否正确改变，需显示最后一个节点
{
    printf("--链表的尾节点是：》%d \n",list->tail->data);
}

void sortList(PList list)      // 对无序链表进行从小到大排序
{
    if(2>list->size)
        return ;
    PNode p = list->head->next;
    PNode q = p->next;
    for(int i = 0;i<list->size-1;i++,p = list->head->next,q = p->next)      //n个数比较n-1趟
    {
        for(int j = 0;j<list->size-1-i;j++,p=q,q=q->next)   //第i趟比较n-i次
        {
            if(p->data > q->data)        //如果前面的数大于后面，则交换
            {
                p->data = p->data + q->data;
                q->data = p->data - q->data;
                p->data = p->data - q->data;
            }
        }
    }
}

void insert_val(PList list,ElemType x)   //链表有序的前提下，给一个值插入
{
    PNode p = list->head->next,q = list->head;
    while(list->head != p)               //找到能插入的位置，会在p、q之间
    {   
        if(x<p->data)
            break;
        q = p;
        p = p->next;
    }
    PNode s = (PNode)malloc(sizeof(Node)); //初始化新节点
    s->data = x;
    q->next = s;              //把新节点插入到链表中（在p,q之间插入）
    s->next = p;
    if(list->head == p)       //如果新节点的值最大，即尾插，需改变尾指针和尾节点的next指向
    {
        list->tail = s;
        list->tail->next=list->head;
    }
    list->size++;
}

PNode find(PList list,ElemType x)       //返回要查找元素的前面一个的地址
{
    PNode p = list->head;
    while(list->tail != p && list->head != p->next && x != p->next->data)
    {
        p = p->next;                   //循环结束，p指向x的前面一个元素
    }                      
    if(list->head == p->next)          //如果p指向最后一个元素，说明没有找到
    {
        printf("--没找到！\n");
        return NULL;
    }
    return p;
}

void del_val(PList list,ElemType x)     //删除指定的值x
{
    if(0 == list->size)
        return ;
    PNode p = find(list,x);
    PNode q = NULL;
    if(NULL != p)
    {
        q = p->next;               //q指向要删除的节点  
        if(q == list->tail)        //若删除最后一个节点，需改变尾指针
        {
            p->next = list->head;
            list->tail = p;
        }
        else
        {
            p->next = q->next;
        }
        free(q);                  //释放要删除的节点
        list->size--;
        printf("--%d已删除!\n",x);
    }
    return ;
}

void modify(PList list,ElemType x1,ElemType x2)    //把原有的x1修改成x2
{
    PNode p = find(list,x1);
    if(NULL != p)
        p->next->data = x2;
    else
        return ;
}

void clear(PList list)       //删除链表的所有节点，但不删除头结点
{
    PNode p = list->head->next;
    PNode q = p;
    while(list->head != p)      
    {
        p = p->next;         //p依次后移，跟屁虫q依次释放节点
        free(q);
        q = p;
    }
    list->tail = list->head;   //修改尾指针和链表长度
    list->head->next = list->head;
    list->size = 0;
    printf("--链表已被清空！\n");
}

void destroy(PList list)   //摧毁链表，包括所有节点和头结点
{
    clear(list);
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
    printf("--链表已被摧毁！\n");
}

PNode prev(PList list,PNode p)     //返回p指向的前面一个节点
{
    if(p != list->head)
    {
        PNode q = list->head->next;
        while(q != list->head && q->next != p)  //依次往后移，知道尾指针的前面一个节点
            q=q->next;
        if(q->next == p)
            return q;
    }
    return NULL;
}

void reserve(PList list)   //逆置链表 
{
    PNode s = (PNode)malloc(sizeof(Node));       //建立一个节点
    s->next = list->tail;
    PNode p = list->tail;
    while(list->tail != list->head->next)        //把原链表的尾节点到第一个节点依次连接到新节点上
    {
        list->tail = prev(list,list->tail);
        list->tail->next = list->head;
        p->next = list->tail;
        p=p->next;
    }
    p->next = s;              //p指向第一个节点，即新链表的最后一个节点，尾指针的next指向头结点s，链表结束
    free(list->head);         //释放原来的头结点
    list->head = s;           //把s变成新的头指针
}

int length(PList list)     //求链表的长度
{
    return list->size;
}

ElemType next(PList list,ElemType x)      //返回x的后继
{
    PNode p = find(list,x);  
    if(NULL == p) 
        return -1;
    if(p->next == list->tail)           //因为是单循环链表，尾节点的下一个元素是第一个节点
    {
        return list->head->next->data;    
    }
    p=p->next;
    return p->next->data;
}

ElemType prio(PList list,ElemType x)         //返回x的前驱
{
    PNode p = find(list,x);
    if(NULL != p)
    {
        if(p == list->head || p == list->tail)
        {
            return list->tail->data;
        }
        return p->data;
    }
    return -1;
}