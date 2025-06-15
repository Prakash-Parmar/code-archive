#include <stdio.h>

double myFunc1();     // Function declaration with an empty parameter list
double myFunc2(void); // Function declaration with `void` in parameter list

int main() {
    double result1 = myFunc1(10);   // This might compile (though it shouldn't in modern C)
    double result2 = myFunc2(10);   // This will cause a compiler error in modern C
    return 0;
}

double myFunc1() {
    return 3.14;
}

double myFunc2(void) {
    return 2.71;
}

