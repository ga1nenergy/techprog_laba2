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
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    InitializeCriticalSection(&cs);
    //cout << "critical section initialized" << endl << endl;
    print(hStdout, "critical section initialized\n");
}

CriticalSection::~CriticalSection()
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    DeleteCriticalSection(&cs);
    print(hStdout, "critical section deleted\n");
}

void CriticalSection::lock()
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    EnterCriticalSection(&cs);
    //cout << "locked critical section" << endl << endl;
    print(hStdout, "locked critical section\n");
}

void CriticalSection::unlock()
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    LeaveCriticalSection(&cs);
    //cout << "unlocked critical section" << endl << endl;
    print(hStdout, "unlocked critical section\n");
}

void CriticalSection::print(HANDLE hStdout, std::string s)
{
    TCHAR msgBuf[BUF_SIZE];
    DWORD dwChars;
    size_t cchStringSize;

    StringCchPrintf(msgBuf, BUF_SIZE, TEXT(s.c_str()));
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
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
    HANDLE hStdout;
    TCHAR msgBuf[BUF_SIZE];
    DWORD dwChars;
    size_t cchStringSize;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    cs->lock();
    if ((count++) <= maxCount)
    {
        cs->unlock();
        //cout << "unlocked. count: " << count << endl;
        StringCchPrintf(msgBuf, BUF_SIZE, TEXT("(lock()) CS is unlocked, total number of threads: %i\n"),
        count);
        StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
        WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
        return;
    }

    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("(lock()) CS is locked, total number of threads: %i. Waiting for event\n"),
    count);
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

    WaitForSingleObject(evnt, INFINITE);
    //cout << "im here" << endl;
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

    HANDLE hStdout;
    TCHAR msgBuf[BUF_SIZE];
    DWORD dwChars;
    size_t cchStringSize;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("(unlock()) CS is locked, total number of threads: %i\n"),
        count);
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
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
