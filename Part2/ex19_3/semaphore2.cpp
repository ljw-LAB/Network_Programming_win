#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

unsigned WINAPI ThreadSend(void *arg);
unsigned WINAPI ThreadRecv(void *arg);

void ErrorHandling(char *message);

HANDLE hSem, hSem2;
int number = 0;

char thread1[] = "A Thread";
char thread2[] = "B Thread";
char thread3[] = "C Thread";

int main(int argc, char** argv)
{
    HANDLE hThread1, hThread2, hThread3;
    DWORD dwThreadID1, dwThreadID2, dwThreadID3;

    hSem = CreateSemaphore(NULL, 0, 1, NULL);
    hSem2 = CreateSemaphore(NULL, 0, 1, NULL);

    if(hSem == NULL || hSem2 == NULL)
    {
        puts("�������� ������Ʈ ���� ����");
        exit(1);
    }

    hThread1 = (HANDLE)_beginthreadex(NULL, 0, ThreadSend, (void *)thread1, 0, (unsigned*)&dwThreadID1);
    hThread2 = (HANDLE)_beginthreadex(NULL, 0, ThreadRecv, (void *)thread2, 0, (unsigned*)&dwThreadID2);
    hThread3 = (HANDLE)_beginthreadex(NULL, 0, ThreadRecv, (void *)thread3, 0, (unsigned*)&dwThreadID3);

    if(hThread1 == 0 || hThread2 == 0 || hThread3 == 0)
    {
        puts("������ ���� ����");
        exit(1);
    }

    if(WaitForSingleObject(hThread1, INFINITE) == WAIT_FAILED)
        ErrorHandling("������ wait ����");
    if(WaitForSingleObject(hThread2, INFINITE) == WAIT_FAILED)
        ErrorHandling("������ wait ����");
    if(WaitForSingleObject(hThread3, INFINITE) == WAIT_FAILED)
        ErrorHandling("������ wait ����");
    
    printf("���� number : %d \n", number);
    CloseHandle(hSem);
    CloseHandle(hSem2);

    return 0;
}

unsigned WINAPI ThreadSend(void *arg)
{
    int i;
    for(i=0; i<4; i++)
    {
        number++;
        printf("���� : %s, number : %d \n", (char*)arg, number);
        ReleaseSemaphore(hSem, 1 , NULL);
        WaitForSingleObject(hSem2, INFINITE);
    }
    return 0;
}

unsigned WINAPI ThreadRecv(void *arg)
{
    int i;
    for(i=0; i<2; i++)
    {
        WaitForSingleObject(hSem, INFINITE);
        number--;
        printf("���� : %s, number : %d \n", (char*)arg, number);
        ReleaseSemaphore(hSem2, 1 , NULL);
    }
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}