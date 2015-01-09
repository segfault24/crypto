#ifndef MYCRYPT_H
#define MYCRYPT_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ROTL(a, s) (((a) << (s))|((a) >> (32-(s))))
#define ROTR(a, s) (((a) >> (s))|((a) << (32-(s))))
#define ENDIAN_SWAP(a) (((a>>24)&0x000000ff)|((a>>8)&0x0000ff00)|((a<<8)&0x00ff0000)|((a<<24)&0xff000000))

uint32_t* md5_pad(const char * const data, const uint64_t len_bytes, uint64_t * const p_len_words);
uint32_t* sha256_pad(const char * const data, const uint64_t len_bytes, uint64_t * const p_len_words);

#endif

