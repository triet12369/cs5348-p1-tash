// handle searching of paths when users execute a program
#ifndef UTILITIES
#define UTILITIES
#include "utilities.c"

char* getValidPath(char* programName);
void cleanArgsArray(char** args);
int getStringArraySizeBytes(char** stringArray, int sizeArr);
#endif