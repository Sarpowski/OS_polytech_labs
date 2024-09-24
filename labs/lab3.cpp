#include "lab3.h"
#include <stdio.h>
#include <iostream>


LPCWSTR lpCommandLine[3] = {
        L"C:\\WINDOWS\\SYSTEM32\\NOTEPAD.EXE temp.txt",
        L"C:\\WINDOWS\\SYSTEM32\\CALC.EXE",
        L"C:\\WINDOWS\\SYSTEM32\\CHARMAP.EXE"
};

STARTUPINFOW startInfo;
PROCESS_INFORMATION processInfo;



void lab3()
{
    ZeroMemory(&startInfo, sizeof(STARTUPINFOW));
    startInfo.cb = sizeof(startInfo);

    WCHAR proc_name[256];
    for (int i = 0; i < 3; i++) {
        wcscpy_s(proc_name, sizeof(proc_name) / sizeof(WCHAR), lpCommandLine[i]);
        wprintf(L"!!!%ls %d\n", lpCommandLine[i], (int)wcslen(lpCommandLine[i]));

        if (!CreateProcessW(NULL, proc_name, NULL, NULL, FALSE,
                            HIGH_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL,
                            &startInfo, &processInfo)) {
            fprintf(stderr, "CreateProcess failed with error %d\n", GetLastError());
            ExitProcess(1);
        }
    }

    std::wcout << L"ProcessHandle=" << processInfo.hProcess << std::endl;
    std::wcout << L"ThreadHandle=" << processInfo.hThread << std::endl;

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
}