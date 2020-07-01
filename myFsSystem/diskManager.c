#include "diskManager.h"

#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

void *share_memory = (void *)0; //share memory
struct shared_men_st *shared_stuff;
void initFsSystem(){
    key_t key = ftok( "/tmp", 66);
    int shmid = shmget(key, SIZEOFDISK, IPC_CREAT|0666);
    if(shmid == -1){
        printf("shmget error!\n");
        exit(1);
    }
    share_memory = shmat(shmid, NULL, 0);
    if(share_memory == -1){
        printf("shmat error!\n");
        exit(1);
    }
    printf("shared memory id: %d\n", shmid);
    printf("memory attached at %X\n", (int)share_memory);
    shared_stuff = (struct shared_men_st*)share_memory;
    printf("shared_stuff addr start at %X\n", shared_stuff);
    //printf("%X", shared_stuff->systemStartAddr);
    //初始化盘块的位示图
    for(int i = 0; i < BLOCK_COUNT; i++)
        shared_stuff->systemStartAddr[i] = '0';
    //用于存放位示图的空间已被占用
    int bitMapSize = BLOCK_COUNT * sizeof(char) / BLOCK_SIZE;//位示图占用盘块数:100
    for(int i=0; i<bitMapSize; i++)//从零开始分配
        shared_stuff->systemStartAddr[i] = '1';   //盘块已被使用
}

//连续分配文件空间和目录空间
int getBlock(int type){
    int startBlock = 0;
    int blockSize;
    if(type == DIRTYPE){
        blockSize = 1; //目录的块数量大小固定为1
    }else if(type == FILETYPE){
        blockSize = 5; //文件的块数量大小固定为5
    }
    //printf("%d", blockSize);
    int sum = 0;
    //实现连续分配
    for(int i = 0;i < BLOCK_COUNT;i++){
	//printf("%c", shared_stuff->systemStartAddr[i]);
        if(shared_stuff->systemStartAddr[i] == '0'){
            if(sum == 0){
                startBlock = i;
            }
	        sum++;
            if(sum == blockSize){
                for(int j = startBlock;j < startBlock+blockSize;j++)
                    shared_stuff->systemStartAddr[j] = '1';
                return startBlock;
            }
        } else {
            sum = 0;
        }
    }
    printf("getBlock failed!\n");
    return -1;
}
int resetBlock(int startBlock, int type){
    int blockSize; 
    if(type == DIRTYPE){
        blockSize = 1; //目录的块数量大小固定为1
    }else if(type == FILETYPE){
        blockSize = 5; //文件的块数量大小固定为5
    }
    for(int i = startBlock;i < startBlock+blockSize;i++){
        shared_stuff->systemStartAddr[i] = '0';
    }
}
void destroyFsSystem(){
    if(shmdt(share_memory) == 1){
        printf("shmdt error!\n");
    }
}

//获取块号地址
char* getBlockAddr(int blockNum){
    return shared_stuff->systemStartAddr+blockNum*BLOCK_SIZE;
}
   
