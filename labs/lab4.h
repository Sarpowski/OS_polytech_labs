#ifndef LAB3CPP_LAB4_H
#define LAB3CPP_LAB4_H


#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>


// Global flag for thread termination
extern bool bExit;

DWORD WINAPI Thread1(LPVOID lpParameter);
DWORD WINAPI Thread2(LPVOID lpParameter);
void lab4();


#endif
