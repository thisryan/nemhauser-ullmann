#include <stdio.h>

#include "algorithm.h"

int main(int argc, char const *argv[])
{

    FILE* file = fopen("instances_01_KP/large_scale/knapPI_1_1000_1000_1", "r");
    if(file == NULL) {
        fprintf(stderr, "Could not open file! \n");
        exit(1);
    }

    int amount, capacity;
    fscanf(file, "%d %d\n", &amount, &capacity);

    KnapsackInput input;
    input.capacity = capacity;
    input.number_items = amount;
    input.profit = malloc(amount * sizeof(long));
    input.weight = malloc(amount * sizeof(long));
    for(int i = 0;i < amount;i++){
        fscanf(file, "%ld %ld\n", input.profit + i, input.weight + i);
    }

    fclose(file);

    printf("Number: %ld, Cap: %ld \n", input.number_items, input.capacity);


    Solution sol = nemhauser_ullmann(input);
    for(int i = 0;i < sol.amount;i++){
        printf("%ld ", sol.items[i]);
    }
    printf("\n");

    long sum = 0;
    for(int i = 0;i < sol.amount;i++){
        sum += input.profit[sol.items[i]-1];
    }

    printf("Result: %ld\n", sum);

    free(sol.items);

    return 0;
}
