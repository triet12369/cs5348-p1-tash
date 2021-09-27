#include "execute.c"

void executeArg();
void executeCommand(char* command, char** args, int num_args);
void executeAllCommand(char** commands, char*** cmd_args, int num_args[], int num_cmds);
// void executeAllArgs();