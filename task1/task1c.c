#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structs

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

//global vars

unsigned char buffer[10000];
char suspectedFile[10000];
link *virusList=NULL;
char *virusName;

//methods

FILE* read_file(){
	char fileName[256];
	printf("%s\n", "Please enter a signature file name");
	fgets(fileName, sizeof(fileName), stdin);
	sscanf(fileName, "%s", fileName);
	return fopen(fileName,"r");
}

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
	printVirus(virus_list-> vir);
	list_print(virus_list-> nextVirus);
}

link* list_append(link* virus_list, virus* data){
	if(virus_list == NULL){
		virus_list = malloc(sizeof(*data) + sizeof(link));
		virus_list-> vir = data;
		virus_list-> nextVirus = NULL;
		return virus_list;
	}
	virus_list-> nextVirus = list_append(virus_list-> nextVirus, data);
	return virus_list;
}

void list_free(link *virus_list){
	if(virus_list == NULL)
		return;
	list_free(virus_list-> nextVirus);
	free(virus_list-> vir);
	free(virus_list);
}
void print_sigs(){
    link *p=virusList;
	list_print(p);
}
void load_sigs(){
	list_free(virusList);
	unsigned short size=0;
	FILE *file= read_file();
	if(!file){
		printf("%s\n", "Error, no such file");
		return;
	}
	virus *v=NULL;
	while(fread(buffer, sizeof(char) ,2,file)!=0){
		size = (unsigned short)(buffer[1]<<8);
		size = size+buffer[0];
		v=malloc(sizeof(virus) + size*sizeof(char) - 18);
		v->SigSize= (unsigned short)(size-18);
		fread(buffer,sizeof(char) , v->SigSize+16, file);
		memcpy(v->virusName, buffer, 16);
		memcpy(v->sig, buffer+16, v->SigSize);
		virusList= list_append(virusList, v);
	}
	fclose(file);	 
}

void detect_virus(char *buffer, unsigned int size){
	link *list = NULL;
	for(unsigned int i =0; i<size; i++){
		list=virusList;
		while(list!=NULL){
			if(memcmp(&buffer[i], list-> vir-> sig, list-> vir-> SigSize) == 0)
				printf("Starting byte: %d, Virus name: %s, Virus size: %d\n", i, list-> vir-> virusName, list-> vir-> SigSize);
			list = list-> nextVirus;
		}
	}
}

void detect(){
	size_t bytes_read;
	FILE *suspected = read_file();
	if(!suspected){
		printf("%s\n", "Error, no such file");
		return;
	}
	bytes_read=fread(suspectedFile, sizeof(char), 10000, suspected);
	detect_virus(suspectedFile, bytes_read);
	fclose(suspected);
}

void kill_virus(char *fileName, int signatureOffset, int signatureSize){
	FILE *virToKill = fopen(fileName, "r+");
	char NOP_str[signatureSize];
	for(unsigned int i =0; i<signatureSize; i++)
		NOP_str[i]=0x90;
	fseek(virToKill, signatureOffset, SEEK_SET);
	fwrite(NOP_str, sizeof(char), signatureSize, virToKill);
	fclose(virToKill);		
}

void fix_file(){
	int offset, sig_size;
	char buffer[256];
	printf("%s\n", "Enter signature offset");
	offset = atoi(fgets(buffer, sizeof(buffer), stdin));
	printf("%s\n", "Enter signature size");
	sig_size = atoi(fgets(buffer, sizeof(buffer), stdin));
	kill_virus(virusName, offset, sig_size);
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
	{"Load signatures", load_sigs},
	{"Print signatures", print_sigs},
	{"Detect viruses", detect},
	{"Fix file", fix_file}, 
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