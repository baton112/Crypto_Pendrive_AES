#include "aes.h"


aes::aes(BYTE *key, int keyLength)
{
	rows = keyLength/32;
	for(int i = 0; i < rows; i++) //row 
	{
		keyArray[i] = new BYTE[4]; //new colums 
	}
}


aes::~aes(void)
{
}

void aes::getByteBlock(BYTE *block)
{
	for(int i = 0; i < 4; i++)//kolumna //colum
	{
		for(int j = 0; j < 4; j++)//wiesz //row 
		{
			stateArray[j][i] = block[i*4+j];
		}
	}
	
}

void aes::getKey(BYTE *key)
{
	for(int i = 0; i < 4; i++)//kolumna //colum
	{
		for(int j = 0; j < 4; j++)//wiesz //row 
		{
			keyArray[j][i] = key[i*4+j];
		}
	}


}