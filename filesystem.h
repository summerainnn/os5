#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <stdint.h>
#define TYPE_DIR 0
#define TYPE_FILE 1
#define VALID 1
#define INVALID 0



typedef struct super_block {
    int32_t magic_num;                  // 幻数
    int32_t free_block_count;           // 空闲数据块数
    int32_t free_inode_count;           // 空闲inode数
    int32_t dir_inode_count;            // 目录inode数
    uint32_t block_map[128];            // 数据块占用位图
    uint32_t inode_map[32];             // inode占用位图
} sp_block;

sp_block * superblock;

struct inode {
    uint32_t size;              // 文件大小
    uint16_t file_type;         // 文件类型（文件/文件夹）
    uint16_t link;              // 连接数
    uint32_t block_point[6];    // 数据块指针
};

struct inode inode[1024];

struct dir_item {               // 目录项一个更常见的叫法是 dirent(directory entry)
    uint32_t inode_id;          // 当前目录项表示的文件/目录的对应inode
    uint16_t valid;             // 当前目录项是否有效 
    uint8_t type;               // 当前目录项类型（文件/目录）
    char name[121];             // 目录项表示的文件/目录的文件名/目录名
};

static char block_buf[1024];
static char spbuf[1024];

/*
* 初始化文件系统
*/
void init_filesystem();
/*
* 寻找空闲的inode
* -1 error inode索引
*/
int find_free_inode();
int find_free_block();
struct dir_item * disk_read_file_block(int blockNum);
void disk_write_back_block(int blockNum,char* buf);

#endif