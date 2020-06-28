#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> 
#include <sys/mman.h> 
#include <malloc.h>
//命令 cat /proc/pid/maps
void maps(char s[8]){
	printf("maps:\n");
	char str[100] = {"cat /proc/"};
	strcat(str,s);
	char l[8] = {"/maps"};
	strcat(str,l);
	printf("%s",str);
	system(str);
}

//命令 cat /proc/pid/status
void status(char s[8]){
	printf("status:\n");
	char str[100] = {"cat /proc/"};
	strcat(str,s);
	char l[8] = {"/status"};
	strcat(str,l);
	printf("%s",str);
	system(str);
}

int main(){
	int *p[8];
	char s[8];
	int pid = getpid();
	sprintf(s,"%d",pid);
	printf("\n分配内存前的maps和status情况\n");
	maps(s);
	status(s);
	//一个int占4个字节
	//128MB = 128 * 1024 KB = 131072 KB
	//128MB = 128 * 1024 * 1024 字节(byte) = 134217728 Byte
	//int数组长度为 128 * 1024 * 1024 / 4
	p[0] = malloc(134217728);
	p[1] = malloc(134217728);
	p[2] = malloc(134217728);
	p[3] = malloc(134217728);
	p[4] = malloc(134217728);
	p[5] = malloc(134217728);
	printf("\n分配6个128MB的内存后的maps和status情况\n");
	maps(s);
	status(s);
	free(p[1]);
	free(p[2]);
	free(p[4]);
	printf("\n释放第2、3、5号的128MB空间后的maps和status情况\n");
	maps(s);
	status(s);
	p[6] = malloc(134217728 * 8);
	printf("\n分配1024MB后的maps和status情况\n");
	maps(s);
	status(s);
	p[7] = malloc(134217728/2);
	printf("\n分配64MB后的maps和status情况\n");
	maps(s);
	status(s);
	free(p[0]);
	free(p[3]);
	free(p[5]);
	free(p[6]);
	free(p[7]);
	return 0;
}

