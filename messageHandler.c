#include "header.h"

// This is a function which will be continously listening for requests
//  void *listener(void *arg)
// {
//     printf("Listening in listner");
//     pthread_detach(pthread_self());
//     recv(sockfd, buf, sizeof(buf), 0);
//     printf("%s\n", buf);
//     if (buf[0] == '5')
//     {
//         memset(buf, 0, sizeof(buf));
//         char delimeter[] = ":";
//         char *requestNumber = strtok(buf, delimeter);
//         char *cliName = strtok(NULL, delimeter);
//         char *message = strtok(NULL, delimeter);
//         printf("%s wants to make a connection\n", cliName);
//         printf("Press 1 to accept and 0 to reject");
//         char choice;
//         scanf("%c", &choice);
//         if (choice == '5')
//         {
//             sprintf(buf, "1");
//             send(sockfd, buf, sizeof(buf), 0);

//             pthread_t send_thread;
//             pthread_create(&send_thread, NULL, sendMsgHandler, NULL);

//             pthread_t recv_thread;
//             pthread_create(&recv_thread, NULL, recvMsgHandler, (void *)name);
//             pthread_join(send_thread, NULL);
//             pthread_join(recv_thread, NULL);
//         }
//         else if (choice == '2')
//         {
//         }
//         else
//         {
//             memset(&choice, 0, sizeof(choice));
//             // sprintf("&")
//         }
//     }
// }