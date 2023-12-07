#include "main.h"

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
    // parse arg
    int iterations = atoi(argv[1]); // iterations
    int l = atoi(argv[2]); // limit of tabu list size
    int n = atoi(argv[3]); // size of tweak group
    // init
    time_t t;
    srand((unsigned)time(&t));
    best = 0;
    memset(&end, 0, sizeof(end));
    for (int i = 0; i < BIT_LENGTH - 1; i++) // set the end status to BIT_LENGTH times of 1
    {
        end++;
        end <<= (uint64_t)1;
    }
    end++;

    // initialize the first value
    uint64_t init;
    for (int i = 0; i < BIT_LENGTH; i++)
    {
        init += rand() % 2;
        init << (uint64_t)1;
    }
    // initialize queue , put init in queue
    init_queue(&queue, l, &init);
    best = Quality(&queue._head->value);
    int i;
    // Start iterations for tabu search
    for (i = 0; i < iterations; i++)
    {
        tabu_search(n);
        printf("Best %-16llu in %d \n", best, i);
        if (best == end)
            break;
    }
    free_queue(&queue);
    
    printf("-----------------------------END-----------------------------\n");
    printf("Best %-16llu in %d \n", best, i);
    printf("Number of Tweaks : %d\n", n);
    printf("Max Tabu list length : %d\n", l);
    printf("-------------------------------------------------------------\n");

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

// Tweak one bit by xor within best value and which value modify one random bit from zero to one
void Tweak(uint64_t *_value)
{
    *_value ^= (uint64_t)1 << (uint64_t)(rand() % BIT_LENGTH);
}

// Evaluate value and return decimal
uint64_t Quality(const uint64_t *_value)
{
    // Find the largest bit place of 1 
    for (int i = BIT_LENGTH - 1; i > 2; i--)
    {
        if ((*_value) & (uint64_t)1 << (uint64_t)i)
            return llabs(*_value - power_2(i - 2));
    }
}
