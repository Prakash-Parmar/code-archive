#include<stdio.h>


void strcopy(char *s, char *t){
	int i=0;

	s = t;

	printf("%s\n", s);
}

int main(){
	
	char name[] = "Prakash";
	char name2[8];

	strcopy(&name2, &name);
	printf("%d\n", '\0');
	return 0;
}
