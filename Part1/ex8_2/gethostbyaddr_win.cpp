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

    int i;
    struct hostent *host;
    struct sockaddr_in addr;
    
    if(argc!=2)
    {
        printf("Usage : %s <IP> \n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) !=0)
        ErrorHandling("WSAStartup() error!");
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);

    if(!host)
        ErrorHandling("gethost... error");
    
    printf("Officially name : %s \n\n", host->h_name);
    puts("Aliases------------------");
    
    for(i=0;host->h_aliases[i];i++)
    {
        puts(host->h_aliases[i]);
    }

    printf("Address Type : %s \n", host->h_addrtype == AF_INET?"AF_INET":"AF_INET6");

    puts("IP Address---------");
    for(i=0;host->h_addr_list[i];i++)
    {
        puts(inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }

    return 0;
}
void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}