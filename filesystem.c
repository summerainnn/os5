#include "filesystem.h"
#include <string.h>
#include <stdio.h>
#include "disk.h"
#include "color.h"
#include "command.h"
#include <stdint.h>

void init_filesystem(){
    if(open_disk()!=0){
        printf("Open Disk Error !");
    }
    disk_read_block(0,spbuf);
    disk_read_block(1,spbuf+512);
    superblock = (sp_block*)spbuf;
    
    if(superblock->magic_num==0x2a49fdb1){
        printf("Read the system successfully\n");
        if(disk_read_block(2,(char *)inode)!=0){
            printf("Read Inode Error!");
        }
    }else{
        // 初始化超级块
        superblock->magic_num = 0x2a49fdb1;
        superblock->free_block_count = 4096 - 34;
        // 1 for super block, 32 for inode table, 1 for folder "root".
        superblock->free_inode_count = 1024;
        superblock->dir_inode_count  = 0;
        memset(superblock->block_map,0,sizeof(superblock->block_map));
        memset(superblock->inode_map,0,sizeof(superblock->inode_map));
        superblock->inode_map[0]=0x7;
        char buf_inode[512];
        struct inode *inode_root = (struct inode *)buf_inode;
        (inode_root+2)->file_type=TYPE_DIR;
        (inode_root+2)->link=1;
        (inode_root+2)->block_point[0] = 34;
        disk_write_block(0,spbuf);
        disk_write_block(1,spbuf+512);
        disk_write_block(2,buf_inode);
        printf("The system was initialized successfully\n");
    }
}

int find_free_inode(){
    for(int i=0;i<32;i++){
        if(superblock->inode_map[i] == 0xffffffff){
            continue;
        }
        uint32_t map = superblock->inode_map[i];
        for (int j = 0; j < 32; j++)
        {
            if((map & 1) == 0){
                superblock->free_inode_count = superblock->free_inode_count - 1;
                superblock->inode_map[i] = superblock->inode_map[i] | ((uint32_t)1<<j);
                disk_write_back_block(0,(char *)superblock);
                return i*32 + j;
            }
            map = map >> 1;
        }
    }
    printf(RED"NO Enough inode");
    shutdown();
    return -1;
}

int find_free_block(){
    for(int i=0;i<128;i++){
        if(superblock->block_map[i] == 0xffffffff){
            continue;
        }
        uint32_t map = superblock->block_map[i];
        for (int j = 0; j < 32; j++)
        {
            if((map & 1) == 0){
                superblock->free_block_count = superblock->free_block_count - 1;
                superblock->block_map[i] = superblock->block_map[i] | ((uint32_t)1<<j);
                disk_write_back_block(0,(char *)superblock); 
                return i*128 + j + 34;
            }
            map = map >> 1;
        }
    }
    printf(RED"NO Enough Block");
    shutdown();
    return -1;

}

struct dir_item * disk_read_file_block(int blockNum){
    disk_read_block(blockNum*2,block_buf);
    disk_read_block(blockNum*2+1,block_buf + 512);
    return (struct dir_item *)block_buf;
}

void disk_write_back_block(int blockNum,char* buf){
    disk_write_block(blockNum*2,buf);
    disk_write_block(blockNum*2+1,buf+512);
}

