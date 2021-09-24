// utilities functions for path

#include "stdio.h"
#include "unistd.h"
#include "globals.h"
#include "error.h"
#include "string.h"

char* _getPath(char* programName, char* basePath, int* status) {
    int length = strlen(programName) + strlen(basePath);
    char* combinedPath = (char*) malloc(length * sizeof(char));
    strcpy(combinedPath, basePath);
    strcat(combinedPath, programName);

    printf("_getPath: combined path is: %s\n", combinedPath);

    if (access(combinedPath, X_OK) == 0) {
        printf("_getPath: check access OK\n");
        *status = 0;
        return combinedPath;
    } else {
        *status = 1;
        error();
        return NULL;
    }
}

char*
getValidPath(char* programName) {
    // scan through PATH to look for the program file
    // if the file is found and is executable, return the first
    // correct path
    if (!programName) return NULL;
    programName[strcspn(programName, "\n")] = 0; // remove trailing \n character
    printf("getValidPath: programName %s with length %zu\n", programName, strlen(programName));
    char* token;
    char* PATH_COPY = (char*) malloc(strlen(PATH) + 1);
    char* validPath = NULL;
    strcpy(PATH_COPY, PATH); // duplicate string since strtok destroys old string

    int status = 1;
    while (status) {
        token = strtok(PATH_COPY, " ");
        validPath = _getPath(programName, token, &status);

        while (token != NULL) {
            token = strtok(NULL, " ");
            if (token) validPath = _getPath(programName, token, &status);
        }
        status = 0;
    }

    printf("getValidPath: %s\n", validPath);
    return validPath;
}
