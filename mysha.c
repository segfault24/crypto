/* 
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mycrypt.h"

// thank you GCC
#define uint128_t unsigned __int128

// for internal purposes
#define MODE_224		224
#define MODE_256		256
#define MODE_384		384
#define MODE_512		512
#define MODE_512_224	5224
#define MODE_512_256	5256

// function definitions (((with excessive parentheses!)))
#define CH(x, y, z) (((x)&(y))^((~(x))&(z)))
#define MAJ(x, y, z) (((x)&(y))^((x)&(z))^((y)&(z)))

#define SIGMA_U0(x) ((ROTR32((x),  2))^(ROTR32((x), 13))^(ROTR32((x), 22)))
#define SIGMA_U1(x) ((ROTR32((x),  6))^(ROTR32((x), 11))^(ROTR32((x), 25)))
#define SIGMA_L0(x) ((ROTR32((x),  7))^(ROTR32((x), 18))^((x)>>3))
#define SIGMA_L1(x) ((ROTR32((x), 17))^(ROTR32((x), 19))^((x)>>10))

#define SIGMA_U2(x) ((ROTR64((x), 28))^(ROTR64((x), 34))^(ROTR64((x), 39)))
#define SIGMA_U3(x) ((ROTR64((x), 14))^(ROTR64((x), 18))^(ROTR64((x), 41)))
#define SIGMA_L2(x) ((ROTR64((x),  1))^(ROTR64((x),  8))^((x)>>7))
#define SIGMA_L3(x) ((ROTR64((x), 19))^(ROTR64((x), 61))^((x)>>6))

const uint32_t K256[] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

const uint64_t K512[] =
{
	0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538,
	0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe,
	0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
	0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
	0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab,
	0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
	0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed,
	0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
	0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
	0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
	0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373,
	0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
	0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c,
	0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6,
	0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
	0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

const uint32_t H256[] =
{
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

const uint32_t H224[] =
{
	0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939, 0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
};

const uint64_t H512[] =
{
	0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
	0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
};

const uint64_t H384[] =
{
	0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939,
	0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
};

int sha_256_224(char* filename, int mode, uint32_t* hash_output)
{
	FILE* file;
	size_t j; // general use loop variable
	int special, done, append_length; // loop control to help process the last block(s)
	
	uint64_t total_len; // total length of message in bytes
	uint32_t read_len; // number of bytes from file to current block

	uint32_t H[8]; // working hash value
	uint32_t W[64]; // message schedule
	uint32_t a, b, c, d, e, f, g, h; // working variables
	uint32_t T_1, T_2; // temporary variables

	// first things first, open the file
	file = fopen(filename, "r");
	if(!file)
	{
		return 1;
	}
	
	// setup initial hash values
	if(mode == MODE_224)
	{
		memcpy(&H, H224, 32);
	}
	else
	{
		memcpy(&H, H256, 32);
	}
	
	// data blocks are 16 words (aka 64 bytes or 512 bits)
	total_len = 0;
	read_len = 0;
	special = 0;
	done = 0;
	append_length = 0;
	while(!done)
	{
		// This block of if/else reads in data from the file and does the
		// processing for the special cases of the last block(s) so that the
		// additional '1' bit and total bit length are appended in the correct
		// places independently of the main processing sections.
		memset(W, 0, 64);
		if(special)
		{
			// This is the special case where the previous block was the end of
			// the file and it had enough empty space for the appended '1' but
			// not for the appended bit length. So we just use an empty block
			// and append the bit length (but not the '1' bit).
			append_length = 1;
			done = 1;
		}
		else
		{
			// copy in the next 16 words (64 bytes) from the file
			read_len = fread(W, 1, 64, file);
			total_len += read_len;
			
			// pad if necessary
			if(read_len<64)
			{
				// append the '1' bit
				((char*)W)[read_len] = 128;
				
				// check if there's enough space to append the length
				if(read_len>55)
				{
					// not enough space to append length
					// tell the next loop to run the special case code
					append_length = 0;
					special = 1;
				}
				else
				{
					// enough space to append length
					// we're done after this block
					append_length = 1;
					done = 1;
				}
			}
		}
		
		if(append_length)
		{
			((char*)W)[56] = (total_len >> 53)&0xff;
			((char*)W)[57] = (total_len >> 45)&0xff;
			((char*)W)[58] = (total_len >> 37)&0xff;
			((char*)W)[59] = (total_len >> 29)&0xff;
			((char*)W)[60] = (total_len >> 21)&0xff;
			((char*)W)[61] = (total_len >> 13)&0xff;
			((char*)W)[62] = (total_len >> 5)&0xff;
			((char*)W)[63] = (total_len << 3)&0xff;
		}
		
		// endian swap all the words
		for(j=0; j<16; j++)
		{
			W[j] = ENDIANSWAP32(W[j]);
		}
		
		// below here is the main body of the SHA processing
		
		// extend the message schedule
		for(j=16; j<64; j++)
		{
			W[j] = SIGMA_L1(W[j-2]) + W[j-7] + SIGMA_L0(W[j-15]) + W[j-16];
		}
		
		// initialize working variables
		a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		e = H[4];
		f = H[5];
		g = H[6];
		h = H[7];
		
		// primary computation loop
		for(j=0; j<64; j++)
		{
			T_1 = h + SIGMA_U1(e) + CH(e, f, g) + K256[j] + W[j];
			T_2 = SIGMA_U0(a) + MAJ(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T_1;
			d = c;
			c = b;
			b = a;
			a = T_1 + T_2;
		}
		
		// store intermediate hash value
		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
		H[5] += f;
		H[6] += g;
		H[7] += h;
	}

	// copy the result into the caller's array
	memcpy(hash_output, &H, 32);
	
	return 0;
}

int sha_512_384(char* filename, int mode, uint64_t* hash_output)
{
	FILE* file;
	size_t j; // general use loop variable
	int special, done, append_length; // loop control to help process the last block(s)
	
	uint128_t total_len; // total length of message in bytes
	uint64_t read_len; // number of bytes from file to current block

	uint64_t H[8]; // working hash value
	uint64_t W[64]; // message schedule
	uint64_t a, b, c, d, e, f, g, h; // working variables
	uint64_t T_1, T_2; // temporary variables

	// first things first, open the file
	file = fopen(filename, "r");
	if(!file)
	{
		return 1;
	}
	
	// setup initial hash values
	for(j=0; j<8; j++)
	{
		if(mode == MODE_384)
		{
			H[j] = H384[j];
		}
		else
		{
			H[j] = H512[j];
		}
	}
	
	//////////////////////////
	//////////////////////////
	for(j=0; j<8; j++)
	{
		printf("H[%u] = %08x%08x\n", (uint32_t)j, (uint32_t)(H[j]>>32), (uint32_t)H[j]);
	}
	printf("\n");
	///////////////////////////
	///////////////////////////
	
	// data blocks are 32 words (aka 128 bytes or 1024 bits)
	total_len = 0;
	read_len = 0;
	special = 0;
	done = 0;
	append_length = 0;
	while(!done)
	{
		// This block of if/else reads in data from the file and does the
		// processing for the special cases of the last block(s) so that the
		// additional '1' bit and total bit length are appended in the correct
		// places independently of the main processing sections.
		memset(W, 0, 128);
		if(special)
		{
			// This is the special case where the previous block was the end of
			// the file and it had enough empty space for the appended '1' but
			// not for the appended bit length. So we just use an empty block
			// and append the bit length (but not the '1' bit).
			append_length = 1;
			done = 1;
		}
		else
		{
			// copy in the next 32 words (128 bytes) from the file
			read_len = fread(W, 1, 128, file);
			total_len += read_len;
			
			// pad if necessary
			if(read_len<128)
			{
				// append the '1' bit
				((char*)W)[read_len] = 128;
				
				// check if there's enough space to append the length
				if(read_len>111)
				{
					// not enough space to append length
					// tell the next loop to run the special case code
					append_length = 0;
					special = 1;
				}
				else
				{
					// enough space to append length
					// we're done after this block
					append_length = 1;
					done = 1;
				}
			}
		}
		
		if(append_length)
		{
			((char*)W)[112] = (total_len >> 117)&0xff;
			((char*)W)[113] = (total_len >> 109)&0xff;
			((char*)W)[114] = (total_len >> 101)&0xff;
			((char*)W)[115] = (total_len >> 93)&0xff;
			((char*)W)[116] = (total_len >> 85)&0xff;
			((char*)W)[117] = (total_len >> 77)&0xff;
			((char*)W)[118] = (total_len >> 69)&0xff;
			((char*)W)[119] = (total_len >> 61)&0xff;
			((char*)W)[120] = (total_len >> 53)&0xff;
			((char*)W)[121] = (total_len >> 45)&0xff;
			((char*)W)[122] = (total_len >> 37)&0xff;
			((char*)W)[123] = (total_len >> 29)&0xff;
			((char*)W)[124] = (total_len >> 21)&0xff;
			((char*)W)[125] = (total_len >> 13)&0xff;
			((char*)W)[126] = (total_len >> 5)&0xff;
			((char*)W)[127] = (total_len << 3)&0xff;
		}
	
		// endian swap all the words
		for(j=0; j<32; j++)
		{
			W[j] = ENDIANSWAP64(W[j]);
		}
		
		////////////////////////
		////////////////////////
		for(j=0;j<16;j++)
		{
			printf("W[%u] = %08x%08x\n", (uint32_t)j, (uint32_t)(W[j]>>32), (uint32_t)W[j]);
		}
		printf("\n");
		////////////////////////
		////////////////////////
		
		// below here is the main body of the SHA processing
		
		// extend the message schedule
		for(j=16; j<80; j++)
		{
			W[j] = SIGMA_L3(W[j-2]) + W[j-7] + SIGMA_L2(W[j-15]) + W[j-16];
		}
		
		// initialize working variables
		a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		e = H[4];
		f = H[5];
		g = H[6];
		h = H[7];
		
		// primary computation loop
		for(j=0; j<80; j++)
		{
			///////////////////////////
			///////////////////////////
			if(j<4)
			{
				printf("t=%02d:  %08x%08x %08x%08x %08x%08x %08x%08x\n",
					(uint32_t)j,
					(uint32_t)a, (uint32_t)(a>>32),
					(uint32_t)b, (uint32_t)(b>>32),
					(uint32_t)c, (uint32_t)(c>>32),
					(uint32_t)d, (uint32_t)(d>>32)
				);
				printf("       %08x%08x %08x%08x %08x%08x %08x%08x\n",
					(uint32_t)e, (uint32_t)(e>>32),
					(uint32_t)f, (uint32_t)(f>>32),
					(uint32_t)g, (uint32_t)(g>>32),
					(uint32_t)h, (uint32_t)(h>>32)
				);
				printf("\n");
			}
			///////////////////////////
			///////////////////////////
			
			T_1 = h + SIGMA_U3(e) + CH(e, f, g) + K512[j] + W[j];
			T_2 = SIGMA_U2(a) + MAJ(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T_1;
			d = c;
			c = b;
			b = a;
			a = T_1 + T_2;
		}
		
		// store intermediate hash value
		H[0] += a;
		H[1] += b;
		H[2] += c;
		H[3] += d;
		H[4] += e;
		H[5] += f;
		H[6] += g;
		H[7] += h;
	}

	// copy the result into the caller's array
	memcpy(hash_output, &H, 64);
	
	return 0;
}

int main(int argc, char *argv[])
{
	int i, j, mode, ret;
	uint32_t h1[8];
	uint64_t h2[8];
	
	if(argc < 3)
	{
		printf("usage: %s func file1 file2 ...\n", argv[0]);
		return 1;
	}
	
	// parse the hash type
	if(!strcmp(argv[1], "224")) { mode = MODE_224; }
	else if (!strcmp(argv[1], "256")) { mode = MODE_256; }
	else if (!strcmp(argv[1], "384")) { mode = MODE_384; }
	else if (!strcmp(argv[1], "512")) { mode = MODE_512; }
	else
	{
		printf("Specified hash mode '%s' not recognized.\n", argv[1]);
		return 1;
	}
	
	// loop through all the files
	for(i=2; i<argc; i++)
	{	
		// pass to the correct hash function
		switch(mode)
		{
		case MODE_224:
			ret = sha_256_224(argv[i], MODE_224, &h1[0]);
			break;
		case MODE_256:
			ret = sha_256_224(argv[i], MODE_256, &h1[0]);
			break;
		case MODE_384:
			ret = sha_512_384(argv[i], MODE_384, &h2[0]);
			break;
		case MODE_512:
			ret = sha_512_384(argv[i], MODE_512, &h2[0]);
			break;
		default:
			printf("How did you reach this code?\n");
			return 1;
		}
		
		// make sure the hashing function didn't error
		if(ret)
		{
			printf("Error hashing file '%s'.\n", argv[i]);
			continue;
		}
		
		// print out the hash
		switch(mode)
		{
		case MODE_224:
			for(j=0; j<7; j++) { printf("%08x", h1[j]); }
			break;
		case MODE_256:
			for(j=0; j<8; j++) { printf("%08x", h1[j]); }
			break;
		case MODE_384:
			for(j=0; j<7; j++) { printf("%08x%08x", (uint32_t)(h2[j]>>32), (uint32_t)h2[j]); }
			break;
		case MODE_512:
			for(j=0; j<8; j++) { printf("%08x%08x", (uint32_t)(h2[j]>>32), (uint32_t)h2[j]); }
			break;
		default:
			printf("How did you reach this code?\n");
			return 1;
		}
		printf("  %s\n", argv[i]);
	}
	
	return 0;
}
