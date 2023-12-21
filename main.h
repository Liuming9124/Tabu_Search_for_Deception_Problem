#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

typedef unsigned long long uint64_t;

typedef struct tabu_list
{
    uint64_t value;
    struct tabu_list *next;
} Tabu_list;
void init_tabu_list(Tabu_list *_list)
{
    _list->next = NULL;
    _list->value = 0;
}

// Queue structure of tabu list
typedef struct queue
{
    int _size; // queue current size
    int _limit; // queue max limit
    Tabu_list *_head;
    Tabu_list *_tail;
} Queue;
void init_queue(Queue *_queue, const int _limit, const uint64_t *value)
{
    Tabu_list *temp = (Tabu_list *)malloc(sizeof(Tabu_list)); // instantiation
    init_tabu_list(temp);
    temp->value = *value;
    _queue->_size = 0;
    _queue->_limit = _limit;
    _queue->_head = temp;
    _queue->_tail = temp;
}
// pop the oldest item in queue
void pop(Queue *_queue)
{
    if (_queue->_size > 0)
    {
        _queue->_size--;
        Tabu_list *temp = _queue->_head;
        _queue->_head = _queue->_head->next;
        free(temp);
    }
}
// push value in the newest place of queue
void push(Queue *_queue, const uint64_t *value)
{
    Tabu_list *temp = (Tabu_list *)malloc(sizeof(Tabu_list));
    init_tabu_list(temp);
    temp->value = *value;
    _queue->_size++;
    if (_queue->_size >= _queue->_limit)
        pop(_queue);
    _queue->_tail->next = temp;
    _queue->_tail = temp;
}
// check whether value in queue or not, return 1:True, 0:False
short has_item(Queue *_queue, uint64_t _value)
{
    Tabu_list *temp = _queue->_head;
    while (temp)
    {
        if (temp->value == _value)
            return 1;
        temp = temp->next;
    }
    return 0;
}
// free all queue
void free_queue(Queue *_queue)
{
    while (_queue->_size > 0)
        pop(_queue);
}
// count power of 2 by shift left
uint64_t power_2(int exp)
{
    uint64_t result = (uint64_t)1 << (uint64_t)exp;
    return result;
}

void Tweak(uint64_t *);
void tabu_search(int);
uint64_t Quality(const uint64_t *);