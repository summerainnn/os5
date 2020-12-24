#ifndef _MAIN_H_
#define _MAIN_H_
#define LS "ls"
#define MKDIR "mkdir"
#define TOUCH "touch"
#define CP "cp"
#define SHUTDOWN "shutdown"


static char buf[128];
static char* argv[128];
int argc = 0;
char whitespace[] = " \t\r\n\v";//空格字符

/*
* 对输入的命令分割储存在argv中
* 此函数复用了shell实验
*/
void sebuf();


#endif