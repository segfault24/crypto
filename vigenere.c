#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int len, i;
    char c, s;
    FILE *file;

    if(argc < 3) {
        printf("usage: %s key filename [r]\n", argv[0]);
    } else {
        len = strlen(argv[1]);
        file = fopen(argv[2], "r");
        if(file == 0) {
            printf("Could not open file\n");
        } else {
            // for each character
            i = 0;
            c = fgetc(file);
            while(c != EOF) {
                c = toupper(c);
                if(isalpha(c)) {
                    s = toupper(argv[1][i]) - 0x41;
                    if(argc == 4) {
                        c = (c-0x41+26-s)%26 + 0x41;
                    } else {
                        c = (c-0x41+s)%26 + 0x41;
                    }
                    i = (i+1)%len;
                }
                printf("%c", c);
                c = fgetc(file);
            }
            printf("\n");
            fclose(file);
        }
    }
}

