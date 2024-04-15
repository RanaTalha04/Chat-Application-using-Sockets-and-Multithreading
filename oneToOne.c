#include "header.h"
pthread_mutex_t mutex;
void *sendMsgHandler(void *arg)
{
    while (1)
    {
        scanf("%s", buf);
        send(sockfd, buf, sizeof(buf), 0);
    }
}
void *recvMsgHandler(void *arg)
{
    char *clientName = (char *)arg;
    while (1)
    {
        recv(sockfd, buf, sizeof(buf), 0);
        printf("%s:%s\n", clientName, buf);
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
            pthread_t send_thread;
            pthread_create(&send_thread, NULL, sendMsgHandler, NULL);

            pthread_t recv_thread;
            pthread_create(&recv_thread, NULL, recvMsgHandler, (void *)cliName);
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