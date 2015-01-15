#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>



class drive
{
public:
	drive(void);
	~drive(void);
	static int ReadSector(int numSector,BYTE* buf);
	static int WriteSector(int numSector, BYTE* buf);
	static void DisplaySector(BYTE* buff);
	

};

