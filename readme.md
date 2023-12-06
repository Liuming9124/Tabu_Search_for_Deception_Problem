# Tabu Search for Deception Problem
## How to Use
### 1. Compile
```
cd /path/to/this/directory
gcc ./main.c -o main
```
### 2. Run
```
./main.exe [iterations] [tabu list length] [number of tweaks] > [output file name]
```
### 3.Example
```
gcc ./main.c -o main
./main.exe 1000 10 5 > result.txt
```

## Structure Description
### 1. Solution
#### 1.1 Contains:
* A single item for 50 bits of uint64_t to solve this 50 bits Deception Problem

#### 1.2 Method:
* void Set (Solution *t) : Init the value of Solution to 0
* void Copy (Solution *t, Solution *object) : Copy the value of object to the value of t
* void Tweak (Solution *t) : Tweak the value of t by one bit

### 2. Tabu_list :
#### 2.1 Contains
* An integer of its current length
* A pointer structure of itself to store the next item.
* A Solution Structure to save 50 bits value.
#### 2.2 Method:
* void Enqueue(Tabu_list *L, Solution *t) : Enqueue the value of t to the tabu list
* void Dequeue(Tabu_list *L) : Dequeue the first item in the tabu list
* int In_tabu_list(Tabu_list *L, int length, Solution *t) : Check if the value of t is in the tabu list
* void Free_tabu_list(Tabu_list *L) : Free the tabu list


## Function Description

### Main Method
1. #### void initialize(Solution *S, Tabu_list *L) : Initialize the solution and tabu list
    * Init some variable and Structure. 
2. #### void tabu_search(int iterations ,int n, int l, Solution *S, Solution *Best, Tabu_list *L)
    * Tabu search : iterations, n: number of tweaks, l: tabu list length, S: local solution, Best: best solution, L: tabu list

### Helper Method
1. #### uint64_t generate_random(int div) : 
    * generate random number divided by div
2. #### uint64_t B2D (uint64_t value) : 
    * Convert binary to decimal
3. #### uint64_t Quality (Solution *t) : 
    * Calculate the quality of the solution
4. #### int power(int base, int exponent) : 
    * Calculate the power of base to exponent

