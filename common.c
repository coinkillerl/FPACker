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