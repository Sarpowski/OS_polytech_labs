#include "lab5.h"



void lab5(){

    int K = 0;
    std::cout<<"enter the K value \n";
    std::cin >> K;
    //(K seconds)

    if (K <= 0) {
        std::cerr << "Please provide a positive number of seconds." << std::endl;
    }

    //waitable timer
    HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (!hTimer) {
        std::cerr << "Failed to create waitable timer!" << std::endl;
    }


    // K seconds
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -static_cast<LONGLONG>(K) * 10000000LL;  // 1 second = 10,000,000 100-nanosecond intervals

    if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, FALSE)) {
        std::cerr << "Failed to set waitable timer!" << std::endl;
        CloseHandle(hTimer);

    }


    for (int i = 1; i <= K; ++i) {
        std::cout << i << " second(s)" << std::endl;
        Sleep(1000);  // Pause for 1 second between each print
    }


    WaitForSingleObject(hTimer, INFINITE);

    std::cout << "Time's up! Exiting program." << std::endl;

    // Clean up
    CloseHandle(hTimer);
}