#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <semaphore.h>
#include "diskManager.h"

#define maxLengthOfFileName 20
#define maxLengthOfDir 50
#define maxLengthOfContent 4*BLOCK_SIZE
#define DIRBLOCKSIZE 1 //默认目录大小1个块
#define FILEBLOCKSIZE 5 //默认目录大小5个块

struct file
{
    int type; 
    char fileName[maxLengthOfFileName]; //文件名称
    int fileSize;
    int startBlock;//起始块号
};

struct dir
{
    int lengthOfDir;    //目录列表长度
    int startBlock;     //起始块号
    struct file dirList[maxLengthOfDir];  //目录列表数组
};
struct fileMemory
{
    int startBlock;
    int dataSize;
    char content[maxLengthOfContent];
    /* 信号量 */
    sem_t *read_sem; //写
    sem_t *write_sem;//读
};


int initRootDir();
int _mkdir(char fileName[]);    //创建目录
int _rmdir(char fileName[]);    //删除目录
int _rename(char oldName[], char newName[]);    //修改名称
int _touch(char fileName[]);    //创建文件
int _rm(char fileName[]);   //删除文件
int _ls();      //查看文件系统目录结构
int _cd(char fileName[]);//跳转目录
int _change();  //修改文件，覆盖之前的文件内容
int _write();   //写文件，接着文件内容后面写
int _read();    //读文件
int creatFileMemory(int blockNum);
int addDir(struct dir *dirtable, char fileName[], int type, int startBlock);
int deleteDir(struct dir* dirtable, int index);
int findIndexOf(struct dir *dirtable, char fileName[]);
void printPath();//输出当前路径
