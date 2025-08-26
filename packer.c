#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>

#include "packer.h"
#include "crc32.h"
#include "util.h"

typedef struct dirent dirent;

int packAll(char* dir) {
	vector_FpacFile files = {0};
	FpacHeader fpacHeader;
	unsigned long filepathBlockSize = 0;
	unsigned long entryBlockSize = 0;
	unsigned long dataBlockSize = 0;
	char working_dir[4096];
	//Change working directory to the one that contains the dir to pack
	realpath(dir, working_dir);
	dirname(working_dir);
	chdir(working_dir);

	//Then get the base name of the dir to pack
	dir = basename(dir);

	fpacHeader.fpac_magic = FPAC_MAGIC;
	fpacHeader.unk = 1;

	if(getAllFilePathsForFpacFileVector(dir, &files) == 0){
		return 0;
	}

	fpacHeader.n_of_files = files.len;
	entryBlockSize = sizeof(FpacFileInfo)*fpacHeader.n_of_files;

	for(int i = 0; i < files.len; i++){
		files.items[i].fileInfo.pad0 = 0;
		files.items[i].fileInfo.file_size = getFileSize(files.items[i].filename);
		//For some god-who-knows-what reason, the crc32 is XOR'd
		files.items[i].fileInfo.filename_crc32 = crc32(files.items[i].filename, strlen(files.items[i].filename)) ^ 0xFFFFFFFF;
	}

	//Sort files by filename in ascending order, then calculate the address of each file's filepath string in the filepath block, plus the filepath block's total size
	qsort(files.items, (size_t)files.len, sizeof(FpacFile), fpacFile_filename_comparator);
	for(int i = 0; i < files.len; i++){
		files.items[i].fileInfo.filename_address = sizeof(fpacHeader)+entryBlockSize+filepathBlockSize;
		filepathBlockSize += strlen(files.items[i].filename)+1;
	}

	//Calculate the address of each file's data section, plus the data block's total size
	fpacHeader.first_file_address = sizeof(fpacHeader)+entryBlockSize+filepathBlockSize;
	for(int i = 0; i < files.len; i++){
		files.items[i].fileInfo.file_address = sizeof(fpacHeader)+entryBlockSize+filepathBlockSize+dataBlockSize;
		dataBlockSize += files.items[i].fileInfo.file_size;
	}

	FILE* new_fpac = fopen("out.pac", "w");
	if(new_fpac == NULL){
		vector_FpacFile_destroy(files);
		return 0;
	}

	//Write header and entry/info block (sorted by hash)
	qsort(files.items, files.len, sizeof(FpacFile), fpacFile_crc32_comparator);
	fwrite(&fpacHeader, sizeof(fpacHeader), 1, new_fpac);
	for(int i = 0; i < files.len; i++){
		fwrite(&files.items[i].fileInfo, sizeof(FpacFileInfo), 1, new_fpac);
	}

	//Write filepath block (sorted by filename)
	qsort(files.items, files.len, sizeof(FpacFile), fpacFile_filename_comparator);
	for(int i = 0; i < files.len; i++){
		fwrite(files.items[i].filename, 1, strlen(files.items[i].filename)+1, new_fpac);
	}

	//Load data of every file, then write all data one by one (sorted by filename)
	for(int i = 0; i < files.len; i++){
		FILE* current_file = fopen(files.items[i].filename, "r");
		if(current_file == NULL){
			vector_FpacFile_destroy(files);
			return 1;
		}
		files.items[i].file_contents = malloc(files.items[i].fileInfo.file_size);
		fread(files.items[i].file_contents, files.items[i].fileInfo.file_size, 1, current_file);
		fwrite(files.items[i].file_contents, files.items[i].fileInfo.file_size, 1, new_fpac);
		free(files.items[i].file_contents);
		fclose(current_file);
	}

	vector_FpacFile_destroy(files);
	fflush(new_fpac);
	fclose(new_fpac);
	return 1;
}

int getAllFilePathsForFpacFileVector(char* path, vector_FpacFile* files){
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
				getAllFilePathsForFpacFileVector(filepath, files);
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