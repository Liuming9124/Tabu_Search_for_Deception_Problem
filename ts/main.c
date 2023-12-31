#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define Bit_length 50

// define a 50 bit solution struct
typedef struct solution {
    uint64_t value : Bit_length;
} Solution;

// method of solution
void Set (Solution *t); // Init the value of solution
void Copy (Solution *t, Solution *object); // Copy the value of object to the value of t
void Tweak (Solution *t); // Tweak the value of t by one bit

// define a tabu list struct
typedef struct tabu_list {
    uint64_t value : Bit_length;
    struct tabu_list *next;
} Tabu_list;

// method of tabu list
void Enqueue(Tabu_list *L, Solution *t);    // Enqueue the value of t to the tabu list
void Dequeue(Tabu_list **L); // Dequeue the first item in the tabu list
int  In_tabu_list(Tabu_list *L, int length, Solution *t); // Check if the value of t is in the tabu list
void Free_tabu_list(Tabu_list *L); // Free the tabu list

// main method of tabu search
void initialize(Solution *S, Tabu_list *L); // initialize the solution and tabu list
void tabu_search(int iterations ,int n, int l, Solution *S, Solution *Best, Tabu_list *L); // tabu search

// helper function
uint64_t generate_random(int div); // generate random number divided by div
uint64_t B2D (uint64_t value); // Convert binary to decimal
uint64_t Quality (Solution *t); // Calculate the quality of the solution
int power(int base, int exponent); // Calculate the power of base to exponent


// main function: tabu search, put iterations, tabu list length, number of tweaks.
int main( int argc, char *argv[]) {
    // initialize random seed
    time_t t;
    srand((unsigned) time(&t));
    
    // define variables
    int iterations = atoi(argv[1]); // number of iterations
    int l = atoi(argv[2]); // tabu_list_length
    int n = atoi(argv[3]); // number of tweaks desired to sample the gradient

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
    free(Best);
    Free_tabu_list(L);

    return 0;
}



void initialize(Solution *S , Tabu_list *L) {
    // Initial S
    S->value = 0;

    // Initial every bit in S
    int i=0;
    for (i = 0; i < Bit_length; ++i) {
        // Toggle the i-th bit
        S->value |= (generate_random(2) << i);
    }

    // Initial Tabu_list
    L->next = NULL;
    L->value = S->value;

}


void tabu_search(int iterations ,int n, int l, Solution *S , Solution *Best, Tabu_list *L){
    printf("Init Value      : %16llu\n", Quality(S));

    Solution *End = NULL;
    End=(Solution*)malloc(sizeof (Solution));
    // set every bit to 1
    int i = 0;
    for (i = 0; i < Bit_length; ++i) {
        // Toggle the i-th bit
        End->value |= (1ULL << i);
    }

    // length of tabu list set to one
    int tabu_length = 1;
    while (1){
        if (iterations==0){
            break;
        }
        iterations--;
        // Evaluate the Best solution
        printf("Best Value %-4d : %16llu , Tabu list size: %d\n", 1000-iterations, Quality(Best),tabu_length );

        // check if match the best solution
        if (Quality(Best) == End->value){
            break;
        }
        // check the tabu list size and update the tabu list
        if (tabu_length>l){
            Dequeue(&L);
            tabu_length--;
        }

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
            
            // check the tabu list size and update the tabu list
            if (tabu_length==l){
                Dequeue(&L);
                tabu_length--;
            }
            // Enqueue R to the tabu list
            Enqueue(L, R);
            tabu_length++;
        }
        if ( Quality(S) > Quality(Best) ){
            // Best <- S
            Copy(Best, S);
        }
    }
    printf("-----------------------------END-----------------------------\n");
    printf("Bit length : %d\n", Bit_length);
    printf("Iterations : %-4d\nBest value : %16llu\n", 1000-iterations, Quality(Best) );
    printf("Number of Tweaks : %d\n", n);
    printf("Max Tabu list length : %d\n", l);
    printf("-------------------------------------------------------------\n");

}





uint64_t generate_random(int div){
    uint64_t r = rand() % div;
    // printf("%llu", r);
    return r;
}
// Convert binary to decimal
uint64_t  B2D(uint64_t value) {
    // Initialize the decimal value
    uint64_t decimal = 0;
    // Initialize the base (power of 2)
    uint64_t base = 1;
    
    // Iterate through each bit position
    int i=0;
    for (i = 0; i < Bit_length; ++i) {
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
    t->value ^= (1ULL << rand() % Bit_length);
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
void Dequeue(Tabu_list **L){
    Tabu_list *temp = *L;
    *L = (*L)->next;
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
// Free the tabu list
void Free_tabu_list(Tabu_list *L){
    Tabu_list *temp = L;
    while (temp->next != NULL){
        temp = temp->next;
        free(L);
        L = temp;
    }
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
uint64_t Quality (Solution *t){
    uint64_t score = 0;
    // n is the biggest bit of one in the solution
    int n = 0;
    // Iterate through each bit position
    int i=0;
    for (i = 0; i < Bit_length; ++i) {
        // Check if the i-th bit is set (1)
        if (t->value & (1ULL << i)) {
            // Find the largest bit of one
            n=i;
        }
    }
    if (n>2){
        // Valuate the solution
        score = llabs(B2D(t->value) - power(2,n-2));
    }

    return score;
}
