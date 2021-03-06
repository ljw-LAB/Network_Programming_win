// #pragma comment( lib, "ws2_32" )

// #include <stdio.h>
// #include <stdlib.h>
// #include <Windows.h>
// #include <process.h>

// DWORD WINAPI ThreadFunction(void* arg);

// int main(int argc, char** argv)
// {
//     HANDLE hThread;
//     DWORD dwThreadID;
//     hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned *)&dwThreadID);

//     if(hThread == 0)
//     {
//         puts("_beginthreadex() error");
//         exit(1);
//     }

//     printf("생성된 쓰레드의 핸들 : %d \n", hThread);
//     printf("생성된 쓰레드의 ID : %d \n", dwThreadID);
//     Sleep(3000);
//     puts("main 함수 종료");

//     return 0;
// }

// DWORD WINAPI ThreadFunction(void* arg)
// {
//     int i;
//     for(i = 0; i < 5; i++)
//     {
//         Sleep(2000);
//         puts("쓰레드 실행 중");
//     }
//     return 0;
// }

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

    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwThreadID);
    if(hThread == 0)
    {
        puts("_beginthreadex() error");
        exit(1);
    }

    printf("생성된 쓰레드의 핸들 : %d \n",hThread);
    printf("생성된 쓰레드의 ID : %d \n", dwThreadID);
    Sleep(3000);
    puts("main 함수 종료");

    return 0;
}

unsigned WINAPI ThreadFunction(void* arg)
{
    int i;
    for(i=0; i<5; i++)
    {
        Sleep(2000);
        printf("쓰레드 실행 중");
    }
    return 0;
}