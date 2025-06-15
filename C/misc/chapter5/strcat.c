#include<stdio.h>


char *strcat(char *s, char *t){
	
	// Store the original pointer to 's' to return it at the end
	char *res = s;
	
	// Move 's' to the end of the first string
	while(*s != '\0')
		s++;

	// Copy each char from 't' to 's'
	while(*t != '\0')
		*s++ = *t++;

	*s = *t;
	
	// return the original pointer to 's' 
	return res;
}


int main(){
	
	char fname[20] = "Prakash";
	char lname[] = " Parmar";
	char *fullName = strcat(fname, lname);
	printf("%s\n", fullName); 
	return 0;
}
