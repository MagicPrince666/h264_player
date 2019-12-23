#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "xag_time_tick.h"

struct timer_wheel timer = {{0}, 0};

void tick(int signo) {
	// 使用二级指针删进行单链表的删除 
	struct timer_node **cur = &timer.slot[timer.current];
	while (*cur) {
		struct timer_node *curr = *cur;
		if (curr->rotation > 0) {
			curr->rotation--;
			cur = &curr->next;
			} else {
				curr->proc(curr->data);
				*cur = curr->next; free(curr);
			}
	}
	timer.current = (timer.current + 1) % TIME_WHEEL_SIZE;
	alarm(1);
}

void add_timer(int len, func action) {
	int pos = (len + timer.current) % TIME_WHEEL_SIZE;
	struct timer_node *node = (timer_node*)malloc(sizeof(struct timer_node));
	// 插入到对应格子的链表头部即可, O(1)复杂度
	node->next = timer.slot[pos];
	timer.slot[pos] = node;
	node->rotation = len / TIME_WHEEL_SIZE;
	node->data = 0; node->proc = action;
}
