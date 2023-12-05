#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// define a 50 bit solution struct
typedef struct solution {
    uint64_t value : 50;
} Solution;

// method of solution
void Set (Solution *t);
void Copy (Solution *t, Solution *object);
void Tweak (Solution *t);

// define a tabu list struct
typedef struct tabu_list {
    int current_length;
    uint64_t value : 50;
    struct tabu_list *next;
} Tabu_list;

// method of tabu list
void Enqueue(Tabu_list *L, Solution *t);
void Dequeue(Tabu_list *L);
int In_tabu_list(Tabu_list *L, int length, Solution *t);

void initialize(Solution *S, Tabu_list *L);
void tabu_search(int iterations ,int n, int l, Solution *S, Solution *Best, Tabu_list *L);

// helper function
unsigned long long  generate_random(int div);
unsigned long long  B2D (uint64_t value);
unsigned long long  Quality (Solution *t);
int power(int base, int exponent);


int main(void) {
    // initialize random seed
    time_t t;
    srand((unsigned) time(&t));
    
    // define variables
    int bit_length = 50; // length of the bit string
    int n = 20; // number of tweaks desired to sample the gradient
    int iterations = 1000; // number of iterations
    int l = 10; // tabu_list_length


    Solution *S = NULL;
    S=(Solution*)malloc(sizeof (Solution));

    Tabu_list *L = NULL;
    L=(Tabu_list*)malloc(sizeof (Tabu_list));

    // initialize
    initialize( S, L);
    
    // define the best solution
    Solution *Best = NULL;
    Best=(Solution*)malloc(sizeof (Solution));
    Copy(Best, S);

    // tabu search : iterations, n: number of tweaks, l: tabu list length, S: local solution, Best: best solution, L: tabu list
    tabu_search( iterations, n, l, S, Best, L);

    // free allocated memory
    free(S);
    free(L);

    return 0;
}



void initialize(Solution *S , Tabu_list *L) {
    // Initial S
    S->value = 0;

    // Initial every bit in S
    int i=0;
    for (i = 0; i < 50; ++i) {
        // Toggle the i-th bit
        S->value |= (generate_random(2) << i);
    }

    // Initial Tabu_list
    L->next = NULL;
    L->value = S->value;
    L->current_length = 1;

}


void tabu_search(int iterations ,int n, int l, Solution *S , Solution *Best, Tabu_list *L){
    // printf("%d %d %llu %llu\n", n, l, B2D(S->value), B2D(L->value));
    printf("Init Value      : %llu\n", Quality(S));

    // length of tabu list set to one
    int tabu_length = 1;
    while (iterations--){
        // check the tabu list size and update the tabu list
        if (tabu_length>l){
            Dequeue(L);
            tabu_length--;
        }
        
        // Evaluate the Best solution
        printf("Best Value %-4d : %llu , Tabu list size: %d\n", 1000-iterations, Quality(Best),tabu_length );

        Solution *R = NULL;
        R = (Solution*)malloc(sizeof (Solution));
        Set(R);
        Copy(R, S);
        Tweak(R);
        
        int count = 0;
        for (count = 0; count < n-1; count++) {
            Solution *W = NULL;
            W = (Solution*)malloc(sizeof (Solution));
            Set(W);
            Copy(W, S);
            Tweak(W);
            if ( !In_tabu_list( L, tabu_length, W) && ( ( Quality(W)>Quality(R) ) || In_tabu_list( L, tabu_length, R) ) ){
                // R <- W
                Copy(R, W);
            }
        }
        if ( !In_tabu_list( L, tabu_length, R) ){
            // S <- R
            Copy(S, R);
            // Enqueue R to the tabu list
            Enqueue(L, R);
            tabu_length++;
        }
        if ( Quality(S) > Quality(Best) ){
            // Best <- S
            Copy(Best, S);
        }
    }
}





unsigned long long generate_random(int div){
    unsigned long long r = rand() % div;
    // printf("%llu", r);
    return r;
}
// Convert binary to decimal
long long unsigned int B2D(uint64_t value) {
    // Initialize the decimal value
    long long unsigned int decimal = 0;
    // Initialize the base (power of 2)
    uint64_t base = 1;
    
    // Iterate through each bit position
    int i=0;
    for (i = 0; i < 50; ++i) {
        // Check if the i-th bit is set (1)
        if (value & (1ULL << i)) {
            // Add the corresponding power of 2 to the decimal value
            decimal += base;
        }

        // Double the base for the next bit position
        base *= 2;
    }

    return decimal;
}
// Initialize the value of solution struct
void Set (Solution *t){
    t->value = 0ULL;
}
// Copy the value of object to the value of t
void Copy (Solution *t, Solution *object){
    // printf("copy\n");
    t->value = object->value;
}

// Tweak the value of t by one bit
void Tweak (Solution *t){
    // toggle the p-th bit
    t->value ^= (1ULL << rand() % 50);
}

// Enqueue the value of t to the tabu list
void Enqueue(Tabu_list *L, Solution *t){
    Tabu_list *temp = L;
    while (temp->next != NULL){
        temp = temp->next;
    }
    temp->next = (Tabu_list*)malloc(sizeof (Tabu_list));
    temp->next->value = t->value;
    temp->next->next = NULL;
}
// Dequeue the first item in the tabu list
void Dequeue(Tabu_list *L){
    Tabu_list *temp = L;
    L = L->next;
    free(temp);
}
// Check if the value of t is in the tabu list
int In_tabu_list(Tabu_list *L, int length, Solution *t){
    Tabu_list *temp = L;
    while (temp->next != NULL){
        if (temp->value == t->value){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

int power(int base, int exponent){
    int result = 1;
    while (exponent != 0){
        result *= base;
        --exponent;
    }
    return result;
}

// Calculate the quality of the solution
unsigned long long  Quality (Solution *t){
    unsigned long long score = 0;
    // n is the biggest bit of one in the solution
    int n = 0;
    // Iterate through each bit position
    int i=0;
    for (i = 0; i < 50; ++i) {
        // Check if the i-th bit is set (1)
        if (t->value & (1ULL << i)) {
            // Find the largest bit of one
            n=i;
        }
    }
    // Valuate the solution
    score = abs(B2D(t->value) - power(2,n-2));

    return score;
}
