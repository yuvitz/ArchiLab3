#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    char sig[];
} virus;

typedef struct link link;
struct link {
    link *nextVirus;
    virus *vir;
};

struct fun_desc{
	char *name;
	void (*func)(void);
};

unsigned char buffer[256];
FILE *file;
struct link *virusList=NULL;

//methods

void copynstring(char *dst, unsigned char *src, unsigned int n){
	unsigned int i;
		for (i = 0; i < n; i++)
		   dst[i] = src[i];
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

void list_print(link *virus_list){
	if(virus_list==NULL)
        
		return;
	list_print(virus_list-> nextVirus);
	printVirus(virus_list-> vir);
}

link* list_append(link* virus_list, virus* data){
	if(virus_list == NULL){
		virus_list = malloc(sizeof(data)+sizeof(link));
		virus_list-> vir = data;
		virus_list-> nextVirus = NULL;
		return virus_list;
	}
	virus_list-> nextVirus = list_append(virus_list-> nextVirus, data);
	return virus_list;
}
/* Free the memory allocated by the list. */
void list_free(link *virus_list){
	if(virus_list == NULL)
		return;
	list_free(virus_list-> nextVirus);
	free(virus_list-> vir);
	free(virus_list);
}
void printSigs(){
    link *p=virusList;
	list_print(p);
}
void loadSigs(){
	//char fileName[256];
    link *p=virusList;
	//unsigned short *size=0;
	//printf("%s\n", "Please enter a signature file name");
	//fgets(fileName, sizeof(fileName), stdin);
	file= fopen("signatures","r");
	if(file){
		virus *v=NULL;
		while(fread(buffer, sizeof(char) ,2,file)!=0){
			v=malloc(buffer[0]*sizeof(char));
			v->SigSize= (unsigned short)(buffer[0]-18);
			fread(buffer,sizeof(char) , v->SigSize+16, file);
			copynstring(v->virusName, buffer, 16);
			copynstring(v->sig, buffer+16, v->SigSize);
			//printVirus(v);
			list_append(p, v);
		}
	}	 
}

void quit(){
	list_free(virusList);
	exit(0);
}	

//end of methods

int main(int argc, char **argv)
{	
	char opt[256];
	int chosen, desc_size;
	struct fun_desc func_array[] ={
	{"Load signatures", loadSigs},
	{"Print signatures", printSigs}, 
	{"Quit", quit}};
	desc_size=sizeof(func_array)/sizeof(*func_array);

	while(1){
		int i;
		printf("%s\n", "Please choose a function");
		for(i=1; i<=desc_size; i++){
			printf("%d", i);
			printf(") %s\n", func_array[i-1].name);
		}
		fgets(opt, sizeof(opt), stdin);
		chosen= atoi(opt)-1;
		printf("%s", "Option: ");
		printf("%s\n", opt);
		if((chosen>=0) && (chosen <desc_size))
			func_array[chosen].func();
		else{
			printf("%s\n", "Not within bounds");
            exit(0);
        }

	}
	return 0;
}
