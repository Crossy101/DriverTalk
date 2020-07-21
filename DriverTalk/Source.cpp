#include <iostream>
#include <Windows.h>

#pragma comment(lib, "winmm.lib")

#include "IDriver.h"
#include "Utils.h"

using namespace std;

bool bootScreenStarted = false;
bool gameStarted = false;

#define BRAKING_PRESSURE 0.9

bool ConnectToGame();

int main(int argc, char* argv[])
{
	Utils* processUtils = new Utils();

	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (bootScreenStarted == false)
	{
		while (bootScreenStarted == false)
		{
			cout << "Looking for game to start!" << endl;

			int id = processUtils->GetProcessID("iRacingSim64DX11.exe");
			bootScreenStarted = true;
			Sleep(1000);
		}
		//Sleep(60000);
		delete processUtils;
		ConnectToGame();
	}
}

bool ConnectToGame()
{
	Utils* processUtils = new Utils();

	DWORD gameID = processUtils->GetProcessID("iRacingSim64DX11.exe");
	if (gameID == 0)
	{
		cout << "Cannot find game!" << endl;
		return false;
	}
	else
	{
		gameStarted = true;
		IDriver* kDriver = new IDriver();

		//WRITE OVER EMPTY MEMORY SPACE
		kDriver->ProtectMem(gameID, processUtils->WriteSpeedAddress, 0x4, 0);
		//Write Speed Adjustment
		for (int i = 0; i < processUtils->WriteSpeedSize - 1; i++)
		{
			int value = (int)*(processUtils->WriteSpeedAdjustment + i);
			PVOID newVal = &value;
			kDriver->WriteVirtualMemory(gameID, processUtils->WriteSpeedAddress + i, 0x1, newVal);
		}

		//WRITE OVER THE ORIGINAL SPEED FUNCTION
		kDriver->ProtectMem(gameID, processUtils->WriteOriginalAddress, 0x4, 0);
		//Write Original Adjustment
		for (int i = 0; i < processUtils->WriteOriginalValueSize - 1; i++)
		{
			int value = (int)*(processUtils->WriteOriginalValue + i);
			PVOID newVal = &value;
			kDriver->WriteVirtualMemory(gameID, processUtils->WriteOriginalAddress + i, 0x1, newVal);
		}
		kDriver->ProtectMem(gameID, processUtils->WriteOriginalAddress, 0x4, 1);

		/*
		//WRITE OVER EMPTY MEMORY SPACE
		kDriver->ProtectMem(gameID, processUtils->WriteSpeedAddress, 0x4, 0);
		//Write Speed Adjustment
		for (int i = 0; i < 4; i++)
		{
			int value = (int)*(processUtils->speedAdustmentValue + i);
			PVOID newVal = &value;
			kDriver->WriteVirtualMemory(gameID, processUtils->speedAdjustmentAddress + i, 0x1, newVal);
		}
		*/
	}
}