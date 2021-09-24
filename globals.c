// read from .tashrc config file then initialize global variables
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "error.h"

#define MAX_CONFIG_ITEMS 20


typedef struct {
    char* parameter;
    char* value;
} CONFIG_ITEM;

// maximum 20 configs
CONFIG_ITEM parsedConfigList[MAX_CONFIG_ITEMS];


// declare global variables
char* path;
char** currentDir;

CONFIG_ITEM _tokenizer(char* line) {
    // split the line into parameter and it's value
    // e.g. DEFAULT_PATH 123 --> DEFAULT_PATH is parameter, 123 is it's value
    // only support 1 value per paramerter for now
    CONFIG_ITEM config = {};
    char* delimiter = " ";
    char* token = strtok(line, delimiter);

    // get parameter
    config.parameter = (char*) malloc(strlen(token) + 1);
    strcpy(config.parameter, token);

    while (token != NULL) {
        config.value = (char*) malloc(strlen(token) + 1);
        strcpy(config.value, token);
        token = strtok(NULL, delimiter);
    }

    // printf("Parameter is: %s \n", config.parameter);
    // printf("Value is: %s \n", config.value);
    return config;
}

void _readFromConfigFile() {
    FILE* fp;
    char* line = NULL;
    size_t bufferSize = 0;
    ssize_t read;
    int lineCount = 0;

    fp = fopen("./.tashrc", "r");
    if(fp == NULL) {
        // cant find config file
        error();
    }

    while ((read = getline(&line, &bufferSize, fp)) != -1) {
        CONFIG_ITEM configItem = {};
        configItem = _tokenizer(line);
        parsedConfigList[lineCount] = configItem;
        lineCount++;
    }

    fclose(fp);
    if (line)
        free(line);
}

char* getConfig(char* parameter) {
    for (int i = 0; i < MAX_CONFIG_ITEMS; ++i) {
        if (strcmp(parameter, parsedConfigList[i].parameter) == 0) {
            return parsedConfigList[i].value;
        }
    }
    return "";
}

void initializeGlobalVariables() {
    // make a list of config items from file
    _readFromConfigFile();
    // for (int i = 0; i < MAX_CONFIG_ITEMS; ++i) {
    //     printf("Parameter is: %s \n", parsedConfigList[i].parameter);
    //     printf("Value is: %s \n", parsedConfigList[i].value);
    // }
    // printf("DEFAULT_PATH is: %s\n", getConfig("DEFAULT_PATH"));

    // set global path variable
    path = getConfig("DEFAULT_PATH");
}