/*
Name: Gage Waller
Date: 2/12/2026
Description: A simple Unix shell
*/

#include <stdio.h> // printf, fgets
#include <stdlib.h> // malloc, exit, getenv
#include <string.h> // strtok, strcmp
#include <unistd.h> // fork, execvp, chdir, getcwd, dup2
#include <sys/types.h>
#include <sys/wait.h> // wait
#include <fcntl.h> // open
#include <errno.h> // errno

#define MAX_INPUT 1024 // max size of input line
#define MAX_ARGS 100 // max num of command argu


// print the current working directory with $
void printPrompt() {
    char cwd[1024];
    // gets current working directory
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        printf("%s$ ", cwd);
        fflush(stdout); // forces prompt to display
    } else {
        perror("getcwd"); // print error if above fails
    }
}

int main() {
    char input[MAX_INPUT]; // stores user input
    
    // main shell loop
    while (1){
        printPrompt(); // display prompt

        // read user input
        if (fgets(input, MAX_INPUT, stdin) == NULL){
            printf("\n");
            break; // exit if ctrl+D pressed
        }

        // removes trailing newline
        input[strcspn(input, "\n")] = 0;

        // skips empty input
        if (strlen(input) == 0)
            continue;

        char *args[MAX_ARGS]; // argument list for execvp
        char *token;
        int i = 0;

        char *inputFile = NULL; // file for input redirect
        char *outputFile = NULL; // file for output redirect


        // token input by spaces
        token = strtok(input, " ");

        while (token != NULL && i < MAX_ARGS - 1){
            
            // checks for input redirect 
            if (strcmp(token, "<") == 0){
                token = strtok(NULL, " ");
                inputFile = token;         
            }
            // checks for output redirect
            else if (strcmp(token, ">") == 0){
                token = strtok(NULL, " ");
                outputFile = token;        
            } else {
                args[i++] = token;            
            }

            token = strtok(NULL, " ");
        }    

        args[i] = NULL; // execvp need NULL ended array

        if (args[0] == NULL)
            continue;

        
        // exit cmd
        if (strcmp(args[0], "exit") == 0){
            break;    
        }

        // cd cmd
        if (strcmp(args[0], "cd") == 0){

            // if no argu - go to HOME dir
            if (args[1] == NULL){
                char *home = getenv("HOME");

                if (chdir(home) != 0) {
                    printf("Error %d (%s)\n", errno, strerror(errno));
                }
            } else {
                if (chdir(args[1]) != 0){
                    printf("Error %d (%s)\n", errno, strerror(errno));            
                }
            }

            continue;
        }

        // fork
        pid_t pid = fork();

        if (pid < 0){
            // fork failed
            perror("fork");
            continue;        
        }

        // child process
        if (pid == 0){
            
            // input redirect
            if (inputFile != NULL){
                int fd = open(inputFile, O_RDONLY);

                if (fd < 0){
                    printf("Error %d (%s)\n", errno, strerror(errno));
                    exit(EXIT_FAILURE);    
                }

                dup2(fd, STDIN_FILENO); // redirect stdin
                close(fd);
            }

            // output redirect
            if (outputFile != NULL){
                int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                if (fd < 0){
                    printf("Error %d (%s)\n", errno, strerror(errno));
                    exit(EXIT_FAILURE);            
                }

                dup2(fd, STDOUT_FILENO); // redirect stdout
                close(fd);
            }
            
            // replace child process with req program
            execvp(args[0], args);

            // if exec fail
            printf("Error %d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);

        } else {
            // parent wait
            wait(NULL);        
        }
    }

    return 0;
}
