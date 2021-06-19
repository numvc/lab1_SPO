#ifndef LAB1SPO_STRUCTURES_H
#define LAB1SPO_STRUCTURES_H
#include "linux/types.h"
#include <stdint.h>
struct ext_dir {

    __le32 inode;
    __le16 rec_len;
    __u8 lenthg;
    char name[255];
};

struct ext_group_desc{
    __u32  bg_inode_table;
};

struct ext_inode {
    __le16 i_mode;
    __le32 i_size;
    union {
        struct {
            __le32 l_i_reserved1;
        } linux1;
        struct {
            __le32 h_i_translator;
        } hurd1;
        struct {
            __le32 m_i_reserved1;
        } masix1;
    } osd1;
    __le32 i_block[15];
    union {
        struct {
            __u8 l_i_frag;
            __u8 l_i_fsize;
            __u16 i_pad1;
            __le16 l_i_uid_high;
            __le16 l_i_gid_high;
            __u32 l_i_reserved2;
        } linux2;
        struct {
            __u8 h_i_frag;
            __u8 h_i_fsize;
            __le16 h_i_mode_high;
            __le16 h_i_uid_high;
            __le16 h_i_gid_high;
            __le32 h_i_author;
        } hurd2;
        struct {
            __u8 m_i_frag;
            __u8 m_i_fsize;
            __u16 m_pad1;
            __u32 m_i_reserved2[2];
        } masix2;
    } osd2;
};

struct ext_super_block {
    __u32 s_log_block_size;
    __u32 s_blocks_per_group;
    __u32 s_inodes_per_group;
    __u16 s_magic;
    __u16 s_inode_size;
    char s_last_mounted[64];
};



#endif