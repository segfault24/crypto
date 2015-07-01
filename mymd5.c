#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mycrypt.h"

#define F(x, y, z) (((x)&(y))|((~x)&(z)))
#define G(x, y, z) (((x)&(z))|((y)&(~z)))
#define H(x, y, z) ((x)^(y)^(z))
#define I(x, y, z) ((y)^((x)|(~z)))

#define FF(a, b, c, d, k, s, i) {\
(a) = ((a)+F((b),(c),(d))+X[(k)]+T[(i)]);\
(a) = ROTL((a),(s));\
(a) = (a)+(b);\
}
#define GG(a, b, c, d, k, s, i) {\
(a) = ((a)+G((b),(c),(d))+X[(k)]+T[(i)]);\
(a) = ROTL((a),(s));\
(a) = (a)+(b);\
}
#define HH(a, b, c, d, k, s, i) {\
(a) = ((a)+H((b),(c),(d))+X[(k)]+T[(i)]);\
(a) = ROTL((a),(s));\
(a) = (a)+(b);\
}
#define II(a, b, c, d, k, s, i) {\
(a) = ((a)+I((b),(c),(d))+X[(k)]+T[(i)]);\
(a) = ROTL((a),(s));\
(a) = (a)+(b);\
}

const uint32_t T[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32_t* md5_pad(const char * const data, const uint64_t len_bytes, uint64_t * const p_len_words) {
	uint64_t tbp, len_bits, p_len_bytes;
	char *c_data;

	// determine the number of bytes needed for padding
	tbp = 64 - (len_bytes)%64;
	if(tbp < 9) {
		tbp = tbp + 56;
	} else {
		tbp = tbp - 8;
	}

	len_bits = len_bytes << 3; // size of original data in bits
	p_len_bytes = len_bytes + tbp + 8; // size of padded data in bytes
	*p_len_words = p_len_bytes >> 2; // size of padded data in words

	c_data = (char *)calloc(p_len_bytes, sizeof(char)); // new zeroed data array
	memcpy(c_data, data, len_bytes); // copy the original data
	c_data[len_bytes] = 128; // append the single '1' bit

	// append the original message length
	c_data[p_len_bytes - 8] = (len_bits)&0xff;
	c_data[p_len_bytes - 7] = (len_bits >> 8)&0xff;
	c_data[p_len_bytes - 6] = (len_bits >> 16)&0xff;
	c_data[p_len_bytes - 5] = (len_bits >> 24)&0xff;
	c_data[p_len_bytes - 4] = (len_bits >> 32)&0xff;
	c_data[p_len_bytes - 3] = (len_bits >> 40)&0xff;
	c_data[p_len_bytes - 2] = (len_bits >> 48)&0xff;
	c_data[p_len_bytes - 1] = (len_bits >> 56)&0xff;

	return (uint32_t*)c_data;
}

// 16 words, aka 64 bytes or 512 bits
uint32_t* rounds(const uint32_t * const M, const uint64_t num_words) {
	uint32_t i, j; // loop variables
	uint32_t X[16]; // local storage of working block
	uint32_t A, B, C, D, AA, BB, CC, DD;
	uint64_t num_blocks;
	uint32_t *hash;

	A = 0x67452301;
	B = 0xefcdab89;
	C = 0x98badcfe;
	D = 0x10325476;

	num_blocks = num_words >> 4;
	// process the blocks one by one
	for(i=0; i<num_blocks; i++) {

		// copy the block to local storage
		for(j=0; j<16; j++) {
			X[j] = M[i*16+j];
		}

		AA = A;
		BB = B;
		CC = C;
		DD = D;

		// preprocessor macros will expand these functions
		FF(A, B, C, D,  0,  7,  0);
		FF(D, A, B, C,  1, 12,  1);
		FF(C, D, A, B,  2, 17,  2);
		FF(B, C, D, A,  3, 22,  3);

		FF(A, B, C, D,  4,  7,  4);
		FF(D, A, B, C,  5, 12,  5);
		FF(C, D, A, B,  6, 17,  6);
		FF(B, C, D, A,  7, 22,  7);

		FF(A, B, C, D,  8,  7,  8);
		FF(D, A, B, C,  9, 12,  9);
		FF(C, D, A, B, 10, 17, 10);
		FF(B, C, D, A, 11, 22, 11);

		FF(A, B, C, D, 12,  7, 12);
		FF(D, A, B, C, 13, 12, 13);
		FF(C, D, A, B, 14, 17, 14);
		FF(B, C, D, A, 15, 22, 15);

		GG(A, B, C, D,  1,  5, 16);
		GG(D, A, B, C,  6,  9, 17);
		GG(C, D, A, B, 11, 14, 18);
		GG(B, C, D, A,  0, 20, 19);

		GG(A, B, C, D,  5,  5, 20);
		GG(D, A, B, C, 10,  9, 21);
		GG(C, D, A, B, 15, 14, 22);
		GG(B, C, D, A,  4, 20, 23);

		GG(A, B, C, D,  9,  5, 24);
		GG(D, A, B, C, 14,  9, 25);
		GG(C, D, A, B,  3, 14, 26);
		GG(B, C, D, A,  8, 20, 27);

		GG(A, B, C, D, 13,  5, 28);
		GG(D, A, B, C,  2,  9, 29);
		GG(C, D, A, B,  7, 14, 30);
		GG(B, C, D, A, 12, 20, 31);

		HH(A, B, C, D,  5,  4, 32);
		HH(D, A, B, C,  8, 11, 33);
		HH(C, D, A, B, 11, 16, 34);
		HH(B, C, D, A, 14, 23, 35);

		HH(A, B, C, D,  1,  4, 36);
		HH(D, A, B, C,  4, 11, 37);
		HH(C, D, A, B,  7, 16, 38);
		HH(B, C, D, A, 10, 23, 39);

		HH(A, B, C, D, 13,  4, 40);
		HH(D, A, B, C,  0, 11, 41);
		HH(C, D, A, B,  3, 16, 42);
		HH(B, C, D, A,  6, 23, 43);

		HH(A, B, C, D,  9,  4, 44);
		HH(D, A, B, C, 12, 11, 45);
		HH(C, D, A, B, 15, 16, 46);
		HH(B, C, D, A,  2, 23, 47);

		II(A, B, C, D,  0,  6, 48);
		II(D, A, B, C,  7, 10, 49);
		II(C, D, A, B, 14, 15, 50);
		II(B, C, D, A,  5, 21, 51);

		II(A, B, C, D, 12,  6, 52);
		II(D, A, B, C,  3, 10, 53);
		II(C, D, A, B, 10, 15, 54);
		II(B, C, D, A,  1, 21, 55);

		II(A, B, C, D,  8,  6, 56);
		II(D, A, B, C, 15, 10, 57);
		II(C, D, A, B,  6, 15, 58);
		II(B, C, D, A, 13, 21, 59);

		II(A, B, C, D,  4,  6, 60);
		II(D, A, B, C, 11, 10, 61);
		II(C, D, A, B,  2, 15, 62);
		II(B, C, D, A,  9, 21, 63);

		A += AA;
		B += BB;
		C += CC;
		D += DD;
	}

	hash = (uint32_t*)malloc(16);
	hash[0] = A;
	hash[1] = B;
	hash[2] = C;
	hash[3] = D;
	return hash;
}

char* mymd5(const char * const data, const uint64_t len) {
	uint32_t *p_data;
	uint64_t i, j, p_len_words;

	uint32_t *i_hash;
	unsigned char u_hash[16];
	char *c_hash;

	p_data = md5_pad(data, len, &p_len_words);
	i_hash = rounds(p_data, p_len_words);

	for(i=0, j=0; j<16; i++, j+=4) {
		u_hash[j] = (unsigned char)(i_hash[i]&0xff);
		u_hash[j+1] = (unsigned char)((i_hash[i]>>8)&0xff);
		u_hash[j+2] = (unsigned char)((i_hash[i]>>16)&0xff);
		u_hash[j+3] = (unsigned char)((i_hash[i]>>24)&0xff);
	}

	// translate hex numbers to ascii string
	c_hash = (char*)malloc(16);
	sprintf(c_hash, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		u_hash[0], u_hash[1], u_hash[2], u_hash[3],
		u_hash[4], u_hash[5], u_hash[6], u_hash[7],
		u_hash[8], u_hash[9], u_hash[10], u_hash[11],
		u_hash[12], u_hash[13], u_hash[14], u_hash[15]);

	return c_hash;
}

void test(char *data) {
	char *hash = mymd5(data, strlen(data));
	printf("md5(\"%s\"): %s\n", data, hash);
}

int main() {
	test("");
	test("abc");
	test("The quick brown fox jumps over the lazy dog");
	test("tacocat");
	return 0;
}
