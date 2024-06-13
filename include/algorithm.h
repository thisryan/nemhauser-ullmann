#pragma once

#include <stdlib.h>

typedef struct {
    long capacity;
    long *weight;
    long *profit; 
    size_t number_items;
} KnapsackInput;

typedef struct {
    long weight;
    long profit;
} Solution;

Solution nemhauser_ullmann(KnapsackInput input);