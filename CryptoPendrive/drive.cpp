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