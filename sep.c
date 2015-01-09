#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *infile;
    FILE **outfile;
    int len, i;
    char c, buf[256];

    if(argc != 3) {
        printf("usage: %s len filename\n", argv[0]);
    } else {
        len = atoi(argv[1]);
        outfile = (FILE **)malloc(256*sizeof(FILE *));

        infile = fopen(argv[2], "r");
        if(infile == 0) {
            printf("Could not open file\n");
        } else {
            for(i=0; i<len; i++) {
                sprintf(buf, "%s_%u", argv[2], i);
                outfile[i] = fopen(buf, "w+");
                if(outfile[i] == 0) {
                    printf("Could not write file\n");
                    return 1;
                }
            }

            i = 0;
            c = fgetc(infile);
            while(c != EOF) {
                fputc(c, outfile[i]);
                i = (i+1)%len;
                c = fgetc(infile);
            }
            fclose(infile);
            for(i=0; i<len; i++) {
                fputc('\n', outfile[i]);
                fclose(outfile[i]);
            }
        }
    }
}

