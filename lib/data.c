#include "data.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int is_regular_file(const char* path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}


int read_input(const char* filename, KnapsackInput* input) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file! \n");
        exit(1);
    }

    int amount, capacity;
    fscanf(file, "%d %d\n", &amount, &capacity);

    input->capacity = capacity;
    input->number_items = amount;
    input->profit = malloc(amount * sizeof(long));
    input->weight = malloc(amount * sizeof(long));
    for (int i = 0;i < amount;i++) {
        fscanf(file, "%ld %ld\n", input->profit + i, input->weight + i);
    }

    fclose(file);
}