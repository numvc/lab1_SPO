#ifndef LAB1_UTILS_H
#define LAB1_UTILS_H

#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "math.h"
#include "output_util.h"

char* new_string(size_t size);
int start_with(char* src, char* substr);
void add_string(struct output_string* output, char* string);
void ftoa(float n, char* res, int afterpoint);

#endif

