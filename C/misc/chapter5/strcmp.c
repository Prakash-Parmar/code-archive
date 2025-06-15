#include<stdio.h>


/* strcmp: return <0 if s<t, 0 if s==t and >0 if s>t */
int strcmp_array(char *s, char *t){
	int i;

	for(i = 0; s[i] == t[i]; i++)
		if(s[i] == '\0')
			return 0;
	return s[i] - t[i];
}

// pointer version of strcmp
int strcmp_pointer(char *s, char *t){
	while(*s == *t){
		if(*s == '\0')
			return 0;
		s++;
		t++;
	}

	return *s - *t;

}

int main(){
	
	char *s = "Pra";
	char *t = "Pral";
	printf("%d \n", strcmp_pointer(s, t));
	return 0;
}
