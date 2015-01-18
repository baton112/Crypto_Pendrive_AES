#pragma once
#include <windows.h>

class aes
{
public:
	aes(BYTE *key, int keyLength);
	~aes(void);
	void getByteBlock(BYTE *block); // przekszta³ca 128 bitów wejscowych na blok do AES // 16 bajtów 
	void getKey(BYTE *key);
private:
	BYTE stateArray[4][4]; // 128 bitowy blok danych do kodowania 
	/// rzad // kolumna  
	BYTE *keyArray[4];
	void getKey(BYTE *key);
	int rows; 


};

