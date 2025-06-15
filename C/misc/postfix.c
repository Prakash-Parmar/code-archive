#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100    /* max size of operand or operator */
#define NUMBER '0'   /* signal that a number was found */

double val[MAXOP];   /* value stack */
int sp = 0;          /* next free stack position */

/* function declarations */
int getop(char s[]);
void push(double f);
double pop(void);
int getch(void);
void ungetch(int c);

int main() {
    int type;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                {
                    double op2 = pop();
                    push(pop() - op2);
                }
                break;
            case '/':
                {
                    double op2 = pop();
                    if (op2 != 0.0)
                        push(pop() / op2);
                    else
                        printf("error: zero divisor\n");
                }
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }
    return 0;
}

/* push: push f onto the value stack */
void push(double f) {
    if (sp < MAXOP) {
        val[sp++] = f;
    } else {
        printf("error: stack full, can't push %g\n", f);
    }
}

/* pop: pop and return top value from stack */
double pop(void) {
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack is empty\n");
        return 0.0;  /* return a default value */
    }
}

/* getop: get next character or numeric operand */
int getop(char s[]) {
    int i, c;
    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';
    if (!isdigit(c) && c != '.')
        return c; /* not a number */
    i = 0;
    if (isdigit(c)) /* collect integer part */
        while (isdigit(s[++i] = c = getch()))
            ;
    if (c == '.') /* collect fraction part */
        while (isdigit(s[++i] = c = getch()))
            ;
    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

int getch(void) {
    // Implement this function based on your requirements.
    return getchar(); // A simple implementation for example.
}

void ungetch(int c) {
    // Implement this function based on your requirements.
    // For example, you could use an array to buffer characters.
}

