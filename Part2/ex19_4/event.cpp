#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

unsigned WINAPI NumberOfA(void *arg);
unsigned WINAPI NumberOfOthers(void *arg);

void ErrorHandling(char *message);
char String[100];
HANDLE hEvent;

int main(int argc, char** argv)
{
    HANDLE hThread1, hThread2;
    DWORD dwThread1ID, dwThread2ID;

    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if(hEvent == NULL)
    {
        puts("Event ������Ʈ ���� ����");
        exit(1);
    }

    hThread1 = (HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, (unsigned *)&dwThread1ID);
    hThread2 = (HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, (unsigned *)&dwThread2ID); 

    if(hThread1 == 0  || hThread2 == 0 )
    {
        puts("������ ���� ����");
        exit(1);
    }

    fputs("���ڿ��� �Է��ϼ��� : ", stdout);
    fgets(String, 100, stdin);
    SetEvent(hEvent);
	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

    //if(WaitForSingleObject(hThread2, INFINITE) == WAIT_FAILED);
    //    ErrorHandling("������ hThread2 wait ���� ���� ");

    //if(WaitForSingleObject(hThread1, INFINITE) == WAIT_FAILED);
    //    ErrorHandling("������ hThread1 wait ���� ���� ");


    ResetEvent(hEvent);
    CloseHandle(&hEvent);
    return 0;
}

unsigned WINAPI NumberOfA(void *arg)
{
    int i;
    int count =0;

    WaitForSingleObject(hEvent, INFINITE);
    for(i=0; String[i]!=0; i++)
    {
        if(String[i] == 'A')
            count++;
    }
    printf("A ������ �� : %d \n", count);
    return 0;
}
unsigned WINAPI NumberOfOthers(void *arg)
{
    int i;
    int count =0;

    WaitForSingleObject(hEvent, INFINITE);
    for(i=0; String[i]!=0; i++)
    {
        if(String[i] != 'A')
            count++;
    }
    printf("A �̿��� ������ �� : %d \n", count-1);
    return 0;

}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// #include <stdio.h>
// #include <windows.h>
// #include <process.h> 
// #define STR_LEN		100

// unsigned WINAPI NumberOfA(void *arg);
// unsigned WINAPI NumberOfOthers(void *arg);

// static char str[STR_LEN];
// static HANDLE hEvent;

// int main(int argc, char *argv[]) 
// {	
// 	HANDLE  hThread1, hThread2;

// 	hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
// 	hThread1=(HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, NULL);
// 	hThread2=(HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, NULL);

// 	fputs("Input string: ", stdout); 
// 	fgets(str, STR_LEN, stdin);
// 	SetEvent(hEvent);

// 	WaitForSingleObject(hThread1, INFINITE);
// 	WaitForSingleObject(hThread2, INFINITE);
// 	ResetEvent(hEvent);
//  	CloseHandle(hEvent);
//     return 0;
// }

// unsigned WINAPI NumberOfA(void *arg) 
// {
// 	int i, cnt=0;
// 	WaitForSingleObject(hEvent, INFINITE);
// 	for(i=0; str[i]!=0; i++)
// 	{
// 		if(str[i]=='A')
// 			cnt++;
// 	}
// 	printf("Num of A: %d \n", cnt);
// 	return 0;
// }
// unsigned WINAPI NumberOfOthers(void *arg) 
// {
// 	int i, cnt=0;
// 	WaitForSingleObject(hEvent, INFINITE);
// 	for(i=0; str[i]!=0; i++) 
// 	{
// 		if(str[i]!='A')
// 			cnt++;
// 	}
// 	printf("Num of others: %d \n", cnt-1);
// 	return 0;
// }