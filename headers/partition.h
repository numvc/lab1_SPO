#ifndef LABSPO1_PARTITION_H
#define LABSPO1_PARTITION_H
#include "dirent.h"
#include <stddef.h>
#include "stdio.h"
#include "string.h"
#include "limits.h"
#include <malloc.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "utils.h"
#include "constants.h"
#include "output_util.h"

struct output_for_partition* get_partitions();
#endif