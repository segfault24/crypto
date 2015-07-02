#ifndef MYCRYPT_H
#define MYCRYPT_H

#include <stdint.h>

#define ROTL32(a, s) (((a) << (s))|((a) >> (32-(s))))
#define ROTR32(a, s) (((a) >> (s))|((a) << (32-(s))))
#define ROTL64(a, s) (((a) << (s))|((a) >> (64-(s))))
#define ROTR64(a, s) (((a) >> (s))|((a) << (64-(s))))

#define ENDIANSWAP32(a) (\
	((a>>24) &0x000000ff) | \
	((a>>8)  &0x0000ff00) | \
	((a<<8)  &0x00ff0000) | \
	((a<<24) &0xff000000) \
)
#define ENDIANSWAP64(a) (\
	((a>>56) &0x00000000000000ff) | \
	((a>>40) &0x000000000000ff00) | \
	((a>>24) &0x0000000000ff0000) | \
	((a>>8) &0x00000000ff000000) | \
	((a<<8) &0x000000ff00000000) | \
	((a<<24) &0x0000ff0000000000) | \
	((a<<40) &0x00ff000000000000) | \
	((a<<56) &0xff00000000000000) \
)

#endif
