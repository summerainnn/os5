#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "command.h"
#include "filesystem.h"
#include "color.h"

int main(){
    printf(WHITE"Welcome to the Ext2!\n");
    init_filesystem();
    while(1){
        printf("➜ Ext2 ");
        fgets(buf,128,stdin);
        sebuf();
        if(strcmp(argv[0],LS)==0){
            ls(argv,argc);
        }
        else if(strcmp(argv[0],MKDIR)==0){
            // printf(MKDIR);
            mkdir(argv,argc);
        }
        else if(strcmp(argv[0],TOUCH)==0){
            printf(TOUCH);
        }
        else if(strcmp(argv[0],CP)==0){
            printf(CP);
        }
        else if(strcmp(argv[0],SHUTDOWN)==0){
            printf(SHUTDOWN);
            shutdown();
        }
        else if(strcmp(argv[0],"\n")==0){
            continue;
        }
        else{
            printf(RED"Ext2: command not found: %s\n", argv[0]);
        }
    }
}

void sebuf()
{
    int i = 0; 
    int j = 0;
    if(buf[0]=='\n'){
        argv[0]="\n";
        return;
    }
    //遍历buf对命令进行分割储存
    for (; buf[j] != '\n' && buf[j] != '\0'; j++){
        while (strchr(whitespace,buf[j])){
            j++;
        }
        argv[i++]=buf+j;
        while (strchr(whitespace,buf[j])==0){
            j++;
        }
        buf[j]='\0';
    }
    argv[i]=0;
    argc=i;
}
