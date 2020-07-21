#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "Utils.h"

DWORD Utils::GetProcessID(const char* process) {
	/*
	Function takes in a string value for the process it is looking for like ST3Monitor.exe
	then loops through all of the processes that are currently running on windows.
	If the process is found it is running, therefore the function returns true.
	*/
	bool procRunning = false;

	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return 0;
	}
	else {
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hProcessSnap, &pe32)) { // Gets first running process
			if (pe32.szExeFile == process) {
				procRunning = true;
			}
			else {
				// loop through all running processes looking for process
				while (Process32Next(hProcessSnap, &pe32)) {
					// Set to an AnsiString instead of Char[] to make compare easier
					if (strcmp(pe32.szExeFile, process) == 0)
					{
						return (DWORD)pe32.th32ProcessID;
					}
				}
			}
			// clean the snapshot object
			CloseHandle(hProcessSnap);
			return 0;
		}
	}
}
