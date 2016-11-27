#include <iostream>
#include <windows.h>
#include "semaphor.h"
#define MAX_THREADS 10

using namespace std;

//DWORD WINAPI threadFunction( LPVOID lpParam );

Semaphor *s = new Semaphor(3);

int main()
{
    HANDLE  hThreadArray[MAX_THREADS];
    DWORD   dwThreadIdArray[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++)
    {
        hThreadArray[i] = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size
            threadFunction,       // thread function name
            (LPVOID)i,          // argument to thread function
            0,                      // use default creation flags
            &dwThreadIdArray[i]);   // returns the thread identifier

        if (hThreadArray[i] == NULL)
           ExitProcess(3);
    }

    WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

    for(int i=0; i<MAX_THREADS; i++)
        CloseHandle(hThreadArray[i]);

    return 0;
}
