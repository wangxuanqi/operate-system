#include<stdio.h>
#include<stdlib.h>

unsigned gb=0;
unsigned mb=0;
unsigned kb=0;

int main(int argc, char* argv[])
{
	unsigned blockSize[] = {1024*1024*1024 , 1024*1024 , 1024};
	int i, count;
	int *G[1000000];
	int *M[1024];
	int *K[1024];
	for (;;gb++) {
		G[gb] = (int*)malloc(blockSize[0] * sizeof(char));
		if(NULL==G[gb]) {
			break;
		}
	}
	for (;;mb++) {
		M[mb] = (int*)malloc(blockSize[0] * sizeof(char));
		if(NULL==M[mb]) {
			break;
		}
	}
	for (;;kb++) {
		K[kb] = (int*)malloc(blockSize[0] * sizeof(char));
		if(NULL==K[kb]) {
			break;
		}
	}
	printf("maxium malloc size = %u G %u M %u KBytes.\n",gb,mb,kb);
	for (;gb>=0;gb--) {
		free(G[gb]);
		if (gb == 0)
			break;
	}
	for (;mb>=0;mb--) {
		free(M[mb]);
		if (mb == 0)
			break;
	}
	for (;kb>=0;kb--) {
		free(K[kb]);
		if (kb == 0)
			break;
	}
	return 0;
}
