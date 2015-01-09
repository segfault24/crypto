#ifndef MYMD5_H
#define MYMD5_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "mycrypt.h"

char* mymd5(const unsigned char *data, const uint64_t len);

#endif

