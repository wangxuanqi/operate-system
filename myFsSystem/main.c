#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "file.h"
void test(){
    _mkdir("test");
    printPath();
    _ls();
    _cd("test");
    printPath();
    _ls();
    _cd("..");
    printPath();
    _ls();
    _cd("test");
    printPath();
    _ls();
}
int main(){
    initFsSystem();//初始化文件系统
    initRootDir();

    //将输入的字符串通过空格分割，因为操作数最多为3，所以只取3条
    while(1){
        char str[100], str1[100], str2[100], str3[100];
	for(int i = 0;i < 100;i++){
		str[i] = '\0';
		str1[i] = '\0';	
		str2[i] = '\0';
		str3[i] = '\0';
	}
        char ch;
        int indexOfStr = 0;
	printPath();
        while((ch = getchar()) != '\n'){
            str[indexOfStr] = ch;
            indexOfStr ++;
        }
        int lengthOfStr = strlen(str);
        indexOfStr = 0;
        int indexOfStr1;
        for(int j = 0;j < 3;j++){
            indexOfStr1 = 0;
            while(indexOfStr < lengthOfStr){ //去除空格
                if(str[indexOfStr] == ' '){
                    indexOfStr++;
                }else{
                    break;
                }
            }
            while(indexOfStr < lengthOfStr){ //赋值直到遇到空格
                if(str[indexOfStr] == ' '){
                    break;
                }else{
                    switch (j){
                    case 0:
                        str1[indexOfStr1] = str[indexOfStr];
                        break;
                    case 1:
                        str2[indexOfStr1] = str[indexOfStr];
                        break;
                    case 2:
                        str3[indexOfStr1] = str[indexOfStr];
                        break;
                    default:
                        break;
                    }
                    indexOfStr1++;
                    indexOfStr++;
                }
            }
        }
        //printf("%s, %s, %s\n", str1, str2, str3);
        

        //执行命令操作
        if(strcmp(str1, "mkdir") == 0){
            if(strlen(str2) == 0)
                printf("input: mkdir [dirName]\n");
            else {
                _mkdir(str2);
            }
        }else if(strcmp(str1, "rmdir") == 0){
            if(strlen(str2) == 0)
                printf("input: rmdir [dirName]\n");
            else {
                _rmdir(str2);
            }
        }else if(strcmp(str1, "rename") == 0){
            if(strlen(str2) == 0 || strlen(str3) == 0)
                printf("input: rename [oldFileName] [newFileName]\n");
            else {
                _rename(str2, str3);
            }
        }else if(strcmp(str1, "touch") == 0){
            if(strlen(str2) == 0)
                printf("input: touch [fileName]\n");
            else {
                _touch(str2);
            }
        }else if(strcmp(str1, "rm") == 0){
            if(strlen(str2) == 0)
                printf("input: rm [fileName]\n");
            else {
                _rm(str2);
            }
        }else if(strcmp(str1, "ls") == 0){
            _ls();
        }else if(strcmp(str1, "cd") == 0){
            if(strlen(str2) == 0)
                printf("input: cd [dirName]\n");
            else {
                _cd(str2);
            }
        }else if(strcmp(str1, "write") == 0){
            if(strlen(str2) == 0 || strlen(str3) == 0)
                printf("input: write [fileName] [content]\n");
            else {
                _write(str2, str3);
            }
        }else if(strcmp(str1, "change") == 0){
            if(strlen(str2) == 0 || strlen(str3) == 0)
                printf("input: change [fileName] [content]\n");
            else {
                _change(str2, str3);
            }
        }else if(strcmp(str1, "read") == 0){
            if(strlen(str2) == 0)
                printf("input: read [fileName]\n");
            else {
                _read(str2);
            }
        }
        else if(strcmp(str1, "exit") == 0){
	        break;
        }else if(strlen(str1) >= 100){
            printf("input error!\n");
        }else{
                printf("\n");
            }
        }
    //test();
    destroyFsSystem();//释放内存
}

