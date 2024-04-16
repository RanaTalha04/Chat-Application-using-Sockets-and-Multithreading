#include "header.h"
pthread_mutex_t mutex;
int quit = 0;
void move_cursor_up()
{
    printf("\033[1A"); // ANSI escape code to move cursor up one line
}
void *sendMsgHandler(void *arg)
{
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        move_cursor_up();
        printf("%s: %s", name, buf);
        // ! will return 0 in true
        send(sockfd, buf, sizeof(buf), 0);
    }
}
void *recvMsgHandler(void *arg)
{
    char *clientName = (char *)arg;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int size = recv(sockfd, buf, sizeof(buf), 0);
        if (size == 0)
        {
            continue;
        }
        printf("%s:%s", clientName, buf);
    }
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
    printf("%s\n", buf);
    if (buf[0] == '1')
    {
        // todo: here will be the code of client found
        while (1)
        {
            sleep(5);
            pthread_t send_thread;
            pthread_create(&send_thread, NULL, sendMsgHandler, NULL);

            pthread_t recv_thread;
            if (pthread_create(&recv_thread, NULL, recvMsgHandler, (void *)cliName) < 0)
            {
                printf("There is some error in recvMsgHandler\n");
            }
            pthread_join(send_thread, NULL);
            pthread_join(recv_thread, NULL);
        }
    }
    else
    {
        printf("Unable to find the client");
        // todo: save the message if for the client if not on the server
    }
}