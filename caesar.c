#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rotation;
    char c;
    FILE *file;

    if(argc != 3) {
        printf("usage: %s rotation filename\n", argv[0]);
    } else {
        file = fopen(argv[2], "r");
        if(file == 0) {
            printf("Could not open file\n");
        } else {
            rotation = (int)atoi(argv[1]);
            // for each character
            c = fgetc(file);
            while(c != EOF) {
                c = tolower(c);
                if(isalpha(c)) {
                    // modular math to the rescue!
                    printf("%c", (c-0x61+rotation)%26+0x61);
                } else {
                    printf("%c", c);
                }
                c = fgetc(file);
            }
            printf("\n");
            fclose(file);
        }
    }
}

