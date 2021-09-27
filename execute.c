#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "error.h"
#include "utilities.h"
#include "builtin.h"

int _getNumberOfArgs(char** args) {
  int numberOfArgs;
  for (numberOfArgs = 0;;) {
    if (args[numberOfArgs] != NULL && (strcmp(args[numberOfArgs], "") != 0)) numberOfArgs++;
    else break;
  }
  return numberOfArgs;
}

int _getSingleIndexOfChar(char** stringArr, int arrSize, char c) {
  // look for index of char in array of strings
  // return -1 if char not found
  // return -2 if multiple instances are found
  int i, count = 0;
  int index = -1;
  for (i = 0; i < arrSize; ++i) {
    if (stringArr[i] != NULL) {
      // printf("checking %s\n", stringArr[i]);
      int k;
      for (k = 0; stringArr[i][k]; k++) {
        if (stringArr[i][k] == c) {
          // printf("found at %d\n", k);
          count++;
          index = i;
        }
      }
    }
  }
  if (count > 1) return -2;
  return index;
}

typedef struct execInfo {
  char* programPath;
  char* redirectTargetOutput; // output.txt
  char** args; // -la > output.txt --> -la
  int numArgs; // 1
} EXEC_INFO;

EXEC_INFO _prepareArgs (char** args, int num_args) {
  // look for redirection characters e.g. '>' '<'
  // and return the args as well as redirection parameters
  // printf("_catchRedirections input args 0: %s\n", args[0]);
  EXEC_INFO execInfo;
  // char** target;
  // loop through the args and find '>'
  char RE_OUT = '>';
  // printf("Index of redirect: %d\n", _getSingleIndexOfChar(args, num_args, RE_OUT));
  int indexOfRedirectInArgs = _getSingleIndexOfChar(args, num_args, RE_OUT);

  if (indexOfRedirectInArgs == -1) {
    execInfo.numArgs = num_args;
    execInfo.args = args;
    execInfo.redirectTargetOutput = NULL;
    return execInfo;
  } else if (indexOfRedirectInArgs == -2) {
    execInfo.numArgs = -1;
    error();
    return execInfo;
  }
  else {
    execInfo.numArgs = indexOfRedirectInArgs;
    if (strlen(args[indexOfRedirectInArgs]) > 1) {
      // '>output.txt' ls >output output2
      if (indexOfRedirectInArgs + 1 == num_args - 1) {
        // extra argurment after e.g. multiple files
        error();
        execInfo.numArgs = -1;
      } else execInfo.redirectTargetOutput = args[indexOfRedirectInArgs] + 1;
    } else if (args[indexOfRedirectInArgs + 1] == NULL) {
      // no target specified
      error();
      execInfo.numArgs = -1;
    } else {
      // > output.txt
      if (indexOfRedirectInArgs + 2 == num_args - 1) {
        // extra argurment after
        error();
        execInfo.numArgs = -1;
      } else execInfo.redirectTargetOutput = args[indexOfRedirectInArgs + 1];
    }
  }

  // copy the part before redirection as args
  if (execInfo.numArgs != -1) {
    int k;
    execInfo.args = (char**) malloc(indexOfRedirectInArgs + 1 * sizeof(char));
    for (k = 0; k <= indexOfRedirectInArgs; ++k) {
      execInfo.args[k] = (char*) malloc(strlen(args[k]) + 1);
      strcpy(execInfo.args[k], args[k]);
    }
  }
  return execInfo;
}

char** const _makeExecvParams(char* path, char** args, int num_args) {
  // execv accepts NULL terminated char* array and the first element has to be
  // the path of the file to run
  // printf("_makeExecvParams %s\n", path);
  char** paramList = (char**) malloc(num_args + 2);

  // Set first item to file path
  paramList[0] = malloc(strlen(path) + 1);
  strcpy(paramList[0], path);
  // Set last item to NULL
  paramList[num_args + 1] = NULL;
  // Set arguments
  int i;
  for (i = 0; i < num_args; ++i) {
    paramList[i + 1] = malloc(strlen(args[i]) + 1);
    strcpy(paramList[i + 1], args[i]);
  }
  // printf("_makeExecvParams: paramList test: %s \n", paramList[0]);
  return paramList;
}

EXEC_INFO getExecInfo (char* programName, char** args, int num_args) {
  // First we need to get the accessable file name
  EXEC_INFO execInfo;
  char* validProgramPath = getValidPath(programName);
  if (!validProgramPath) {
    error();
    execInfo.numArgs = -1;
    return execInfo;
  }
  execInfo = _prepareArgs(args, num_args);
  execInfo.programPath = (char*) malloc(strlen(validProgramPath) + 1);
  strcpy(execInfo.programPath, validProgramPath);
  return execInfo;
}

void executeArg(char* programName, char** args, int num_args) {
  // printf("executeArg: programName is %s, arguments 1: %s \n", programName, args[0]);
  // First we need to get the accessable file name
  char* validProgramPath = getValidPath(programName);
  if (!validProgramPath) {
    error();
    return;
  }

  // catch redirections
  EXEC_INFO execInfo = getExecInfo(programName, args, num_args);
  if (execInfo.numArgs == -1) return;

  // store STDOUT and STDERR for later
  int STDOUT = dup(STDOUT_FILENO);
  int STDERR = dup(STDERR_FILENO);
  // make new process and execv it
  pid_t pid;
  char** const paramList = _makeExecvParams(validProgramPath, execInfo.args, execInfo.numArgs);
  pid = fork();
  if (pid == 0) {
    // child
    if (execInfo.redirectTargetOutput != NULL) {
      // redirect output
      int fileDescriptor = open(execInfo.redirectTargetOutput, O_WRONLY | O_CREAT | O_TRUNC, 0777);
      if (fileDescriptor == -1) {
        error();
        return;
      }
      // Replace STDOUT and STDERR with fileDescriptor
      dup2(fileDescriptor, STDOUT_FILENO);
      dup2(fileDescriptor, STDERR_FILENO);
      close(fileDescriptor);
    }
    execv(validProgramPath, paramList);
  } else if (pid == -1) error();
  else {
    // parent
    wait(NULL); // wait for children to finish executing
    dup2(STDOUT, STDOUT_FILENO);
    dup2(STDERR, STDERR_FILENO);
  }
};

void executeAllCommand(char** commands, char*** cmd_args, int num_args[], int num_cmds) {
  // EXEC_INFO** execList = (EXEC_INFO*) malloc(sizeof(EXEC_INFO) * num_cmds);
  EXEC_INFO execInfo;
  pid_t pids[num_cmds];
  int i;
  int STDOUT = dup(STDOUT_FILENO);
  int STDERR = dup(STDERR_FILENO);
  for (i = 0; i < num_cmds; ++i) {
    execInfo = getExecInfo(commands[i], &cmd_args[i][i], num_args[i]);
    // store STDOUT and STDERR for later
    // make new process and execv it
    // pid_t pid;
    // printf("child running %s %s %s %d\n", execInfo.programPath, execInfo.args[0], execInfo.redirectTargetOutput, execInfo.numArgs);
    char** const paramList = _makeExecvParams(execInfo.programPath, execInfo.args, execInfo.numArgs);
    pids[i] = fork();
    if (pids[i] == 0) {
      // child
      // printf("child running %s %s %d\n", execInfo.programPath, execInfo.args[0], execInfo.numArgs);
      if (execInfo.redirectTargetOutput != NULL) {
        // redirect output
        int fileDescriptor = open(execInfo.redirectTargetOutput, O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (fileDescriptor == -1) {
          error();
          return;
        }
        // Replace STDOUT and STDERR with fileDescriptor
        dup2(fileDescriptor, STDOUT_FILENO);
        dup2(fileDescriptor, STDERR_FILENO);
        close(fileDescriptor);
      }
      execv(execInfo.programPath, paramList);
    } else if (pids[i] == -1) error();
  }
  for (i = 0; i < num_cmds; ++i) {
      if (pids[i] != 0) {
        // parent
        waitpid(pids[i], NULL, 0); // wait for children to finish executing
        dup2(STDOUT, STDOUT_FILENO);
        dup2(STDERR, STDERR_FILENO);
      }
    }
}

void executeCommand(char* command, char** cmd_args, int num_args) {
  if(strcmp(command, BUILTIN_NAMES[0])==0 || strcmp(command, BUILTIN_NAMES[1])==0){            
      //printf("exit worked");
      usrexit(num_args);
  } else if(strcmp(command, BUILTIN_NAMES[2])==0 || strcmp(command, BUILTIN_NAMES[3])==0){
      // printf("%s %s command arguments are",cmd_args[i][0],cmd_args[i][1]);
      //printf(" the i value is : %d",i);
      usrchdir(cmd_args);
      // printf("cd worked");
  } else if(strcmp(command, BUILTIN_NAMES[4])==0 || strcmp(command, BUILTIN_NAMES[5])==0){
      usrpath(cmd_args, num_args);
  } else {
      executeArg(command, cmd_args, num_args);
  }
}
