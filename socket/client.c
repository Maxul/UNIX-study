#include "socket.h"

int main()
{
    int client_sockfd;
    ssize_t data_length;

    struct sockaddr_in server_addr;

    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];

    if ((client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(client_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        return EXIT_FAILURE;
    }

    printf("connect with destination host...\n");

    while (1)
    {
        printf("Input your word:> ");
        scanf("%s", sendbuf);

        send(client_sockfd, sendbuf, strlen(sendbuf), 0);
        if (strcmp(sendbuf, "quit") == 0)
            break;
        data_length = recv(client_sockfd, recvbuf, BUFFER_SIZE, 0);
        recvbuf[data_length] = '\0';
        printf("recv data is: %s\n", recvbuf);
    }

    close(client_sockfd);
    return 0;
}

