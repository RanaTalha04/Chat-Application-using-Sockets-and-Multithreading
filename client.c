#include "header.h"
char buf[300];
int main(int argc, char **argv)
{
    makeConnection();
    if (!login())
    {
        printf("Log in Failed\n");
        return 1;
    }
    char choice;
    printf("\n\n This is your name %s\n\n", password);
    do
    {
        printf("How do you want to continue\n");
        printf("Press 1 for one To one Conversation\n");
        printf("Press 2 to make a group\n");
        printf("Press 0 to exit\n");
        printf("Enter you choice: ");
        scanf("%c", &choice);
        printf("THIS IS CHOICE %c-\n", choice);
        if (choice == '1')
        {
            oneToOne();
        }

    } while (choice != '0');
}
