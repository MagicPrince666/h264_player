#include "queue.h" 
#include <string.h>  

#define q_debug 0

//初始化队列  
void Queue::Init_Queue(circularQueue *q)  
{  
    q->base = new ElemType[ MAXSIZE * sizeof(ElemType) ];  
    if (!q->base) exit(0);  
    q->front = q->rear = 0;  
}  

// Queue::~Init_Queue(circularQueue *q)
// {
//     delete[] q->base;
// }

//入队列操作  
uint32_t Queue::Insert_Queue(circularQueue *q, ElemType* e,uint32_t len)  
{  
    if ((q->rear+1)%MAXSIZE == q->front) 
    {
    #if q_debug
        printf("the queue is full\n");
    #endif
        return 0; //队列已满时，不执行入队操作 
    }

    if(q->rear + len <= MAXSIZE)
    { 
        memcpy(q->base + q->rear,e,len);//将元素放入队列尾部  
        q->rear += len;
    #if q_debug
        printf("queue rear:%u\n",q->rear);
    #endif
    }
    else if((q->rear + len)%MAXSIZE < q->front)
    {
        q->size = MAXSIZE - q->rear;
        memcpy(q->base + q->rear,e,q->size);//将元素放入队列尾部
        memcpy(q->base,e + q->size,len - q->size);
        q->rear = len - q->size;
    }
    else 
    {
    #if q_debug
        printf("not enough buffer\n");
    #endif
        return 0;
    }
    return len;
}  
  
//出队列操作  
uint32_t Queue::Delete_Queue(circularQueue *q, ElemType *e,uint32_t len)  
{  
    if (q->front == q->rear)  return 0;  //空队列，直接返回  
        
    if(q->rear > q->front) //头部元素出队 
    { 
        memcpy(e,q->base + q->front,len);
        q->front = (q->front + len) % MAXSIZE;
    }
    else{
        q->size = MAXSIZE - q->front;
        memcpy(e,q->base + q->front,q->size);
        memcpy(e + q->size,q->base,len - q->size);
        q->front = len - q->size;      
    }  
    return len;
}  

//队列判空
bool Queue::Queue_Empty(circularQueue *q)
{
    return (q->front == q->rear);
}

//队列长度
int Queue::Queue_Size(circularQueue *q)
{
    int size = q->rear - q->front;
    if(size > 0)
        return size;
    else
    {
        size = MAXSIZE - q->rear + q->front;
        return size;
    }
}

//队列判满
bool Queue::Queue_Full(circularQueue *q)
{
    return (q->front == ((q->rear + 1) % MAXSIZE));
}

//销毁队列
void Queue::Destroy_Queue(circularQueue *q)
{
    delete[] q->base;
}