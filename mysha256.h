#ifndef MYSHA256_H
#define MYSHA256_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "mycrypt.h"

char* mysha256(const unsigned char * const data, const uint64_t len);

#endif

