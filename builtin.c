#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "error.h"
#include "globals.h"
#include <errno.h>
#include <string.h>



void usrexit(int num_args){
   if (num_args > 0) error();
   else exit(0);
//exit program 
}

void usrchdir(char ***p,int index){
   char cwd[PATH_MAX];

   //if(strcmp(cmd_args[0][0],"")==0 ){
   if(strcmp(p[index][0],"") ==0){
      //printf("no arguments passed in. fail \n");
      error();
   }
   else{
         if(strcmp(p[index][1],"") ==0){

         //printf("only one argument passed so it works. %s %zu \n", p[index][0], strlen(p[index][0]));
         //p[index][0][strcspn(p[index][0], "\n")] = 0; // remove trailing \n character
         int x = chdir(p[index][0]);
         if (!x) CURRENT_DIR = &p[index][0];
         else {
            error();
            //printf("Change dir failed \n");
            //printf("ERRNO: %s\n", strerror(errno));
         }
         getcwd(cwd, sizeof(cwd));
         // printf("Currrent working directory is: %s\n", cwd);
         // printf("%d \n",x);      
      }
      else{
         //printf("more than one argument passed in. error.\n");
         error();
      }

   }  
}

// void usrpath(char** args, int num_args){
//    int arraySize = getStringArraySizeBytes(args, num_args);
//    // char** newPath = (char**) malloc(arraySize * sizeof(char*));
//    char newPath[arraySize];
//    newPath[0] = '\0'; // null terminate to use strcat
//    int i;
//    // printf("usrpath  %s\n", args[0]);
//    // printf("usrpath input size %d\n", getStringArraySizeBytes(args, num_args));
//    for (i = 0; i < num_args; ++i) {
//       strcat(newPath, args[i]);
//       strcat(newPath, "/ ");
//    }
//    // while (inputPath[index][i] != NULL) {
//    //    if (i != 0) strcat(newPath, " ");
//    //    strcat(newPath, inputPath[index][i]);
//    //    i++;
//    // }
//    // newPath[strcspn(newPath, "\n")] = 0; // remove trailing \n character
//    // printf("usrpath: newPath is %s with length %zu\n", newPath, strlen(newPath));
//    PATH = (char*) malloc(strlen(newPath) + 1);
//    strcpy(PATH, newPath);
//    // PATH = newPath;
//    // printf("newPATH %s\n", PATH);
// }
void usrpath(char** args, int num_args) {
   int arraySize = getStringArraySizeBytes(args, num_args);
   PATH = (char*) malloc(arraySize + 1);
   PATH[0] = '\0';
   int i;
   for (i = 0; i < num_args; ++i) {
      strcat(PATH, args[i]);
      strcat(PATH, "/ ");
   }
}