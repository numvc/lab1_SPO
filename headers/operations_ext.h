#ifndef LABSPO1_OPERATIONS_EXT_H
#define LABSPO1_OPERATIONS_EXT_H

#include <ftw.h>
#include <stdio.h>
#include "utils.h"
#include "../headers/constants.h"
#include "structures.h"
#include "filesystem_utils.h"
void execute_ext_cp(struct state* fs_state, char* src, char* target);
char* execute_ls(struct state* fs_state);
char* execute_pwd(struct state* fs_state);
char* execute_ext_cd(struct state* fs_state, char* target);
char* execute_help();
#endif
