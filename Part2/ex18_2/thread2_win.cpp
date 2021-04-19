#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h> /*for _beginthreadex, _endthreadex*/
#pragma comment( lib, "ws2_32" )

unsigned WINAPI ThreadFunction(void* arg);

int main(int argc, char** argv)
{
    HANDLE hThread;
    DWORD dwThreadID;
    DWORD dw;

    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwThreadID);
    if(hThread == 0)
    {
        puts("_beginthreadex() error");
        exit(1);
    }

    printf("������ �������� �ڵ� : %d \n",hThread);
    printf("������ �������� ID : %d \n", dwThreadID);
    Sleep(3000);

    dw = WaitForSingleObject(hThread, INFINITE);
    
    if(dw == WAIT_FAILED)
    {
        puts("������ wait ����");
        exit(1);
    }

    printf("main �Լ� ����, %s ���� \n", dw==WAIT_OBJECT_0?"����":"������");

    return 0;
}

unsigned WINAPI ThreadFunction(void* arg)
{
    int i;
    for(i=0; i<5; i++)
    {
        Sleep(2000);
        printf("������ ���� �� \n");
    }
    return 0;
}