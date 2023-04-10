/*
This C program demonstrates the use of signals and forks in the Linux operating system.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#define ID_LENGTH 8              // Define a constant ID length
#define PROC_NAME_PREFIX "fork_" // Define a prefix for process names

// Function to generate a random ID string of a specified length
void generate_random_id(char *id, int length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < length; ++i) {
        id[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    id[length] = '\0';      // Add null-terminator to the end of the ID string
}

// Function to change the process name
void set_process_name(char *argv[], const char *name) {
    strncpy(argv[0], name, strlen(name) + 1); // Copy the name to the first element of the argv array
    argv[1] = NULL;     // Set the second element to NULL
}

// Function to handle signals received by the program
void signal_handler(int signal, char *argv[]) {
    if (signal == SIGUSR1) {    // If received SIGUSR1 signal
        printf("Received SIGUSR1. Terminating process: %d\n", getpid()); 
        exit(0);                // Terminate the process
    } else if (signal == SIGUSR2) { // If received SIGUSR2 signal
        pid_t pid = fork();     // Create a new fork
        if (pid == 0) {         // If executing in the child process
            char id[ID_LENGTH + 1]; // Generate a random ID
            generate_random_id(id, ID_LENGTH); 
            char proc_name[ID_LENGTH + strlen(PROC_NAME_PREFIX) + 1]; // Generate a process name using the ID and prefix
            snprintf(proc_name, sizeof(proc_name), "%s%s", PROC_NAME_PREFIX, id); 
            set_process_name(argv, proc_name); // Set the process name
            printf("Created new fork. Process: %d, ID: %s, Name: %s\n", getpid(), id, proc_name);
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));  // Initialize the random number generator using the current time

    printf("Main process started. Process: %d, Name: %s\n", getpid(), argv[0]);

    while (1) { // Enter an infinite loop to wait for signals
        int signum;
        sigset_t sigset;
        sigemptyset(&sigset);   // Initialize the signal set
        sigaddset(&sigset, SIGUSR1); // Add SIGUSR1 and SIGUSR2 signals to the set
        sigaddset(&sigset, SIGUSR2);

        sigprocmask(SIG_BLOCK, &sigset, NULL); // Block the signals
        sigwait(&sigset, &signum);  // Wait for a signal to be received

        signal_handler(signum, argv); // Handle the signal
    }

    return 0;
}
