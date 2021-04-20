#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <process.h>

#pragma comment( lib, "ws2_32" )

#define BUFSIZE 100

unsigned WINAPI ClinetConn(void *arg);
void SendMSG(char* message, int len);
void ErrorHandling(char *message);

int clntNumber = 0;
SOCKET clntSocks[10];
HANDLE hMutex;

int main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET servSock;
    SOCKET clntSock;

    SOCKADDR_IN servAddr;
    SOCKADDR_IN clntAddr;
    int clntAddrSize;

    HANDLE hThread;
    DWORD dWThreadID;

    if(argc !=2 )
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");
    hMutex = CreateMutex(NULL, FALSE, NULL);
    if(hMutex == NULL)
    {
        ErrorHandling("CreateMutex() error");
    }

    servSock = socket(PF_INET, SOCK_STREAM, 0);
    if(servSock == INVALID_SOCKET)
        ErrorHandling("socket() error");
    
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");
    
    if(listen(servSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");
    
    while(1)
    {
        clntAddrSize = sizeof(clntAddr);
        clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
        if(clntSock == INVALID_SOCKET)
            ErrorHandling("accept() error");
        WaitForSingleObject(hMutex, INFINITE);
        clntSocks[clntNumber++] = clntSock;
        ReleaseMutex(hMutex);
        printf("새로운 연결, 클라이언트 IP : %s \n", inet_ntoa(clntAddr.sin_addr));
        hThread = (HANDLE)_beginthreadex(NULL, 0, ClinetConn, (void *)clntSock, 0, (unsigned*)&dWThreadID);

        if(hThread == 0)
        {
            ErrorHandling("쓰레드 생성 오류");
        }
    }
    WSACleanup();
    return 0;
}
unsigned WINAPI ClinetConn(void *arg)
{
    SOCKET clntSock = (SOCKET)arg;
    int strLen = 0;
    char message[BUFSIZE];
    int i;

    while((strLen = recv(clntSock, message, BUFSIZE, 0)) != 0)
        SendMSG(message, strLen);
    
    WaitForSingleObject(hMutex, INFINITE);
    for(i=0; i<clntNumber; i++)
    {
        if(clntSock == clntSocks[i])
        {
            for(;i<clntNumber-1;i++)
                clntSocks[i] = clntSocks[i+1];
            break;
        }
    }

    clntNumber--;
    ReleaseMutex(hMutex);

    closesocket(clntSock);
    return 0;
}

void SendMSG(char* message, int len)
{
    int i;
    WaitForSingleObject(hMutex, INFINITE);
    for(i=0; i<clntNumber;i++)
        send(clntSocks[i], message, len, 0);
    ReleaseMutex(hMutex);

}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
