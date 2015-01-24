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
		key[i] = i;

	}
	aes tmp(key, 128);
	//BYTE dane[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	BYTE dane[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
	
		/*for(int i = 0; i< 16 ; i++)
	{
		dane[i] = 16+i;

	}*/
	//BYTE dane[] = "aaaaaaaaaaaaaaaa";
	tmp.cipher( dane);
	std::cout << "rozszyfrowne " << std::endl;
	tmp.inv_cipher(dane);

	getchar();
	return 0;
}

