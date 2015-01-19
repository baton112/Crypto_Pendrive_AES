#include "aes.h"


aes::aes(BYTE *key, int keyLength)
{
	rows = keyLength/32;
	for(int i = 0; i < rows; i++) //row 
	{
		keyArray[i] = new BYTE[4]; //new colums 
	}
	RoundKeySchedule = new BYTE[240];

	for(int i = 0 ; i < keyLength/8;i++)
	{
		RoundKeySchedule[i] = key[i];
	}

	expandKey();

	//w0 = kolumna pierwsza key array 
	//w1 = kol 2 keyArray 
	//w2 = kol 3 keyArray
	//w3 = kol 4 keyArray
	///xor z XOR’ed with the input block before the round-based processing begins
	// potem magiczne xory na nich i magiczna funkcja g i zapelniamy cala tablice keySchedule 
	PrintExpandedKey(RoundKeySchedule, 10);

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

//word - 4 bajty 
UINT32 aes::getWord(BYTE *tab, int i)
{
	UINT32 tmp = 0;



	return tmp; 
}

/*
void aes::setKey_enc()
{
    unsigned int i;

	switch( keySize )
    {
		case 128: roundsToDo = 10; break;
        case 192: roundsToDo = 12; break;
        case 256: roundsToDo = 14; break;
    }


	for( i = 0; i < ( keySize >> 5 ); i++ )
    {
        GET_UINT32_LE( RK[i], key, i << 2 );
    }

    switch( ctx->nr )
    {
        case 10:

            for( i = 0; i < 10; i++, RK += 4 )
            {
                RK[4]  = RK[0] ^ RCON[i] ^
                ( (uint32_t) FSb[ ( RK[3] >>  8 ) & 0xFF ]       ) ^
                ( (uint32_t) FSb[ ( RK[3] >> 16 ) & 0xFF ] <<  8 ) ^
                ( (uint32_t) FSb[ ( RK[3] >> 24 ) & 0xFF ] << 16 ) ^
                ( (uint32_t) FSb[ ( RK[3]       ) & 0xFF ] << 24 );

                RK[5]  = RK[1] ^ RK[4];
                RK[6]  = RK[2] ^ RK[5];
                RK[7]  = RK[3] ^ RK[6];
            }
            break;

        case 12:

            for( i = 0; i < 8; i++, RK += 6 )
            {
                RK[6]  = RK[0] ^ RCON[i] ^
                ( (uint32_t) FSb[ ( RK[5] >>  8 ) & 0xFF ]       ) ^
                ( (uint32_t) FSb[ ( RK[5] >> 16 ) & 0xFF ] <<  8 ) ^
                ( (uint32_t) FSb[ ( RK[5] >> 24 ) & 0xFF ] << 16 ) ^
                ( (uint32_t) FSb[ ( RK[5]       ) & 0xFF ] << 24 );

                RK[7]  = RK[1] ^ RK[6];
                RK[8]  = RK[2] ^ RK[7];
                RK[9]  = RK[3] ^ RK[8];
                RK[10] = RK[4] ^ RK[9];
                RK[11] = RK[5] ^ RK[10];
            }
            break;

        case 14:

            for( i = 0; i < 7; i++, RK += 8 )
            {
                RK[8]  = RK[0] ^ RCON[i] ^
                ( (uint32_t) FSb[ ( RK[7] >>  8 ) & 0xFF ]       ) ^
                ( (uint32_t) FSb[ ( RK[7] >> 16 ) & 0xFF ] <<  8 ) ^
                ( (uint32_t) FSb[ ( RK[7] >> 24 ) & 0xFF ] << 16 ) ^
                ( (uint32_t) FSb[ ( RK[7]       ) & 0xFF ] << 24 );

                RK[9]  = RK[1] ^ RK[8];
                RK[10] = RK[2] ^ RK[9];
                RK[11] = RK[3] ^ RK[10];

                RK[12] = RK[4] ^
                ( (uint32_t) FSb[ ( RK[11]       ) & 0xFF ]       ) ^
                ( (uint32_t) FSb[ ( RK[11] >>  8 ) & 0xFF ] <<  8 ) ^
                ( (uint32_t) FSb[ ( RK[11] >> 16 ) & 0xFF ] << 16 ) ^
                ( (uint32_t) FSb[ ( RK[11] >> 24 ) & 0xFF ] << 24 );

                RK[13] = RK[5] ^ RK[12];
                RK[14] = RK[6] ^ RK[13];
                RK[15] = RK[7] ^ RK[14];
            }
            break;
    }

   // return( 0 );
}
*/

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
    /* c is 16 because the first sub-key is the user-supplied key */
	unsigned char c = 16;
	unsigned char i = 1;
    unsigned char a;

	/* We need 11 sets of sixteen bytes each for 128-bit mode */
    while(c < 176) {
            /* Copy the temporary variable over from the last 4-byte
                * block */
            for(a = 0; a < 4; a++) 
				t[a] = RoundKeySchedule[a + c - 4];
            /* Every four blocks (of four bytes), 
                * do a complex calculation */
            if(c % 16 == 0) {
				schedule_core(t,i);
				i++;
			}
            for(a = 0; a < 4; a++) {
				RoundKeySchedule[c] = RoundKeySchedule[c - 16] ^ t[a];
                    c++;
            }
    }
}

