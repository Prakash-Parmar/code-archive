#include<stdio.h>
#include <string.h>

#define MAXLINES 5/* max #lines to be sorted */

char *lineptr[MAXLINES]; /* pointers to text lines */
int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

#define MAXLEN 1000 

int getl(char *, int);
char *alloc(int);

#define ALLOCSIZE 10000 /* size of available space */
static char allocbuf[ALLOCSIZE]; /* storage for alloc */
static char *allocp = allocbuf; /* next free position */

char *alloc(int n) /* return pointer to n characters */
{
	if (allocbuf + ALLOCSIZE - allocp >= n) { 
		allocp += n;
		return allocp - n; /* old p */
	} else /* not enough room */
		/* it fits */
		return 0;
}


void afree(char *p) /* free storage pointed to by p */
{
	if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
	allocp = p;
}

int getl(char s[], int lim){
	int c, i;
	
	for(i = 0; i < lim - 1 && (c=getchar()) != EOF && c != '\n'; ++i)
		s[i] = c;
	if(c == '\n'){
		s[i] = c;
             	++i;
	}
	s[i] = '\0';
	return i;
}

int readlines(char *lineptr[], int maxlines){

	int len, nlines;
	char *p, line[MAXLEN];
	
	nlines = 0;
	while((len = getl(line, MAXLEN)) > 0){
		printf("%d\n", nlines);
		if(nlines >= maxlines || (p = alloc(len)) == NULL)
			return -1;
		else{
			line[len-1] = '\0';
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	}
	return nlines;
}

/* writelines: write output lines */
void writelines(char *lineptr[], int nlines)
{
	int i;
	for (i = 0; i < nlines; i++)
		printf("%s\n", lineptr[i]);
}

int main(){
	int nlines;
	nlines = readlines(lineptr, MAXLINES);
	printf("%d\n", nlines);
	writelines(lineptr, nlines);
	return 0;
}
