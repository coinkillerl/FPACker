#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "packer.h"
#include <sys/stat.h>
#include <dirent.h>
typedef struct dirent dirent;

void packAll(char* dir) {
	vector_FpacFile files;
	scanDirRecursive(dir, &files);
	for(int i = 0; i < files.len; i++){
		printf("%s \n", files.items[i].filename);
	}

	while(files.len > 0){
		free(files.items[files.len-1].filename);
		vector_FpacFile_pop_back(&files);
	}
	vector_FpacFile_clear(&files);
}

int scanDirRecursive(char* path, vector_FpacFile* files){
	DIR* dir = opendir(path);
	if(dir == NULL){
		return 0;
	}
	dirent* inode_current;
	FpacFile tmpFile;
	int filepathLength;
	char* filepath = malloc(4096);
	while((inode_current = readdir(dir)) != NULL){
		strncpy(filepath, path, 4095);
		if(inode_current->d_type == DT_REG){
			strncat(filepath, "/", 2);
			strncat(filepath, inode_current->d_name, 256);
			filepathLength = strlen(filepath);
			tmpFile.filename = malloc(filepathLength+1);
			strncpy(tmpFile.filename, filepath, filepathLength+1);
			vector_FpacFile_push_back(files, tmpFile);
		}
		else if(inode_current->d_type == DT_DIR){
			if(strcmp(inode_current->d_name, ".") != 0 && strcmp(inode_current->d_name, "..") != 0){
				strncat(filepath, "/", 2);
				strncat(filepath, inode_current->d_name, 256);
				scanDirRecursive(filepath, files);
			}
		}
		else{
			printf("WARNING : unsupported inode type\n");
		}
	}

	free(filepath);
	closedir(dir);
	return 1;
}