#include<stdio.h>
#include <stdlib.h>

char* creverse(char *src){
	char *temp = (char *)malloc(20*sizeof(char));
	int offset = 0;
	
	// find the index of null terminating char
	while(src[offset] != '\0'){
		offset++;
	}
	
	offset--;
	int offsetTemp = 0;
	while(offset>=0){
		temp[offsetTemp++] = src[offset];
		offset--;
	}

	temp[offsetTemp] = '\0';
	
	return temp;

}

int main(){
	
	char *str = "Parmar Prakash";
	printf("Original string: %s\n", str);
	printf("Reversed string: %s\n", creverse(str));
	return 0;
}
