#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mysha256.h"

// function definitions (((with excessive parantheses!)))
#define CH(x, y, z) (((x)&(y))^((~(x))&(z)))
#define MAJ(x, y, z) (((x)&(y))^((x)&(z))^((y)&(z)))

#define SIGMA_U0(x) ((ROTR((x),  2))^(ROTR((x), 13))^(ROTR((x), 22)))
#define SIGMA_U1(x) ((ROTR((x),  6))^(ROTR((x), 11))^(ROTR((x), 25)))
#define SIGMA_L0(x) ((ROTR((x),  7))^(ROTR((x), 18))^((x)>>3))
#define SIGMA_L1(x) ((ROTR((x), 17))^(ROTR((x), 19))^((x)>>10))

const uint32_t K[] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

uint32_t k; // num zeroes to pad
uint32_t l; // length of message in bits
uint32_t m; // number of bits in message block

uint32_t* sha256_rounds(const uint32_t * const p_data, const uint64_t num_blocks) {

    uint32_t M[64]; // local copy of working message block
    uint32_t W[64]; // message schedule
    uint32_t i, j; // loop variables
    uint32_t a, b, c, d, e, f, g, h; // working variables
    uint32_t T_1, T_2; // temporary variables

    uint32_t *H;

    // setup initial hash values
    H = (uint32_t*)malloc(32);
    H[0] = 0x6a09e667; H[1] = 0xbb67ae85; H[2] = 0x3c6ef372; H[3] = 0xa54ff53a;
    H[4] = 0x510e527f; H[5] = 0x9b05688c; H[6] = 0x1f83d9ab; H[7] = 0x5be0cd19;

    for(i=0; i<num_blocks; i++) {
        // copy the current block to M[]
        for(j=0; j<64; j++) {
            M[j] = p_data[i*64+j];
        }

        // prepare the message schedule
        for(j=0; j<16; j++) {
            W[j] = M[j];
        }
        for(j=16; j<64; j++) {
            W[j] = SIGMA_L1(W[j-2]) + W[j-7] + SIGMA_L0(W[j-15]) + W[j-16];
        }

        //for(j=0; j<64; j++) {
        //    if(j%4 == 0) {printf("\n");}
        //    printf("%08x ", W[j]);
        //}
        //printf("\n\n");

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
        for(j=0; j<64; j++) {
            T_1 = h + SIGMA_U1(e) + CH(e, f, g) + K[j] + W[j];
            T_2 = SIGMA_U0(a) + MAJ(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T_1;
            d = c;
            c = b;
            b = a;
            a = T_1 + T_2;

            //printf("%02u %08x %08x %08x %08x %08x %08x %08x %08x\n", j, a, b, c, d, e, f, g, h);
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

    return H;
}

char* mysha256(const unsigned char * const data, const uint64_t len) {
    uint32_t *p_data;
    uint64_t i, j, p_len_words;

    uint32_t *i_hash;
    unsigned char u_hash[32];
    char *c_hash;

/////////////
    p_data = sha256_pad(data, len, &p_len_words);
/////////////
    //for(i=0; i<p_len_words; i++) {
    //    printf("%08x ", p_data[i]);
    //}
    //printf("\n");

    // WHY ENDIAN SWAP??? BECAUSE FUCK, IDK, IT JUST WORKS!
    for(i=0; i<p_len_words; i++) {
        p_data[i] = ENDIAN_SWAP(p_data[i]);
    }
/////////////
    i_hash = sha256_rounds(p_data, p_len_words>>4);
/////////////

    /*for(i=0, j=0; j<32; i++, j+=4) {
        u_hash[j] = (unsigned char)(i_hash[i]&0xff);
        u_hash[j+1] = (unsigned char)((i_hash[i]>>8)&0xff);
        u_hash[j+2] = (unsigned char)((i_hash[i]>>16)&0xff);
        u_hash[j+3] = (unsigned char)((i_hash[i]>>24)&0xff);
    }*/

    for(i=0, j=0; j<32; i++, j+=4) {
        u_hash[j] = (unsigned char)((i_hash[i]>>24)&0xff);
        u_hash[j+1] = (unsigned char)((i_hash[i]>>16)&0xff);
        u_hash[j+2] = (unsigned char)((i_hash[i]>>8)&0xff);
        u_hash[j+3] = (unsigned char)((i_hash[i])&0xff);
    }

    // translate hex numbers to ascii string
    c_hash = (char*)malloc(32);
    sprintf(c_hash, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", \
        u_hash[0], u_hash[1], u_hash[2], u_hash[3],\
        u_hash[4], u_hash[5], u_hash[6], u_hash[7],\
        u_hash[8], u_hash[9], u_hash[10], u_hash[11],\
        u_hash[12], u_hash[13], u_hash[14], u_hash[15],\
        u_hash[16], u_hash[17], u_hash[18], u_hash[19],\
        u_hash[20], u_hash[21], u_hash[22], u_hash[23],\
        u_hash[24], u_hash[25], u_hash[26], u_hash[27],\
        u_hash[28], u_hash[29], u_hash[30], u_hash[31]);

    return c_hash;
}


