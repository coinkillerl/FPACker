#include "common.h"
#include <string.h>
int check_fpac_magic(FILE* fpac){
	uint32_t fpac_magic;
	rewind(fpac);
	fread(&fpac_magic, 1, 4, fpac);
	rewind(fpac);
	if(fpac_magic == FPAC_MAGIC){
		return 1;
	}
	else return 0;
}

int fpacFile_crc32_comparator(const void* a, const void* b){
	FpacFile* aa = (FpacFile*)a;
	FpacFile* bb = (FpacFile*)b;
	if(aa->fileInfo.filename_crc32 < bb->fileInfo.filename_crc32) return -1;
	if(aa->fileInfo.filename_crc32 > bb->fileInfo.filename_crc32) return 1;
	return 0;
}

//FIXME : This type of sorting is not correct, the filepaths in FPAC archives probably use natural sorting
int fpacFile_filename_comparator(const void* a, const void* b){
	FpacFile* aa = (FpacFile*)a;
	FpacFile* bb = (FpacFile*)b;
	return strcasecmp(aa->filename, bb->filename);
}

void vector_FpacFile_destroy(vector_FpacFile vec){
	while(vec.len > 0){
		free(vec.items[vec.len-1].filename);
		vector_FpacFile_pop_back(&vec);
	}
	vector_FpacFile_clear(&vec);
}