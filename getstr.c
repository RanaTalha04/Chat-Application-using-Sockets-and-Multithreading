#include "header.h"
void getstr(char *str, int len)
{
    int c;
    // while (c == '\n')
    // {
    //     // Discard the character
    // }
    int i;
    for (i = 0; i < len; i++)
    {
        int a = getchar();
        if ((int)a == 10)
        {
            continue;
        }
        else if (a == '.')
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