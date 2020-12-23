#include "command.h"
#include "filesystem.h"
#include "disk.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ls(char* argv[], int argc){
    // char buf[512];
    // struct dir_item *temp;
    // int j =0;
    
    // for (int i = 0;i<6||j<inode->link; i++)
    // {
    //     disk_read_block(inode[2].block_point[i]*2,buf);
    //     temp = (struct dir_item*)buf;
    //     if(temp->valid == VALID){
            // if(temp->type == TYPE_FILE){
            //     printf(WHITE"%s\t",temp->name);
            // }else{
            //     printf(BLUE"%s",temp->name);
            // }
            // printf(WHITE"\t");
    //         j++;
    //     }
        
    // }
    // printf(WHITE"\n");
    disk_read_block(2,root_buf);
    struct inode *root = (struct inode *)root_buf;
    struct dir_item *rootDir; 
    rootDir = disk_read_file_block((root+2)->block_point[0]);
    for (int i = 0; i < 8; i++)
    {
        if((rootDir+i)->valid==VALID){
            if((rootDir+i)->type == TYPE_FILE){
                // printf(WHITE"%s\t,%d",(rootDir+i)->name,i);
            }else{
                printf(BLUE"%s,%d",(rootDir+i)->name,i);
            }
            printf(WHITE"\t");
        }
    }
    printf(WHITE"\n");
}

void mkdir(char* argv[], int argc){
    printf("?%d",superblock->free_inode_count);
    if(superblock->free_inode_count == 0){
        printf(RED"No enough inode !");
        printf(WHITE"\n");
    }
    int freeInode = find_free_inode();
    int freeBlock = find_free_block();
    
    disk_read_block(2,root_buf);
    // disk_read_block(3,root_buf+512);
    // 读取第一个
    struct inode *root = (struct inode *)root_buf;
    // printf("point:%d\n",(root+2)->block_point[0]);
    struct dir_item *rootDir; 
    rootDir = disk_read_file_block((root+2)->block_point[0]);
    printf("rootDir:%d\n",rootDir->valid);
    for (int i = 0; i < 111; i++)
    {
        if((rootDir+i)->valid == INVALID){
            printf("kongxian i:%d\n",i);
            struct dir_item newFile;
            (rootDir+i)->valid=VALID;
            (rootDir+i)->type=TYPE_DIR;
            strcpy((rootDir+i)->name,argv[1]);
            (rootDir+i)->inode_id=freeInode;
            // block回写
            printf("回写：%d\n",(root+2)->block_point[0]);
            disk_write_back_block((root+2)->block_point[0],(char *)rootDir);
            break;
        }
    }
    
    
    
}

void touch(char* argv[], int argc){

}

void cp(char* argv[], int argc){

}

void shutdown(){
    printf("Thanks for using Ext2!\n");
    close_disk();
    exit(0);
}