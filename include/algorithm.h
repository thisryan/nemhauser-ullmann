#pragma once

#include <stdlib.h>

typedef struct {
    long *weight;
    long *profit; 
    size_t number_items;
} KnapsackInput;

typedef struct {
    size_t *items;
    size_t amount;
} Solution;

Solution nemhauser_ullmann(KnapsackInput input);