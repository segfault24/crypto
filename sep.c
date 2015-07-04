/* This program separates the input file into 'len' number of files
 * and each 'filename_x' contains every x+a*len character for a>=0.
 * It currently ignores all non-alphabetic characters.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	FILE *infile;
	FILE **outfile;
	int len, i;
	char c, buf[256];

	if(argc != 3)
	{
		printf("usage: %s len filename\n", argv[0]);
		return 1;
	}
	
	len = atoi(argv[1]);
	
	// allocate the array for output file handlers
	outfile = (FILE **)malloc(256*sizeof(FILE *));
	if(!outfile)
	{
		printf("Could not allocate memory.\n");
		return 1;
	}
	
	// open the input file
	infile = fopen(argv[2], "r");
	if(!infile)
	{
		printf("Could not open file '%s' to read.\n", buf);
		return 1;
	}
	
	// open each of the output files
	for(i=0; i<len; i++)
	{
		sprintf(buf, "%s_%u", argv[2], i);
		outfile[i] = fopen(buf, "w+");
		if(outfile[i] == 0)
		{
			printf("Could not open file '%s' to write.\n", buf);
			return 1;
		}
	}
	
	// separate the input files character by character
	i = 0;
	c = fgetc(infile);
	while(c != EOF)
	{
		if(isalpha(c))
		{
			fputc(c, outfile[i]);
			i = (i+1)%len;
		}
		c = fgetc(infile);
	}
	
	// close the input and output files
	fclose(infile);
	for(i=0; i<len; i++)
	{
		fputc('\n', outfile[i]);
		fclose(outfile[i]);
	}
	
	return 0;
}

