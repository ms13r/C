#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

typedef struct CMD_ {
    char * name;
    char * args[100];
} command;

void parse_command(char * cmd, command * arr, int location);

int main(int argc, char *argv[]){
    char input[512];
    char *cmd, *cmd_args, *quit_locator;
    FILE * stream = stdin;
    command array[200];
    int i, c, numberofitems = 0, index = 0, QUIT_FLAG = 0, QUIT_INDEX = 0;
    pid_t child, child_status;

    if(argc == 2){
        stream = fopen(argv[1], "r");   // Attempt to open file if there is a second argument (batch mode)
        if(stream == NULL){
            fprintf(stderr, "\nError!\nCould not open file: %s\nAborting program.\n", argv[0]);
            exit(1);
        }
    }
    else if(argc > 2){    // Invalid number of arguments
        fprintf(stderr, "\nError!\nInvalid number of command line arguments.\nAborting program.");
        exit(1);
    }

    if(stream == stdin){ printf("prompt> "); }
    while(fgets(input, 512, stream) != NULL){
        // Check for the existence of quit in the line, and set the flag 
        quit_locator = strstr(input, "quit");
        if(quit_locator == NULL){ QUIT_FLAG = 0; }
        else{ QUIT_FLAG = 1; }

        // Zero out the memory within the array and reset the number of commands on the line and argument index
        memset(array, 0, sizeof array);
        numberofitems = 0;
        index = 0;

        if(strlen(input) > 512){
            // Flush the stream
            printf("\nError!\nThe line of length %d is too long.\n", (int)strlen(input));
            while((c = getchar()) != '\n'){ continue; }     // Flush the buffer so it does not produce erraneous input
        }
        else{
            // Begin to tokenize instruction
            cmd = strtok(input, ";\n");         // Tokenize instructions based on semicolon or newline
            array[numberofitems].name = cmd;
            while(cmd != NULL){
                numberofitems++;
                cmd = strtok(NULL, ";\n");      // Continue parsing input for more commands
                array[numberofitems].name = cmd;
            }
        }
        // Parse through the commands and get the arguments
        for(i = 0; i < numberofitems; i++){
            cmd_args = strtok(array[i].name, " ");      // Tokenize name and gather arguments
            array[i].args[index] = cmd_args;
            if((QUIT_FLAG == 1) && (strcmp(cmd_args, "quit") == 0))
                QUIT_INDEX = i;
            while(cmd_args != NULL){
                index++;
                cmd_args = strtok(NULL, " ");   // Continue parsing arguments if there are any
                array[i].args[index] = cmd_args;    // Add arguments to struct (Adds NULL for execvp)
            }
            index = 0;
        }

        // Execute commands using fork
        for(i = 0; i < numberofitems; i++){
            if(QUIT_FLAG == 1 && i == QUIT_INDEX){ continue; }
            // Fork a child process; if execvp returns, make sure to handle error
            child = fork();
            if(child == 0){
                printf("\nAttempting to execute command: %s\n\n", array[i].name);
                execvp(array[i].args[0], array[i].args);        // Attempt to execute command
                fprintf(stderr, "\nError! Command could not be executed or does not exist: %s\n", array[i].name);
            }
            else if(child == (pid_t)(-1)){ fprintf(stderr, "Fork failed.\n"); } // If in parent, fork failed
        }

        for(i = 0; i < numberofitems; i++){
          if(QUIT_FLAG == 1 && i == QUIT_INDEX){ continue; }    // If quit, do not wait and skip
          child_status = wait(&c);
          printf("\nChild returned with status: %d\n", c);
        }

        if(QUIT_FLAG == 1){ break; }    // If a quit was present, exit the shell
        if(stream == stdin){ printf("prompt> "); }
    }
    return 0;
}