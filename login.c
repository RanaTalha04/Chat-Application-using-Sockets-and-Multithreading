#include "header.h"
char name[20];
char password[20];
int login()
{
    int choice;
    int flag = 0;
    do
    {

        printf("How do you want to continue?\n");
        printf("Press 1 for login and 2 for signup");
        scanf("%d", &choice);
        if (choice == 1)
        {
            char name[10], password[10];
            printf("Enter your name: ");
            scanf("%s", name);
            printf("Enter your password: ");
            scanf("%s", password);
            //! auth=1 and login=0 signup = 2
            sprintf(buf, "%d%d:%s:%s", 1, 1, name, password);

            send(sockfd, &buf, sizeof(buf), 0);

            memset(buf, 0, sizeof(buf));
            recv(sockfd, &buf, sizeof(buf), 0);
            if (buf[0] == '0')
            {
                return 0;
            }
            return 1;
        }
        else
        {
            char name[10], password[10];
            printf("Enter your name: ");
            scanf("%s", name);
            printf("Enter your password: ");
            scanf("%s", password);
            //! auth=1 and login=0 signup = 2
            sprintf(buf, "%d%d:%s:%s", 1, 2, name, password);

            send(sockfd, &buf, sizeof(buf), 0);

            memset(buf, 0, sizeof(buf));
            recv(sockfd, &buf, sizeof(buf), 0);
            printf("%s\n", buf);
        }
    } while (!flag);
}