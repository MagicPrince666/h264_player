#include "ringbuffer.h"
#include <string.h>
#include <stdio.h>


#define Min(x, y) ((x) < (y) ? (x) : (y))

//extern pthread_mutex_t mut;//声明互斥变量

cycle_buffer* RingBuffer::ring_init(int length)
{
    printf("Init cycle buffer\n");
    
    cycle_buffer* buffer = (cycle_buffer *)malloc(sizeof(cycle_buffer));
    if (!buffer) {
        printf("malloc cycle buffer error\n");
        return NULL;
    }
    memset(buffer, 0, sizeof(RingBuffer)); 

    buffer->size = DEFAULT_BUF_SIZE;  
    buffer->in   = 0;
    buffer->out  = 0;  

    buffer->buf = (unsigned char *)malloc(buffer->size);  
    if (!buffer->buf){
        free(buffer);
        printf("malloc cycle buffer size error\n");
        return NULL;
    }
    memset(buffer->buf, 0, DEFAULT_BUF_SIZE);

    //pthread_mutex_init(&mut,NULL);

    return buffer;
}

int RingBuffer::destroy(cycle_buffer* buffer)
{
    if (!buffer->buf){
       free(buffer->buf);
       buffer->buf = NULL;
       printf("free buffer->buf\n");
    }
    if (!buffer){
       free(buffer);
       buffer = NULL;
       printf("free buffer\n");
    }
    printf("Clean cycle buffer\n");
    return 0;
}

int RingBuffer::Reset(cycle_buffer* buffer)
{
    if (buffer == NULL)
    {
        return -1;
    }
     
    buffer->in   = 0;
    buffer->out  = 0;
    memset(buffer->buf, 0, buffer->size);
    
    printf("RingBuffer cleaned\n");

    return 0;
}

int RingBuffer::empty(cycle_buffer* buffer)
{
    return buffer->in == buffer->out;
}

//get buffer size canbe usb
int RingBuffer::overage(cycle_buffer* buffer)
{
    int overage = buffer->in - buffer->out;

    if(overage > 0)
        return DEFAULT_BUF_SIZE - overage;
    else
        return DEFAULT_BUF_SIZE + overage;
}

int RingBuffer::write(cycle_buffer* buffer, uint8_t *data,unsigned int length)
{
    unsigned int len = 0;

    length = Min(length, buffer->size - buffer->in + buffer->out);  
    len    = Min(length, buffer->size - (buffer->in & (buffer->size - 1)));

    //pthread_mutex_lock(&mut);
    memcpy(buffer->buf + (buffer->in & (buffer->size - 1)), data, len);
    memcpy(buffer->buf, data + len, length - len);
    //pthread_mutex_unlock(&mut);
 
    buffer->in += length;

    return length;
}

int RingBuffer::read(cycle_buffer* buffer, uint8_t *target,unsigned int amount)
{
    unsigned int len = 0;  

    amount = Min(amount, buffer->in - buffer->out);
    len    = Min(amount, buffer->size - (buffer->out & (buffer->size - 1)));
 
    //pthread_mutex_lock(&mut);
    memcpy(target, buffer->buf + (buffer->out & (buffer->size - 1)), len);
    memcpy(target + len, buffer->buf, amount - len);
    //pthread_mutex_unlock(&mut);
 
    buffer->out += amount;
 
    return amount;
}

