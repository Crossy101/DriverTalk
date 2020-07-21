#include <string>
#include <locale> 
#include <ntstatus.h>

#include "IDriver.h"
#include "Utils.h"

using namespace std;

IDriver::IDriver()
{
	this->hDriver = CreateFileA("\\\\.\\AgileVPS", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
}

NTSTATUS IDriver::ReadVirtualMemory(DWORD pid, ptr_t base, ptr_t size, PVOID buffer)
{
	DWORD bytes = 0;
	COPY_MEMORY copyMem = { 0 };

	copyMem.pid = pid;
	copyMem.targetPtr = base;
	copyMem.localbuf = (ULONGLONG)buffer;
	copyMem.size = size;
	copyMem.write = FALSE;

	if (this->hDriver == INVALID_HANDLE_VALUE)
		return STATUS_INTERRUPTED;

	if (DeviceIoControl(this->hDriver, IO_DISPATCH_READWRITE, &copyMem, sizeof(copyMem), nullptr, 0, &bytes, NULL))
		return STATUS_SUCCESS;
}

NTSTATUS IDriver::WriteVirtualMemory(DWORD pid, ptr_t base, ptr_t size, PVOID buffer)
{
	DWORD bytes = 0;
	COPY_MEMORY copyMem = { 0 };

	copyMem.pid = pid;
	copyMem.targetPtr = base;
	copyMem.localbuf = (ULONGLONG)buffer;
	copyMem.size = size;
	copyMem.write = TRUE;

	if (this->hDriver == INVALID_HANDLE_VALUE)
		return STATUS_INTERRUPTED;

	if (DeviceIoControl(this->hDriver, IO_DISPATCH_READWRITE, &copyMem, sizeof(copyMem), nullptr, 0, &bytes, NULL))
		return STATUS_SUCCESS;
}

NTSTATUS IDriver::WriteBytesVirtualMemory(DWORD pid, ptr_t base, ptr_t size, byte* allBytes)
{
	DWORD bytes = 0;
	COPY_MEMORY copyMem = { 0 };
	

	for (int i = 0; i < size - 1; i++)
	{
		PVOID buffer = (PVOID)(*(allBytes + i));

		copyMem.pid = pid;
		copyMem.targetPtr = (base + i);
		copyMem.localbuf = (ULONGLONG)buffer;
		copyMem.size = 1;
		copyMem.write = TRUE;

		if (this->hDriver == INVALID_HANDLE_VALUE)
			return STATUS_INTERRUPTED;

		if (DeviceIoControl(this->hDriver, IO_DISPATCH_READWRITE, &copyMem, sizeof(copyMem), nullptr, 0, &bytes, NULL))
			return STATUS_SUCCESS;
	}
}

NTSTATUS IDriver::ProtectMem(DWORD pid, ptr_t base, ptr_t size, DWORD protection)
{
	DWORD bytes = 0;
	PROTECT_MEMORY protectMem = { 0 };

	protectMem.pid = pid;
	protectMem.base = base;
	protectMem.size = size;
	protectMem.newProtection = protection;

	// Not loaded
	if (this->hDriver == INVALID_HANDLE_VALUE)
		return STATUS_DEVICE_DOES_NOT_EXIST;

	if (!DeviceIoControl(this->hDriver, IO_DISPATCH_PROTECT, &protectMem, sizeof(protectMem), nullptr, 0, &bytes, NULL))
		return STATUS_SUCCESS;

	return STATUS_SUCCESS;
}

/*
void IDriver::AllocateVirtualMemory(ULONG processID, ULONGLONG addressToAllocate, SIZE_T sizeOfMemoryToAllocate)
{
	info ReadRequest;

	ReadRequest.pid = processID;

	if (addressToAllocate != NULL)
		ReadRequest.address = addressToAllocate;
	else
		ReadRequest.address = NULL;

	if (ReadRequest.size > 0)
		ReadRequest.size = sizeOfMemoryToAllocate;
	else
		ReadRequest.size = 0;

	DeviceIoControl(hDriver, IO_DISPATCH_ALLOCATE, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0);

	cout << "New Address: " << ReadRequest.address << endl;
}

void IDriver::ProtectVirtualMemory(int processID, ULONGLONG addressToProtect, SIZE_T sizeOfMemoryToProtect, int newProtection)
{
	info ReadRequest;

	ReadRequest.pid = processID;
	ReadRequest.protectionAddress = addressToProtect;
	ReadRequest.protectionSize  = sizeOfMemoryToProtect;
	ReadRequest.protectionRequested = newProtection;

	DeviceIoControl(hDriver, IO_DISPATCH_PROTECT, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0);
}

void IDriver::GetBaseAddress(int processID)
{
	info ReadRequest;

	ReadRequest.pid = processID;

	if (DeviceIoControl(hDriver, IO_DISPATCH_MODBASE, &ReadRequest, sizeof(ReadRequest), &ReadRequest, sizeof(ReadRequest), 0, 0))
		this->gameModuleBaseAddress = (ULONGLONG)ReadRequest.value;
	else
		cout << "Cannot get base address!" << endl;

	//1073741824
	//1073741824
}
*/
