/* This program counts the frequency of each letter and bigram
 * in a file then outputs the results to stdout.
 */

/*char bigram[2][26] = {
	{'t','h','i','e','a','r','n','a','o','n','h','e','s',
	 'e','e','t','i','o','e','h','i','o','t','a','t','e'},
	{'h','e','n','r','n','e','d','t','n','t','a','s','t',
	 'n','d','o','t','u','a','i','s','r','i','s','e','t'}};*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void print_usage()
{
	printf("usage: freq [-a|-b|-c] [-n] [-z] filename\n");
	printf("   -a   print all counts\n");
	printf("   -b   print only bigram counts\n");
	printf("   -c   print only character counts (default)\n");
	printf("   -n   don't sort before printing\n");
	printf("   -z   print zero counts\n");
	//TODO: printf("   -i   print natural frequencies info\n");
}

int main(int argc, char* argv[])
{
	int file_arg, flag_sort, flag_single, flag_bigram, flag_zeroes;
	
	FILE *file;
	int i, j, t_i, t_c;
	char b, c;
	int ccount[26][2]; // single character counts, 0:char 1:count
	int bcount[26*26][3]; // bigram counts, 0:char1 1:char2 2:count 
	
	// default behaviors
	file_arg = -1;
	flag_sort = 1;
	flag_single = 0;
	flag_bigram = 0;
	flag_zeroes = 0;
	
	// parse each command line argument
	for(i=1; i<argc; i++)
	{
		// it's a flag
		if(argv[i][0]=='-')
		{
			// go through all the flags in the argument
			j = 1;
			c = argv[i][j];
			while(c != '\0')
			{
				// which flag is it
				switch(c)
				{
					case 'a':
						flag_single = 1;
						flag_bigram = 1;
						break;
					case 'b':
						flag_bigram = 1;
						break;
					case 'c':
						flag_single = 1;
						break;
					case 'n':
						flag_sort = 0;
						break;
					case 'z':
						flag_zeroes = 1;
						break;
					default:
						printf("Unknown option '%c'.\n", c);
						print_usage();
						return 1;
				}
				j++;
				c = argv[i][j];
			}
		}
		else
		{
			file_arg = i;
		}
	}
	if(!flag_single && !flag_bigram)
	{
		flag_single = 1;
	}
	
	// initialize the counter arrays
	for(i=0; i<26; i++)
	{
		ccount[i][0] = 0x41+i;
		ccount[i][1] = 0;
		for(j=0; j<26; j++)
		{
			bcount[i*26+j][0] = 0x41+i;
			bcount[i*26+j][1] = 0x41+j;
			bcount[i*26+j][2] = 0;
		}
	}
	
	// open the input file
	if(file_arg == -1)
	{
		printf("No file specified.\n");
		print_usage();
		return 1;
	}
	file = fopen(argv[file_arg], "r");
	if(file == 0)
	{
		printf("Could not open file '%s' to read.\n", argv[file_arg]);
		return 1;
	}
	
	// read the first two characters
	do
	{
		b = toupper(fgetc(file));
	} while(!isalpha(b) && b != EOF);
	ccount[b-0x41][1]++; // process the first one special
	do
	{
		c = toupper(fgetc(file));
	} while(!isalpha(c) && c != EOF);
	
	// for each character and each pair of characters
	while(c != EOF)
	{
		ccount[c-0x41][1]++;
		bcount[(b-0x41)*26+(c-0x41)][2]++;
		
		// move onto the next character, update pair
		b = c;
		do
		{
			c = toupper(fgetc(file));
		} while(!isalpha(c) && c != EOF);
	}
	fclose(file);
	
	if(flag_sort)
	{
		// sort the single character array
		for(i=0; i<26; i++)
		{
			t_i = i;
			t_c = ccount[i][1];
			for(j=i+1; j<26; j++)
			{
				if(ccount[j][1]>t_c)
				{
					t_i = j;
					t_c = ccount[j][1];
				}
			}
			// swap em
			b = ccount[t_i][0];
			ccount[t_i][0] = ccount[i][0];
			ccount[t_i][1] = ccount[i][1];
			ccount[i][0] = b;
			ccount[i][1] = t_c;
		}
		
		// sort the bigram array
		for(i=0; i<26*26; i++)
		{
			t_i = i;
			t_c = bcount[i][2];
			for(j=i+1; j<26*26; j++)
			{
				if(bcount[j][2]>t_c)
				{
					t_i = j;
					t_c = bcount[j][2];
				}
			}
			// swap em
			b = bcount[t_i][0];
			c = bcount[t_i][1];
			bcount[t_i][0] = bcount[i][0];
			bcount[t_i][1] = bcount[i][1];
			bcount[t_i][2] = bcount[i][2];
			bcount[i][0] = b;
			bcount[i][1] = c;
			bcount[i][2] = t_c;
		}
	}
	
	// print out counts
	if(flag_single)
	{
		for(i=0; i<26; i++)
		{
			if(ccount[i][1]!=0 || flag_zeroes)
			{
				printf("%c:%u\n", ccount[i][0], ccount[i][1]);
			}
		}
	}
	if(flag_bigram)
	{
		for(i=0; i<26*26; i++)
		{
			if(bcount[i][2]!=0 || flag_zeroes)
			{
				printf("%c%c:%u\n", bcount[i][0], bcount[i][1], bcount[i][2]);
			}
		}
	}
	
	return 0;
}

