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
	int *p[1000000];
	char s[8];
	int pid = getpid();
	sprintf(s,"%d",pid);
	printf("\n分配内存前的maps和status情况\n");
	maps(s);
	status(s);
	int i;
	
	unsigned sum = 1*1024*1024*1024/4; 
	for(i = 0;i < 1000000;i++){
		p[i] = (int *)malloc(sum*sizeof(int));
		if(p[i] == NULL)
			break;
	}
	
	printf("耗光虚拟内存后的maps和status情况 ,max = %d GB",i);
	maps(s);
	status(s);
	for(;i > -1;i--)
		free(p[i]);
	return 0;
}

