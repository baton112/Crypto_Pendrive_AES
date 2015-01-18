#pragma once
#include <windows.h>

class aes
{
public:
	aes(BYTE *key, int keyLength);
	~aes(void);
	void getByteBlock(BYTE *block); // przekszta�ca 128 bit�w wejscowych na blok do AES // 16 bajt�w 
	void getKey(BYTE *key);
private:
	BYTE stateArray[4][4]; // 128 bitowy blok danych do kodowania 
	/// rzad // kolumna  
	BYTE *keyArray[4];
	void getKey(BYTE *key);
	int rows; 


};

