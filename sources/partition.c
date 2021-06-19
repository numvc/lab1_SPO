#include "../headers/partition.h"


char* get_fs(char* path) {

    FILE* fd;
    int flag = 0;
    char* dev_path = strdup(path);
    size_t len = LINE_MAX * sizeof (char);
    size_t path_len = PATH_MAX * sizeof (char);
    char* fs_path = new_string(path_len);
    char* result = new_string(len);
    char* number = malloc(len);
    char* output = malloc(len);
    char* FS_TYPE_PROPERTY = "E:ID_FS_TYPE";

    strcat(fs_path, "/run/udev/data/b");
    strcat(dev_path, "/dev");


    fd = fopen(dev_path, "rb");
    getline(&number, &len, fd);
    fclose(fd);

    number[strlen(number) - 1] = '\0';

    strcat(fs_path, number);
    fd = fopen(fs_path, "rb");
    while (getline(&output, &len, fd) != -1){
        if (start_with(output, FS_TYPE_PROPERTY) == 1) {
            size_t result_len = strlen(FS_TYPE_PROPERTY) + 1;
            strcat(result, output);
            flag = 1;
            result += result_len;
        }
    }
    if (flag == 0) strcat(result, "\n");
    free(fs_path);
    free(output);
    free(number);
    fclose(fd);
    return result;
}

double get_size(char* path){
    unsigned long long result;
    int fd = open(path, O_RDONLY);
    ioctl(fd,  BLKGETSIZE64, &result);
    close(fd);
    return (double) result / (1024 * 1024 * 1024);
}

struct output_for_partition* get_partitions() {
    DIR* sys_dir = opendir("/sys/block/");
    struct dirent* sys_entry;
    char sub_dir_name[PATH_MAX] = {'\0'};
    struct output_for_partition* out = malloc(sizeof(struct output_for_partition));
    struct output_string* outputString = malloc(sizeof(struct output_string));
    outputString->result = new_string(1024);
    outputString->size_string = 1024;
    outputString->free_space = 1024;
    if (sys_dir != NULL) {
        sys_entry = readdir(sys_dir);
        while (sys_entry != NULL) {
            if (start_with(sys_entry->d_name, "sd")) {
                strcpy(sub_dir_name, "/sys/block/");
                char* path = new_string(PATH_MAX);
                char* path_for_fs = new_string(PATH_MAX);

                strcat(path, "/dev/");
                strcat(path, sys_entry->d_name);

                strcat(path_for_fs, "/sys/block/");
                strcat(path_for_fs, sys_entry->d_name);
                strcat(path_for_fs, "/");

                strcat(sub_dir_name, sys_entry->d_name);

                out->d_name = strdup(sys_entry->d_name);
                out->size = get_size(path);


                DIR* sys_sub_dir = opendir(sub_dir_name);
                if (sys_sub_dir != NULL) {
                    struct dirent* sys_sub_entry = readdir(sys_sub_dir);
                    while (sys_sub_entry != NULL) {
                        if (start_with(sys_sub_entry->d_name, "sd")) {

                            char* size = malloc(20 * sizeof (char));
                            memset(size, 0, strlen(size));
                            memset(path, 0, PATH_MAX);

                            add_string(outputString, "/dev/");
                            strcat(path, "/dev/");
                            strcat(path, sys_sub_entry->d_name);

                            strcat(path_for_fs, sys_sub_entry->d_name);
                            ftoa(get_size(path), size, 3);

                            add_string(outputString, sys_sub_entry->d_name);
                            add_string(outputString, "\t\tpartition\t\t\t");
                            add_string(outputString, size);
                            add_string(outputString, "\t\t\t\t");
                            add_string(outputString, get_fs(path_for_fs));
                            add_string(outputString, "");

                            free(size);

                            path_for_fs[strlen(path_for_fs) - strlen(sys_sub_entry->d_name)] = '\0';
                        }
                        sys_sub_entry = readdir(sys_sub_dir);
                    }
                } else {
                   printf("Не удалось прочесть.\n");
                }
                free(path);
            }
            sys_entry = readdir(sys_dir);
        }

    } else {
        printf("Не удалось прочесть.\n");
    }
    out->information = outputString->result;
    return out;
}