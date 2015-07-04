/* This program implements the classical vigenere cipher
 * and outputs only the substituted alphabetic characters.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	int keylen, i;
	char c, s;
	FILE *file;
	
	if(argc < 3)
	{
		printf("usage: %s key filename [r]\n", argv[0]);
		return 1;
	}
	
	keylen = strlen(argv[1]);
	
	// open the input file
	file = fopen(argv[2], "r");
	if(file == 0)
	{
		printf("Could not open file '%s' to read.\n", argv[2]);
		return 1;
	}
	
	// substitute each character according to the key
	i = 0;
	c = fgetc(file);
	while(c != EOF)
	{
		if(isalpha(c))
		{
			s = toupper(argv[1][i]) - 0x41; // shift value from key
			if(argc == 4)
			{ // decrypt (subtract key instead of adding)
				c = (toupper(c)-0x41+26-s)%26 + 0x41;
			}
			else
			{ // encrypt (add key)
				c = (toupper(c)-0x41+s)%26 + 0x41;
			}
			i = (i+1)%keylen;
		}
		putchar(c);
		c = fgetc(file);
	}
	printf("\n");
	
	// close the input file
	fclose(file);
	
	return 0;
}

