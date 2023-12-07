#include "test.h"

#define BIT_LENGTH 50

uint64_t best, end;
Queue queue;

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Please enter argment");
        return 0;
    }
    // parser arg
    int iterations = atoi(argv[1]);
    int l = atoi(argv[2]);
    int n = atoi(argv[3]);
    // init
    time_t t;
    srand((unsigned)time(&t));
    best = 0;
    memset(&end, 0, sizeof(end));
    for (int i = 0; i < BIT_LENGTH - 1; i++)
    {
        end++;
        end <<= (uint64_t)1;
    }
    end++;

    uint64_t init;
    for (int i = 0; i < BIT_LENGTH; i++)
    {
        init += rand() % 2;
        init << (uint64_t)1;
    }
    init_queue(&queue, l, &init);
    best = Quality(&queue._head->value);
    int i;
    for (i = 0; i < iterations; i++)
    {
        tabu_search(n);
        if (best == end)
            break;
    }
    free_queue(&queue);
    printf("Best %llu in %d \n", best, i);
    return 0;
}

void tabu_search(int group_size)
{
    for (int i = 0; i < group_size; i++)
    {

        uint64_t temp = best;
        Tweak(&temp);
        if ((!has_item(&queue, temp)) && (Quality(&temp) > Quality(&best)))
        {
            push(&queue, &temp);
            best = temp;
        }
    }
}

void Tweak(uint64_t *_value)
{
    *_value ^= (uint64_t)1 << (uint64_t)(rand() % BIT_LENGTH);
}

uint64_t Quality(const uint64_t *_value)
{
    for (int i = BIT_LENGTH - 1; i > 2; i--)
    {
        if ((*_value) & (uint64_t)1 << (uint64_t)i)
            return llabs(*_value - power_2(i - 2));
    }
}
