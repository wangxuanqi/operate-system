#include<stdio.h>
#include<stdlib.h>

#define SIZEOFDISK 100*1024*1024
#define BLOCK_SIZE 1024                     //盘块大小
#define BLOCK_COUNT SIZEOFDISK/BLOCK_SIZE  //系统盘块数目
//1为目录，0为文件
#define DIRTYPE 1
#define FILETYPE 0


struct  shared_men_st{
    char systemStartAddr[BLOCK_COUNT];
};


void initFsSystem();
//分配文件空间和目录空间
int getBlock(int type);
int resetBlock(int startBlock, int type);
void destroyFsSystem();
char* getBlockAddr(int blockNum);
