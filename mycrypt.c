#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

uint32_t* sha256_pad(const char * const data, const uint64_t len_bytes, uint64_t * const p_len_words) {
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
    c_data[p_len_bytes - 8] = (len_bits >> 56)&0xff;
    c_data[p_len_bytes - 7] = (len_bits >> 48)&0xff;
    c_data[p_len_bytes - 6] = (len_bits >> 40)&0xff;
    c_data[p_len_bytes - 5] = (len_bits >> 32)&0xff;
    c_data[p_len_bytes - 4] = (len_bits >> 24)&0xff;
    c_data[p_len_bytes - 3] = (len_bits >> 16)&0xff;
    c_data[p_len_bytes - 2] = (len_bits >> 8)&0xff;
    c_data[p_len_bytes - 1] = (len_bits)&0xff;

    return (uint32_t*)c_data;
}

