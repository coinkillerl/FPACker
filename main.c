#include <stdio.h>
#include "unpacker.h"
#include "util.h"
#include "packer.h"
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
		if(check_fpac_magic(fpac) == 0){
			printf("Bad fpac magic, aborting\n");
			return 3;
		}
		FpacFileInfo* fileInfos = get_all_fileInfo(fpac);
		unpack_all(fileInfos, fpac);
		fclose(fpac);
		return 0;
	}

	if(strcmp(argv[1], "pack") == 0){
		if(argc < 3){
			printf("No dir\n");
			return 2;
		}
		if(strcmp(argv[2], ".")  == 0 || strcmp(argv[2], "..") == 0){
			printf("Do not use . or ..\n");
			return 4;
		}
		if(packAll(argv[2]) == 0){
			printf("ERROR : Packing archive has failed\n");
			return 1;
		}

		return 0;
	}
	else{
		printf("Invalid operation\n \n");
		printHelp();
		return 2;
	}

}
