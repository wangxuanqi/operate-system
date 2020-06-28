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
	int *p;
	char s[8];
	int pid = getpid();
	sprintf(s,"%d",pid);
	//一个int占4个字节
	//128MB = 128 * 1024 KB = 131072 KB
	//128MB = 128 * 1024 * 1024 字节(byte) = 134217728 Byte
	//int数组长度为 1900 * 1024 * 1024 / 4
	int sum = 1900 * 1024 * 1024 / 4 ;
	p = (int*)malloc(sum*sizeof(int));
	//相隔4KB进行读写就是相隔1024个int进行读写
	int i  = 0;
	for(;i<sum;i += 1024){
		p[i] ++;//write
	}
	printf("\n进行写操作后,看status的情况...\n");
	status(s);
	getchar();
	free(p);
	return 0;
}

