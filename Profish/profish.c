// Author: Hassan
// EXTRA CREDIT FEATURES: EC1, EC2, EC3, EC4 implemented
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <errno.h>
#include <sys/time.h>
#include <stdarg.h>

#define MAX_COMMAND_LENGTH 65
#define DELIMITER_LENGTH 80

// Function prototype
void printDelimiterLine(const char *label, const char *command);

// Function to execute the command
void executeCommand(char *command, char *args[]) {
    pid_t pid;
    int status;
    struct rusage usage;
    struct timeval start_time, end_time;
    long elapsed_time_ms;

    // Start timing
    gettimeofday(&start_time, NULL);

    // Create a temporary file to capture the command output
    FILE *output_file = fopen("profish_output.txt", "w");
    if (output_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        // Redirect stdout to the temporary file
        dup2(fileno(output_file), STDOUT_FILENO);
        fclose(output_file);

        // Execute the command
        execvp(command, args);

        // If execvp fails, print an error and exit
        perror("execvp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        // Wait for the child process to terminate
        waitpid(pid, &status, 0);
        fclose(output_file);

        // End timing
        gettimeofday(&end_time, NULL);
        elapsed_time_ms = (end_time.tv_sec - start_time.tv_sec) * 1000 +
                          (end_time.tv_usec - start_time.tv_usec) / 1000;

        // Print captured output
        printDelimiterLine(command, "");

        // Display command output
        output_file = fopen("profish_output.txt", "r");
        if (output_file != NULL) {
            char output[BUFSIZ]; // Use a standard buffer size
            while (fgets(output, sizeof(output), output_file) != NULL) {
                printf("%s", output);
            }
            fclose(output_file);
        } else {
            printf("Failed to open output file.\n");
        }

        // Print delimiter line
        printf("%.*s\n", DELIMITER_LENGTH, "--------------------------------------------------------------------------------");

        // Profile command's resource usage
        if (getrusage(RUSAGE_CHILDREN, &usage) == -1) {
            perror("getrusage");
            exit(EXIT_FAILURE);
        }

        // Print profiling results
        printf("Total elapsed time: %ld ms\n", elapsed_time_ms);
        printf("\x1b[34mRuntime:\x1b[0m\n");
        printf("Total CPU user time: %ld microsec\n", usage.ru_utime.tv_sec * 1000000 + usage.ru_utime.tv_usec);
        printf("Total CPU kernel time: %ld microsec\n", usage.ru_stime.tv_sec * 1000000 + usage.ru_stime.tv_usec);
        printf("Linux CPU time acct: %ld microsec\n", (usage.ru_utime.tv_sec + usage.ru_stime.tv_sec) * 1000000 + (usage.ru_utime.tv_usec + usage.ru_stime.tv_usec));

        printf("\x1b[34mTime components:\x1b[0m\n");
        printf("CPU user time: %ld sec \n", usage.ru_utime.tv_sec);
        printf("CPU kernel time: %ld sec \n", usage.ru_stime.tv_sec);
        printf("CPU user time: %ld microsec\n", usage.ru_utime.tv_usec);
        printf("CPU kernel time: %ld microsec\n", usage.ru_stime.tv_usec);

        printf("\x1b[34mMemory metrics:\x1b[0m\n");
        printf("Max resident set size: %ld\n", usage.ru_maxrss);
        printf("Integral shared memory size: %ld\n", usage.ru_ixrss);
        printf("Integral unshared data size: %ld\n", usage.ru_idrss);
        printf("Integral unshared stack size: %ld\n", usage.ru_isrss);
        printf("Page reclaims: %ld\n", usage.ru_minflt);
        printf("Page faults: %ld\n", usage.ru_majflt);
        printf("Swaps: %ld\n", usage.ru_nswap);

        printf("\x1b[34mI/O operations:\x1b[0m\n");
        printf("Block input ops: %ld\n", usage.ru_inblock);
        printf("Block output ops: %ld\n", usage.ru_oublock);
        printf("IPC msgs sent: %ld\n", usage.ru_msgsnd);
        printf("IPC msgs recv'd: %ld\n", usage.ru_msgrcv);

        printf("\x1b[34mInterrupts metrics:\x1b[0m\n");
        printf("Signals recv'd: %ld\n", usage.ru_nsignals);
        printf("Voluntary context switches: %ld\n", usage.ru_nvcsw);
        printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);

        // Remove temporary output file
        remove("profish_output.txt");
    }
}

// Function to print an 80-character delimiter line
void printDelimiterLine(const char *label, const char *command) {
    if (label != NULL && command != NULL) {
        int label_length = strlen(label);
        int command_length = strlen(command);
        int num_dashes = DELIMITER_LENGTH - label_length - command_length - 8; // 8 accounts for the static characters "CMD: " and " "
        printf("-----CMD: %s %s ", command, label);
        for (int i = 0; i < num_dashes; i++) {
            printf("-");
        }
        printf("\n");
    } else {
        for (int i = 0; i < DELIMITER_LENGTH; i++) {
            printf("-");
        }
        printf("\n");
    }
}

int main() {
    char input[MAX_COMMAND_LENGTH + 1];
    char *command;
    char **args = NULL; // Dynamic allocation for storing arguments
    char *token;
    int arg_count = 0;

    // Read user input
    printf("profish>");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove trailing newline

    // Parse command and arguments
    token = strtok(input, " ");
    command = token;
    args = malloc(sizeof(char *) * 2); // Allocate memory for the command and a NULL terminator
    if (args == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    args[arg_count++] = token;
    while ((token = strtok(NULL, " ")) != NULL) {
        args = realloc(args, sizeof(char *) * (arg_count + 2)); // Resize memory for additional argument and NULL terminator
        if (args == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        args[arg_count++] = token;
    }
    args[arg_count] = NULL; // Null-terminate the arguments array

    // Execute the command
    executeCommand(command, args);

    // Free dynamically allocated memory
    free(args);

    return 0;
}
