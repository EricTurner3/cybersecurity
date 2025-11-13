
/*
    win64_dll_createprocess.c
    13 Nov 2025

    Build: x86_64-w64-mingw32-gcc -shared -o win64_proc.dll win64_dll_createprocess.c -s -fpermissive
*/
#include <windows.h>

// Function to be executed in the new thread
DWORD WINAPI LaunchPowerShellThread(LPVOID lpParam) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Command to execute a simple PowerShell command
    LPSTR command = "powershell.exe";

    // Create the process
    if(!CreateProcess(NULL,command,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)
    ) {
        // Handle error
        // You might want to log this error in a real application
        return 1;
    }

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Create a new thread to launch PowerShell
        CreateThread(NULL, 0, LaunchPowerShellThread, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}