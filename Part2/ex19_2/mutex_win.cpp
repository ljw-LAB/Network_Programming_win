#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

unsigned WINAPI ThreadIncrement(void *arg);
void ErrorHandling(char *message);

char thread1[] = "A Thread";
char thread2[] = "B Thread";

int number = 0;
HANDLE hMutex;

int main(int argc, char** argv)
{
    HANDLE hThread1, hThread2;
    DWORD dwThreadID1, dwThreadID2;

    hMutex = CreateMutex(NULL, FALSE, NULL);

    if(hMutex == NULL)
    {
        puts("���ؽ� ������Ʈ ���� ����");
        exit(1);
    }

    hThread1 = (HANDLE)_beginthreadex(NULL, 0, ThreadIncrement, (void*)thread1, 0, (unsigned *)&dwThreadID1);
    hThread2 = (HANDLE)_beginthreadex(NULL, 0, ThreadIncrement, (void*)thread2, 0, (unsigned *)&dwThreadID2);

    if(hThread1 == 0 || hThread2 == 0)
    {
        puts("������ ���� ����");
        exit(1);
    }

    if(WaitForSingleObject(hThread1, INFINITE) == WAIT_FAILED)
        ErrorHandling("������ wait ����");
    if(WaitForSingleObject(hThread2, INFINITE) == WAIT_FAILED)
        ErrorHandling("������ wait ����");
    
    printf("���� number : %d \n", number);

    CloseHandle(hMutex);

    return 0;
}

unsigned WINAPI ThreadIncrement(void *arg)
{
    int i;
    for(i=0; i < 5; i++)
    {
        WaitForSingleObject(hMutex, INFINITE);
        Sleep(100);
        number++;
        printf("���� : %s, number : %d \n", (char*)arg, number);
        ReleaseMutex(hMutex);
    }
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
