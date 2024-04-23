#ifndef FUNCTION_H
#define FUNCTION_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

// Global Variables
extern char name[20];
extern char password[20];
extern char buf[300];
extern struct sockaddr_in server_addr;
extern int sockfd;
extern int port;

int login();
void makeConnection();
void oneToOne();

void getstr(char *str, int len);
#endif