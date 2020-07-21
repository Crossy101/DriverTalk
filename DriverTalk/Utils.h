#pragma once
#include "IDriver.h"

class Utils
{
public:
	DWORD GetProcessID(const char* process);

	ptr_t WriteSpeedAddress = 0x14BF10000;
	byte WriteSpeedAdjustment[21] = { 0xF3, 0x44, 0x0F, 0x59, 0x05, 0x17, 0x00, 0x00, 0x00, 0xF3, 0x44, 0x0F, 0x11, 0x03, 0xE9, 0x1E, 0xDE, 0xAA, 0xF4, 0x00, 0x00 };
	int WriteSpeedSize = 21;

	ptr_t WriteOriginalAddress = 0x1409BDE2C;
	byte WriteOriginalValue[5] = { 0xE9, 0xCF, 0x21, 0x55, 0x0B };
	int WriteOriginalValueSize = 6;

	ptr_t speedAdjustmentAddress = 0x14BF10020;
	byte speedAdustmentValue[4] = { 0x9A, 0x99, 0x19, 0x3E };
	//E9 20 56 69 0B 90 90 140898332
	//1409BE671

	//add [rax],al

	byte clearMemory[300] = { 0x00 };


	//FORGET THIS
	//1409BDE31

	//MAIN BYTES: F3 44 0F 11 03 41 0F 28 7B E0
};

