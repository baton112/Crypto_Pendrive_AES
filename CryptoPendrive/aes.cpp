#include "aes.h"


aes::aes(BYTE *key, int keyLength)
{
	rows = keyLength/32;
	for(int i = 0; i < rows; i++) //row 
	{
		keyArray[i] = new BYTE[4]; //new colums 
	}
	KeySchedule = new WORD[44];
	//w0 = kolumna pierwsza key array 
	//w1 = kol 2 keyArray 
	//w2 = kol 3 keyArray
	//w3 = kol 4 keyArray
	///xor z XOR’ed with the input block before the round-based processing begins
	// potem magiczne xory na nich i magiczna funkcja g i zapelniamy cala tablice keySchedule 
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