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

    // printf("_getPath: combined path is: %s\n", combinedPath);

    if (access(combinedPath, X_OK) == 0) {
        // printf("_getPath: check access OK\n");
        *status = 0;
        return combinedPath;
    } else {
        *status = 1;
        return NULL;
    }
}

char* getValidPath(char* programName) {
    // scan through PATH to look for the program file
    // if the file is found and is executable, return the first
    // correct path
    if (!programName) return NULL;
    // printf("getValidPath PATH %s\n", PATH);
    // programName[strcspn(programName, "\n")] = 0; // remove trailing \n character
    // printf("getValidPath: programName %s with length %zu\n", programName, strlen(programName));
    char* token;
    char* PATH_COPY = (char*) malloc(strlen(PATH) + 1);
    char* validPath = NULL;
    strcpy(PATH_COPY, PATH); // duplicate string since strtok destroys old string

    int status = 1;
    while (status) {
        token = strtok(PATH_COPY, " ");
        // printf("PATH token %s\n", token);
        if (token) validPath = _getPath(programName, token, &status);

        while (token != NULL) {
            token = strtok(NULL, " ");
            if (token) validPath = _getPath(programName, token, &status);
        }
        status = 0;
    }

    // printf("getValidPath: %s\n", validPath);
    return validPath;
}

void cleanArgArray(char** args, int num_args) {
    // convert empty strings to NULL
    if (args == NULL) return;
    // printf("Num args %d\n", num_args);
    int i;
    for (i = 0; i < num_args; ++i) {
        // printf("cleanArgArray %s %d\n", args[i], i);
        if (strcmp(args[i], "") == 0 || strcmp(args[i], "\n") == 0) {
            args[i] = NULL;
        };
    }
}

int getStringArraySizeBytes(char** stringArr, int sizeArr) {
    int i;
    int size = 0;
    for (i = 0; i < sizeArr; ++i) {
        size += strlen(stringArr[i]);
        if (i != 0) size += 2;
    }
    return size;
}
