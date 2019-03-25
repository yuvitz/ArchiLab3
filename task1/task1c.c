#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;

// typedef struct link link;
 
// struct link {
//     link *nextVirus;
//     virus *vir;
// };

struct fun_desc
{
	char *name;
	void (*func)(void);
};

char buffer[256];
char fileTocheck[100000];
//link *virusList;

void copynstring(char *dst, char *src, unsigned int n){
	unsigned int i;
		for (i = 0; i < n; i++)
		   dst[i] = src[i];
		//return dst;
}

void printHexChar(char *str, unsigned int len){
	unsigned int i;
	for (i = 0; i < len; i++)
		printf("%#02X ", str[i] &0xff);
	printf("\n");
}

void printVirus(virus *v){
	printf("%s%s\n", "Virus name: ", v->virusName);
	printf("%s%d\n", "Virus size: ", v->SigSize);
	printf("signature:\n");
	printHexChar(v->sig, v->SigSize);
	printf("\n");
}

// void printVirusList(link *virus_list){
// 	if(virus_list==NULL)
// 		return;
// 	printVirus(virus_list-> vir);
// 	printVirusList(virus_list-> nextVirus);
// }

int main(int argc, char **argv)
{	
	FILE *file= fopen("signatures","r");
	virus *v=NULL;

	while(fread(buffer,sizeof(char) ,2,file)!=0){
		v=malloc(buffer[0]*sizeof(char));
		v->SigSize=buffer[0]-18;		
		fread(buffer,sizeof(char) , v->SigSize+16, file);
		copynstring(v->virusName, buffer, 16);
		copynstring(v->sig, buffer+16, v->SigSize);	
		printVirus(v);
	// 	memcpy(v->virusName, virus_name, strlen(virus_name));
	// 	memcpy(v->sig, virus_sig, strlen(virus_sig));
	// 	printf("%d\n", v->SigSize);
	// 	printf("%s\n", v->virusName);
	// 	printf("%s\n", v->sig);
	 }
	return 0;
}