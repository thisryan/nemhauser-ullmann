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

ParetoSolution empty_solution() {
    return (ParetoSolution) {
        .weight = 0,
            .profit = 0,
            .prev_item = -1,
            .item_index = -1,
            .prev_set_index = -1,
    };
}

bool Pareto_solution_dominates(ParetoSolution* sol1, ParetoSolution* sol2) {
    return sol1->profit >= sol2->profit
        && sol1->weight <= sol2->weight
        && (sol1->profit > sol2->profit || sol1->weight < sol2->weight);
}

typedef struct {
    ParetoSolution* solutions;
    size_t amount;
    size_t index;
} ParetoSet;
