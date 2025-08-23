#include "common.h"
#include <string.h>
int check_fpac_magic(FILE* fpac){
	char fpac_magic[4];
	fread(fpac_magic, 1, 4, fpac);
	rewind(fpac);
	if(strcmp("FPAC", fpac_magic) == 0){
		return 0;
	}
	else return 1;
}