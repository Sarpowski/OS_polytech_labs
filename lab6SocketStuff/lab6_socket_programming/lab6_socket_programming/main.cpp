#include <winsock2.h>  
#include <stdio.h>      
#include <string.h>     

#define PORT 40842      
#define BUFFER_SIZE 256  

int main(int argc, char* argv[]) {
    SOCKET serverSock, clientSock;                       //  Объявите дескрипторы сокетов сервера и клиента
    struct sockaddr_in serverAddr, clientAddr;           // Определите структуры адресов сервера и клиента
    char buffer[BUFFER_SIZE];                            // Буфер для хранения полученных данных
    int addrLen = sizeof(clientAddr);                    // Длина структуры адреса клиента

    WSADATA wsaData;                                     
    // Initialize Winsock library
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    // Создайте сокет для сервера
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET) {
        printf("Socket creation failed with error: %d\n", WSAGetLastError());
        WSACleanup();  
        return 1;
    }

    serverAddr.sin_family = AF_INET;                   // IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;            
    serverAddr.sin_port = htons(PORT);                 

    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(serverSock);  
        WSACleanup();            
        return 1;
    }

    // Слушайте входящие соединения (максимум 1 ожидающее соединение)
    if (listen(serverSock, 1) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(serverSock);  
        WSACleanup();             
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept an incoming connection from a client
    clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSock == INVALID_SOCKET) {
        printf("Accept failed with error: %d\n", WSAGetLastError());
        closesocket(serverSock);  
        WSACleanup();             
        return 1;
    }

    printf("Client connected!\n");

    // Отправить сообщение 
    const char* message = "Hello from server!";
    send(clientSock, message, strlen(message), 0);

    // Receive data from the client
    memset(buffer, 0, BUFFER_SIZE);                    
    if (recv(clientSock, buffer, BUFFER_SIZE, 0) > 0) { 
        printf("Received from client: %s\n", buffer);  
    }

    // Close the client socket
    closesocket(clientSock);
    // Close the server socket
    closesocket(serverSock);
    // Clean up Winsock library
    WSACleanup();

    return 0;
}