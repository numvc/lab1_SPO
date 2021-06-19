#ifndef LABSPO1_OUTPUT_UTIL_H
#define LABSPO1_OUTPUT_UTIL_H

#include "stdio.h"
#include "string.h"

struct output_for_partition {
    char* d_name;
    double size;
    char* information;

};

struct output_string {
    int size_string;
    int free_space;
    char* result;
};

void print_partitions(struct output_for_partition* output);

#endif
