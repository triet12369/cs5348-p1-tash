#ifndef BUILTIN
#define BUILTIN
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "error.h"
#include "globals.h"
#include <errno.h>
#include <string.h>



void usrexit(){
   exit(0);
//exit program 
}

void usrchdir(char ***p,int index){
   char cwd[PATH_MAX];
   if(p[index][1] == NULL){

      printf("only one argument passed so it works. %s %zu \n", p[index][0], strlen(p[index][0]));
      p[index][0][strcspn(p[index][0], "\n")] = 0; // remove trailing \n character
      int x = chdir(p[index][0]);
      if (!x) CURRENT_DIR = &p[index][0];
      else {
         error();
         printf("Change dir failed \n");
         printf("ERRNO: %s\n", strerror(errno));
      }
      getcwd(cwd, sizeof(cwd));
      printf("Currrent working directory is: %s\n", cwd);
      printf("%d \n",x);      
   }
   else{
      printf("more than one argument passed in. error.\n");
      error();
   }
}

void usrpath(char ***inputPath, int index){
   char newPath[100];
   newPath[0] = '\0'; // null terminate to use strcat
   int i = 0;
   while (inputPath[index][i] != NULL) {
      if (i != 0) strcat(newPath, " ");
      strcat(newPath, inputPath[index][i]);
      i++;
   }
   newPath[strcspn(newPath, "\n")] = 0; // remove trailing \n character
   // printf("usrpath: newPath is %s with length %zu\n", newPath, strlen(newPath));
   strcpy(PATH, newPath);
}

#endif