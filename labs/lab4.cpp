#include "lab4.h"

bool bExit = FALSE;

DWORD WINAPI Thread1(LPVOID lpParameter)
{
    char str[100]; // Declare a buffer to store input

    printf("Thread1 begins\n");
    while(!bExit) {
        // Wait for user input
        gets(str);
        printf("User input: %s\n", str);

        // If "exit", terminate the thread
        if(!strcmp(str, "exit")) {
            bExit = TRUE;
            printf("Thread1 finished\n");
            break;
        }

        // If "next", pass control to Thread 2 (use SetEvent, etc. here)
        if(!strcmp(str, "next")) {
            printf("Thread1 releasing control to Thread2\n");
            // Use SetEvent or similar to signal Thread 2
        }
    }
    return 0;
}
DWORD WINAPI Thread2(LPVOID lpParameter)
{
    // Implement similar logic to Thread1
    printf("Thread2 begins\n");

    char str[100]; // Declare a buffer to store input
    while(!bExit) {
        // Wait for user input
        gets(str);
        printf("User input: %s\n", str);

        // If "exit", terminate the thread
        if(!strcmp(str, "exit")) {
            bExit = TRUE;
            printf("Thread2 finished\n");
            break;
        }

        // If "next", pass control to Thread 1
        if(!strcmp(str, "next")) {
            printf("Thread2 releasing control to Thread1\n");
            // Use SetEvent or similar to signal Thread 1
        }
    }
    return 0;
}

void lab4(){
    printf("I am main! Hello!\n");

    // Create Thread 1 and Thread 2
    HANDLE hThread1 = CreateThread(NULL, 0, Thread1, NULL, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, Thread2, NULL, 0, NULL);

    // Wait for both threads to finish
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    // Clean up
    CloseHandle(hThread1);
    CloseHandle(hThread2);

}