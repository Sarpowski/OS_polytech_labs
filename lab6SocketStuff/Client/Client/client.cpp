#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT "40842"
#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {
    SOCKET clientSock = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    char buffer[BUFFER_SIZE];
    int iResult;

    if (argc < 2) {
        printf("Usage: %s hostname\n", argv[0]);
        return 1;
    }

    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(argv[1], PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        clientSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (clientSock == INVALID_SOCKET) {
            printf("Socket creation failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        // Connect to server
        iResult = connect(clientSock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(clientSock);
            clientSock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (clientSock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    printf("Connected to server!\n");

    const char* sendMessage = "Hello from client!";
    iResult = send(clientSock, sendMessage, (int)strlen(sendMessage), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(clientSock);
        WSACleanup();
        return 1;
    }

    ZeroMemory(buffer, BUFFER_SIZE);
    iResult = recv(clientSock, buffer, BUFFER_SIZE - 1, 0);
    if (iResult > 0) {
        buffer[iResult] = '\0';  
        printf("Received from server: %s\n", buffer);
    }
    else if (iResult == 0) {
        printf("Connection closed by server\n");
    }
    else {
        printf("recv failed with error: %d\n", WSAGetLastError());
    }

    // Cleanup
    closesocket(clientSock);
    WSACleanup();

    return 0;
}