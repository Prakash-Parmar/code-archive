
#include <stdio.h>

#define MAXLEN 1000 /* maximum input line length */

/* functions */
int  getLine(char [], int);

/* getLine function: read a line into s, return length */
int getLine(char s[], int lim)
{
	int c, i;

	for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
		s[i] = c;
	if (c == '\n')
		s[i++] = c;
	s[i] = '\0';
	return i;
}

int rightmost(char s[], char t[]){
	int right = -1;

	for(int i=0; s[i] != '\0' ; i++){
		for(int j=i; s[j] != '\0'; j++){
			int start = j;
			int k = 0;
			for(k=0; t[k]==s[j] && t[k]!='\0'; k++, j++){
				
			}
			if(t[k]=='\0'){
				right = start;
			}
		}
	}
	

	return right;
}


int main(void)
{
	char line[MAXLEN], modLine[MAXLEN];

	while (getLine(line, MAXLEN) > 0) {	
		int right = rightmost(line, "pra");
		printf("%d\n", right);
	}
	return 0;
}
