#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

unsigned WINAPI ThreadSummation(void* arg);
void ErrorHandling(char *message);

int sum = 0;
int sum1[] = {1, 5};
int sum2[] = {6, 10};

CRITICAL_SECTION cs;

int main(int argc, char** argv)
{
    HANDLE hThread1, hThread2;
    DWORD dwThreadID1, dwThreadID2;

    InitializeCriticalSection(&cs);

    hThread1 = (HANDLE)_beginthreadex(NULL, 0, ThreadSummation, (void*)sum1, 0, (unsigned *)&dwThreadID1);
    hThread2 = (HANDLE)_beginthreadex(NULL, 0, ThreadSummation, (void*)sum2, 0, (unsigned *)&dwThreadID2); 

    if(hThread1 == 0 || hThread2 == 0)
        ErrorHandling("������ ���� ����");

    if(WaitForSingleObject(hThread1, INFINITE) == WAIT_FAILED)
        ErrorHandling("������ wait ����");
    if(WaitForSingleObject(hThread2, INFINITE) == WAIT_FAILED)
        ErrorHandling("������ wait ����");

    printf("main �Լ� ����, sum = %d \n", sum);
    DeleteCriticalSection(&cs);

    return 0;    
}

unsigned WINAPI ThreadSummation(void* arg)
{
    int start = ((int*)arg)[0];
    int end = ((int*)arg)[1];

    for(; start <= end; start++)
    {
        EnterCriticalSection(&cs);
        sum += start;
        LeaveCriticalSection(&cs);
    }
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}