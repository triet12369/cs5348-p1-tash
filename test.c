
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include "process.h"

#define EXIT_CODE_SUCCESS 0
#define EXIT_CODE_ERROR 1
#define TASH_PROMPT "tash> "

void usrexit();
void usrchdir(const char***,int);

int
tashLoop()
{
	int exitCode = EXIT_CODE_SUCCESS;
	int shouldRun = 1;
	char argsLine[20];
	
	while(shouldRun) {


      char *usr_input_string;
      
      size_t buffsize = 20;
      usr_input_string= (char*) malloc (buffsize*sizeof(char));

      
      //create dynamic 2D char arrays to store 1. user commands 2. user arguments per process 
      
      //maximum number of commands a user can have in 1 line
      int num_cmds=20;
      //maximum number of arguments a user can provide for a command 
      int num_args_per_cmd=5;
      //maximum number of characters per argument
      int cmd_arg_len = 30;
      
      //used calloc cause it gets initialized to zero at declaration which we can use as a check

      char **usr_cmds_separated=(char**) calloc(num_cmds, sizeof(char*));
      char **usr_cmd = (char**) calloc(num_cmds, sizeof(char*));
      char ***cmd_args = (char***) calloc(num_cmds, sizeof(char**));

      if(cmd_args==NULL || usr_cmd==NULL || usr_cmds_separated==NULL || usr_input_string==NULL){
            perror("unable to allocate memory requested.");
            exit(1);
         }

      for (int i=0; i<num_cmds; i++){

         usr_cmd[i]=(char*)calloc (cmd_arg_len,sizeof(char));
         usr_cmds_separated[i]=(char*)calloc (cmd_arg_len,sizeof(char));
         cmd_args[i]= (char**)calloc (num_args_per_cmd,sizeof(char*));
         

         if(cmd_args[i]==NULL || usr_cmd[i]==NULL || usr_cmds_separated[i]==NULL){
            perror("unable to allocate memory requested.");
            exit(1);
         }

         for(int j=0; j<num_args_per_cmd; j++){
            cmd_args[i][j]=(char*)calloc (cmd_arg_len,sizeof(char));
            if(cmd_args[i][j]==NULL){
               perror("unable to allocate memory requested.");
               exit(1);
            }          
         }
         
         
      }

      // read input from tash      
      size_t num_characters = getline(&usr_input_string, &buffsize, stdin);

   
      

      //tokenize into separate commands first      
      char* token = strtok(usr_input_string, "&");
            
      int actual_num_cmds=0;
      while (token != NULL) {

         usr_cmds_separated[actual_num_cmds]=token;
         actual_num_cmds++;
         token = strtok(NULL, "&");
      }

      
      
      


      for (int j=0; j<(actual_num_cmds); j++){
         token = strtok(usr_cmds_separated[j]," ");
         if(token!=NULL){
            usr_cmd[j] = token;
         }

         int k=0;

         while(token!=NULL){
            token=strtok(NULL, " ");
            cmd_args[j][k]=token;
            k++;
         }
      }

      //by this point usr_cmd[] will have the cmds, cmd_args[][] will have the cmd args
      //ex usr_cmd[0] will have cmd_args[0][j] : how many cmd args? check with zero

      //actual_num_cmds
      printf("%d the actual number of commands",actual_num_cmds);
      
      //WE NEED TO HANDLE THE ENTER (\n)
      char *var[]={"exit","exit\n","cd","cd\n","path","path\n"};
      /*
      if(strcmp(usr_cmd[0],var)==0){
         printf("%c value stored in usr_cmd[0]",(*usr_cmd[5]+'0'));
      }
      */
      //unused slots have zero stored as an integer.

      for(int i=0; i<actual_num_cmds;i++){
         if(strcmp(usr_cmd[i],var[0])==0 || strcmp(usr_cmd[i],var[1])==0){            
            printf("exit worked");
            usrexit();
         }
         if(strcmp(usr_cmd[i],var[2])==0 || strcmp(usr_cmd[i],var[3])==0){
            printf("%s %s command arguments are",cmd_args[i][0],cmd_args[i][1]);
            usrchdir(&cmd_args[i],i);
            printf("cd worked");
            
         }
      }

      //need to confirm zero



      //check usr_cmd exists
         //first lets compare to built in as it runs serially 

         //then lets check path variable and here we will fork babyyyy



		printf(TASH_PROMPT);

      //need to change this val later
      shouldRun =0;
	
	}

	return exitCode;
}


void path(){
//overwrite path files
}

void usrexit(){
   exit(0);
//exit program 
}

void usrchdir(const char ***p,int index){
 
   if(p[index][1]==NULL){
      printf("only one argument passed so it works.");
      int x=chdir(p[index][0]);
      printf("%d",x);      
   }
   else{
      printf("more than one argument passed in. error.\n");
   }
}


int main()
{
	printf(TASH_PROMPT);
	
	// actual tash main logic
	int exitCode;
	exitCode = tashLoop();
	return exitCode;
} 

