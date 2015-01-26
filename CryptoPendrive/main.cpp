#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "drive.h"
#include "aes.h"

using namespace std;


char PATH[100] = "\\\\.\\J:";
BYTE CLUSTER_SIZE;
int FAT_OFFSET;
INT64 FAT_SIZE;
int ROOTdir_cluster;

int main()
{

	cout << "WItam " << endl;
	cout << "Wprowadz symbol dysku do zaszyfrowania" << endl;
	//WYBOR DYSTKU 

	//cin >> PATH;
	cout << "Wybrany dysk to " << PATH << endl;

	
	//sprawdzenie czy dysk zostal zaszyfrowany 
	BYTE buffor[512] ;
	drive::ReadSector(0, buffor);
	//drive::DisplaySector(buffor);

	drive::PrintDiscGemetry();
	

	int fileSystem = drive::ChceckFileSystem();


	//szyfrowanie pliku fat 32
	drive::SetClusterSize_FAT_OFFSET();

	//drive::PrintFiles();
	
	drive d;
	
	cout << "Wybierz nazwe pliku. " << PATH << endl;
	int nrPliku = 0;
	std::cin >> nrPliku;
	BYTE key[256/8] = {	 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00 };
	aes crypto(key, 128);
	//	drive::WriteSector(0, buffor);
	//d.CypherFile(crypto, nrPliku);
	d.FindFirstSector(nrPliku, crypto);
	cout << "Zaszyfrowano " << PATH << endl;
	

	


	switch (fileSystem)
	{
	case 0: 
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
	
						drive d(crypto, false);
						cout << "Odszyfrowano " << endl;
				}
				break;
			
			default:
				break;
			}
			
		}
		break;
	case 1:
	case 2:
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

					drive d(crypto, true);
					cout << "Zaszyfrowano " << endl;
				}
				break;
			
			default:
				break;
			}
			
		}
		break;
	
	default:
		break;
	}
	

	
	

	system("pause");
	getchar();
	return 0;
}

