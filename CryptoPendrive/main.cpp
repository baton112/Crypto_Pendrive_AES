#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "drive.h"

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


	getchar();
	return 0;
}

