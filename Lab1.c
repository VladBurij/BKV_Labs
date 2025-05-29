#include <stdio.h>
#include <ctype.h>

int isvowel(int c)
{
	switch(tolower(c))
	{
		case 'a': 
		case 'e':
		case 'i': 
		case 'o': 
		case 'u':
		case 'y': return 1;
	}
	return 0;
}

int main(int argc, char **argv)
{
    FILE *F; 
    int c, letter, string, digit, punct, cons, vowel, space, lower, upper, total=0;
    while ( --argc ) {
        F=fopen(argv[argc],"r");
   	    total=letter=lower=upper=digit=punct=cons=vowel=space=0;
        string=1;
   	    while ( (c=fgetc(F))!=EOF) {
		    total++;
            if (c == '\n') string++;
   	        if (isalpha(c)) letter++;
            if (islower(c)) lower++;
            if (isupper(c)) upper++;
            if (isdigit(c)) digit++;
            if (ispunct(c)) punct++;
            if (isspace(c)) space++;
   	        if (isvowel(c)) vowel++;
   	        else cons++;
   	    }
   	printf("FILE %s: total=%d strings=%d digits=%d puncts=%d space=%d letters=%d lowers=%d uppers=%d vowels=%d, cons=%d\n",argv[argc], total, string, digit, punct, space, letter, lower, vowel, cons);
    }
 	return 0;
}
