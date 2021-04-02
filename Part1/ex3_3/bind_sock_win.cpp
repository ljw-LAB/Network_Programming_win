#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(int argc, char** argv)
{
    WSADATA wsaData;

    SOCKET hservsock;
    char* servIP = "127.0.0.1";
    char* servPort = "9190";
    SOCKADDR_IN servAddr;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error! \n");
    
    hservsock = socket(PF_INET, SOCK_STREAM, 0);
    if(hservsock == INVALID_SOCKET)
        ErrorHandling("socket() error! \n");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIP);
    servAddr.sin_port = htons(atoi(servPort));

    if(bind(hservsock, (SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");
    
    printf("파일 디스크립터 %d의 소켓에 주소 할당까지 완료 ! \n\n", hservsock);
    WSACleanup();

    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}