#include "../headers/filesystem_utils.h"
void append_new_path(char *path, char *part) {
    strcat(path, "/");
    strcat(path, part);
}

int check_power(unsigned int target, unsigned  int power){
    while (target != 1){
        if (target % power != 0) {
            return 0;
        }
        target = target/power;
    }
    return 1;
}
unsigned int get_inode_group_offset(int numb_group, struct state* fs_state) {
    if (numb_group == 0 || check_power(numb_group, 3) == 1 || check_power(numb_group, 5) == 1 || check_power(numb_group, 7) == 1) {
        return numb_group * fs_state->superBlock.s_blocks_per_group * fs_state->block_size + fs_state->groupDesc.bg_inode_table * fs_state->block_size;
    }
    return fs_state->block_size * numb_group * fs_state->superBlock.s_blocks_per_group + 2 * fs_state->block_size;
}

unsigned int get_inode_offset(int numb_group, int numb_inode, struct state* fs_state) {
    if (numb_group == 0 && numb_inode < sys_inodes_count) {
            return numb_inode * fs_state->superBlock.s_inode_size / 2;
    }else{
        return (numb_inode - 1) * fs_state->superBlock.s_inode_size;
    }
}

unsigned int block_offset(int block, struct state* fs_state) {
    return block * fs_state->block_size;
}


struct ext_super_block read_super_block(char* path) {
    int fd;
    struct ext_super_block superBlock;

    fd = open(path, O_RDONLY);

    lseek(fd, base_offset, SEEK_SET);
    read(fd, &superBlock, sizeof (struct ext_super_block));

    close(fd);

    return superBlock;
}

struct ext_group_desc read_group_descriptions(struct state* fs_state){

    int fd;
    struct ext_group_desc groupDesc;

    fd = open(fs_state->device_path, O_RDONLY);

    lseek(fd, fs_state->block_size, SEEK_SET);
    read(fd, &groupDesc, sizeof (struct ext_group_desc));

    close(fd);
    return groupDesc;
}

char* fgets_wrapper(char *buffer, size_t buflen, FILE *fp);

int read_inode_filename(struct state* fs_state, char* filename) {
    unsigned int inode_file = -1;
    struct ext_inode cur_inode;
    struct ext_dir* dir;
    unsigned int size = 0;
    void* ptr;

    read_inode(fs_state, fs_state->current_inode, &cur_inode);
    dir = read_block(fs_state, &cur_inode);
    ptr = dir;

    while (dir->lenthg != 0){
        char file_name[FILENAME_MAX];
        memcpy(file_name, dir->name, dir->lenthg);
        file_name[dir->lenthg] = 0;
        if (strcmp(file_name,filename) == 0){
            inode_file = dir->inode;
            break;
        }
        dir = (void *) dir + dir->rec_len;
        size += dir->rec_len;
    }
    free(ptr);
    return inode_file;

}

void read_inode(struct state* fs_state, int64_t inode_num, struct ext_inode* inode) {
    unsigned int num_group = inode_num / fs_state->superBlock.s_inodes_per_group;
    inode_num = inode_num % fs_state->superBlock.s_inodes_per_group;
    int fd = open(fs_state->device_path, O_RDONLY);

    unsigned  int offset_block =
            get_inode_group_offset(num_group, fs_state) + get_inode_offset(num_group, inode_num, fs_state);

    lseek(fd, offset_block, SEEK_SET);
    read(fd, inode, sizeof(struct ext_inode));

    close(fd);
}

char* read_data(struct state* fs_state, struct ext_inode* inode, int index) {
    int fd = open(fs_state->device_path, O_RDONLY);
    char* block = malloc(fs_state->block_size);

    lseek(fd, block_offset(inode->i_block[index], fs_state), SEEK_SET);
    read(fd, block, fs_state->block_size);

    close(fd);
    return block;
}

struct ext_dir* read_block(struct state* fs_state, struct ext_inode* inode) {
    int fd = open(fs_state->device_path, O_RDONLY);
    unsigned char* block = malloc(fs_state->block_size);
    struct ext_dir* entry;

    lseek(fd, block_offset(inode->i_block[0], fs_state), SEEK_SET);
    read(fd, block, fs_state->block_size);

    entry = (struct ext_dir*) block;
    close(fd);
    return entry;
}

struct state* setup(char* device_path) {
    struct ext_super_block superBlock = read_super_block(device_path);
    struct ext_group_desc groupDesc;
    struct state* fs_state = NULL;

    if (superBlock.s_magic == magic_num) {
        int block_size = default_block_size << superBlock.s_log_block_size;
        fs_state = malloc(sizeof (struct state));
        fs_state->block_size = block_size;
        strcat(fs_state->current_path, superBlock.s_last_mounted);
        strcat(fs_state->device_path, device_path);
        fs_state->superBlock = superBlock;
        fs_state->current_inode = root_inode;

        groupDesc = read_group_descriptions(fs_state);
        fs_state->groupDesc = groupDesc;
    }else printf("Неизвестная файловая система.\n");
    return fs_state;
}
