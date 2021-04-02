#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(void)
{
    WSADATA wsaData;
    short hostOrdPort = 0x1234;
    short netOrdPort;

    long hostOrdPAdd = 0x12345678;
    long netOrdAdd;

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");
    
    netOrdPort = htons(hostOrdPort);
    netOrdAdd = htonl(hostOrdPAdd);

    printf("Host ordered port : %x \n", hostOrdPort);
    printf("Network ordered port : %x \n", netOrdPort);

    printf("Host ordered Address : %x \n", hostOrdPAdd);
    printf("Network ordered Address : %x \n", netOrdAdd);

    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}