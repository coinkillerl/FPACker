#pragma once
#include "vector.h"
#include "common.h"
VECTOR_DEFINE(FpacFile)

void packAll(char* dir);
int scanDirRecursive(char* path, vector_FpacFile* files);