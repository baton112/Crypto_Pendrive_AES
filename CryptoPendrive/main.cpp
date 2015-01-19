#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "drive.h"
#include "aes.h"

using namespace std;




int main()
{
	cout << "WItam " << endl;
	cout << "Wprowadz symbol dysku do zaszyfrowania" << endl;
	//WYBOR DYSTKU 



	cout << "Wybrany dysk to " << PATH << endl;
	/*
	BYTE buffor[512] ;
	//czyta mbr z pendriva 
	drive::ReadSector(0, buffor);
	drive::DisplaySector(buffor);
	//zapelnia go glupotami
	for(int i = 0; i < 255; i++)
	{
		buffor[i] = i;
	}
	drive::WriteSector(0, buffor);
	drive::DisplaySector(buffor);
	*/


	cout << endl;
	BYTE key[128/8] = {	 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00};
	aes tmp(key, 128);
	


	getchar();
	return 0;
}

