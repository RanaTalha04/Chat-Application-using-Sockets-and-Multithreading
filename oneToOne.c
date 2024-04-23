#include "header.h"
int count;
int exitFlag = 0;
void move_cursor_up()
{
    printf("\033[1A"); // ANSI escape code to move cursor up one line
}
void *sendMsgHandler(void *arg)
{
    while (1)
    {
        memset(buf, sizeof(buf), 0);
        getstr(buf, sizeof(buf));
        move_cursor_up();
        printf("You: %s\n", buf);
        send(sockfd, buf, sizeof(buf), 0);
        if (!strcmp(buf, "exit"))
        {
            exitFlag = 1;
            break;
        }
    }
    pthread_exit(NULL);
}
void *recvMsgHandler(void *arg)
{
    char *clientName = (char *)arg;
    while (1)
    {
        if (exitFlag)
        {
            break;
        }
        recv(sockfd, buf, sizeof(buf), 0);
        printf("%s:%s\n", clientName, buf);
    }
    pthread_exit(NULL);
}
void oneToOne()
{
    char cliName[20];
    printf("Enter the name of client you want to make connections:");
    scanf("%s", cliName);
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "1:%s", cliName);
    send(sockfd, &buf, sizeof(buf), 0);
    recv(sockfd, buf, sizeof(buf), 0);
    system("clear");
    if (buf[0] == '1')
    {
        // todo: here will be the code of client found

        pthread_t send_thread;
        pthread_create(&send_thread, NULL, sendMsgHandler, NULL);
        pthread_t recv_thread;
        pthread_create(&recv_thread, NULL, recvMsgHandler, (void *)cliName);
        pthread_join(send_thread, NULL);
        pthread_join(recv_thread, NULL);
    }
    else
    {
        printf("Unable to find the client\n");
        // todo: save the message if for the client if not on the server
    }
}