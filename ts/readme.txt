Tabu Search for Deception Problem

## How to Use

1. Compile
```
cd /path/to/this/directory
gcc ./main.c -o main
```

2. Run
- iterations : integer
- tabu list length : integer
- number of tweaks : integer , must less than 50
- test times : how many times you need to try and get back the best data
- output file name

```
./main.exe [iterations] [tabu list length] [number of tweaks] [test times] > [output file name]
```


3.Example
```
gcc ./main.c -o main
./main.exe 1000 10 12 10000 > result.txt
```

4. Show Result
4.1 Input value
- Max Tabu list length
- Number of Tweaks
- Iterations
- Times
4.2 Result
- Best Times : The less Iterations to Find the Best Value
- **Best Compare of Tweaks** : The less Tweaks to Find the Best Value -> Best times * Number of Tweaks
- Best Value: show the best Value of Deception problem
- Average times : Average Iterations to find the best value
- Average Compare of Tweaks : Average times / Number of Tweaks

## Structure and Method Description

1. tabu_list

1.1 item:
- value: uint64_t to solve this 50 bits Deception Problem
- struct tabu_list *next: Pointer to next item

1.2 Method:
- init_tabu_list(Tabu_list *_list)



2. queue :
2.1 item:
- _size: store current of queue 
- _limit: Max limit of queue
- Tabu_list *_head: ptr point to head of queue
- Tabu_list *_tail: ptr point to tail of queue

2.2 Method:
- void init_queue(Queue *_queue, const int _limit, const uint64_t *value) : Initialize queue
- void pop(Queue *_queue) : Pop the oldest item in the tabu list
- void push(Queue *_queue, const uint64_t *value) : push value in the newest place of queue
- short has_item(Queue *_queue, uint64_t _value) : check whether value in queue or not, return 1:True, 0:False
- void free_queue(Queue *_queue) : Free the tabu list


## Main Function Description

1. void tabu_search(int group_size) : 
- Tabu_search will compare group_size times to update the best value
2. void Tweak(uint64_t *_value) : 
- Tweak one bit by xor within best value and which value modify one random bit from zero to one
3. uint64_t Quality(const uint64_t *_value) : 
- f(x) = |B2D(s)-2^(n-2)|, s(i) belongs to {0,1}, n>2
- Evaluate value and return decimal
