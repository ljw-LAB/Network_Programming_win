#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>

#define BUFSIZE 100

typedef struct
{
    SOCKET hClntSock;
    SOCKADDR_IN clntAddr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct
{
    OVERLAPPED overlapped;
    char buffer[BUFSIZE];
    WSABUF wsaBuf;
}PER_IO_DATA, *LPPER_IO_DATA;

DWORD WINAPI CompletionThread(LPVOID CompletionPortIO);
void ErrorHandling(char *message);

int main(int argc, char** argv)
{
    WSADATA wsaData;
    HANDLE hCompletionPort;
    SYSTEM_INFO SystemInfo;
    SOCKADDR_IN servAddr;
    LPPER_IO_DATA PerIoData;
    LPPER_HANDLE_DATA perHandleData;

    SOCKET hServSock;
    DWORD RecvBytes;
    int i;
    DWORD Flags;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error");
    
    hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    GetSystemInfo(&SystemInfo);

    for(i=0; i<SystemInfo.dwNumberOfProcessors;i++)
		_beginthreadex(NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL);
    
    hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    bind(hServSock, (SOCKADDR*)&servAddr, sizeof(&servAddr));
    listen(hServSock, 5);

    while(TRUE)
    {
        SOCKET hClntSock;
        SOCKADDR_IN clntAddr;
        int addrLen = sizeof(clntAddr);

        hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen);
        perHandleData = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
        perHandleData->hClntSock = hClntSock;
        memcpy(&(perHandleData->clntAddr), &clntAddr, addrLen);
        CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)perHandleData, 0);
        PerIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
        memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
        PerIoData->wsaBuf.len = BUFSIZE;
        PerIoData->wsaBuf.buf = PerIoData->buffer;
        Flags = 0;

        WSARecv(perHandleData->hClntSock, &(PerIoData->wsaBuf),1, &RecvBytes,&Flags, &(PerIoData->overlapped), NULL);
    }
    return 0;
}

DWORD WINAPI CompletionThread(LPVOID pComPort)
{
    HANDLE hCompletionPort = (HANDLE)pComPort;
    DWORD BytesTransferred;
    LPPER_HANDLE_DATA PerHandleData;
    LPPER_IO_DATA PerIoData;
    DWORD flags;

    while(1)
    {
        GetQueuedCompletionStatus(hCompletionPort, &BytesTransferred, (LPDWORD)&PerHandleData, (LPOVERLAPPED*)&PerIoData, INFINITE);

    
        if(BytesTransferred == 0)
        {
            closesocket(PerHandleData->hClntSock);
            free(PerHandleData);
            free(PerIoData);
            continue;
        }

        PerIoData->wsaBuf.len = BytesTransferred;
        WSASend(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL);
        memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
        PerIoData->wsaBuf.len = BUFSIZE;
        PerIoData->wsaBuf.buf = PerIoData->buffer;

        flags =0;
        WSARecv(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, &flags, &(PerIoData->overlapped), NULL);
    }
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}