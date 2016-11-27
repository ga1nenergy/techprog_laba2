#include "semaphor.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
//#include <iostream>
#include <string>
#include <iostream>
#define BUF_SIZE 255

using namespace std;


//Critical section
CriticalSection::CriticalSection()
{
    InitializeCriticalSection(&cs);
    cout << "critical section initialized" << endl << endl;
}

CriticalSection::~CriticalSection()
{
    DeleteCriticalSection(&cs);
    cout << "critical section deleted" << endl << endl;
}

void CriticalSection::lock()
{
    EnterCriticalSection(&cs);
    cout << "locked critical section" << endl << endl;
}

void CriticalSection::unlock()
{
    LeaveCriticalSection(&cs);
    cout << "unlocked critical section" << endl << endl;
}

//Semaphor
Semaphor::Semaphor(int maxCount) : maxCount(maxCount), count(0)
{
    cs = new CriticalSection();
    evnt = CreateEvent(
            NULL,
            TRUE,
            FALSE,
            TEXT("MyEvent"));
}

Semaphor::~Semaphor()
{
    delete cs;
}

void Semaphor::lock()
{
    cs->lock();
    if ((count++) <= maxCount)
    {
        cs->unlock();
        cout << "unlocked. count: " << count << endl;
        return;
    }
    WaitForSingleObject(evnt, INFINITE);
    cout << "im here" << endl;
}

void Semaphor::unlock()
{
    cs->lock();
    if ((count--) <= maxCount)
    {
        cs->unlock();
        SetEvent(evnt);
        //ResetEvent(evnt);
        return;
    }
    cout << "locked. count: " << count << endl;
}

DWORD WINAPI threadFunction(LPVOID lpParam)
{
    HANDLE hStdout;
    int sleepTIme;

    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;
    extern Semaphor *s;

    s->lock();

    // Make sure there is a console to receive output results.

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;

    // Cast the parameter to the correct data type.

    int sleepTime = (int)lpParam * 1000; //ms to sec

    // Print the parameter values using thread-safe functions.

    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Thread name: %i\nSleep time = %i\nPreparing to fall asleep...\n"),
        (int)GetCurrentThreadId(), sleepTime);
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
    Sleep(sleepTime);
    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Thread %i had a good snap\n"),
        (int)GetCurrentThreadId());
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
    /*cout << "Thread name: " << (int)GetCurrentThreadId() << endl << "Sleep time = " << sleepTime << endl << "Preparing to fall asleep...\n";
    Sleep(sleepTime);
    cout << "Thread " << (int)GetCurrentThreadId() << " had a good snap" << endl;*/
    s->unlock();

    return 0;
}
