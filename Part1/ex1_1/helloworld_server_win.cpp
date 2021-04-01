#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET hservSock;
    SOCKET hClntSock;
    SOCKADDR_IN servAddr;
    SOCKADDR_IN clntAddr;

    int szClntAddr;
    char message[] = "Hello World! \n";

    if(argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");
    
    hservSock = socket(PF_INET, SOCK_STREAM, 0);
    if(hservSock == INVALID_SOCKET)
        ErrorHandling("socket() error!");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(hservSock, (SOCKADDR*) &servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error!");
    if(listen(hservSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error!");

    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hservSock, (SOCKADDR*)&clntAddr, &szClntAddr);
    if(hClntSock == INVALID_SOCKET)
        ErrorHandling("accept() error!");

    send(hClntSock, message, sizeof(message), 0);   
    closesocket(hClntSock);    
    WSACleanup();

    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}