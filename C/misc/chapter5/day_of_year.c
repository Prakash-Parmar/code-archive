#include<stdio.h>

static char daytab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}	
};

bool isLeapYear(int year){
	return year%4 == 0 && year%100 != 0 || year%400 == 0;
}

int day_of_year(int year, int month, int day){
	int i;
	bool leap = isLeapYear(year);

	for(i=1; i< month; i++) 
		day += daytab[leap][i];
	
	return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday){
	int i;
	int leap = isLeapYear(year);

	for(i = 1; yearday > daytab[leap][i]; i++)
		yearday -= daytab[leap][i];

	*pmonth = i;
	*pday = yearday;	
}

int main(){

	return 0;
}
