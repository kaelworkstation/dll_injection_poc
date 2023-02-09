#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <tchar.h>
#include <vector>

DWORD WINAPI Main_Thread(HMODULE hModule)
{
    MessageBoxA(NULL, "Hi", "Hi", MB_OK);
  
    return 0;

}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main_Thread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

