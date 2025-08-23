#include <stdio.h>
#include "unpacker.h"
#include "util.h"
#include <string.h>

int main(int argc, char* argv[]){

	if(argc < 2){
		printf("No operation selected\n \n");
		printHelp();
		return 2;
	}

	if(strcmp(argv[1], "unpack-all") == 0){
		if(argc < 3){
			printf("No file\n");
			return 2;
		}

		FILE* fpac = fopen(argv[2], "rb");
		if(fpac == NULL){
			printf("Error opening file, aborting\n");
			return 1;
		}

		//Check FPAC magic
		char fpac_magic[5];
		fread(fpac_magic, 1, 4, fpac);
		if(strcmp(fpac_magic, "FPAC") != 0){
			printf("Bad fpac magic, aborting\n");
			return 3;
		}
		FpacFileInfo* fileInfos = get_all_fileInfo(fpac);
		unpack_all(fileInfos, fpac);
		fclose(fpac);
	}
	else{
		printf("Invalid operation\n \n");
		printHelp();
		return 2;
	}


	return 0;
}
