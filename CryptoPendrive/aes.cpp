#include "aes.h"


aes::aes(BYTE *key, int keyLength)
{
	keySize = keyLength;
	
	RoundKeySchedule = new BYTE[240]; // max ilosc kluczy potrzebnych przy 256 bitach klucza 

	for(int i = 0 ; i < keyLength/8;i++)
	{
		RoundKeySchedule[i] = key[i];
	}

	expandKey();


	PrintExpandedKey(RoundKeySchedule, 14);

}

void aes::PrintExpandedKey(BYTE *tab, int rounds)
{
	for(int i = 0; i <= rounds ; i++)
	{
		for(int j=0; j < 16; j++)
		{
			if((int)tab[i*16+j] < 16)
				std::cout << std::hex << "0"<< (int)tab[i*16+j] << " ";
			else 
				std::cout << std::hex << (int)tab[i*16+j] << " ";
		}
		std::cout << std::endl;
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

void aes::returnByteBlock(BYTE *block)
{
	for(int i = 0; i < 4; i++)//kolumna //colum
	{
		for(int j = 0; j < 4; j++)//wiesz //row 
		{
			 block[i*4+j] = stateArray[j][i];
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

void rotate(unsigned char *in) {
        unsigned char a,c;
        a = in[0];
        for(c=0;c<3;c++) 
                in[c] = in[c + 1];
        in[3] = a;
        return;
}


/* Calculate the rcon used in key expansion */
unsigned char rcon(unsigned char in) {
        unsigned char c=1;
        if(in == 0)  
                return 0; 
        while(in != 1) {
		unsigned char b;
		b = c & 0x80;
		c <<= 1;
		if(b == 0x80) {
			c ^= 0x1b;
		}
                in--;
        }
        return c;
}


void schedule_core(unsigned char *in, unsigned char i) {
        char a;
        /* Rotate the input 8 bits to the left */
        rotate(in);
        /* Apply Rijndael's s-box on all 4 bytes */
        for(a = 0; a < 4; a++) 
                in[a] = s[in[a]];
        /* On just the first byte, add 2^i to the byte */
        in[0] ^= rcon(i);
}

void aes::expandKey()
{
	unsigned char t[4];
    
	unsigned char i = 1;
    unsigned char a;

	if(keySize == 128)
	{
		/* c is 16 because the first sub-key is the user-supplied key */
		unsigned char c = 16;
		NumberOfRounds = 10;
		/* We need 11 sets of sixteen bytes each for 128-bit mode */
		while(c < 176) 
		{
				/* Copy the temporary variable over from the last 4-byte
					* block */
				for(a = 0; a < 4; a++) 
					t[a] = RoundKeySchedule[a + c - 4];
				/* Every four blocks (of four bytes), 
					* do a complex calculation */
				if(c % 16 == 0) 
				{
					schedule_core(t,i);
					i++;
				}
				for(a = 0; a < 4; a++) 
				{
					RoundKeySchedule[c] = RoundKeySchedule[c - 16] ^ t[a];
					c++;
				}
		}
	}
	if (keySize == 192)
	{
		NumberOfRounds = 12;
        unsigned char c = 24;

        while(c < 208) 
		{
                /* Copy the temporary variable over */
                for(a = 0; a < 4; a++) 
                        t[a] = RoundKeySchedule[a + c - 4]; 
                /* Every six sets, do a complex calculation */
                if(c % 24 == 0) 
				{
                        schedule_core(t,i);
						i++;
				}
                for(a = 0; a < 4; a++) 
				{
                        RoundKeySchedule[c] = RoundKeySchedule[c - 24] ^ t[a];
                        c++;
                }
        }
	}
	if(keySize==256)
	{

		NumberOfRounds = 14;
        unsigned char c = 32;
        while(c < 240) 
		{
                /* Copy the temporary variable over */
                for(a = 0; a < 4; a++) 
                        t[a] = RoundKeySchedule[a + c - 4]; 
                /* Every eight sets, do a complex calculation */
                if(c % 32 == 0) 
				{
                    schedule_core(t,i);
					i++;
				}
                /* For 256-bit keys, we add an extra sbox to the
                 * calculation */
                if(c % 32 == 16) {
                        for(a = 0; a < 4; a++) 
                                t[a] = s[t[a]];
                }
                for(a = 0; a < 4; a++) {
                        RoundKeySchedule[c] = RoundKeySchedule[c - 32] ^ t[a];
                        c++;
                }
        }

	}

}


void aes::AddRoundKey(BYTE *roundKey) // 16 bajtow 
{
	for(int i = 0 ; i < 4; i++)
	{
		//XOR bytow klucza i stanu
		for(int j = 0; j < 4; j++)//wiesz //row 
		{
			stateArray[j][i] ^= roundKey[i*4+j]; //xor z kluczem 
			
		}
	}
}

//podmienia elementy tablicy stanu wedlug ich w tablic s 
void aes::SubBytes()
{
	for(int i = 0; i < 4 ; i++) 
	{
		for(int j = 0; j < 4; j++)
		{
			stateArray[i][j] = s[stateArray[i][j]];
		}
	}
}

void aes::InvSubBytes()
{
	for(int i = 0; i < 4 ; i++) 
	{
		for(int j = 0; j < 4; j++)
		{
			stateArray[i][j] = inv_s[stateArray[i][j]];
		}
	}
}




// Performs the ShiftRows step. All rows are shifted cylindrically to the left.
void aes::ShiftRows()
{

	BYTE t;
	// Shift left by 1
	t = stateArray[1][0];
	stateArray[1][0] = stateArray[1][1];
	stateArray[1][1] = stateArray[1][2];
	stateArray[1][2] = stateArray[1][3];
	stateArray[1][3] = t;
	// Shift left by 2
	t = stateArray[2][0];
	stateArray[2][0] = stateArray[2][2];
	stateArray[2][2] = t;
	t = stateArray[2][1];
	stateArray[2][1] = stateArray[2][3];
	stateArray[2][3] = t;
	// Shift left by 3
	t = stateArray[3][0];
	stateArray[3][0] = stateArray[3][3];
	stateArray[3][3] = stateArray[3][2];
	stateArray[3][2] = stateArray[3][1];
	stateArray[3][1] = t;
}


void aes::InvShiftRows(){
	BYTE t;

	t = stateArray[1][3];
	stateArray[1][1] = stateArray[1][0];
	stateArray[1][2] = stateArray[1][1];
	stateArray[1][3] = stateArray[1][2];
	stateArray[1][0] = t;

	t = stateArray[2][0];
	stateArray[2][0] = stateArray[2][2];
	stateArray[2][2] = t;
	t = stateArray[2][1];
	stateArray[2][1] = stateArray[2][3];
	stateArray[2][3] = t;

	t = stateArray[3][0];
	stateArray[3][0] = stateArray[3][1];
	stateArray[3][1] = stateArray[3][2];
	stateArray[3][2] = stateArray[3][3];
	stateArray[3][3] = t;
}

BYTE aes::GMul(BYTE a, BYTE b) 
{ // Galois Field (256) Multiplication of two Bytes
   BYTE p = 0;
   BYTE counter;
   BYTE hi_bit_set;
   for (counter = 0; counter < 8; counter++) {
      if ((b & 1) != 0) {
         p ^= a;
      }
      hi_bit_set = (BYTE) (a & 0x80);
      a <<= 1;
      if (hi_bit_set != 0) {
         a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */
      }
      b >>= 1;
   }
   return p;
}

void aes::MixColumns() { // 's' is the main State matrix, 'ss' is a temp matrix of the same dimensions as 's'.

	BYTE ss[4][4];
   //Array.Clear(ss, 0, ss.Length); 
	
	for (int c = 0; c < 4; c++) 
	{
		ss[0][c] = (BYTE) (GMul(0x02, stateArray[0][c]) ^ GMul(0x03, stateArray[1][c]) ^ stateArray[2][c] ^ stateArray[3][ c]);
		ss[1][c] = (BYTE) (stateArray[0][c] ^ GMul(0x02, stateArray[1][c]) ^ GMul(0x03, stateArray[2][c]) ^ stateArray[3][c]);
		ss[2][c] = (BYTE) (stateArray[0][c] ^ stateArray[1][c] ^ GMul(0x02, stateArray[2][c]) ^ GMul(0x03, stateArray[3][c]));
		ss[3][c] = (BYTE) (GMul(0x03, stateArray[0][c]) ^ stateArray[1][c] ^ stateArray[2][c] ^ GMul(0x02, stateArray[3][c]));
	}

	//kopiowanie ss do s 
	for(int i = 0; i < 4 ; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			stateArray[i][j] = ss[i][j];
		}
	}
   
}

void aes::InvMixColumns() { // 's' is the main State matrix, 'ss' is a temp matrix of the same dimensions as 's'.

	BYTE ss[4][4];
	//Array.Clear(ss, 0, ss.Length); 

	for (int c = 0; c < 4; c++)
	{
		ss[0][c] = (BYTE)(GMul(0x0e, stateArray[0][c]) ^ GMul(0x0b, stateArray[1][c]) ^ GMul(0x0d, stateArray[2][c]) ^ GMul(0x09,stateArray[3][c]));
		ss[1][c] = (BYTE)(GMul(0x09,stateArray[0][c] )^ GMul(0x0e, stateArray[1][c]) ^ GMul(0x0b, stateArray[2][c]) ^ GMul(0x0d,stateArray[3][c]));
		ss[2][c] = (BYTE)(GMul(0x0d,stateArray[0][c] )^ GMul(0x09,stateArray[1][c]) ^ GMul(0x0e, stateArray[2][c]) ^ GMul(0x0b, stateArray[3][c]));
		ss[3][c] = (BYTE)(GMul(0x0b, stateArray[0][c]) ^ GMul(0x0d,stateArray[1][c]) ^ GMul(0x09,stateArray[2][c]) ^ GMul(0x0e, stateArray[3][c]));
	}

	//kopiowanie ss do s 
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			stateArray[i][j] = ss[i][j];
		}
	}

}

void aes::cipher(BYTE *tab) 
{
	//PrintStateArray();

	for(int i = 0 ; i < 16; i++) 
		std::cout <<std::hex << tab[i];

	std::cout <<std::endl;

	getByteBlock(tab);

	std::cout << "Po wczytaniu bloku z danymi : " <<std::endl;
	PrintStateArray();
	

	AddRoundKey(RoundKeySchedule); // raczej dziala dobrze 

	std::cout << "Po dodaniu klucza rundy" <<std::endl;
	PrintStateArray();

	//for round = 1 step 1 to Nr–1
	for(int round = 1; round < NumberOfRounds ; round++)
	{
		SubBytes(); //dzialaa na bank
		//std::cout << "SUB BYTES" <<std::endl;
		//PrintStateArray();
		ShiftRows(); //skopiowane z neta
		//std::cout << "SHIFT ROWS" <<std::endl;
		//PrintStateArray();
		MixColumns(); //skopiowane z neta 
		//std::cout << "MIX COLUMS" <<std::endl;
		//PrintStateArray();
		//AddRoundKey(&RoundKeySchedule[round*4, (round+1)*4-1]);
		AddRoundKey(&RoundKeySchedule[round*16]);
		//std::cout << "ADD ROUND KEY" <<std::endl;
		//PrintStateArray();
	}

	SubBytes();
	ShiftRows();

	AddRoundKey( &RoundKeySchedule[NumberOfRounds*16]);
	

	PrintStateArray();

	returnByteBlock(tab);

}


void aes::inv_cipher(byte *tab)
{
	
	for(int i = 0 ; i < 16; i++) 
		std::cout <<std::hex << tab[i];

	std::cout <<std::endl;

	getByteBlock(tab);

	std::cout << "Po wczytaniu bloku z danymi : " <<std::endl;
	PrintStateArray();
	

	AddRoundKey( &RoundKeySchedule[NumberOfRounds*16]);

	//for round = Nr-1 step -1 downto 1
	for(int round = NumberOfRounds-1; round >=1 ;round--)
	{
		InvShiftRows();
		InvSubBytes();
		AddRoundKey(&RoundKeySchedule[round*16]);
		InvMixColumns();
	}
	
	InvShiftRows();
	InvSubBytes();

	AddRoundKey(RoundKeySchedule);
	
	PrintStateArray();
}

void aes::PrintStateArray()
{

	for(int i = 0; i < 4 ; i++)
	{
		for(int j=0; j < 4; j++)
		{
			if((int)stateArray[i][j] < 16)
				std::cout << std::hex << "0"<< (int)stateArray[i][j] << " ";
			else 
				std::cout << std::hex << (int)stateArray[i][j] << " ";
		}
		std::cout << std::endl;
	}

}