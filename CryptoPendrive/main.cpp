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
	// http://www.samiam.org/key-schedule.html
	BYTE key[256/8] = {	 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00 };
	for(int i = 0; i< 16 ; i++)
	{
		key[i] = 16+i;

	}
	aes tmp(key, 128);
	//BYTE dane[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	BYTE dane[16];
	for(int i = 0; i< 16 ; i++)
	{
		dane[i] = 16+i;

	}
	//BYTE dane[] = "aaaaaaaaaaaaaaaa";
	tmp.cipher( dane);
	

	getchar();
	return 0;
}

