#include<stdio.h>

#define FIRST(a) a
#define JOIN(a, b) FIRST(a ## b)

int main() {
    int xy = 42;
    printf("%d\n", JOIN(x, y));  // This expands to FIRST(xy), which should print 42.
    return 0;
}

