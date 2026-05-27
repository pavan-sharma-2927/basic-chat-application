// client.c
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888
#define BUFFER_SIZE 1024

SOCKET sock;

char client_name[] = "Ramu Kaka";
char server_name[] = "Doerimon";

// Thread for receiving messages
DWORD WINAPI receive_msg(LPVOID arg)
{
    char buffer[BUFFER_SIZE];

    while (1)
    {
        int recv_size = recv(sock, buffer, BUFFER_SIZE - 1, 0);

        if (recv_size <= 0)
        {
            printf("\n%s disconnected.\n", server_name);
            break;
        }

        buffer[recv_size] = '\0';

        printf("\n%s: %s", server_name, buffer);
        printf("%s: ", client_name);
        fflush(stdout);
    }

    return 0;
}

int main()
{
    WSADATA wsa;

    struct sockaddr_in server;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Connected to %s\n\n", server_name);

    CreateThread(NULL, 0, receive_msg, NULL, 0, NULL);

    char message[BUFFER_SIZE];

    while (1)
    {
        printf("%s: ", client_name);

        fgets(message, BUFFER_SIZE, stdin);

        send(sock, message, strlen(message), 0);
    }

    closesocket(sock);

    WSACleanup();

    return 0;
}
