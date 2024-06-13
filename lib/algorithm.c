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

void ParetoSet_init(ParetoSet* set) {
    set->amount = 100;
    set->index = 0;
    set->solutions = malloc(set->amount * sizeof(ParetoSolution));
}

void ParetoSet_add(ParetoSet* set, ParetoSolution solution) {
    if (set->index >= set->amount) {
        set->amount *= 2;
        set->solutions = realloc(set->solutions, set->amount * sizeof(ParetoSolution));
    }

    set->solutions[set->index++] = solution;
}

// Packing means cutting the memory to exact size since i dont need to add elements anymore
void ParetoSet_pack(ParetoSet* set) {
    set->solutions = realloc(set->solutions, set->index * sizeof(ParetoSolution));
    set->amount = set->index;
}

ParetoSet ParetoSet_calculate_plus_i(ParetoSet* set, long weight, long profit, size_t item_index) {
    ParetoSet result;
    result.amount = set->amount;
    result.solutions = malloc(result.amount * sizeof(ParetoSolution));

    for (size_t i = 0;i < set->amount;i++) {
        result.solutions[i].weight = set->solutions[i].weight + weight;
        result.solutions[i].profit = set->solutions[i].profit + profit;
        result.solutions[i].item_index = item_index;
        result.solutions[i].prev_set_index = i;
        result.solutions[i].prev_item = set->solutions[i].item_index;
    }

    return result;
}
