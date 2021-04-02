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
    
    char* addr1 = "1.2.3.4";
    char* addr2 = "1.2.3.256";
    unsigned long convAddr;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");
    
    convAddr = inet_addr(addr1);
    if(convAddr == INADDR_NONE)
        printf("Error Occur : %d \n", convAddr);
    else
        printf("Unsigned char long addr(network ordered) : %x \n", convAddr);
    
    convAddr = inet_addr(addr2);
    if(convAddr == INADDR_NONE)
        printf("Error Occur : %d \n", convAddr);
    else
        printf("Unsigned char long addr(network ordered) : %x \n", convAddr);
    
    WSACleanup();
    return 0;

}

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}