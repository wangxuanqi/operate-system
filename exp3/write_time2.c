#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> 
#include <sys/mman.h> 
#include <malloc.h>
#include <time.h>



int main(){
	int *p;
	clock_t start, finish;
	pthread_t id[16];
	char s[8];
	int i,ret;
	int pc[16];
	int sum =1500 * 1024 * 1024 / 4;
	int x = sum /16;
	for(i=0;i<16;i++){
		pc[i] = i * x;
	}
	int pid = getpid();
	sprintf(s,"%d",pid);
	//一个int占4个字节
	//128MB = 128 * 1024 KB = 131072 KB
	//128MB = 128 * 1024 * 1024 字节(byte) = 134217728 Byte
	//int数组长度为 1500 * 1024 * 1024 /4																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																															

	
	p = (int*)malloc(sum*sizeof(int));
	//相隔4KB进行读写就是相隔1024个int进行读写																																																																												
	start = clock();
	for(i=0;i<16;i++){ 
		for(int j = 0;j < 100;j++){//重复访问100次
			int k = pc[i],maxK = pc[i]+x;
			for(int k = pc[i];k < maxK;k += 1024)
				p[k] = k;//write
		}
			
	}
	finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;	
	printf("有分组的情况下完成访问运行的时间为：%f seconds\n", duration );
	free(p);
	return 0;
}



