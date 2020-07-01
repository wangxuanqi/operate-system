#include<string.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>


#include "file.h"


// const char * _read_sem = "read_sem";
// const char * _write_sem = "write_sem";
struct dir *currenDir;
char path[100];


//初始化根目录
int initRootDir(){
    int dirBlockNum = getBlock(DIRTYPE);
    //printf("dir: %d\n", dirBlockNum);
    if(dirBlockNum == -1){
        printf("init root dir failed!\n");
        return -1;
    }
    currenDir = (struct dir*)getBlockAddr(dirBlockNum);
    currenDir->lengthOfDir = 0;
    currenDir->startBlock = dirBlockNum;
    strcat(path, "wangxuanqi2017153006");//根目录名称
    strcat(path, "/");
}
//创建目录
int _mkdir(char fileName[]){
    //文件名称长度不能超出最大长度
    if(strlen(fileName) >= maxLengthOfFileName){
        printf("file name is too long!\n");
        return 0;
    }
    //获取分配的盘块号
    int dirBlockNum = getBlock(DIRTYPE);
    if(dirBlockNum == -1)
        return -1;
    if(addDir(currenDir, fileName, DIRTYPE, dirBlockNum) == -1){  //在现有目录的子目录列表中加入创建的新目录
        return -1;
    }
    struct dir *newDir = (struct dir*)getBlockAddr(dirBlockNum);//初始化起始地址为列表结构
    newDir->lengthOfDir = 0;
    //在新创建的目录中加入父目录
    if(addDir(newDir, "..", DIRTYPE, currenDir->startBlock) == -1)
        return -1;
    return 0;
}
int _rmdir(char fileName[]){
    if(strcmp(fileName, "..") == 0){
        printf(".. can't delete!\n");
        return -1;
    }
    int index = findIndexOf(currenDir, fileName);
    if(index == -1){
        printf("the dir can't be found!\n");
        return -1;
    }
    if(currenDir->dirList[index].type == FILETYPE){
        printf("it isn't a dir!\n");
        return -1;
    }
    resetBlock(currenDir->dirList[index].startBlock, DIRBLOCKSIZE);//释放目录内存空间
    deleteDir(currenDir, index);//从当前目录中移除
    return 0;
}
//修改文件名称
int _rename(char oldName[], char newName[]){
    int index = findIndexOf(currenDir, oldName);
    //文件没有找到
    if(index == -1){
        printf("the file can't be found!\n");
        return -1;
    }
    //父目录名称不能修改
     if(strcmp(oldName, "..") == 0){
        printf(".. can't change!\n");
        return -1;
    }
    if(strlen(newName) >= maxLengthOfFileName){
        printf("the new file name is too long!\n");
        return -1;
    }
    strcpy(currenDir->dirList[index].fileName, newName);
    return 0;
}
int _touch(char fileName[]){
    //文件名称长度不能超出最大长度
    if(strlen(fileName) >= maxLengthOfFileName){
        printf("file name is too long!\n");
        return 0;
    }
    //获取分配的盘块号
    int dirBlockNum = getBlock(FILETYPE);
    if(dirBlockNum == -1)
        return -1;
    if(creatFileMemory(dirBlockNum) == -1){
	printf("createFile error!\n");
        return -1;
    }
    if(addDir(currenDir, fileName, FILETYPE, dirBlockNum) == -1){  //创建的新目录加入到现有目录的子目录列表中
        return -1;
    }
    return 0;
}
int _rm(char fileName[]){
    if(strcmp(fileName, "..") == 0){
        printf(".. can't delete!\n");
        return -1;
    }
    int index = findIndexOf(currenDir, fileName);
    if(index == -1){
        printf("the file can't be found!\n");
        return -1;
    }
    if(currenDir->dirList[index].type == DIRTYPE){
        printf("the file isn't a file!\n");
        return -1;
    }
    resetBlock(currenDir->dirList[index].startBlock, FILEBLOCKSIZE);//释放目录内存空间
    deleteDir(currenDir, index);//从当前目录中移除
    int fileBlock = currenDir->dirList[index].startBlock;
    struct fileMemory* myFile = (struct fileMemory*)getBlockAddr(fileBlock);
    //释放信号量
    sem_close(myFile->read_sem);
    sem_close(myFile->write_sem);
    sem_unlink("read");
    sem_unlink("write");
    return 0;
}
int _ls(){
    int length = currenDir->lengthOfDir;
    printf("fileName\ttype\tstartBlock\tsize\n");
    for(int i = 0;i < length;i++){
        struct file f = currenDir->dirList[i];
        printf("%s\t\t", f.fileName);
	if(f.type == DIRTYPE){
	    printf("dir");
	}else if(f.type == FILETYPE){
	    printf("file");
	}
	printf("\t%d\t\t%d\n", f.startBlock, f.fileSize);
    }
}
int _cd(char fileName[]){
    int index = findIndexOf(currenDir, fileName);
    //文件没有找到
    if(index == -1){
        printf("the file can't be found!\n");
        return -1;
    }
    if(currenDir->dirList[index].type == FILETYPE){
        printf("the file isn't a dir!\n");
        return -1;
    }
    int block = currenDir->dirList[index].startBlock;
    currenDir = (struct dir*)getBlockAddr(block);
    //修改路径
    if(strcmp(fileName, "..") == 0){
        int pathLength = strlen(path);
        for(int i = pathLength-2;i >= 0;i--){
            if(path[i] == '/'){
                path[i+1] = '\0';
                break;
            }
        }
    }else{
        strcat(path, fileName);
        strcat(path, "/");
    }
    return 0;
}
int _change(char fileName[], char writeContent[]){
    int index = findIndexOf(currenDir, fileName);
    //文件没有找到
    if(index == -1){
        printf("the file can't be found!\n");
        return -1;
    }
    if(currenDir->dirList[index].type == DIRTYPE){
        printf("the file isn't a file, can't be writen!\n");
        return -1;
    }
    int fileBlock = currenDir->dirList[index].startBlock;
    struct fileMemory* myFile = (struct fileMemory*)getBlockAddr(fileBlock);
    sem_wait(myFile->write_sem);
    //休息10s模拟修改操作
    sleep(5);   
    myFile->content[0] = '\0';
    strcat(myFile->content, writeContent);
    sleep(5);
    sem_post(myFile->read_sem);
}
int _write(char fileName[], char writeContent[]){
    int index = findIndexOf(currenDir, fileName);
    //文件没有找到
    if(index == -1){
        printf("the file can't be found!\n");
        return -1;
    }
    if(currenDir->dirList[index].type == DIRTYPE){
        printf("the file isn't a file, can't be writen!\n");
        return -1;
    }
    int fileBlock = currenDir->dirList[index].startBlock;
    struct fileMemory* myFile = (struct fileMemory*)getBlockAddr(fileBlock);
	//int write, read;
	//sem_getvalue(myFile->write_sem, &write);
	//sem_getvalue(myFile->read_sem, &read);
	//printf("%d, %d", write, read);
    sem_wait(myFile->write_sem);
    //休息10s模拟写操作
    sleep(5);
    strcat(myFile->content, writeContent);
    sleep(5);
    sem_post(myFile->read_sem);
}
int _read(char fileName[]){
    int index = findIndexOf(currenDir, fileName);
    //文件没有找到
    if(index == -1){
        printf("the file can't be found!\n");
        return -1;
    }
    if(currenDir->dirList[index].type == DIRTYPE){
        printf("the file isn't a file, can't be read!\n");
        return -1;
    }
    int fileBlock = currenDir->dirList[index].startBlock;
    struct fileMemory* myFile = (struct fileMemory*)getBlockAddr(fileBlock);
    sem_wait(myFile->read_sem);
    //休息10s模拟读操作
    sleep(5);
    printf("%s\n", myFile->content);
    sleep(5);
    sem_post(myFile->write_sem);
}
int creatFileMemory(int blockNum){
    struct fileMemory* fM = (struct fileMemory*)getBlockAddr(blockNum);
    fM->startBlock = blockNum;
    fM->dataSize = 0;
    fM->write_sem = sem_open("write", O_CREAT, 0644, 1);
    if(fM->write_sem == SEM_FAILED)
    {
        perror("sem_open error");
        exit(1);
    }
    fM->read_sem = sem_open("read", O_CREAT, 0644, 0);
    if(fM->read_sem == SEM_FAILED)
    {
        perror("sem_open error");
        exit(1);
    }
	//int write, read;
	//sem_getvalue(fM->write_sem, &write);
	//sem_getvalue(fM->read_sem, &read);
	//printf("%d, %d", write, read);
    return 0;
}
//将文件增添到目录列表的后面，长度+1
int addDir(struct dir* dirtable, char fileName[], int type, int startBlock){
    if(findIndexOf(dirtable, fileName) == -1){ //目录列表不能有同名文件
        int length = dirtable->lengthOfDir;
        if(length >= maxLengthOfDir){
            printf("the dir's length limints!\n");
            return -1;
        }
	    strcpy(dirtable->dirList[length].fileName, fileName);
        dirtable->dirList[length].type = type;
        dirtable->dirList[length].startBlock = startBlock;
        
        if(type == FILETYPE){
            dirtable->dirList[length].fileSize = FILEBLOCKSIZE*BLOCK_SIZE; //文件大小为(块数量*块大小)
        }else if(type == DIRTYPE){
            dirtable->dirList[length].fileSize = DIRBLOCKSIZE*BLOCK_SIZE; //文件大小为(块数量*块大小)
        }
        dirtable->lengthOfDir ++;
        return 0;
    }
    printf("the file exist!\n");
    return -1;

}
//删除目录列表中特定位置的文件
int deleteDir(struct dir* dirtable, int index){
    int length = dirtable->lengthOfDir;
    for(int i = index;i < length-1;i++){
        dirtable->dirList[i] = dirtable->dirList[i+1];//将后面的文件覆盖前面的文件
    }
    dirtable->lengthOfDir--;
}
//查找文件在目录列表的位置
int findIndexOf(struct dir* dirtable, char fileName[]){
    int length = dirtable->lengthOfDir;
    int index = -1;
    for(int i = 0;i < length;i++){
	//printf("%s,%s\n", fileName, dirtable->dirList[i].fileName);
        if(strcmp(fileName, dirtable->dirList[i].fileName) == 0){
            index = i;
            return index;//如果找到返回文件位置
        }
    }
    return index;//如果未找到则返回-1
}
void printPath(){
    printf("%s ", path);
}
