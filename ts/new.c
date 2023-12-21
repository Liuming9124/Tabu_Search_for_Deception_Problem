#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

typedef struct queue
{
    int _size;
    int _limit;
    Tabu_list *_head;
    Tabu_list *_tail;
} Queue;
void init_queue(Queue *_queue, const int _limit, const uint64_t *value)
{
    Tabu_list *temp = (Tabu_list *)malloc(sizeof(Tabu_list));
    init_tabu_list(temp);
    temp->value = *value;
    _queue->_size = 0;
    _queue->_limit = _limit;
    _queue->_head = temp;
    _queue->_tail = temp;
}
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
void free_queue(Queue *_queue)
{
    while (_queue->_size > 0)
        pop(_queue);
    free(_queue);
}

uint64_t power_2(int exp)
{
    uint64_t result = (uint64_t)1 << (uint64_t)exp;
    return result;
}

void Tweak(uint64_t *_value)
{
    printf("%llu\n", *_value);
    *_value ^= (uint64_t)1 << (uint64_t)(rand() % 50);
}

int main(int argc, char const *argv[])
{
    Queue queue;
    uint64_t uint = 0;
    init_queue(&queue, 4, &uint);
    uint++;
    push(&queue, &uint);
    uint++;
    push(&queue, &uint);
    uint++;
    push(&queue, &uint);
    uint++;
    push(&queue, &uint);
    uint++;
    push(&queue, &uint);
    uint++;
    push(&queue, &uint);
    uint++;
    push(&queue, &uint);
    uint++;
    push(&queue, &uint);
    uint++;
    push(&queue, &uint);
    Tabu_list *list = queue._head;
    while (list)
    {
        printf("%llu\n", list->value);
        list = list->next;
    }

    return 0;
}
