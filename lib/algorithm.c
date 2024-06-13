#include "algorithm.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct ParetoSolution ParetoSolution;

struct ParetoSolution {
    long weight;
    long profit;
    size_t prev_item;
    size_t item_index;
    size_t prev_set_index;
};

typedef struct {
    ParetoSolution* solutions;
    size_t amount;
    size_t index;
} ParetoSet;
