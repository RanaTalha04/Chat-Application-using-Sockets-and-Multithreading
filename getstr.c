#include "header.h"
void getstr(char *str, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        int a = getchar();
        if (a == '\n' && i != 0)
        {
            break;
        }
        else
        {
            str[i] = a;
        }
    }
    str[i] = '\0';
}