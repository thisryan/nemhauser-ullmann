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

void ParetoSet_cleanup(ParetoSet* set) {
    free(set->solutions);
}

ParetoSet ParetoSet_combine(ParetoSet* set, long profit, long weight, long capacity) {
    ParetoSet result;
    ParetoSet_init(&result);

    size_t set_pointer = 0;
    size_t add_pointer = 0;


    ParetoSet_add(&result, set->solutions[set_pointer]);
    set_pointer++;

    while (set_pointer < set->index && set->solutions[set_pointer].weight <= capacity) {
        if (set->solutions[set_pointer].weight < set->solutions[add_pointer].weight + weight) {
            if (!Pareto_solution_dominates(result.solutions + (result.index - 1), set->solutions + set_pointer)) {
                ParetoSet_add(&result, set->solutions[set_pointer]);
            }
            set_pointer++;
        }
        else {
            ParetoSolution sol = {
                .profit = set->solutions[add_pointer].profit + profit,
                .weight = set->solutions[add_pointer].weight + weight
            };
            add_pointer++;
            if (sol.weight > capacity) continue;
            if (!Pareto_solution_dominates(result.solutions + (result.index - 1), &sol)) {
                ParetoSet_add(&result, sol);
            }
        }
    }

    while (add_pointer < set->index && set->solutions[add_pointer].weight + weight <= capacity) {
        ParetoSolution sol = {
            .profit = set->solutions[add_pointer].profit + profit,
            .weight = set->solutions[add_pointer].weight + weight
        };
        if (!Pareto_solution_dominates(result.solutions + (result.index - 1), &sol)) {
            ParetoSet_add(&result, sol);
        }
        add_pointer++;
    }

    ParetoSet_pack(&result);
    return result;
}

Solution nemhauser_ullmann(KnapsackInput input) {
    ParetoSet last;
    ParetoSet_init(&last);
    ParetoSet_add(&last, empty_solution());
    ParetoSet_pack(&last);

    for (int i = 1;i <= input.number_items;i++) {
        ParetoSet new = ParetoSet_combine(&last, input.profit[i - 1], input.weight[i - 1], input.capacity);

        ParetoSet_cleanup(&last);
        last = new;
    }

    ParetoSolution best_solution = empty_solution();
    ParetoSet* n_set = &last;
    for (int i = 0;i < n_set->amount;i++) {
        if (n_set->solutions[i].profit > best_solution.profit && n_set->solutions[i].weight <= input.capacity) {
            best_solution = n_set->solutions[i];
        }

    }

    ParetoSet_cleanup(&last);
    return (Solution) { .profit = best_solution.profit, .weight = best_solution.weight };
}