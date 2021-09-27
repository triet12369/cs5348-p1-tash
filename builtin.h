#ifndef BUILTIN
#define BUILTIN
#include "builtin.c"

char *BUILTIN_NAMES[]={"exit","exit\n","cd","cd\n","path","path\n"};

void usrexit(int num_args);
void usrchdir(char** args);
void usrpath(char **args, int index);
#endif