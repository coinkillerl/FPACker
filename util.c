#include "util.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>

int strLastIndexOfChr(char* str, char target){
	int current = 0;
	int lastIndex = -1;
	while(str[current] != 0){
		if(str[current] == target) lastIndex = current;
		current++;
	}
	return lastIndex;
}

int mkpath(char* str, mode_t mode, int isFilePath){
	char* paths[255];
	char str_clone[4096];
	strncpy(str_clone, str, 4096);
	char* rest = str_clone;
	char current_path[4096];
	int nOfDirs = 0;

	int i = 0;
	while((paths[i] = strtok_r(rest, "/", &rest)) != NULL){
		i++;
	}

	while(paths[nOfDirs] != NULL){
		nOfDirs++;
	}

	if(isFilePath) nOfDirs -= 1;

	if(nOfDirs < 1) return -2;

	strncpy(current_path, paths[0], 4096);

	for(int lv = 0; lv < nOfDirs; lv++){
		strncat(current_path, "/", 1);
		mkdir(current_path, mode);
		if(lv < nOfDirs-1) strncat(current_path, paths[lv+1], 4096);
	}
	return 0;
}

void printHelp(){
	printf("FPACker by CoinKillerL - HELP\n \n");
	printf("unpack-all <file.pac>\n");
	printf("\tUnpack every file in the selected archive\n \n");
}
