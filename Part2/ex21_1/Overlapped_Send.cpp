#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment( lib, "ws2_32" )

void ErrorHandling(char *message);

int main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN recvAddr;

    WSABUF dataBuf;
    char message[] = "Network is Computer";
    DWORD sendBytes = 0; // å�� int�� �Ǿ� ����, �Լ���ü�� LPDWORD�� DWORD�� �����͸� ����

    WSAEVENT event;
    WSAOVERLAPPED overlapped;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
       ErrorHandling("WSAStartup error \n");
    
    hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if(hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");
    
    memset(&recvAddr, 0, sizeof(recvAddr));
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_addr.s_addr = inet_addr(argv[1]);
    recvAddr.sin_port = htons(atoi(argv[2]));

    if(connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
        ErrorHandling("connect error");
    
    event = WSACreateEvent();
    memset(&overlapped, 0, sizeof(overlapped));
    overlapped.hEvent = event;
    dataBuf.len = strlen(message);
    dataBuf.buf = message;

    if(WSASend(hSocket, &dataBuf, 1, &sendBytes, 0, &overlapped, NULL) ==SOCKET_ERROR )
    {
        if(WSAGetLastError() != WSA_IO_PENDING)
            ErrorHandling("WSARecv() error");
    }

    puts("������ ���� �߿� �ٸ� ���� �ϰ� �ֽ��ϴ�");
    puts("������ ���� ���� Background�� ���� �߿� �ֽ��ϴ�");

    WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE);
    WSAGetOverlappedResult(hSocket, &overlapped, &sendBytes, FALSE, NULL);
    printf("���۵� ����Ʈ �� : %d \n", sendBytes);

    closesocket(hSocket);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}