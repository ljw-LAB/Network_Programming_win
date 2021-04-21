#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment( lib, "ws2_32" )

#define BUFSIZE 100
void CompressSockets(SOCKET* hSockArray, int omitIndex, int total);
void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total);
void ErrorHandling(char *message);

int main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET hServSock;
    SOCKADDR_IN servAddr;

    SOCKET hSockArray[WSA_MAXIMUM_WAIT_EVENTS];
    SOCKET hClntSock;
    int clntLen;
    SOCKADDR_IN clntAddr;

    WSAEVENT hEventArray[WSA_MAXIMUM_WAIT_EVENTS];
    WSAEVENT newEvent;
    WSANETWORKEVENTS netEvents;

    int sockTotal = 0;
    int index, i;
    char message[BUFSIZE];
    int strLen;

    if(argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 )
        ErrorHandling("WSAStartup() error!");
    
    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if(hServSock == INVALID_SOCKET)
        ErrorHandling("socket() error");
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(hServSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");
    
    newEvent = WSACreateEvent();
    if(WSAEventSelect(hServSock, newEvent, FD_ACCEPT) == SOCKET_ERROR)
        ErrorHandling("WSAEventSelect() error");

    if(listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen error");
    
    hSockArray[sockTotal] = hServSock;
    hEventArray[sockTotal] = newEvent;
    sockTotal++;
    while(1)
    {
        index = WSAWaitForMultipleEvents(sockTotal, hEventArray, FALSE, WSA_INFINITE, FALSE);
        index = index-WSA_WAIT_EVENT_0;

        for(i=index; i<sockTotal; i++)
        {
            index = WSAWaitForMultipleEvents(1, &hEventArray[i], TRUE, 0, FALSE);
            if((index == WSA_WAIT_FAILED || index == WSA_WAIT_TIMEOUT)) continue;
            else
            {
                index =i;
                WSAEnumNetworkEvents(hSockArray[index], hEventArray[index], &netEvents);
                if(netEvents.lNetworkEvents & FD_ACCEPT)
                {
                    if(netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
                    {
                        puts("Accept Error");
                        break;
                    }

                    clntLen =sizeof(clntAddr);
                    hClntSock = accept(hSockArray[index], (SOCKADDR*)&clntAddr, &clntLen);
                    newEvent = WSACreateEvent();
                    WSAEventSelect(hClntSock, newEvent, FD_READ|FD_CLOSE);
                    hEventArray[sockTotal] = newEvent;
                    hSockArray[sockTotal] = hClntSock;
                    sockTotal++;
                    printf("���� ����� ������ �ڵ� %d \n", hClntSock);
                }

                if(netEvents.lNetworkEvents & FD_READ)
                {
                    if(netEvents.iErrorCode[FD_READ_BIT] != 0)
                    {
                        puts("Read Error");
                        break;
                    }
                    strLen = recv(hSockArray[index-WSA_WAIT_EVENT_0], message, sizeof(message), 0);
                    send(hSockArray[index-WSA_WAIT_EVENT_0], message, strLen, 0);
                }
                if(netEvents.lNetworkEvents & FD_CLOSE)
                {
                    if(netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
                    {
                        puts("Close Error");
                        break;
                    }

                    WSACloseEvent(hEventArray[index]);
                    closesocket(hSockArray[index]);
                    printf("���� �� ������ �ڵ� : %d \n", hSockArray[index]);
                    sockTotal--;
                    CompressSockets(hSockArray, index, sockTotal);
                    CompressEvents(hEventArray, index, sockTotal);
                }
            }
        }
    }

    WSACleanup();
    return 0;
}

void CompressSockets(SOCKET* hSockArray, int omitIndex, int total)
{
    int i;
    for(i = omitIndex ; i < total ; i++)
        hSockArray[i] = hSockArray[i+1];
}

void CompressEvents(WSAEVENT* hEventArray, int omitIndex, int total)
{
    int i;
    for(i = omitIndex ; i < total ; i++)
        hEventArray[i] = hEventArray[i+1];
}
void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
