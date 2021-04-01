#pragma comment(lib, "ws2_32")
//#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ErroHandling(char* message);

int main(int argc, char* argv)
{
    WSADATA wsaData;
    SOCKET hTCPSock;
    SOCKET hUDPSock;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErroHandling("WSAStartup() Error");

    hTCPSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(hTCPSock == INVALID_SOCKET)
        ErroHandling("socket() Error");

    hUDPSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(hUDPSock == INVALID_SOCKET)
        ErroHandling("socket() Error");
    
    printf("������ TCP ������ �ڵ� : %d \n", hTCPSock);
    printf("������ UDP ������ �ڵ� : %d \n", hUDPSock);

    WSACleanup();

    return 0;
}

void ErroHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
