#include <stdio.h>

#include "algorithm.h"

int main(int argc, char const *argv[])
{

    long profits[] = {1, 5, 6, 7};
    long weights[] = {2 , 2, 10, 6};
    long capacity = 12;

    KnapsackInput input = {
        .capacity = capacity,
        .profit = profits,
        .weight = weights,
        .number_items = 4
    };

    Solution sol = nemhauser_ullmann(input);

    for(int i = 0;i < sol.amount;i++){
        printf("%ld ", sol.items[i]);
    }
    printf("\n");

    free(sol.items);

    return 0;
}
