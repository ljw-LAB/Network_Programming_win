#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <process.h>

#pragma comment( lib, "ws2_32" )


#define BUFSIZE 100
#define NAMESIZE 20

unsigned WINAPI SendMSG(void *arg);
unsigned WINAPI RecvMSG(void *arg);
void ErrorHandling(char *message);

char name[NAMESIZE] = "[Default]";
char message[BUFSIZE];

int main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET sock;

    SOCKADDR_IN servAddr;
    HANDLE hThread1, hThread2;
    DWORD dwThreadID1, dwThreadID2;

    if(argc != 4)
    {
        printf("Usage : %s <IP> <port> <name> \n", argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error");
    
    sprintf(name, "[%s]", argv[3]);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
        ErrorHandling("socket() error");
    
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("connect() error");
    
    hThread1 = (HANDLE)_beginthreadex(NULL, 0, SendMSG, (void*)sock, 0, (unsigned*)&dwThreadID1);
    hThread2 = (HANDLE)_beginthreadex(NULL, 0, RecvMSG, (void*)sock, 0, (unsigned*)&dwThreadID2);

    if(hThread1 == 0 || hThread2 == 0)
    {
        ErrorHandling("������ ���� ����");
    }

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    closesocket(sock);
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

unsigned WINAPI SendMSG(void *arg)
{
    SOCKET sock = (SOCKET)arg;
    char nameMessage[NAMESIZE+BUFSIZE];
    while(1)
    {
        fgets(message, BUFSIZE, stdin);
        sprintf(nameMessage, "%s %s", name, message);
        if(!strcmp(message, "q\n"))
        {
            closesocket(sock);
            exit(0);
        }
        send(sock, nameMessage, strlen(nameMessage), 0);
    }
}

unsigned WINAPI RecvMSG(void *arg)
{
    SOCKET sock =(SOCKET)arg;
    char nameMessage[NAMESIZE+BUFSIZE];
    int strLen;
    while(1)
    {
        strLen = recv(sock, nameMessage, NAMESIZE+BUFSIZE-1, 0);
        if(strLen == -1) return 1;

        nameMessage[strLen] = 0;
        fputs(nameMessage, stdout);
    }
}