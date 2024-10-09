

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>


#include "labs/lab3.h"
#include "labs/lab4.h"
#include "labs/lab5.h"

//ЛАБОРАТОРНАЯ РАБОТА СОВМЕСТНОЕ ИСПОЛЬЗОВАНИЕ ОБЪЕКТОВ
void lab6_objects(){
    HANDLE hMutex = CreateMutex(NULL, FALSE, TEXT("MyMutex"));
    if (hMutex == NULL) {
        std::cerr << "CreateMutex failed: " << GetLastError() << std::endl;
        return;
    }

    while (true) {
        DWORD dwWaitResult = WaitForSingleObject(hMutex, INFINITE);
        if (dwWaitResult == WAIT_OBJECT_0) {
            std::string input;
            std::cout << "Enter command (A/B/exit): ";
            std::getline(std::cin, input);

            if (input == "A") {
                std::cout<<"process A\n";
                ReleaseMutex(hMutex);
            }else if(input =="B"){
                std::cout<<"process B\n";
                ReleaseMutex(hMutex);
            }
            else if (input == "exit") {

                std::cout<<"exiting the process \n";
                ReleaseMutex(hMutex);
                break;
            }
        } else {
            std::cerr << "WaitForSingleObject failed: " << GetLastError() << std::endl;
            break;
        }
    }

    CloseHandle(hMutex);

}




int main(int argc, char* argv[]) {
    lab6_objects();
    return 0;
}



