#include "socket.h"

int main()
{
    int server_sockfd;
    int client_sockfd;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE];
    ssize_t data_length;

    if ( (server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    int on = 1;
    if ( (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) <0 )
    {
        perror("setsockopt");
        return EXIT_FAILURE;
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if ( bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
    {
        perror("bind");
        return EXIT_FAILURE;
    }

    if ( listen(server_sockfd, 5) < 0 )
    {
        perror("listen");
        return EXIT_FAILURE;
    }

    while (1)
    {
        printf("Listening on port: %d\n", SERVER_PORT);

        client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&addr_len);
        if ( client_sockfd < 0 )
        {
            perror("accept");
            continue;
        }
        printf("\nrecv client data...\n");
        printf("IP is %s\n", inet_ntoa(client_addr.sin_addr));
        printf("Port is %d\n", htons(client_addr.sin_port));
        while (1)
        {
            data_length = recv(client_sockfd, buffer, BUFFER_SIZE, 0);
            if (data_length < 0)
            {
                perror("recv");
                continue;
            }
            buffer[data_length] = '\0';
            if (strcmp(buffer, "quit") == 0)
                break;//goto out;
            printf("len: %ld\trecv data is %s\n", data_length, buffer);
            send(client_sockfd, buffer, data_length, 0);
        }
    }
out:
    close(client_sockfd);
    close(server_sockfd);
    return 0;
}

