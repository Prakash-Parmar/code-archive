#include<stdio.h>
#include<string.h>

int strend(char *s, char *t){
	
	int sizeS = strlen(s);
	int sizeT = strlen(t);

	if(sizeS < sizeT)
		return 0;
	
	int cnt = sizeS - sizeT;
	while(cnt != 0){
		s++;
		cnt--;
	}
	
	
	while(*s++ == *t++)
		if(*s == '\0')
			return 1;
	

	return 0;

}

int main(int argc, char *argv[]){

	printf("%d\n", strend(argv[1], argv[2]));
	return 0;
}
