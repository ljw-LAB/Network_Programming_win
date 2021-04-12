#pragma comment(lib, "ws2_32")

#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 100

void ErrorHandling(char* message);

int main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET hServSock;
    SOCKADDR_IN servAddr;

    int arrIndex;
    int clntLen;
    SOCKET hClntSock;
    SOCKADDR_IN clntAddr;

    fd_set reads, temps;

    char message[BUFSIZE];
    int strLen;
    TIMEVAL timeout;

    if(argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");
    
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if(hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error");
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");

    if(listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");

    FD_ZERO(&reads);
    FD_SET(hServSock, &reads);

    while(1)
    {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if(select(0, &temps, 0, 0, &timeout) == SOCKET_ERROR)
            ErrorHandling("select() error");
        for(arrIndex = 0; arrIndex < reads.fd_count ; arrIndex++)
        {
            if(FD_ISSET(reads.fd_array[arrIndex], &temps))
            {
                if(reads.fd_array[arrIndex] == hServSock)
                {
                    clntLen = sizeof(clntAddr);
                    hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntLen);
                    FD_SET(hClntSock, &reads);
                    printf("클라이언트 연결 : 소켓 핸들 %d \n", hClntSock);
                }
                else
                {
                    strLen = recv(reads.fd_array[arrIndex], message, BUFSIZE-1, 0);
                    if(strLen == 0)
                    {
                        closesocket(temps.fd_array[arrIndex]);
                        printf("클라이언트 종료 : 소켓 핸들 %d \n", reads.fd_array[arrIndex]);
                        FD_CLR(reads.fd_array[arrIndex], &reads);
                    }
                    else
                    {
                        send(reads.fd_array[arrIndex], message, strLen, 0);
                    }
                }
            } //if(FD_ISSET(reads.fd_array[arrIndex], &temps))
        } //for(arrIndex=0;arrIndex < reads.fd_count; arrIndex)
    } //while(1)

    WSACleanup();
    return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}