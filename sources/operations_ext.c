#include "../headers/operations_ext.h"

char* get_path(char* target){
    char* final_path = malloc(strlen(target));
    strcpy(final_path, target);
    long size = strlen(target);
    while (final_path[size] != '/') {
        size --;
    }
    final_path[size] = '\0';
    return final_path;
}


char* execute_ls(struct state* fs_state){
    struct ext_inode inode;
    struct ext_dir *entry;
    void* ptr;
    char* result;
    unsigned int num_inode = fs_state->current_inode;
    struct output_string* outputString = malloc(sizeof(struct output_string));
    outputString->result = new_string(1024);
    outputString->size_string = 1024;
    outputString->free_space = 1024;

    read_inode(fs_state, num_inode, &inode);
    entry = read_block(fs_state, &inode);
    ptr = entry;

    unsigned int size;
    size = 0;
    while (entry->lenthg){
        char file_name[FILENAME_MAX];
        memcpy(file_name, entry->name, entry->lenthg);
        file_name[entry->lenthg] = 0;
        add_string(outputString, "\t");
        add_string(outputString, file_name);
        add_string(outputString, "\n");
        entry = (void *) entry + entry->rec_len;
        size += entry->rec_len;
    }
    free(ptr);
    result = outputString->result;
    free(outputString);
    return result;
}

void execute_cd_back(struct state* fs_state) {
    struct ext_inode inode;
    struct ext_dir *entry;
    void* ptr;
    read_inode(fs_state, fs_state->current_inode, &inode);

    entry = read_block(fs_state, &inode);
    ptr = entry;
    entry = (void *) entry + entry->rec_len;

    if (fs_state->current_inode != entry->inode){
        fs_state->current_inode = entry->inode;
        char* final_path = get_path(fs_state->current_path);
        memset(fs_state->current_path, 0, strlen(fs_state->current_path));
        strcat(fs_state->current_path, final_path);
    }

    free(ptr);
}

char* execute_cd(struct state* fs_state, char* target) {

    if (strcmp("..", target) == 0) execute_cd_back(fs_state);
    else {
        unsigned int size;
        void *ptr;
        struct ext_dir *entry;
        char symbol[1] = {'/'};
        int flag = 0;

        struct ext_inode inode;
        read_inode(fs_state, fs_state->current_inode, &inode);

        entry = read_block(fs_state, &inode);
        ptr = entry;
        size = 0;
        while (entry->lenthg != 0) {
            char file_name[FILENAME_MAX];
            memcpy(file_name, entry->name, entry->lenthg);
            file_name[entry->lenthg] = 0;
            if (strcmp(file_name, target) == 0) {
                fs_state->current_inode = entry->inode;
                strcat(fs_state->current_path, symbol);
                flag = 1;
                break;
            }
            entry = (void *) entry + entry->rec_len;
            size += entry->rec_len;
        }
        free(ptr);
        if (flag == 0) return "Не удалось найти директорию ";
        return NULL;
    }
    return NULL;
}

char* execute_ext_cd(struct state* fs_state, char* target) {
    char* result = NULL;
    if (strcmp(".", target) != 0 && strcmp("./", target) != 0){
        char* copy_target = strdup(target);
        copy_target = strtok(copy_target, "/");
        while (copy_target != NULL) {
            result = execute_cd(fs_state, copy_target);
            if (result != NULL) return result;
            copy_target = strtok(NULL, "/");
        }
    }
    return NULL;
}

void cp_file(struct state* fs_state, struct ext_inode* inode, char* target){
    char* block;
    int fd = open(target, O_RDWR | O_APPEND | O_CREAT, file_mode);
    int i = 0;
    int index = 0;

    while (i < inode->i_size) {
    	int curr_block_size = fs_state->block_size;
    	if(inode->i_size - i < fs_state->block_size){
    		curr_block_size = inode->i_size - i;
    	}
    	block = read_data(fs_state, inode, index);
    	write(fd, block, curr_block_size);
     	i += curr_block_size;
    	index ++;
    }
}

void cp_dir(struct state* fs_state, struct ext_inode* inode, char* target){
    struct ext_dir* dir;
    struct ext_inode tmp_inode;
    char* path;
    void* ptr;

    dir = read_block(fs_state, inode);
    ptr = dir;
    struct stat directory = {0};
    if (stat(target, &directory) == -1){
        mkdir(target, file_mode);
    }

    unsigned int size;
    size = 0;
    while (dir->lenthg != 0){
        char file_name[FILENAME_MAX];
        memcpy(file_name, dir->name, dir->lenthg);
        file_name[dir->lenthg] = 0;
        if (strcmp(file_name, ".") != 0 && strcmp(file_name, "..") != 0){
            read_inode(fs_state, dir->inode, &tmp_inode);
            path = malloc(PATH_MAX);
            memset(path, 0, sizeof (char ) * PATH_MAX);
            strcat(path, target);
            append_new_path(path, file_name);
            if (S_ISDIR(tmp_inode.i_mode)){
                cp_dir(fs_state, &tmp_inode, path);
            }else if (S_ISREG(tmp_inode.i_mode)){
                cp_file(fs_state, &tmp_inode, path);
            }
        }
        dir = (void *) dir + dir->rec_len;
        size += dir->rec_len;
    }
    free(ptr);
}

char* change_path(char* target) {
    setenv("SPO", home_path, 1);
    char* home = getenv("SPO");
    char* copy_path = strdup(target);
    char* result_path = malloc(strlen(target));
    memset(result_path, 0, strlen(result_path));
    copy_path = strtok(copy_path, "/");
    while (copy_path != NULL){
        if (strcmp("~", copy_path) == 0) {
            strcat(result_path, home);
        }else{
            append_new_path(result_path, copy_path);
        }
        copy_path = strtok(NULL, "/");
    }
    return result_path;
}

void execute_ext_cp(struct state* fs_state, char* src, char* target) {
    unsigned int inode_num;
    struct ext_inode inode;

    char* path;
    char* abs_path = change_path(target);
    inode_num = read_inode_filename(fs_state, src);
    if (inode_num != -1 && abs_path != NULL){
        read_inode(fs_state, inode_num, &inode);
        path = malloc(PATH_MAX*sizeof (char));
        memset(path, 0, PATH_MAX * sizeof (char));
        strcat(path, abs_path);
        append_new_path(path, src);
        if (S_ISDIR(inode.i_mode)){
            cp_dir(fs_state, &inode, path);
        }else if (S_ISREG(inode.i_mode)){
            cp_file(fs_state, &inode, path);
        }
    }
    free(abs_path);
}



char* execute_pwd(struct state* fs_state){
    return fs_state->current_path;
}

char* execute_help(){
    char* result;
    struct output_string* outputString = malloc(sizeof(struct output_string));
    outputString->result = new_string(1024);
    outputString->size_string = 1024;
    outputString->free_space = 1024;
    add_string(outputString, "ls - показывает содержимое директории.\n");
    add_string(outputString, "cp filename path - скопировать указанный файл.\n");
    add_string(outputString, "pwd  - вывести путь файла\n");
    add_string(outputString, "cd filename - перейти в указанную директорию\n");
    add_string(outputString, "quit - завершить работу");
    result = outputString->result;
    free(outputString);
    return result;
}
