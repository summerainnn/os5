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
                    printf(WHITE"%s",(rootDir+i)->name);
                }else{
                    printf(BLUE"%s",(rootDir+i)->name);
                }
                printf(WHITE"\t");
            }
        } 
    } 
    
    printf(WHITE"\n");
}

void mkdir(char* argv[], int argc){
    if(argc == 1){
        printf(RED"usage: mkdir directory");
        printf(WHITE"\n");
        return;
    }
    if(superblock->free_inode_count == 0){
        printf(RED"No enough inode !");
        printf(WHITE"\n");
        return;
    }
    int freeInode = find_free_inode();
    int freeBlock = find_free_block();
    
    disk_read_block(2,root_buf);
    argc--;
    while (argc>0)
    {
        if(isNameNotExist(argv[argc],TYPE_DIR)==1){
            creatfile(argv[argc--],TYPE_DIR);
        }else{
            printf(RED"%s already exists",argv[argc--]);
            printf(WHITE"\n");
        } 
    }
}

void touch(char* argv[], int argc){
    if(argc == 1){
        printf(RED"usage: touch file");
        printf(WHITE"\n");
        return;
    }
    if(superblock->free_inode_count == 0){
        printf(RED"No enough inode !");
        printf(WHITE"\n");
        return;
    }
    int freeInode = find_free_inode();
    int freeBlock = find_free_block();
    disk_read_block(2,root_buf);
    argc--;
    while (argc>0)
    {
        if(isNameNotExist(argv[argc],TYPE_FILE)==1){
            creatfile(argv[argc--],TYPE_FILE);
        }else{
            printf(RED"%s already exists",argv[argc--]);
            printf(WHITE"\n");
        }
    }
}

void cp(char* argv[], int argc){
    if(argc != 3){
        printf(RED"usage: cp source_file target_file");
        printf(WHITE"\n");
        return;
    }
    if(isNameNotExist(argv[1],TYPE_FILE)==0){
        if(isNameNotExist(argv[2],TYPE_FILE)==1){
            creatfile(argv[2],TYPE_FILE);
        }else{
            printf(RED"%s already exists",argv[2]);
            printf(WHITE"\n");
        }
    }else{
        printf(RED"%s doesn't exist!",argv[1]);
        printf(WHITE"\n");
    }
    
}

void shutdown(){
    printf("Thanks for using Ext2!\n");
    close_disk();
    exit(0);
}

void creatfile(char* name,int file_type){
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
                struct dir_item newFile;
                (rootDir+i)->valid=VALID;
                (rootDir+i)->type=file_type;
                strcpy((rootDir+i)->name,name);
                (rootDir+i)->inode_id=find_free_inode();
                // block回写
                disk_write_back_block((root+2)->block_point[j],(char *)rootDir);
                return;
            }
        } 
    }
}
int isNameNotExist(char* name,int file_type){
    struct inode *root = (struct inode *)root_buf;
    struct dir_item *rootDir;
    for(int j = 0;j<6;j++){
       rootDir = disk_read_file_block((root+2)->block_point[j]);
        for (int i = 0; i < 8; i++)
        {
            if((rootDir+i)->valid==VALID){
                if(file_type == (rootDir+i)->type){
                    if(strcmp((rootDir+i)->name,name)==0){
                        return 0;
                    }
                }
            }
        } 
    } 
    return 1;
}