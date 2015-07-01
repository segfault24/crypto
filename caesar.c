/* This program implements the classical caesar cipher
 * and outputs only the substituted alphabetic characters.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
	int rotation;
	char c;
	FILE *file;

	if(argc != 3) {
		printf("usage: %s rotation filename\n", argv[0]);
		return 1;
	}
	
	rotation = (int)atoi(argv[1]);
	
	// open the input file
	file = fopen(argv[2], "r");
	if(file == 0) {
		printf("Could not open file '%s' to read.\n", argv[2]);
		return 1;
	}
	
	// substitute each character according to 'rotation'
	c = fgetc(file);
	while(c != EOF) {
		if(isalpha(c)) {
			// modular math to the rescue!
			c = (toupper(c)-0x41+rotation)%26+0x41;
			printf("%c", c);
		}
		c = fgetc(file);
	}
	printf("\n");
	
	// close the input file
	fclose(file);

	return 0;
}

