#include<stdio.h>
int main()
{
    char str[30];
    gets(str);
    printf("Using printf%s:\n",str); 
    printf("Using put:");
    puts(str);
    return 0;
}
