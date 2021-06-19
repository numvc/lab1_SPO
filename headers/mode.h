#ifndef LABSPO1_MODE_H
#define LABSPO1_MODE_H

#include "getopt.h"
#include "string.h"
#include "stdio.h"

enum modes {
    UNDEFINED = 0,
    PARTITIONS = 1,
    EXT3 = 2
};

struct program {
    enum modes mode;
    char* path;
};

struct program parse_args(const int argc, char** argv);

#endif
