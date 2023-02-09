#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <TlHelp32.h>


DWORD GetProcName(const char* procName)
{
	DWORD procID = 0;
	HANDLE hSNAP = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSNAP != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSNAP, &procEntry))
		{
			do
			{
				if (!_stricmp(procEntry.szExeFile, procName))
				{

					procID = procEntry.th32ProcessID;
					break;


				}

			} while (Process32Next(hSNAP, &procEntry));

		}


	}
	CloseHandle(hSNAP);
	return procID;


}

int main()
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 0);
	HANDLE hProc;
	HANDLE rThread;
	PVOID remote_buffer;
	LPTHREAD_START_ROUTINE pLoadLibrary = NULL;
	const char dll_path[] = "";
	size_t dll_size = strlen(dll_path);


	int processID = GetProcName("Process.exe");



	if (!processID)
	{
		MessageBox(NULL, "Process process could not be found.", "Warning", MB_OK);
		exit(1);
	}

	std::cout << "[*] Process ID: " << processID << std::endl;


	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(processID));
	if (!hProc)
	{
		MessageBox(NULL, "Handle couldn't be Established", "Warning", MB_OK);
		exit(1);
	}
	std::cout << '\n' << "[*] Allocating Memory...";
	remote_buffer = VirtualAllocEx(hProc, NULL, dll_size, MEM_COMMIT | PAGE_READWRITE, PAGE_EXECUTE_READWRITE);

	std::cout << '\n' << "[*] Injecting Dll...";

	WriteProcessMemory(hProc, remote_buffer, (LPVOID)dll_path, dll_size, NULL);




	pLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

	if (!pLoadLibrary)
	{
		MessageBox(NULL, "Failure getting LoadLibraryA", "Error", MB_OK);
		exit(1);
	}

	rThread = CreateRemoteThread(hProc, NULL, 0, pLoadLibrary, remote_buffer, 0, NULL);

	if (!rThread)
	{
		MessageBox(NULL, "Error Creating Thread", "Error", MB_OK);
		exit(1);

	}

	std::cout << '\n' << "[*] Injection Complete.";

	CloseHandle(hProc);



	return 0;




}