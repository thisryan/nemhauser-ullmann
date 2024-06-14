#include <stdio.h>

#include "algorithm.h"
#include "data.h"

int main(int argc, char const* argv []) {
    if(argc < 2){
        fprintf(stderr, "Please provide a filepath \n");
        exit(1);
    }

    // TODO: Support directories
    if(!is_regular_file(argv[1])) {
        fprintf(stderr, "Currently only direct paths to fils are supported \n");
        exit(1);
    }

    KnapsackInput input;
    read_input(argv[1], &input);

    printf("Number: %ld, Cap: %ld \n", input.number_items, input.capacity);

    Solution sol = nemhauser_ullmann(input);
    printf("Result: %ld\n", sol.profit);

    free(input.profit);
    free(input.weight);


    return 0;
}
