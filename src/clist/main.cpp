#include "clist.h"

int main()
{
    List mylist;
    InitList(&mylist);
    ElemType item = 0;
    int pos = 0;
    int chose = 1;
    PNode p = NULL;
    while(chose)
    {
        menu();
        printf("给出想要操作的序号：\n");
        scanf("%d",&chose);
        switch(chose)
        {
        case 0:
            destroy(&mylist);
            chose = 0;
            break;
        case 1:
            printf("输入要尾插的数据[-1结束]：\n");
            while(scanf("%d",&item),item!=-1)
            {
                Create_t(&mylist,item);
            }
            break;
        case 2:
            printf("输入要头插的数据：\n");
            while(scanf("%d",&item),item!=-1)
            {
                Create_h(&mylist,item);
            }
            break;
        case 3:
            showList(&mylist);
            break;
        case 4:
            del_back(&mylist);
            break;
        case 5:
            del_front(&mylist);
            break;
        case 6:
            printf("给出要插入的数：\n");
            scanf("%d",&item);
            insert_val(&mylist,item);
            break;
        case 7:
            show_tail(&mylist);
            break;
        case 8:
            printf("输入要查找的数：\n");
            scanf("%d",&item);
            p = find(&mylist,item);
            if(NULL!=p)
                printf("%d\n",p->next->data);
            break;
        case 9:
            printf("输入要删除的数：\n");
            scanf("%d",&item);
            del_val(&mylist,item);
            break;
        case 10:
            sortList(&mylist);
            break;
        case 11:
            printf("输入要修改的数和修改后的数\n");
            scanf("%d %d",&item,&pos);
            modify(&mylist,item,pos);
            break;
        case 12:
            clear(&mylist);
            break;            
        case 13:
            destroy(&mylist);
            break;
        case 14:
            reserve(&mylist);
            break;
        case 15:
            printf("链表长度为：%d\n",length(&mylist));
            break;
        case 16:
            printf("输入想要找哪个一数的后继：\n");
            scanf("%d",&item);
            printf("%d 的后继是：%d\n",item,next(&mylist,item));
            break;
        case 17:
            printf("输入想要找哪个一数的前驱：\n");
            scanf("%d",&item);
            printf("%d 的前驱是：%d\n",item,prio(&mylist,item));
            break;
        default:
            printf("重新输入\n");
            break;
        }   
    }
    return 0;
}