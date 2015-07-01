/* This program counts the frequency of each letter and bigram
 * in a file then outputs the results to stdout.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
	FILE *file;
	int i, j, t_i, t_c, c_point, b_point;
	char b, c, found;
	int ccount[26][2]; // 0:char 1:count
	int bcount[676][3]; // 0:char1 1:char2 2:count 
	/*char bigram[2][26] = {
		{'t','h','i','e','a','r','n','a','o','n','h','e','s',
		 'e','e','t','i','o','e','h','i','o','t','a','t','e'},
		{'h','e','n','r','n','e','d','t','n','t','a','s','t',
		 'n','d','o','t','u','a','i','s','r','i','s','e','t'}};*/

	if(argc < 2) {
		printf("usage: %s filename\n", argv[0]);
		return 1;
	}
	
	// open the input file
	file = fopen(argv[1], "r");
	if(file == 0) {
		printf("Could not open file '%s' to read.\n", argv[1]);
		return 1;
	}
	
	// zero the counter arrays
	memset(ccount, 0, sizeof(ccount[0][0])*26*2);
	memset(bcount, 0, sizeof(bcount[0][0])*676*3);
	c_point = 0;
	b_point = 0;

	// for each character and each pair of characters
	b = toupper(fgetc(file));
	c = toupper(fgetc(file));
	while(b != EOF) {
		// --- single character ---
		// search the array
		found = 0;
		for(i=0; i<26; i++) {
			// if we find an existing entry
			if(b == ccount[i][0]) {
				ccount[i][1]++;
				found = 1;
				break;
			}
		}
		// add an entry
		if(found == 0 && isalpha(b)) {
			ccount[c_point][0] = b;
			ccount[c_point][1] = 1;
			c_point++;
		}

		// --- bigrams ---
		// search the array
		found = 0;
		for(i=0; i<676; i++) {
			// if we find an existing entry
			if(b == bcount[i][0] && c == bcount[i][1]) {
				bcount[i][2]++;
				found = 1;
				break;
			}
		}
		// add an entry
		if(found == 0 && isalpha(b) && isalpha(c)) {
			bcount[b_point][0] = b;
			bcount[b_point][1] = c;
			bcount[b_point][2] = 1;
			b_point++;
		}

		// move onto the next character, update pair
		b = c;
		c = toupper(fgetc(file));
	}
	fclose(file);

	// sort the single character array
	for(i=0; i<26; i++) {
		t_i = i;
		t_c = ccount[i][1];
		for(j=i+1; j<26; j++) {
			if(ccount[j][1]>t_c) {
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
	for(i=0; i<676; i++) {
		t_i = i;
		t_c = bcount[i][2];
		for(j=i+1; j<676; j++) {
			if(bcount[j][2]>t_c) {
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

	// print out frequencies
	if(argc == 3) {
		printf("%c", (bcount[i][0]-0x41+22)%26+0x41);
	} else {
		for(i=0; i<26; i++) {
			printf("'%c':\t%u\t\t'%c%c':\t%u\n",
				ccount[i][0], ccount[i][1],
				bcount[i][0], bcount[i][1], bcount[i][2]);
		}
	}
	return 0;
}

