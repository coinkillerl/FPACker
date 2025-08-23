#include <stdio.h>
#include "unpacker.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "util.h"

FpacFileInfo* get_all_fileInfo(FILE* fpac){
	int n_of_files;
	fseek(fpac, 4, SEEK_SET);
	fread(&n_of_files, 4, 1, fpac);
	FpacFileInfo* fileInfos = malloc(sizeof(FpacFileInfo)*n_of_files);
	fseek(fpac, 8, SEEK_CUR);
	for(int i = 0; i < n_of_files; i++){
		fread(&fileInfos[i].filename_crc32, 4, 1, fpac);
		fseek(fpac, 4, SEEK_CUR);
		fread(&fileInfos[i].filename_address, 8, 1, fpac);
		fread(&fileInfos[i].file_size, 8, 1, fpac);
		fread(&fileInfos[i].file_address, 8, 1, fpac);
	}

	return fileInfos;
}

void unpack_file(FpacFileInfo fileInfo, FILE* fpac){
	char* data = malloc(fileInfo.file_size);
	char filepath[256];

	fseek(fpac, fileInfo.file_address, SEEK_SET);
	fread(data, 1, fileInfo.file_size, fpac);
	fseek(fpac, fileInfo.filename_address, SEEK_SET);
	fgets(filepath, 256, fpac);

	mkpath(filepath, 0777, 1);
	FILE* unpacked_file = fopen(filepath, "w");
	if(unpacked_file == NULL){
		printf("Error writing file %s : aborting\n", filepath);
		exit(1);
	}
	fwrite(data, fileInfo.file_size, 1, unpacked_file);
	fflush(unpacked_file);
	fclose(unpacked_file);
	free(data);
}

void unpack_all(FpacFileInfo* fileInfos, FILE* fpac){
	int n_of_files;
	fseek(fpac, 4, SEEK_SET);
	fread(&n_of_files, 4, 1, fpac);
	for(int i = 0; i < n_of_files; i++){
		unpack_file(fileInfos[i], fpac);
	}
}