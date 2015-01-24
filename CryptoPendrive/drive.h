#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "aes.h"


#define PATH "\\\\.\\J:"

class drive
{
public:
	drive(void);
	~drive(void);
	static int ReadSector(int numSector,BYTE* buf);
	static int WriteSector(int numSector, BYTE* buf);
	static void DisplaySector(BYTE* buff);
	static LONGLONG NumberOfSectors();
	// 0 - nie znam // 1 - FAT32  // 2 - NTFS 
	static int ChceckFileSystem();
	static void SelectDrive(char d);
	static void CypherSector(BYTE* buf, aes crypto);
	static void InvCypherSector(BYTE* buf, aes crypto);
	
};

