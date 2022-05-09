#include <stdio.h>
#include <stdlib.h>
#include "shunt.h"

int main(void)
{
    char *buffer;
    char *postfix;
    size_t count;
    buffer = NULL;
    printf("Enter infix expression. +, -, *, /, ^, and () are allowed.\n");
    getline(&buffer, &count, stdin);
    postfix = (char*)malloc(sizeof(char) * count);
    if(postfix == NULL)
    {
        printf("cannot allocate string\n");
        return -1;
    }
    if(c_infix2postfix(buffer, postfix, count) < 0)
    {
        printf("Failed\n");
        fflush(stdout);
        free((void*)buffer);
        free((void*)postfix);
        return -1;
    }
    printf("Infix: %s\nPostfix: %s\n", buffer, postfix);
    fflush(stdout);
    free((void*)buffer);
    free((void*)postfix);
    return 0;
}
    
    
