#include "filesystem.h"

#ifndef UTILS_H
#define UTILS_H

extern int getCurrentSegment();

printCWD(char* path_str, byte current_dir);
void locateNode(byte current_dir, char name[14], int file, int* idx);
void locateIndex(byte current_dir, char path[64], int file, int *output);

#endif