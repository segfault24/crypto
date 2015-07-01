#ifndef MYCRYPT_H
#define MYCRYPT_H

#include <stdint.h>

#define ROTL(a, s) (((a) << (s))|((a) >> (32-(s))))
#define ROTR(a, s) (((a) >> (s))|((a) << (32-(s))))
#define ENDIANSWAP32(a) (((a>>24)&0x000000ff)|((a>>8)&0x0000ff00)|((a<<8)&0x00ff0000)|((a<<24)&0xff000000))

char* mymd5(const char *data, const uint64_t len);
char* mysha256(const char * const data, const uint64_t len);

#endif
