#ifndef LABSPO1_STATE_H
#define LABSPO1_STATE_H

#include "limits.h"
#include "structures.h"

struct state {
    char device_path[PATH_MAX];
    char current_path[PATH_MAX];
    unsigned int current_inode;
    int block_size;
    struct ext_super_block superBlock;
    struct ext_group_desc groupDesc;
};

#endif //LABSPO1_STATE_H
