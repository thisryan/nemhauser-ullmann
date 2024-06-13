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

void ParetoSet_cleanup(ParetoSet *set){
    free(set->solutions);
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

// The approach implemented in this funcution is the naive approach in quadtraing time
// TODO: Implement faster approach
ParetoSet ParetoSet_combine(ParetoSet* set1, ParetoSet* set2) {
    ParetoSet result;
    ParetoSet_init(&result);

    for (int i = 0;i < set1->amount;i++) {
        bool dominated = false;

        for (int j = 0;j < set2->amount;j++) {
            if (Pareto_solution_dominates(set2->solutions + j, set1->solutions + i)) {
                dominated = true;
                break;
            }
        }

        if (!dominated) {
            ParetoSet_add(&result, set1->solutions[i]);
        }
    }

    for (int i = 0;i < set2->amount;i++) {
        bool dominated = false;

        for (int j = 0;j < set1->amount;j++) {
            if (Pareto_solution_dominates(set1->solutions + j, set2->solutions + i)) {
                dominated = true;
                break;
            }
        }

        if (!dominated) {
            ParetoSet_add(&result, set2->solutions[i]);
        }
    }

    ParetoSet_pack(&result);
    return result;
}

Solution Solution_reconstruct(ParetoSet *sets, ParetoSolution final_solution){
    size_t amount = 10;
    size_t index = 0;
    size_t *items = malloc(amount * sizeof(size_t));

    ParetoSolution cur = final_solution;
    while(cur.item_index != -1) {
        items[index++] = cur.item_index;

        if(cur.prev_item == -1) {
            break;
        }

        cur = sets[cur.prev_item].solutions[cur.prev_set_index];

        if(index >= amount){
            amount *= 2;
            items = realloc(items, amount * sizeof(size_t));
        }
    }

    items = realloc(items, index * sizeof(size_t));
    return (Solution) {
        .amount = index,
        .items = items
    };
}
