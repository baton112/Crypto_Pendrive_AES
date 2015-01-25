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

	//sprawdzenie czy dysk zostal zaszyfrowany 
	BYTE buffor[512] ;
	drive::ReadSector(0, buffor);
	drive::DisplaySector(buffor);

	drive::ChceckFileSystem();

	if(buffor[510] == 0x55 && buffor[511] == 0xaa)
	{
		cout << "Dysk nie zaszyfrowany " << endl;
		cout << "1 - Zaszyfroj " << endl;
		
		int opcja = 0;
		cin >> opcja;
		switch (opcja)
		{
		case 1:
			{
				BYTE key[256/8] = {	 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00 };
				aes crypto(key, 128);

				drive::CypherDrive(crypto);
				//drive::ReadSector(0, buffor);
				//drive::CypherSector(buffor, crypto);
				//drive::WriteSector(0, buffor);
				cout << "Zaszyfrowano " << endl;
			}
			break;
			
		default:
			break;
		}

	}
	else 
	{
		cout << "Dysk zaszyfrowany " << endl;
		cout << "1 - Odszyfrij " << endl;
		int opcja = 0;
		cin >> opcja;
		switch (opcja)
		{
		case 1:
			{
				BYTE key[256/8] = {	 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00 };
				aes crypto(key, 128);
				drive::ReadSector(0, buffor);
				drive::InvCypherSector(buffor, crypto);
				drive::WriteSector(0, buffor);
				cout << "Odszyfrowano " << endl;
			}
			break;
			
		default:
			break;
		}

	}

	

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





	/*
	cout << endl;
	BYTE key[256/8] = {	 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00 };
	for(int i = 0; i< 16 ; i++)
	{
		key[i] = i;

	}*/


	/*
	aes tmp(key, 128);
	//BYTE dane[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	BYTE dane[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
	
	std::cout << " Przed szyfrowaniem" << std::endl;
	for(int i = 0; i < 16 ; i++)
	{
		if((int)dane[i] < 16)
			std::cout << std::hex << "0"<< (int)dane[i] << " ";
		else 
			std::cout << std::hex << (int)dane[i] << " ";
	}
	std::cout << std::endl;
	tmp.cipher( dane);
	std::cout << " PO szyfrowaniem" << std::endl;
	for(int i = 0; i < 16 ; i++)
	{
		if((int)dane[i] < 16)
			std::cout << std::hex << "0"<< (int)dane[i] << " ";
		else 
			std::cout << std::hex << (int)dane[i] << " ";
	}
	std::cout << std::endl;
	tmp.inv_cipher(dane);
	std::cout << " PO rozszyfrowaniu" << std::endl;
	for(int i = 0; i < 16 ; i++)
	{
		if((int)dane[i] < 16)
			std::cout << std::hex << "0"<< (int)dane[i] << " ";
		else 
			std::cout << std::hex << (int)dane[i] << " ";
	}*/

	drive::ReadSector(0, buffor);

	system("pause");
	getchar();
	return 0;
}

