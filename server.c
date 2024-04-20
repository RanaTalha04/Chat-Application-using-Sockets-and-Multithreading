#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define MAX_CLIENTS 5
#define PORT 8080
struct client
{
    char name[100];
    int sockfd;
};
pthread_mutex_t rw_mutex;
pthread_mutex_t mutex;
struct client clifd[MAX_CLIENTS];
int read_count = 0;
char buf[300];
char name[20];
char password[20];
void clearClients()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        clifd[i].sockfd = 0;
    }
}
void printClients()
{
    printf("\n\n----------Printing details\n\n");
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        // if (clifd[i].sockfd == 0)
        // {
        //     continue;
        // }
        printf("%s %d\n", clifd[i].name, clifd[i].sockfd);
    }
}
int searchClient(const char *name)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (!strcmp(clifd[i].name, name))
        {
            return clifd[i].sockfd;
        }
    }
    return 0;
}
void makeConnection(int sockfd, int clifd)
{
    while (1)
    {
        recv(sockfd, buf, sizeof(buf), 0);
        printf("%s", buf);
        send(clifd, buf, sizeof(buf), 0);
    }
}
int auth(char *str)
{
    char delimiter[] = ":";
    char *reqType = strtok(str, delimiter);
    char *temp1 = strtok(NULL, delimiter);
    strcpy(name, temp1);
    temp1 = strtok(NULL, delimiter);
    strcpy(password, temp1);
    FILE *file = fopen("auth.txt", "r");
    char tempName[10], tempPassword[10];
    int flag = 0;
    while (fscanf(file, "%s %s", tempName, tempPassword) == 2)
    {
        if (!strcmp(name, tempName) && !strcmp(password, tempPassword))
        {
            return 1;
        }
    }
    return 0;
}
void *handleClient(void *arg)
{
    printClients();
    pthread_detach(pthread_self());
    int index = *((int *)arg);
    printf("Assigning the client on %d\n", index);
    // Todo: an error is coming it is sending the an incremented index so will fix it later
    index--;
    int sockfd = clifd[index].sockfd;
    printf("Index : %d CLIFD: %d\n", index, sockfd);
    memset(buf, 0, 300);
    int size = recv(sockfd, buf, sizeof(buf), 0);
    printf("Buffer: %d\n", size);
    // login and signup

    if (buf[0] == '1')
    {
        printf("Auth request\n");
        if (buf[1] == '1')
        {

            if (auth(buf))
            {
                sprintf(buf, "%s", "1:You are logged In successfully");
                strcpy(clifd[index].name, name);
                printf("THis is your name: %s\n", clifd[index].name);
                send(sockfd, &buf, sizeof(buf), 0);

                // Here i will write the code for one to one converstation
                // todo: make the function in seprate file to make the code readable
                recv(sockfd, buf, sizeof(buf), 0);
                printf("This is buffer : %s\n", buf);
                if (buf[0] == '1')
                {
                    printf("End to End Chat\n");
                    char delimiter[] = ":";
                    char *reqType = strtok(buf, delimiter);
                    char *cliName = strtok(NULL, delimiter);
                    printf("This is cliname:%s\n", cliName);
                    int clifd = searchClient(cliName);
                    if (searchClient(cliName))
                    {
                        memset(buf, 0, sizeof(buf));
                        sprintf(buf, "1:%s", "Client Found");
                        send(sockfd, buf, strlen(buf), 0);
                        makeConnection(sockfd, clifd);
                    }
                }
                else
                {
                    printf("Invalid request %c\n", buf[0]);
                }
            }
            else
            {
                sprintf(buf, "%s", "0:Invalid Credentials");
                send(sockfd, &buf, sizeof(buf), 0);
            }
        }
        // signup
        else if (buf[1] == '2')
        {
            char delimiter[] = ":";
            char *reqType = strtok(buf, delimiter);
            char *name = strtok(NULL, delimiter);
            char *password = strtok(NULL, delimiter);
            FILE *file = fopen("auth.txt", "a");
            fprintf(file, "%s %s\n", name, password);
            fclose(file);
            sprintf(buf, "%s", "1:You are registered successfully");
            send(sockfd, &buf, sizeof(buf), 0);
        }
        else
        {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s", "0:Invalid Request");
            send(sockfd, &buf, sizeof(buf), 0);
        }
    }

    //! Free the client space
    clifd[index].sockfd = 0;
    pthread_exit(NULL);
}
int main(int argc, char *argv[])
{

    // Port number taken as an argument from the command line
    // if (argc != 2)
    // {
    //      //Error if the port number is not provided
    //     perror("Please provide a port number i.e ./server.c <port number>\n");
    //     exit(EXIT_FAILURE);
    // }
    // int port = atoi(argv[1]);
    int port = PORT;
    // setting up the socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // setting up the socket address

    struct sockaddr_in address;
    address.sin_family = AF_INET;         // Domain
    address.sin_addr.s_addr = INADDR_ANY; // Socket address
    address.sin_port = htons(port);       // Socket Port

    if (sockfd < 0)
    {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }
    listen(sockfd, MAX_CLIENTS);
    pthread_t threads[MAX_CLIENTS];
    clearClients();
    int i = 0;
    while (1)
    {
        // client Socket
        if (clifd[i].sockfd == 0)
        {

            clifd[i].sockfd = accept(sockfd, NULL, NULL);
            printf("clifd[i].sockfd:%d\n", clifd[i].sockfd);
            if (clifd[i].sockfd == -1)
            {
                perror("accept");
                continue;
            }
            int status = pthread_create(&threads[i], NULL, handleClient, (void *)&i);
            if (status)
            {
                perror("Accept Failed");
                continue;
            }
            // send messsage to clients
            // send(clifd, buf, sizeof(buf), 0);
        }
        i++;
        i = i % MAX_CLIENTS;
    }

    return 0;
}