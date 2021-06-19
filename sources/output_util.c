#include <malloc.h>
#include <math.h>
#include "../headers/output_util.h"
#include "../headers/constants.h"

void print_partitions(struct output_for_partition* output){
    printf("/dev/");
    printf("%s", output->d_name);
    printf("\t\tdisk");
    printf("\t\t\t%.3f GB\n", output->size);
    printf("%s", output->information);
}




