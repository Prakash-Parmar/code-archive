#include<stdio.h>
#include<stdlib.h>

int main(){
	FILE *file_to_read = fopen("test.txt", "r");
	FILE *file_to_write = fopen("test2.txt", "w");

	char c;
	while((c=fgetc(file_to_read)) != EOF){
		if(c == '.')
				c = '!';
		fputc(c, file_to_write);
	}

	fclose(file_to_read);
	fclose(file_to_write);

	return 1;

}
