#include<stdio.h>
#include<ctype.h>

int getch(void);
void ungetch(int);

int getint(int *pn){
	int c, sign;
	
	while(isspace(c = getch()))
		;
	
	if(!isdigit(c) && c != EOF && c != '+' && c != '-'){
		ungetch(c);
		return 0;
	}
	
	sign = (c == '-') ? -1 : 1;
	if(c == '+' || c == '-'){
		c = getch();
	}
	
	if(!isdigit(c)){
		ungetch(c);
		return 0;
	}

	for(*pn = 0; isdigit(c); c = getch()){
		*pn = 10 * *pn + (c - '0');
	}
	*pn *= sign;
	
	if(c != EOF){
		ungetch(c);
	}

	return c;
}


char buf[10];
int bufp = 0;

int getch(void){
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch (int c ){
	if(bufp >= 10){
		printf("ungetchL too many characters");
	}else{
		buf[bufp++] = c;
	}
}

int main(){

	int n, array[1000];
	
	for(n = 0; n < 1000 && getint(&array[n]) != EOF; n++){
		printf("%d ", array[n]);
	}

	int i = 0;
	for(i; i<n; i++){
		printf("%d\n", array[i]);
	}

	return 0;
}
