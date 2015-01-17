#include "drive.h"


drive::drive(void)
{
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

    device = CreateFile("\\\\.\\J:",    // Drive to open
        GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if(device != NULL)
    {
        // Read one sector
        SetFilePointer (device, numSector*512, NULL, FILE_BEGIN) ;

        if (!ReadFile(device, sector, 512, &bytesRead, NULL))
        {
            //Print("Error in reading1 floppy disk\n",numligne++);
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

    device = CreateFile("\\\\.\\J:",    // Drive to open
        GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ|FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if(device != NULL)
    {
        // Read one sector
        SetFilePointer (device, numSector*512, NULL, FILE_BEGIN) ;


		if(!WriteFile(device,buf, 512,&bytesRead, NULL))
        {
            //Print("Error in reading1 floppy disk\n",numligne++);
			std::cout << " ERROR reading disc" << std::endl; 
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
	std::cout << "Odczytano z boot sektora : "<< system << std::endl;
	if(system == "MSDOS5.0") return 1;
	else if(system == "NTFS    ") return 2;
	else 
	{
		std::cout << "Nie rozpoznano systemu lub jest zakodowany";
		return 0;
	}
}

LONGLONG drive::NumberOfSectors()
{
	BYTE buffor[512];
	if(ChceckFileSystem() == 0) return 0; 
	else if(ChceckFileSystem() == 1) // fat32 - offset 20h - DWORD 
	{
		ReadSector(0, buffor);
		//DWORD size = buffor[0x20];
		DWORD size;
		memcpy(&size, &buffor[0x20], sizeof(DWORD));
		return (LONGLONG)size;

	}else if(ChceckFileSystem() == 2 ) //NTFS - offset 28h - LONGLONG  
	{
		ReadSector(0, buffor);
		LONGLONG   size;
		memcpy(&size, &buffor[0x28], sizeof(LONGLONG));
		return size;
	}
	else return 0;


}