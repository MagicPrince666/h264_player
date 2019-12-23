#ifndef __XAG_TIME_TICK_H_
#define __XAG_TIME_TICK_H_

#define TIME_WHEEL_SIZE 8

typedef void (*func)(int data);

struct timer_node {
    struct timer_node *next;
    int rotation;
    func proc;
    int data;
};

struct timer_wheel {
    struct timer_node *slot[TIME_WHEEL_SIZE];
    int current;
};

void tick(int signo);
void add_timer(int len, func action);

#endif
