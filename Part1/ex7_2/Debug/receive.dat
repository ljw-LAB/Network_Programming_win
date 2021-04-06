#pragma comment(lib, "ws2_32")
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 30

void ErrorHandling(char* message);

int main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET hServsock;
    SOCKET hClntsock;
    char buf[BUFSIZE];

    FILE* fp;
    SOCKADDR_IN servAddr;
    SOCKADDR_IN clntAddr;
    int clntAddrSize;
    int len;

    if(argc!=2)
    {
        printf("Usage : %s <PORT> \n", argv[0]);
        exit(1);
    }

    fp = fopen("file_server_win.c", "r");
    if(fp == NULL)
        ErrorHandling("File open error");
    
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error");

    hServsock =socket(PF_INET, SOCK_STREAM, 0);
    if(hServsock == -1)
        ErrorHandling("socket() error");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(hServsock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");
    
    if(listen(hServsock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");

    clntAddrSize = sizeof(clntAddr);
    hClntsock = accept(hServsock, (SOCKADDR*)&clntAddr, &clntAddrSize);
    if(hClntsock == INVALID_SOCKET)
        ErrorHandling("accept() error");

    while(1)
    {
        len = fread(buf, sizeof(char), BUFSIZE, fp);
        send(hClntsock, buf, len, 0);
        if(feof(fp)) break;
    }
    if(shutdown(hClntsock, SD_SEND) == SOCKET_ERROR)
        ErrorHandling("shutdown error");
    
    len = recv(hClntsock, buf, BUFSIZE-1, 0);
    buf[len] =0;
    fputs(buf, stdout);

    fclose(fp);
    closesocket(hClntsock);
    WSACleanup();
    return 0;
}
void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}