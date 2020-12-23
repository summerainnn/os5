#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H
#include <stdio.h>

char root_buf[512];

void ls(char* argv[], int argc);
void mkdir(char* argv[], int argc);
void touch(char* argv[], int argc);
void cp(char* argv[], int argc);
void shutdown();

#endif