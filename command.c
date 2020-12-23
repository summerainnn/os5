#include "command.h"
#include "filesystem.h"
#include "disk.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ls(char* argv[], int argc){
    disk_read_block(2,root_buf);
    struct inode *root = (struct inode *)root_buf;
    struct dir_item *rootDir;
    for(int j = 0;j<6;j++){
       rootDir = disk_read_file_block((root+2)->block_point[j]);
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
    } 
    
    printf(WHITE"\n");
}

void mkdir(char* argv[], int argc){
    if(superblock->free_inode_count == 0){
        printf(RED"No enough inode !");
        printf(WHITE"\n");
    }
    int freeInode = find_free_inode();
    int freeBlock = find_free_block();
    
    disk_read_block(2,root_buf);
    argc--;
    while (argc>0)
    {
        creatfile(argv[argc--]);
    }
}

void touch(char* argv[], int argc){
    if(superblock->free_inode_count == 0){
        printf(RED"No enough inode !");
        printf(WHITE"\n");
    }
    int freeInode = find_free_inode();
    int freeBlock = find_free_block();
    disk_read_block(2,root_buf);
    struct inode *root = (struct inode *)root_buf;
    

}

void cp(char* argv[], int argc){
    
}

void shutdown(){
    printf("Thanks for using Ext2!\n");
    close_disk();
    exit(0);
}

void creatfile(char* name){
    struct inode *root = (struct inode *)root_buf;
    struct dir_item *rootDir; 
    for(int j = 0; j< 6;j++){
        if((root+2)->block_point[j]==0){
            (root+2)->block_point[j] = find_free_block();
            disk_write_block(2,root_buf);
        }
        rootDir = disk_read_file_block((root+2)->block_point[j]);
       for (int i = 0; i < 8; i++)
        {
            if((rootDir+i)->valid == INVALID){
                printf("kongxian i:%d\n",i);
                struct dir_item newFile;
                (rootDir+i)->valid=VALID;
                (rootDir+i)->type=TYPE_DIR;
                strcpy((rootDir+i)->name,name);
                (rootDir+i)->inode_id=find_free_inode();
                // block回写
                printf("回写：%d\n",(root+2)->block_point[j]);
                disk_write_back_block((root+2)->block_point[j],(char *)rootDir);
                return;
            }
        } 
    }
}