#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mymd5.h"
#include "mysha256.h"

void test(char *data) {
    char *hash;

    hash = mymd5(data, strlen(data));
    printf("md5(\"%s\"): %s\n", data, hash);
    hash = mysha256(data, strlen(data));
    printf("sha256(\"%s\"): %s\n", data, hash);

    printf("\n");
}

int main() {
    test("");
    test("abc");
    test("The quick brown fox jumps over the lazy dog");
    test("tacocat");

    return 0;
}

