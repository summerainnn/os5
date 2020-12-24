#include "disk.h"
#include "filesystem.h"
#include <stdio.h>

inline int get_disk_size()
{
        return 4*1024*1024; 
}

static FILE* disk;

static int create_disk()
{
        FILE* tmp = fopen("disk","w");
        for(int i = 0; i < get_disk_size(); i++){
                // 写入0（1个Byte），并把tmp向后移动
                fputc(0,tmp);
        }
        fclose(tmp);
}

int open_disk()
{
        if(disk != 0){
                return -1;
        }
        disk = fopen("disk","r+");
        if(disk == 0){
                create_disk();
                disk = fopen("disk","r+");
                if(disk == 0){
                        return -1;
                }
        }
        return 0;
}

int disk_read_block(unsigned int block_num, char* buf)
{
        if(disk == 0){
                return -1;
        }
        if(block_num * DEVICE_BLOCK_SIZE >= get_disk_size()){
                return -1;
        }
        if(fseek(disk, block_num * DEVICE_BLOCK_SIZE, SEEK_SET)){
                return -1;
        }
        if(fread(buf, DEVICE_BLOCK_SIZE,1,disk) != 1){
                return -1;
        }
        return 0;
}

int disk_write_block(unsigned int block_num, char* buf)
{
        // printf("something change in block_num:%d\n",block_num);
        // if(block_num==0){
        //         printf("magic_num:%x\n",((sp_block *)buf)->magic_num);
        //         printf("magic_num:%x\n",((sp_block *)buf)->inode_map[0]);
        //         printf("magic_num:%x\n",((sp_block *)buf)->block_map[0]);
        //         printf("magic_num:%x\n",((sp_block *)buf)->free_inode_count);
        //         printf("magic_num:%x\n",((sp_block *)buf)->free_block_count);
        // }
        if(disk == 0){
                return -1;
        }
        if(block_num * DEVICE_BLOCK_SIZE >= get_disk_size()){
                return -1;
        }
        // 文件指针定向到block_num * 512处
        if(fseek(disk, block_num * DEVICE_BLOCK_SIZE, SEEK_SET)){
                return -1;
        }
        // 将buf指针后的512字节内容写入到disk文件指针所知的位置
        if(fwrite(buf,DEVICE_BLOCK_SIZE,1,disk) != 1){
                return -1;
        }
        return 0;
}

int close_disk()
{
        if(disk == 0){
                return -1;
        }
        int r = fclose(disk);
        disk = 0;
        return r;
}