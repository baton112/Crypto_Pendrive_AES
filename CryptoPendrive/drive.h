#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "aes.h"
#include <winioctl.h>
#include <stdio.h>


extern char PATH[100];

//#define PATH "\\\\.\\PhysicalDrive2"
//#define PATH "\\.\\Device\\HarddiskVolume11"
// QueryDosDevice("J:",logical, sizeof(logical));
      
//#define PATH "\\\\.\\PhysicalDrive1"

//#define wszDrive L"\\\\.\\PhysicalDrive2"
//#define wszDrive L"\\\\.\\J:"



//  QueryDosDevice("J:",logical, sizeof(logical));
      
class drive
{
private: 
	HANDLE device;
public:
	drive(aes crypto, bool cipher);
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
	
	static void CypherDrive(aes crypto);
	static void InvCypherDrive(aes crypto);

	static void ListDrives();
	static BOOL GetDriveGeometry(char* Path, DISK_GEOMETRY *pdg);
	static void PrintDiscGemetry();
};

