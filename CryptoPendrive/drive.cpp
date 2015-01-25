#include "drive.h"


void writeLongName(BYTE *tab)
{
	for(int i = 1; i < 0x0b ; i+=2) 
		std::cout << tab[i];
	for(int i = 0x0e; i < 0x1a ;i+=2) 
		std::cout << tab[i];
	for(int i = 0x1c; i < 0x20 ; i+=2) 
		std::cout << tab[i];

}

drive::drive(void)
{
	SetClusterSize_FAT_OFFSET();

	
	FATtable = new BYTE[FAT_SIZE*512];

	DWORD bytesRead;
	HANDLE device = NULL;

	device = CreateFile(PATH,    // Drive to open
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,        // Share Mode
		NULL,                   // Security Descriptor
		OPEN_EXISTING,          // How to create
		0,                      // File attributes
		NULL);                  // Handle to template
	DWORD bytesReturned;

	DeviceIoControl(device, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, 0);

	//przeczytanie tablicy fat 
	
	for(int i = 0; i < 1000; i++) // bo za dlugo dziala 
	{
		if(device != NULL)
		{
			// Read one sector
			SetFilePointer (device, (i+FAT_OFFSET)*512, NULL, FILE_BEGIN) ;
			if (!ReadFile(device, &FATtable[i*512], 512, &bytesRead, NULL))
			{
				std::cout << " ERROR reading disc" << std::endl; 
			}
		}

    }
	drive::DisplaySector(FATtable );


	//ROOT Directory 
	cluster_begin = 2*FAT_SIZE + FAT_OFFSET; // drugi klaste // root directory 
	ROOTdir = new BYTE[CLUSTER_SIZE*512];

	for(int i = 0; i < CLUSTER_SIZE; i++)
	{
		if(device != NULL)
		{
			// Read one sector
			SetFilePointer (device, i*512+cluster_begin*512, NULL, FILE_BEGIN) ;
			if (!ReadFile(device, &ROOTdir[i*512], 512, &bytesRead, NULL))
			{
				std::cout << " ERROR reading disc" << std::endl; 
			}
		}
	}

	drive::DisplaySector(ROOTdir);

	//32 - wpis w directory 
	BYTE fileEnd = 0;
	std::cout << " _________  PLIKI NA DYSKU ________ " << std::endl;
	for(int i = 0; i < CLUSTER_SIZE*512; i+=32)
	{
		if(ROOTdir[i] != 0xE5 && ROOTdir[i] != 0x00 && ROOTdir[i+0x0b] != 0x0f)  //krutka nazwa 
		{
			std::cout << i/32 << " - ";
			PrintFileName(&ROOTdir[i]);
		}
		else if(ROOTdir[i] != 0xE5 && ROOTdir[i] != 0x00 && ROOTdir[i] == 0x01) //dluga nazwa 
		{
			writeLongName(&ROOTdir[i]);
			for(int j = 0 ; j < fileEnd; j++)
			{
				writeLongName(&ROOTdir[i-j*32]);
			}
			std::cout << std::endl;
			fileEnd = 0;
		}
		else if(fileEnd == 0 )
		{
			fileEnd = ROOTdir[i] - 0x40;
		}
	}

	 // Close the handle
	CloseHandle(device);
	
}

drive::drive(aes crypto, bool cipher)
{
	BYTE buffor[512]; 
	HANDLE device = NULL;
	DWORD bytesRead;

	//char logical[65536];
   // QueryDosDevice("J:",logical, sizeof(logical));
    //  
	//std::cout<< logical ;
	
	device = CreateFile(PATH,    // Drive to open
        GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);  

	DWORD bytesReturned;

	DeviceIoControl(device, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, 0);

	ULONG64 numberOfSectors = 1;
	if(cipher) //szyfrujemy
	{
		for(int i = 0 ; i < numberOfSectors; i++)
		{
			//odczytanie 
			SetFilePointer (device, i*512, NULL, FILE_BEGIN) ;
			if (!ReadFile(device, buffor, 512, &bytesRead, NULL))
			{
				//Print("Error in reading1 floppy disk\n",numligne++);
				std::cout << " ERROR reading disc" << std::endl; 
				Sleep(1000);
			}
			
			for(int j=0; j < 512; j+= 128/8)
			{
				crypto.cipher(&buffor[j]);
			}
			
			// zapisanie 
			SetFilePointer (device, i*512, NULL, FILE_BEGIN) ;
			if(!WriteFile(device,buffor, 512,&bytesRead, NULL))
			{
				//Print("Error in reading1 floppy disk\n",numligne++);
				std::cout << " ERROR writing disc " << i  << std::endl; 
				Sleep(1000);
			}
			std::cout << "Postep " <<  std::dec << i << std::endl;

		}
	}
	else 
	{
		for(int i = 0 ; i < numberOfSectors; i++)
		{
			//odczytanie 
			SetFilePointer (device, i*512, NULL, FILE_BEGIN) ;
			if (!ReadFile(device, buffor, 512, &bytesRead, NULL))
			{
				//Print("Error in reading1 floppy disk\n",numligne++);
				std::cout << " ERROR reading disc" << std::endl; 
				Sleep(1000);
			}
			
			for(int j=0; j < 512; j+= 128/8)
			{
				crypto.inv_cipher(&buffor[j]);
			}

			if(i == 0 && buffor[510] !=0x55 && buffor[511]!= 0xAA ) //sprawdzenie czy system jest zaszyfrowany 
			{
				std::cout << "!!!!!!!!!  ZLY KLUCZ !!!!!!!!!" <<std::endl;
				break;
			}

			// zapisanie 
			SetFilePointer (device, i*512, NULL, FILE_BEGIN) ;
			if(!WriteFile(device,buffor, 512,&bytesRead, NULL))
			{
				//Print("Error in reading1 floppy disk\n",numligne++);
				std::cout << " ERROR writing disc " << i  << std::endl; 
				Sleep(1000);
			}
			std::cout << "Postep " <<  std::dec << i << std::endl;
		}
	}
	CloseHandle(device);
}

drive::~drive(void)
{
}

int drive::ReadSector(int numSector,BYTE* buf)
{
    int retCode = 0;
    BYTE sector[512];
    DWORD bytesRead;
    HANDLE device = NULL;

	device = CreateFile(PATH,    // Drive to open
        GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template
	DWORD bytesReturned;

	DeviceIoControl(device, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, 0);

    if(device != NULL)
    {
        // Read one sector
        SetFilePointer (device, numSector*512, NULL, FILE_BEGIN) ;
        if (!ReadFile(device, sector, 512, &bytesRead, NULL))
        {
			std::cout << " ERROR reading disc" << std::endl; 
		}
        else
        {
            // Copy boot sector into buffer and set retCode
            memcpy(buf,sector, 512);
			retCode=1;
        }

        CloseHandle(device);
        // Close the handle
    }



    return retCode;
}

int drive::WriteSector(int numSector, BYTE* buf)
{
	int retCode = 0;
    BYTE sector[512];
    DWORD bytesRead;
    HANDLE device = NULL;

	device = CreateFile(PATH,    // Drive to open
        GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

	DWORD bytesReturned;

	DeviceIoControl(device, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, 0);

    if(device != NULL)
    {
        // Read one sector
        SetFilePointer (device, numSector*512, NULL, FILE_BEGIN) ;


		if(!WriteFile(device,buf, 512,&bytesRead, NULL))
        {
            //Print("Error in reading1 floppy disk\n",numligne++);
			std::cout << " ERROR writing disc " << numSector  << std::endl; 
		}
        else
        {
            // Copy boot sector into buffer and set retCode
            //memcpy(buf,sector, 512);
			retCode=1;
        }

        CloseHandle(device);
        // Close the handle
    }
	return retCode;

}

void drive::DisplaySector(BYTE* buff)
{
	for(int i = 0; i < 32; i++)
	{
		//wyswietlenie szestnastkowo 
		for(int j = 0 ; j < 16; j++)
		{
			if((int)buff[i*16+j] < 16)
				std::cout << std::hex << "0"<< (int)buff[i*16+j] << " ";
			else 
				std::cout << std::hex << (int)buff[i*16+j] << " ";
		}

		//watos ASCII
		for(int j = 0 ; j < 16; j++)
		{
				std::cout  << buff[i*16+j];
		}

		std::cout << std::endl;

	}

}


// 0 - nie rozpoznano systemu, 1 - FAT32, 2- NTFS
int drive::ChceckFileSystem()
{
	//sprawdza offset 52H
	BYTE buff[512] ;
	drive::ReadSector(0, buff);
	
	std::string system; 

	for(int i = 0x03 ; i < 0x0B; i++) // miejsce w mbr gdzie przymana jest nazwa systemu
	{
		system += buff[i];

	}
	//std::cout << "Odczytano z boot sektora : "<< system << std::endl;
	if(system == "MSDOS5.0") return 1;
	else if(system == "NTFS    ") return 2;
	else 
	{
		std::cout << "Nie rozpoznano systemu lub jest zakodowany";
		return 0;
	}
}

BOOL drive::GetDriveGeometry(char* Path, DISK_GEOMETRY *pdg)
{
	HANDLE hDevice = INVALID_HANDLE_VALUE;  // handle to the drive to be examined 
	BOOL bResult   = FALSE;                 // results flag
	DWORD junk     = 0;                     // discard results

	hDevice = CreateFile(Path,//wszPath,          // drive to open
						0,                // no access to the drive
						FILE_SHARE_READ | // share mode
						FILE_SHARE_WRITE, 
						NULL,             // default security attributes
						OPEN_EXISTING,    // disposition
						0,                // file attributes
						NULL);            // do not copy file attributes

	if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
	{
	return (FALSE);
	}

	bResult = DeviceIoControl(hDevice,                       // device to be queried
							IOCTL_DISK_GET_DRIVE_GEOMETRY, // operation to perform
							NULL, 0,                       // no input buffer
							pdg, sizeof(*pdg),            // output buffer
							&junk,                         // # bytes returned
							(LPOVERLAPPED) NULL);          // synchronous I/O

	
	CloseHandle(hDevice);

	return (bResult);
}

void drive::PrintDiscGemetry()
{
  DISK_GEOMETRY pdg = { 0 }; // disk drive geometry structure
  BOOL bResult = FALSE;      // generic results flag
  ULONGLONG DiskSize = 0;    // size of the drive, in bytes

  bResult = GetDriveGeometry (PATH, &pdg);

  if (bResult) 
  {
    printf("Drive path      = %s\n",   PATH);
    wprintf(L"Cylinders       = %I64d\n", pdg.Cylinders);
    wprintf(L"Tracks/cylinder = %ld\n",   (ULONG) pdg.TracksPerCylinder);
    wprintf(L"Sectors/track   = %ld\n",   (ULONG) pdg.SectorsPerTrack);
    wprintf(L"Bytes/sector    = %ld\n",   (ULONG) pdg.BytesPerSector);

    DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
               (ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;
    wprintf(L"Disk size       = %I64d (Bytes)\n"
            L"                = %.2f (Gb)\n", 
            DiskSize, (double) DiskSize / (1024 * 1024 * 1024));
  } 
  else 
  {
    wprintf (L"GetDriveGeometry failed. Error %ld.\n", GetLastError ());
  }



}

LONGLONG drive::NumberOfSectors()
{
	PrintDiscGemetry();
	
	BYTE buffor[512];
	if(ChceckFileSystem() == 0) return 0; 
	else if(ChceckFileSystem() == 1) // fat32 - offset 20h - DWORD 
	{
		ReadSector(0, buffor);
		//DWORD size = buffor[0x20];
		DWORD size;
		memcpy(&size, &buffor[0x20], sizeof(DWORD));
		//return (LONGLONG)size;
		std::cout << "ROZMIAR " << size;

	}else if(ChceckFileSystem() == 2 ) //NTFS - offset 28h - LONGLONG  
	{
		ReadSector(0, buffor);
		LONGLONG   size;
		memcpy(&size, &buffor[0x28], sizeof(LONGLONG));
		//return size;
		std::cout << "ROZMIAR " << size;
	}
	//else return 0;
	

	BOOL bResult = FALSE;      // generic results flag
	
	DISK_GEOMETRY pdg = { 0 }; // disk drive geometry structure
	bResult = GetDriveGeometry (PATH, &pdg);

	if (bResult) 
	{
		return pdg.Cylinders.QuadPart * (ULONG) pdg.TracksPerCylinder * (ULONG) pdg.SectorsPerTrack;
	}
	else 
	{
		wprintf (L"GetDriveGeometry failed. Error %ld.\n", GetLastError ());
	}
	return 0;
}

void drive::SelectDrive(char d)
{
	

}

void drive::CypherSector(BYTE* buf, aes crypto)
{
	for(int i=0; i < 512; i+= 128/8)
	{
		crypto.cipher(&buf[i]);
	}
}

void drive::InvCypherSector(BYTE* buf, aes crypto)
{
	for(int i=0; i < 512; i+= 128/8)
	{
		crypto.inv_cipher(&buf[i]);
	}
}

void zero(BYTE *buf)
{
	for(int i = 0 ; i < 512 ; i++)
	{
		buf[i] = 0;

	}
}

void drive::CypherDrive(aes crypto)
{
	BYTE buffor[512] ;
	LONGLONG numberOfSectors = drive::NumberOfSectors();
	std::cout << numberOfSectors << std::endl;

	//for(int i = 0; i < numberOfSectors; i++)
	for(int i = 0 ; i < 100 ; i++)
	{
		drive::ReadSector(i, buffor);
		drive::CypherSector(buffor, crypto);
		//zero(buffor);
		drive::WriteSector(i, buffor);
		//std::cout << "Postep " << ((double)i/(double)numberOfSectors)*100.0 << std::endl;
		std::cout << "Postep " <<  std::dec << i << std::endl;
	}


}

void drive::InvCypherDrive(aes crypto)
{
	BYTE buffor[512] ;
	LONGLONG numberOfSectors = drive::NumberOfSectors();
	std::cout << numberOfSectors << std::endl;

	//for(int i = 0; i < numberOfSectors; i++)
	for(int i = 0 ; i < 100 ; i++)
	{
		drive::ReadSector(i, buffor);
		drive::InvCypherSector(buffor, crypto);
		drive::WriteSector(i, buffor);
		//std::cout << "Postep " << ((double)i/(double)numberOfSectors)*100.0 << std::endl;
		std::cout << "Postep " << std::dec << i << std::endl;
	}


}

void drive::SetClusterSize_FAT_OFFSET()
{
	BYTE buffor[512];
	drive::ReadSector(0, buffor);
	//drive::DisplaySector(buffor);
	CLUSTER_SIZE = buffor[0x0D]; // offset wielkosci klastra 
	FAT_OFFSET = 0;
	FAT_OFFSET = buffor[0x0F] << 8 ;  //fat offset 
	FAT_OFFSET += buffor[0x0E];

	std::cout << "Rozmiar klastra " << std::dec << (int) CLUSTER_SIZE<<std::endl;
	std::cout << "OFFSET FAT  " <<std::dec << FAT_OFFSET<<std::endl;

	//rozmiar tablic fat 
	FAT_SIZE = 0;
	FAT_SIZE +=  buffor[0x27] ;
	FAT_SIZE <<= 8;
	FAT_SIZE +=  buffor[0x26];
	FAT_SIZE <<= 8;
	FAT_SIZE +=  buffor[0x25] ;
	FAT_SIZE <<= 8;
	FAT_SIZE +=  buffor[0x24] ;


	std::cout << "FAT SIZE   " <<std::dec << FAT_SIZE  << "  "  << std::hex << FAT_SIZE  <<std::endl;

	ROOTdir_cluster = 0;
	ROOTdir_cluster = buffor[0x2C];
	std::cout << "ROOT CLUSTER    " <<std::dec <<ROOTdir_cluster   <<std::endl;
	
}

void drive::PrintFileName(BYTE *rootEntry)
{
	for(int j = 0; j < 8; j++)
	{
		std::cout << rootEntry[j];
	}
	std::cout <<"." ;
	for(int j = 8; j < 8+3; j++)
	{
		std::cout << rootEntry[j];
	}
	std::cout << std::endl;

}

void drive::CypherFile( aes crypto, int cluster)
{
	//0x14 - high word
	//0x1a - low word 
	//int clusterNumber = FATtable[0x14*cluster];
	BYTE *tab = new BYTE[512];
	for(int i = 0; i < CLUSTER_SIZE; i++)
	{
		drive::ReadSector(cluster_begin +(cluster-3) *CLUSTER_SIZE + i , tab);
		std::cout << "sektor " <<  std::dec << cluster_begin +(cluster-3) *CLUSTER_SIZE + i << "    " << "klaster " << cluster << std::endl;
		drive::CypherSector(tab, crypto);
		drive::WriteSector(cluster_begin +(cluster-3) *CLUSTER_SIZE + i, tab);
	}

	
	if(FATtable[cluster*4] != 0xFF)
		CypherFile(crypto, FATtable[cluster*4]);


}

void drive::InvCypherFile( aes crypto, int cluster)
{
	//0x14 - high word
	//0x1a - low word 
	//int clusterNumber = FATtable[0x14*cluster];
	BYTE *tab = new BYTE[512];
	for(int i = 0; i < CLUSTER_SIZE; i++)
	{
		//dres drugiego klastra + (cluster - 2) *512*CLUSTER_SIZE
		drive::ReadSector(cluster_begin +(cluster - 3) *CLUSTER_SIZE + i , tab);
		drive::InvCypherSector(tab, crypto);
		drive::WriteSector(cluster_begin +(cluster - 3) *CLUSTER_SIZE + i, tab);
	
	}

	if(FATtable[cluster*4] != 0xFF)
		CypherFile(crypto, FATtable[cluster*4]);

}

void drive::FindFirstSector(int i, aes crypto)
{
	char cluster = ROOTdir[i*32+0x1a];
	CypherFile(crypto, cluster);


}