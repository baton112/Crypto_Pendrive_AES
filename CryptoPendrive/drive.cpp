#include "drive.h"



drive::drive(void)
{
}

drive::drive(aes crypto, bool cipher)
{
	BYTE buffor[512]; 
	device = NULL;
	DWORD bytesRead;

	char logical[65536];
    QueryDosDevice("J:",logical, sizeof(logical));
      
	std::cout<< logical ;
	
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
}

drive::~drive(void)
{
	 CloseHandle(device);
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

	//DeviceIoControl(device, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesReturned, 0);
    if(device != NULL)
    {
        // Read one sector
        SetFilePointer (device, numSector*512, NULL, FILE_BEGIN) ;
jeszczeRaz:
        if (!ReadFile(device, sector, 512, &bytesRead, NULL))
        {
            //Print("Error in reading1 floppy disk\n",numligne++);
			std::cout << " ERROR reading disc" << std::endl; 
			Sleep(1000);
			goto jeszczeRaz;
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
	jeszczeRaz:
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
			Sleep(1000);
			goto jeszczeRaz;
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



void drive::ListDrives(){

	DWORD dwSize = MAX_PATH;
	char szLogicalDrives[MAX_PATH] = { 0 };
	DWORD dwResult = GetLogicalDriveStrings(dwSize, szLogicalDrives);
	int i = 0;

	if (dwResult > 0 && dwResult <= MAX_PATH)
	{
		
		char* szSingleDrive = szLogicalDrives;
		char devices[10];
		while (*szSingleDrive)
		{
			
			printf("%d : %s\n", i,szSingleDrive);
			devices[i] = szSingleDrive[0];
			// get the next drive
			szSingleDrive += strlen(szSingleDrive) + 1;
			i++;
		}
	//	std::cout << "wprowadz numer" << std::endl;;
		std::cin >> i;
		szSingleDrive = szLogicalDrives;

		PATH[4] = devices[i];

	}



}