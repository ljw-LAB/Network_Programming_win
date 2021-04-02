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
    char *strAddr = "203.211.218.102:9190";
    char strAddBuf[50];
    SOCKADDR_IN servAddr;

    int size = sizeof(servAddr);

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    if(WSAStringToAddress(strAddr, AF_INET, NULL, (SOCKADDR *)&servAddr, &size) == SOCKET_ERROR)
        ErrorHandling("WSAStringToAddress() error!");

    size = sizeof(strAddBuf);

    if(WSAAddressToString((SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddBuf, &size) == SOCKET_ERROR)
        ErrorHandling("WSAStringToAddress() error!");

    printf("최종 변환 결과 : %s \n", strAddBuf);

    WSACleanup();
    
    return 0;

}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}