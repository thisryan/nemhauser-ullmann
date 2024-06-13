#include "algorithm.h"

#include <stdbool.h>
#include <stdio.h>

/*
* Currently im playing fast and loose with the definitions, since not every solution of type "ParetoSolutions"
* is actually a ParetoSolution
*/
// TODO: Fix Naming

typedef struct ParetoSolution ParetoSolution;

struct ParetoSolution {
    long weight;
    long profit;
};

ParetoSolution empty_solution() {
    return (ParetoSolution) {
        .weight = 0,
            .profit = 0,
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
    }

    return result;
}

// The approach implemented in this funcution is the naive approach in quadtraing time
// TODO: Implement faster approach
ParetoSet ParetoSet_combine(ParetoSet* set1, ParetoSet* set2, long capacity) {
    ParetoSet result;
    ParetoSet_init(&result);

    for (int i = 0;i < set1->amount;i++) {
        bool dominated = false;
        if (set1->solutions[i].weight > capacity) continue;

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

        if (set2->solutions[i].weight > capacity) continue;

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

        if(cur.prev == NULL) {
            break;
        }

        cur = *cur.prev;

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

Solution nemhauser_ullmann(KnapsackInput input) {
    ParetoSet* pareto_sets = malloc((input.number_items + 1) * sizeof(ParetoSet));
    ParetoSet_init(pareto_sets + 0);
    ParetoSet_add(pareto_sets + 0, empty_solution());
    ParetoSet_pack(pareto_sets + 0);

    for (int i = 1;i <= input.number_items;i++) {
        ParetoSet* prev_set = pareto_sets + (i - 1);
        ParetoSet p_plus_one = ParetoSet_calculate_plus_i(prev_set, input.weight[i-1], input.profit[i-1], i);
        pareto_sets[i] = ParetoSet_combine(prev_set, &p_plus_one, input.capacity);

        ParetoSet_cleanup(&p_plus_one);
    }

    ParetoSolution best_solution = empty_solution();
    ParetoSet *n_set = pareto_sets + input.number_items;
    for(int i = 0;i < n_set->amount;i++){
        if(n_set->solutions[i].profit > best_solution.profit && n_set->solutions[i].weight <= input.capacity) {
            best_solution = n_set->solutions[i];
        }
        
    }

    Solution sol = Solution_reconstruct(pareto_sets, best_solution);

    for(int i = 0;i <= input.number_items;i++){
        ParetoSet_cleanup(pareto_sets + i);
    }
    free(pareto_sets);

    return sol;
}