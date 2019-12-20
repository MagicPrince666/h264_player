#ifndef __RINGBUFFER_H_
#define __RINGBUFFER_H_

#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>

#define DEFAULT_BUF_SIZE 1*1024*1024

typedef struct 
{  
    uint8_t*  buf;
    unsigned int   size;
    unsigned int   in;
    unsigned int   out;
}cycle_buffer;  


class RingBuffer {
public:
    //cycle_buffer* buffer;
    static cycle_buffer* ring_init(int length);
    static int destroy(cycle_buffer* buffer);

    static int read(cycle_buffer* buffer, uint8_t *target,unsigned int amount);
    static int write(cycle_buffer* buffer, uint8_t *data,unsigned int length);
    static int empty(cycle_buffer* buffer);
    static int overage(cycle_buffer* buffer);
    static int Reset(cycle_buffer* buffer);

protected:   
    
    
};

#endif