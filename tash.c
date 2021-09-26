//need to increase buffsize ? 
//included error.h for displaying error incase memory allocation occurs
//need to deallocate memory by free( )
//moved memory allocation portion out of the while loop : we dont want to repeat allocation every getline
//should we exit program if memory allocation fails ? i think yes.
//do a check here to see if returned token is a newline. 
//if it is, ignore it. 
//Assumption (valid) : this is the first value stored in user_input_string.
//so if user only presses Enter, case is handled.
// case 1 : Enter  ;  go to a newline and display prompt
// case 5 : "  " & "  " Enter; error 
// case 3 : & "  " Enter; error
// case 4 : "   " & Enter; error
// case 2 : "  " Enter; go to a newline and display prompt
// case 5 : cmd1 Enter; usual | also get rid of newline 
// case 6 : cmd1 args Enter; usual | same as above
// case 7 : cmd1 args& cmd2 Enter; not usual but project1 specific (usual creates one process in background)
// case 8 : cmd1 args&cmd2 args Enter ...so on; project1 specific 

#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "execute.h"
#include "utilities.h"
#include "builtin.h"
#include "error.h"

#define EXIT_CODE_SUCCESS 0
#define EXIT_CODE_ERROR 1
#define TASH_PROMPT "tash> "

int tashLoop(FILE *);
void _batchmode(char *);

void _batchmode(char * argv){
   FILE *file_handle; 
   file_handle=fopen(argv,"r");
   if(file_handle==NULL) error();
   else{
      tashLoop(file_handle);
   }
}




int
tashLoop(FILE * arg_file)
{
   int batchmode_on=0;

   if(arg_file !=NULL) batchmode_on=1;

	int exitCode = EXIT_CODE_SUCCESS;
	int shouldRun = 1;
   int line_size =0;
	// char argsLine[20];
   char *usr_input_string;
      
   size_t buffsize = 20;
   /* usr_input_string= (char*) malloc (buffsize*sizeof(char));

      
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

   //If Allocation of memory fails :
   if(cmd_args==NULL || usr_cmd==NULL || usr_cmds_separated==NULL || usr_input_string==NULL){
      error();     
   }

   for (int i=0; i<num_cmds; i++){

      usr_cmd[i]=(char*)calloc (cmd_arg_len,sizeof(char));
      usr_cmds_separated[i]=(char*)calloc (cmd_arg_len,sizeof(char));
      cmd_args[i]= (char**)calloc (num_args_per_cmd,sizeof(char*));
      

      if(cmd_args[i]==NULL || usr_cmd[i]==NULL || usr_cmds_separated[i]==NULL){
         error();
      }

      for(int j=0; j<num_args_per_cmd; j++){
         cmd_args[i][j]=(char*)calloc (cmd_arg_len,sizeof(char));
         if(cmd_args[i][j]==NULL){
            error();
         }          
      }
      
      
   } */

   int handle_erroneous_inputs =1;
   int CASE_5;

   while(handle_erroneous_inputs && line_size>=0){
      
      CASE_5=0;

      while(shouldRun && (line_size>=0)) {

         //printf("Current PATH is: %s\n", PATH);
         //if (CURRENT_DIR) printf("(%s)", *CURRENT_DIR);
         if(!batchmode_on) printf(TASH_PROMPT);


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
         int num_args[num_cmds];

         //If Allocation of memory fails :
         if(cmd_args==NULL || usr_cmd==NULL || usr_cmds_separated==NULL || usr_input_string==NULL){
            error();
            usrexit(0);    
         }

         for (int i=0; i<num_cmds; i++){

            usr_cmd[i]=(char*)calloc (cmd_arg_len,sizeof(char));
            usr_cmds_separated[i]=(char*)calloc (cmd_arg_len,sizeof(char));
            cmd_args[i]= (char**)calloc (num_args_per_cmd,sizeof(char*));
            

            if(cmd_args[i]==NULL || usr_cmd[i]==NULL || usr_cmds_separated[i]==NULL){
               error();
               usrexit(0);
            }

            for(int j=0; j<num_args_per_cmd; j++){
               cmd_args[i][j]=(char*)calloc (cmd_arg_len,sizeof(char));
               if(cmd_args[i][j]==NULL){
                  error();
                  usrexit(0);
               }          
            }
            
            
         }



         // read input from tash      
         // size_t num_characters = getline(&usr_input_string, &buffsize, stdin);
         if(batchmode_on) line_size=getline(&usr_input_string, &buffsize, arg_file);
         else getline(&usr_input_string,&buffsize,stdin);
      
      


         
         // printf("before removing enter : %s",usr_input_string);
         char* token = strtok(usr_input_string, "\n");
         int actual_num_cmds=0;
      

         
         if(token==NULL){            
               // CASE 1
               //printf("detected user entering only Enter.");               
         }
         else{
            int r;
            int ampexists=0;
            
            for(r=0; r<strlen(token); r++){
               if(token[r]=='&'){
                  //printf("found an ampersand \n");
                  ampexists=1;
               }
            }
         
            if(ampexists){
               token = strtok(token, "&");
               if(token==NULL){
                  //printf("caught only ampersand error");               
                  error();
                  break;
               }
               else{
                  
                  usr_cmds_separated[actual_num_cmds]=token;
                  token = strtok(NULL, "&");
                  if(token==NULL){
                     //printf("Caught rest of the errors due to ampersand");
                     //handles wrong parallel command input types : "    " &, &"    ", EXCEPT "   "&"     " (when its whitespace)
                     //that error can be caught in usr_cmds_separated. if that values is NULL after removing whitespace.                      
                     error();
                     break;
                  }
                  while (token != NULL) {
                     actual_num_cmds++;              
                     usr_cmds_separated[actual_num_cmds]=token;
                     token = strtok(NULL,"&");
                  }
                  actual_num_cmds +=1;
               }

            }
            else{
               usr_cmds_separated[actual_num_cmds]=token;
               actual_num_cmds=1;
            }
            
         
            //separating out to specifically identify user commands
            for (int j=0; j<(actual_num_cmds); j++){
               token = strtok(usr_cmds_separated[j]," ");
               if(token!=NULL){
                  usr_cmd[j] = token;
                  int k=0;

                  token =strtok(NULL," ");

                  while(token!=NULL){
                     //printf("argument was set for command \n");
                     cmd_args[j][k]=token;
                     k++;
                     token=strtok(NULL, " ");
                  }
                  num_args[j] = k;
               }
               else{
                  //printf("caught error case 5");
                  CASE_5=1;
                  error(); // "   " & "   " CASE 5
                  break;
               }


            }

            if(CASE_5) break;


            
            //WE NEED TO HANDLE THE ENTER (\n)
            char *var[]={"exit","exit\n","cd","cd\n","path","path\n"};
            /*
            if(strcmp(usr_cmd[0],var)==0){
               printf("%c value stored in usr_cmd[0]",(*usr_cmd[5]+'0'));
            }
            */
            //unused slots have zero stored as an integer.

            for(int i=0; i<actual_num_cmds;i++){
               if (usr_cmds_separated[i]) {
                  cleanArgArray(&cmd_args[i][i], num_args[i]);
                  if(strcmp(usr_cmd[i],var[0])==0 || strcmp(usr_cmd[i],var[1])==0){            
                     //printf("exit worked");
                     usrexit(num_args[i]);
                  } else if(strcmp(usr_cmd[i],var[2])==0 || strcmp(usr_cmd[i],var[3])==0){
                     // printf("%s %s command arguments are",cmd_args[i][0],cmd_args[i][1]);
                     //printf(" the i value is : %d",i);
                     usrchdir(&cmd_args[i],i);
                     // printf("cd worked");
                  } else if(strcmp(usr_cmd[i],var[4])==0 || strcmp(usr_cmd[i],var[5])==0){
                     usrpath(&cmd_args[i],i);
                  } else {
                     executeArg(usr_cmd[i], &cmd_args[i][i], num_args[i]);
                  }
               }
            }

         }

      }

      //need to deallocate memory by free( ) here

   }
   fclose(arg_file);
	return exitCode;
}

// main(int argc, char *argv[])
// use this to get args for tash in batch mode
int
main(int argc, char* argv[])
{

	//printf("Welcome to Tash!\n");
   initializeGlobalVariables(); // read from tashrc and set the global variables

	// printf("The PATH variable is %s", PATH);

	// actual tash main logic
   if (argc==1){      
      int exitCode;
	   exitCode = tashLoop(NULL);
	   return exitCode;
   }
   else if(argc==2){
      _batchmode(argv[1]);
   }
   else{
      error();
   }
	
}

/*
int main(int argc, char** argv) {
    std::cout << "Have " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
}
*/