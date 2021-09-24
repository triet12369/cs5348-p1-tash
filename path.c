// utilities functions for path

#include "stdio.h"
#include "unistd.h"
#include "globals.h"
#include "error.h"
#include "string.h"

char*
getPath(char* prog) {
    // TODO: add support for multiple paths
    int length = strlen(prog) + strlen(path);
    char* combinedPath = (char*) malloc(length * sizeof(char));
    strcpy(combinedPath, path);
    strcat(combinedPath, prog);

    printf("combined path is: %s", combinedPath);

    if (access(combinedPath, X_OK) == 0) {
        return combinedPath;

    } else {
        error();
        return "\0";
    }
}
