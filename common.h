#pragma once
#include <stdint.h>
#include <stdio.h>

#define FPAC_MAGIC 1128353862
typedef struct{
	uint32_t fpac_magic;
	uint32_t n_of_files;
	uint32_t first_file_address;
	uint32_t unk;
}FpacHeader;

typedef struct{
	uint32_t filename_crc32;
	uint32_t pad0;
	uint64_t filename_address;
	uint64_t file_size;
	uint64_t file_address;
}FpacFileInfo;

typedef struct{
	FpacFileInfo fileInfo;
	char* filename;
	uint8_t* file_contents;
}FpacFile;

int check_fpac_magic(FILE* fpac);