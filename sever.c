// server.c
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888
#define BUFFER_SIZE 1024

SOCKET client_socket;

char server_name[] = "Doerimon";
char client_name[] = "Ramu Kaka";

// Thread for receiving messages
DWORD WINAPI receive_msg(LPVOID arg)
{
    char buffer[BUFFER_SIZE];

    while (1)
    {
        int recv_size = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (recv_size <= 0)
        {
            printf("\n%s disconnected.\n", client_name);
            break;
        }

        buffer[recv_size] = '\0';

        printf("\n%s: %s", client_name, buffer);
        printf("%s: ", server_name);
        fflush(stdout);
    }

    return 0;
}

int main()
{
    WSADATA wsa;
    SOCKET server_socket;

    struct sockaddr_in server, client;
    int client_len = sizeof(client);

    WSAStartup(MAKEWORD(2, 2), &wsa);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr*)&server, sizeof(server));

    listen(server_socket, 1);

    printf("Waiting for client...\n");

    client_socket = accept(server_socket, (struct sockaddr*)&client, &client_len);

    printf("%s connected.\n\n", client_name);

    CreateThread(NULL, 0, receive_msg, NULL, 0, NULL);

    char message[BUFFER_SIZE];

    while (1)
    {
        printf("%s: ", server_name);
        fgets(message, BUFFER_SIZE, stdin);

        send(client_socket, message, strlen(message), 0);
    }

    closesocket(client_socket);
    closesocket(server_socket);

    WSACleanup();

    return 0;
}
