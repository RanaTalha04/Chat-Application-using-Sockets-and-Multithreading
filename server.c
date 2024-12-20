#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define BUFFER_SIZE 300
#define MAX_CLIENTS 5
#define PORT 8080
struct client
{
    char name[100];
    int sockfd;
};
struct client clifd[MAX_CLIENTS];
int read_count = 0;
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
char *makeFilename(const char *name, const char *cliName)
{
    printf("\n\n\nThis is name:%s\tThis is cliName:%s\n\n\n", name, cliName);
    char *fileName = malloc(strlen(name) + strlen(cliName) + 5);
    int i = 0, j = 0;
    while (name[i] != '\0')
    {
        fileName[i] = name[i];
        i++;
    }
    fileName[i] = ',';
    i++;
    while (cliName[j] != '\0')
    {
        fileName[i] = cliName[j];

        i++;
        j++;
    }
    fileName[i] = '.';
    i++;
    fileName[i] = 't';
    i++;
    fileName[i] = 'x';
    i++;
    fileName[i] = 't';
    i++;
    fileName[i] = '\0';
    printf("This is fileName in the filename function:%s\n", fileName);
    return fileName;
}
int saveMsg(const char *name, const char *cliName, const char *msg)
{
    printf("\n\nThis is name %s\nThis is clientName %s\nThis is message%s\n", name, cliName, msg);
    char *filename = makeFilename(name, cliName);
    printf("This is file Name:%s\n", filename);
    FILE *file = fopen(filename, "a");
    fprintf(file, "%s:%s\n", name, msg);
    fclose(file);
    return 1;
}
void makeConnection(int sockfd, int clifd, char *buf)
{
    while (1)
    {
        recv(sockfd, buf, BUFFER_SIZE, 0);
        printf("Message recived :%s", buf);
        if (!strcmp(buf, "exit"))
        {
            send(sockfd, buf, BUFFER_SIZE, 0);
            break;
        }
        send(clifd, buf, BUFFER_SIZE, 0);
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
    char buf[300];
    char clientName[20];
    pthread_detach(pthread_self());
    int index = *((int *)arg);
    printf("Assigning the client on %d\n", index);
    index--;
    int sockfd = clifd[index].sockfd;
    printf("Index : %d CLIFD: %d\n", index, sockfd);
    do
    {
        memset(buf, 0, 300);
        int size = recv(sockfd, buf, BUFFER_SIZE, 0);

        if (buf[0] == '1')
        {
            printf("Auth request\n");
            if (buf[1] == '1')
            {

                if (auth(buf))
                {
                    sprintf(buf, "%s", "1:You are logged In successfully");
                    strcpy(clifd[index].name, name);
                    strcpy(clientName, clifd[index].name);
                    printf("THis is your name: %s\n", clifd[index].name);
                    send(sockfd, &buf, BUFFER_SIZE, 0);
                    recv(sockfd, buf, BUFFER_SIZE, 0);
                    printf("This is buffer : %s\n", buf);

                    if (buf[0] == '1')
                    {
                        printf("End to End Chat\n");
                        char delimiter[] = ":";
                        char *temp = strtok(buf, delimiter);
                        temp = strtok(NULL, delimiter);
                        char cliName[20];
                        strcpy(cliName, temp);
                        printf("This is cliname:%s\n", cliName);
                        int clifd = searchClient(cliName);
                        printf("THis is clifd%d\n", clifd);

                        if (clifd)
                        {
                            printf("\n\n\n\nCLIENT FOUND\n\n\n\n");
                            memset(buf, 0, BUFFER_SIZE);
                            sprintf(buf, "1:%s", "Client Found");
                            printf("Sending \"%d\"this message :%s-\n", sockfd, buf);
                            send(sockfd, buf, strlen(buf), 0);
                            printClients();
                            makeConnection(sockfd, clifd, buf);
                        }
                        // if the client is not online
                        else
                        {
                            sprintf(buf, "0:Client not found : \"%s\"", clientName);
                            send(sockfd, buf, BUFFER_SIZE, 0);
                            do
                            {
                                recv(sockfd, buf, BUFFER_SIZE, 0);
                                saveMsg(clientName, cliName, buf);
                                printf("\n\n\nThis is clientName:%s\nThis is cliName:%s\nThis is buffer:%s\n\n\n", clientName, cliName, buf);
                            } while (strcmp(buf, "exit"));
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
                    send(sockfd, &buf, BUFFER_SIZE, 0);
                }
            }
            // signup
            else if (buf[1] == '2')
            {
                // 1:1:talha:1122

                char delimiter[] = ":";
                char *reqType = strtok(buf, delimiter);
                char *name = strtok(NULL, delimiter);
                char *password = strtok(NULL, delimiter);
                FILE *file = fopen("auth.txt", "a");
                fprintf(file, "%s %s\n", name, password);
                fclose(file);
                sprintf(buf, "%s", "1:You are registered successfully");
                send(sockfd, &buf, BUFFER_SIZE, 0);
            }
            else
            {
                memset(buf, 0, BUFFER_SIZE);
                sprintf(buf, "%s", "0:Invalid Request");
                send(sockfd, &buf, BUFFER_SIZE, 0);
            }
        }
    } while (buf[0] != '0');

    //! Free the client space
    clifd[index].sockfd = 0;
    memset(clifd[index].name, 0, sizeof(clifd[index].name));

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
            // send(clifd, buf, BUFFER_SIZE , 0);
        }
        i++;
        i = i % MAX_CLIENTS;
    }

    return 0;
}