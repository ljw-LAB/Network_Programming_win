#pragma comment(lib, "ws2_32")

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define TTL 64
#define BUFSIZE 30

void ErrorHandling(char* message);

int main(int argc, char** argv)
{
    WSADATA wsaData;
    
    SOCKET hSendSock;
    SOCKADDR_IN multiAddr;
    int multiTTL = TTL;
    int state;
    FILE *fp;
    char buf[BUFSIZE];

    if(argc != 3)
    {
        printf("Usage : %s <Multicast IP> <port> \n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error! \n");
    
    hSendSock = socket(PF_INET, SOCK_DGRAM, 0);
    if(hSendSock == INVALID_SOCKET)
        ErrorHandling("socket() error");
    
    memset(&multiAddr, 0, sizeof(multiAddr));
    multiAddr.sin_family = AF_INET;
    multiAddr.sin_addr.s_addr = inet_addr(argv[1]);
    multiAddr.sin_port = htons(atoi(argv[2]));

    state = setsockopt(hSendSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*) &multiTTL, sizeof(multiTTL));

    if(state == SOCKET_ERROR)
        ErrorHandling("setsockopt() error");
    
    if((fp=fopen("news.txt", "r")) == NULL)
        ErrorHandling("fopen() error");
    
    while(!feof(fp))
    {
        fgets(buf, BUFSIZE, fp);
        sendto(hSendSock, buf, strlen(buf), 0, (SOCKADDR*)&multiAddr, sizeof(multiAddr));
        Sleep(200);
    }
    closesocket(hSendSock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}