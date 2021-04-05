#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 1024
void ErrorHandling(char* message);

int main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET hSocket;
    char message[BUFSIZE];
    int strLen;

    SOCKADDR_IN servAddr;

    if(argc!=3)
    {
        printf("Usage : %s <IP> <PORT> ", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        ErrorHandling("WSAStartuo error()");
    
    hSocket = socket(PF_INET, SOCK_STREAM, 0);
    
    if(hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");
    
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("connect() error");

    while(1)
    {
        fputs("������ �޽����� �Է��ϼ��� (q to quit) : ", stdout);
        fgets(message, BUFSIZE, stdin);

        if(!strcmp(message,"q\n"))
            break;
        send(hSocket, message, strlen(message), 0);

        strLen = recv(hSocket, message, BUFSIZE-1, 0);
        message[strLen] = 0;
        printf("�����κ��� ���۵� �޽��� : %s \n", message);
    }

    closesocket(hSocket);
    WSACleanup();
    return 0;
}


void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}