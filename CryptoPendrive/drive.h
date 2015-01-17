#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>


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
	
	
};

