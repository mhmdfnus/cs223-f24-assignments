#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pwd.h>
#include <string.h>

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void print_prompt() {
    char cwd[1024];
    char hostname[1024];
    struct passwd *pw = getpwuid(geteuid());

    getcwd(cwd, sizeof(cwd));               // Get current working directory
    gethostname(hostname, sizeof(hostname)); // Get the hostname

    printf(ANSI_COLOR_GREEN "%s@%s" ANSI_COLOR_RESET ":" ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "$ ", 
           pw->pw_name, hostname, cwd); // Print a colored prompt
}

int main() {
    char *input;
    char *args[100];
    
    while (1) {
        print_prompt(); // Print custom prompt
        input = readline(""); // Read input
        if (strlen(input) > 0) add_history(input); // Save to history if not empty

        if (strcmp(input, "exit") == 0) { // Exit command
            free(input);
            break;
        }

        // Parse input into command and arguments
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        
        // Ensure args array is NULL-terminated
        args[i] = NULL;

        // Skip empty commands
        if (args[0] == NULL) {
            free(input);
            continue;
        }

        pid_t pid = fork(); // Create a child process
        if (pid == 0) {
            // Child process: Execute the command
            if (execvp(args[0], args) == -1) {
                perror("Error executing command"); // Report error if execvp fails
            }
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("Fork failed"); // Report error if fork fails
        } else {
            // Parent process: Wait for the child to finish
            int status;
            waitpid(pid, &status, 0);
        }

        free(input); // Free the input buffer
    }

    return 0;
}
