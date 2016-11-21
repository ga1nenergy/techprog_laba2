#ifndef SEMAPHOR_H_INCLUDED
#define SEMAPHOR_H_INCLUDED
#include <windows.h>

class CriticalSection
{
    CRITICAL_SECTION cs;

public:
    void unlock();
    void lock();
};

class Semaphor
{
    int count;
    int maxCount;
    CriticalSection* cs;
    HANDLE *evnt;

public:
    Semaphor(int maxCount);
    ~Semaphor();
    void lock();
    void unlock();
};

DWORD WINAPI threadFunction(LPVOID lpParam);

#endif // SEMAPHOR_H_INCLUDED
