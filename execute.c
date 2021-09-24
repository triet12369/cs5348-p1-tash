#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "error.h"
#include "utilities.h"

char** const _makeExecvParams(char* path, char** args) {
  // execv accepts NULL terminated char* array and the first element has to be
  // the path of the file to run

  // I don't know the number of args since args is a pointer to pointer so
  // I'll have to loop through args once to get the number
  int numberOfArgs;
  for (numberOfArgs = 0;;) {
    if (args[numberOfArgs] != NULL) numberOfArgs++;
    else break;
  }
  char** paramList = (char**) malloc(numberOfArgs + 2);

  // Set first item to file path
  paramList[0] = malloc(strlen(path) + 1);
  strcpy(paramList[0], path);
  // Set last item to NULL
  paramList[numberOfArgs + 1] = NULL;
  // Set arguments
  int i;
  for (i = 0; i < numberOfArgs; ++i) {
    paramList[i + 1] = malloc(strlen(args[i]) + 1);
    strcpy(paramList[i + 1], args[i]);
  }
  // printf("_makeExecvParams: paramList test: %s \n", paramList[2]);
  return paramList;
}

void executeArg(char* programName, char** args) {
  printf("executeArg: programName is %s, arguments 1: %s \n", programName, args[0]);
  // First we need to get the accessable file name
  char* validProgramPath = getValidPath(programName);
  if (!validProgramPath) error();

  // make new process and execv it
  pid_t pid;
  char** const paramList = _makeExecvParams(validProgramPath, args);
  pid = fork();
  if (pid == 0) {
    // child
    execv(validProgramPath, paramList);
  } else if (pid == -1) error();
  else {
    // parent
    wait(NULL); // wait for children to finish executing
  }
};
