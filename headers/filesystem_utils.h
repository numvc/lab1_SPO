#ifndef LAB1_FILESYSTEM_UTILS_H
#define LAB1_FILESYSTEM_UTILS_H
#include "../headers/structures.h"
#include <fcntl.h>
#include <unistd.h>
#include "stdlib.h"
#include "string.h"
#include <stdio.h>
#include "../headers/constants.h"
#include "../headers/state.h"

char* fgets_wrapper(char *buffer, size_t buflen, FILE *fp);
struct ext_group_desc read_group_descriptions(struct state* fs_state);
void read_inode(struct state* fs_state, int64_t inode_num, struct ext_inode* inode);
struct ext_dir* read_block(struct state* fs_state, struct ext_inode* inode);
struct state* setup(char* device_path);
int read_inode_filename(struct state* fs_state, char* filename);
void append_new_path(char *path, char *part);
char* read_data(struct state* fs_state, struct ext_inode* inode, int index);

#endif
