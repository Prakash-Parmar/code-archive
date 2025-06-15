#include <stdio.h>

#define TABSTOP=8
#define MAXLINE=100

int getline(char line[]);
int entab(char line[], char result[]);

int main(){
	
	char line[MAXLINE];
	char result[MAXLINE];
	
	while(getline(line) > 0){
			entab(line, result);
			printf("%s, %d", result, entab);
	}
	
	return 0;
}

// function to read line
int getline(char line[]){
	int index = 0;
	int c;

	while(index < MAXLINE - 1 && (c = getchar()) != EOF && c != '\n'){
		s[index] = c;
		index++;
	}

	if(c == '\n'){
		s[index] = c;
		index++;
	}

	s[index] = '\0';
	
	return index;

}

// function to replace blank space by space and tab
int entab(char line[], char result[]){
	int indexL = 0;
	int indexR = 0;
	while(line[indexL] != '\0'){
		if(line[indexL]==' '){
			int start = indexL;
			int end = indexL;
			while(line[indexL+1] != '\0' && line[indexL+1] == ' '){
				indexL++;
			}
			end = indexL;
			int nextStop = (TABSTOP	% start);
		}else{
			result[indexR] = line[indexL];
			indexR++;
			indexL++;
		}
	}

	result[indexR] = '\0';
	return indexR+1;
	
}
