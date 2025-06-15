#include<stdio.h>
#include<ctype.h>

int buf[10];
int bufp = 0;

int getch(void){
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c){
	if(bufp >= 10)
		printf("ungetch: too many character");
	else
		buf[bufp++] = c;
}

float getfloat(float *pn){
	int c, sign;
	
	// Skip all the white space
	while(isspace(c = getch()))
		;
	
	if (!isdigit(c) && c != EOF && c != '+' && c != '-'){
		ungetch(c);
		return 0;
	}

	sign = (c == '-') ? -1 : 1;

	if(c == '+' || c == '-')
		c = getch();

	for(*pn = 0; isdigit(c) ; c = getch())
		*pn = *pn * 10 + (c - '0');

	if(c == '.'){
		c = getch();
	}
	float div = 1;
	for(*pn; isdigit(c); c = getch()){
		*pn = *pn * 10 + (c - '0');
		div *= 10;
	}

	*pn = *pn / div;
	*pn = *pn * sign;

	if(c != EOF){
		ungetch(c);
	}

	return c;
}

int main(){
	
	int n;
	float array[100];

	for(n  = 0; n < 100 && getfloat(&array[n]) != EOF; n++)
		printf("%f\n", array[n]);	

	return 0;
}
