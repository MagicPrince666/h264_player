#include "listop.h"  
  
 /**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
int list_empty(struct list_head *head)
{
	return head->next == head;
} 

static  void __check_head(struct list_head* head)  
{  
    if ((head->next == 0) && (head->prev == 0)) {  
        INIT_LIST_HEAD(head);  
    }  
}  
  
/* 
 * Insert a new entry between two known consecutive entries. 
 * 
 * This is only for internal list manipulation where we know 
 * the prev/next entries already! 
 */  
static  void __list_add(struct list_head* new_list,  
                        struct list_head* prev,  
                        struct list_head* next)  
{  
  
    next->prev = new_list;  
    new_list->next = next;  
    new_list->prev = prev;  
    prev->next = new_list;  
  
}  
  
  
/* 
 * Delete a list entry by making the prev/next entries 
 * point to each other. 
 * 
 * This is only for internal list manipulation where we know 
 * the prev/next entries already! 
 */  
static  void __list_del(struct list_head* prev,  
                        struct list_head* next)  
{  
  
    next->prev = prev;  
    prev->next = next;  
}  
  
  
  
/** 
 * list_add - add a new entry 
 * @new: new entry to be added 
 * @head: list head to add it after 
 * 
 * Insert a new entry after the specified head. 
 * This is good for implementing stacks. 
 */  
void list_add(struct list_head* new_list, struct list_head* head)  
{  
    __check_head(head);  
    __list_add(new_list, head, head->next);  
}  
  
/** 
 * list_add_tail - add a new entry 
 * @new: new entry to be added 
 * @head: list head to add it before 
 * 
 * Insert a new entry before the specified head. 
 * This is useful for implementing queues. 
 */  
void list_add_tail(struct list_head* new_list, struct list_head* head)  
{  
    __check_head(head);  
    __list_add(new_list, head->prev, head);  
}  
  
  
/** 
 * list_del - deletes entry from list. 
 * @entry: the element to delete from the list. 
 * Note: list_empty on entry does not return true after this, the entry is in an undefined state. 
 */  
void list_del(struct list_head* entry)  
{  
    __list_del(entry->prev, entry->next);  
}  
  
/** 
 * list_del_init - deletes entry from list and reinitialize it. 
 * @entry: the element to delete from the list. 
 */  
void list_del_init(struct list_head* entry)  
{  
    __list_del(entry->prev, entry->next);  
    INIT_LIST_HEAD(entry);  
}  
  
/** 
 * list_move - delete from one list and add as another's head 
 * @list: the entry to move 
 * @head: the head that will precede our entry 
 */  
void list_move(struct list_head* list, struct list_head* head)  
{  
    __check_head(head);  
    __list_del(list->prev, list->next);  
    list_add(list, head);  
}  
  
/** 
 * list_move_tail - delete from one list and add as another's tail 
 * @list: the entry to move 
 * @head: the head that will follow our entry 
 */  
void list_move_tail(struct list_head* list,  
                    struct list_head* head)  
{  
    __check_head(head);  
    __list_del(list->prev, list->next);  
    list_add_tail(list, head);  
}  
  
  
/** 
 * list_splice - join two lists 
 * @list: the new list to add. 
 * @head: the place to add it in the first list. 
 */  
void list_splice(struct list_head* list, struct list_head* head)  
{  
    struct list_head* first = list;  
    struct list_head* last  = list->prev;  
    struct list_head* at    = head->next;  
  
    first->prev = head;  
    head->next  = first;  
  
    last->next = at;  
    at->prev   = last;  
}  
  
struct list_head* list_dequeue( struct list_head* list ) {  
    struct list_head* next, *prev, *result = ((list_head*)0);  
  
    prev = list;  
    next = prev->next;  
  
    if ( next != prev ) {  
        result = next;  
        next = next->next;  
        next->prev = prev;  
        prev->next = next;  
        result->prev = result->next = result;  
    }  
  
    return result;  
}  
