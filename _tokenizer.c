

char* tokenize(char** usr_input_string) {
    char* token = strtok(usr_input_string, "&");
    int actual_num_cmds=0;
      while (token != NULL) {
         usr_cmds_separated[actual_num_cmds]=token;
         actual_num_cmds++;
         token = strtok(NULL, "&");
      }

      for (int j=0; j<(actual_num_cmds); j++) {
         token = strtok(usr_cmds_separated[j]," ");
         printf("token: %s\n", token);
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
      return token;
}