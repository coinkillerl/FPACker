#include <stdio.h>
#include "common.h"
FpacFileInfo* get_all_fileInfo(FILE* fpac);
void unpack_file(FpacFileInfo fileInfo, FILE* fpac);
void unpack_all(FpacFileInfo* fileInfos, FILE* fpac);

