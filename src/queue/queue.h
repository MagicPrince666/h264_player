#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h>  
#include <stdio.h> 
#include <inttypes.h>

#define MAXSIZE 32768*4  
typedef uint8_t ElemType ;  
  
typedef struct    
{  
    ElemType *base; //存储内存分配基地址  
    uint32_t front;      //队列头索引  
    uint32_t rear;       //队列尾索引  
    uint32_t size;
}circularQueue; 

class Queue
{

public:
    circularQueue q; 
    void Init_Queue(circularQueue *q);
    //void ~Init_Queue(circularQueue *q);

    uint32_t Insert_Queue(circularQueue *q, ElemType* e,uint32_t len);
    uint32_t Delete_Queue(circularQueue *q, ElemType *e,uint32_t len);
    bool Queue_Empty(circularQueue *q);
    bool Queue_Full(circularQueue *q);
    int Queue_Size(circularQueue *q);
    void Destroy_Queue(circularQueue *q);


};

#endif