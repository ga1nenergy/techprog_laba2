#include "semaphor.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#define BUF_SIZE 255

using namespace std;
//Critical section
/*void CriticalSection::lock()
{
    EnterCriticalSection(cs);
}

void CriticalSection::unlock()
{
    LeaveCriticalSection(cs);
}

//Semaphor
Semaphor::Semaphor(int maxCount, std::string name = NULL) : maxCount(maxCount), count(0)
{
    char* c_name = new char;
    if (name)
        c_name = name.c_str();
    cs = new CriticalScetion();
    evnt = new Event(
            NULL,
            TRUE,
            FASLE,
            TEXT(c_name);
}

void Semaphor::lock()
{
    cs->lock();
    count++;
    if (count < maxCount)
    {
        cs->unlock()
        return;
    }
    cs->unlock();
    evnt->Wait()
}

void Semaphor::unlock()
{
    cs->lock();
    if ((count--) >= maxCount)
    {
        evnt->Set();
        cs->unlock();
    }
}*/

DWORD WINAPI threadFunction(LPVOID lpParam)
{
    HANDLE hStdout;
    int sleepTIme;

    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;

    // Make sure there is a console to receive output results.

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;

    // Cast the parameter to the correct data type.

    int sleepTime = (int)lpParam;

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
    return 0;
}
