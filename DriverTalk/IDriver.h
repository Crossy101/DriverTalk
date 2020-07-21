#pragma once
#include <iostream>
#include <Windows.h>

#define IO_DISPATCH_READWRITE    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0100, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) //WRITING
#define IO_DISPATCH_PROTECT    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0101, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) //WRITING

using ptr_t = uint64_t;     // Generic pointer in remote process
using module_t = ptr_t;  // Module base pointer

typedef struct info_t {
	int pid = 0;
	ULONGLONG address;
	ULONGLONG value;
	size_t size;
	ULONGLONG data;

	int protectionRequested;
	size_t protectionSize;
	ULONGLONG protectionAddress;
}info, * p_info;

typedef struct _COPY_MEMORY
{
	ULONGLONG localbuf;         // Buffer address
	ULONGLONG targetPtr;        // Target address
	ULONGLONG size;             // Buffer size
	ULONG     pid;              // Target process id
	BOOLEAN   write;            // TRUE if write operation, FALSE if read
} COPY_MEMORY, * PCOPY_MEMORY;

typedef struct _PROTECT_MEMORY
{
	ULONGLONG base;             // Region base address
	ULONGLONG size;             // Region size
	ULONG     pid;              // Target process id
	ULONG     newProtection;    // New protection value
} PROTECT_MEMORY, * PPROTECT_MEMORY;


class IDriver
{
public:
	HANDLE hDriver;
	ULONGLONG gameImageBaseNormal = 0x0;
	ULONGLONG gameImageBaseAddress = 0x140000000;
	ULONGLONG gameBaseAddressRead = 0x0;

	ULONGLONG gameModuleBaseAddress = 0x0;

	IDriver();


	NTSTATUS ReadVirtualMemory(DWORD pid, ptr_t base, ptr_t size, PVOID buffer);
	NTSTATUS WriteVirtualMemory(DWORD pid, ptr_t base, ptr_t size, PVOID buffer);
	NTSTATUS WriteBytesVirtualMemory(DWORD pid, ptr_t base, ptr_t size, BYTE* allBytes);
	NTSTATUS ProtectMem(DWORD pid, ptr_t base, ptr_t size, DWORD protection);

	void AllocateVirtualMemory(ULONG processID, ULONGLONG addressToAllocate, SIZE_T sizeOfMemoryToAllocate);
	void GetBaseAddress(int processID);
};

