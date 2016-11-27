#ifndef SEMAPHOR_H_INCLUDED
#define SEMAPHOR_H_INCLUDED
#include <windows.h>
#include <string>

using namespace std;

class CriticalSection
{
    CRITICAL_SECTION cs;

public:
    CriticalSection();
    ~CriticalSection();
    void unlock();
    void lock();
};

class Semaphor
{
    int count;
    int maxCount;
    CriticalSection* cs;
    HANDLE evnt;
    bool locked;

public:
    Semaphor(int maxCount);
    ~Semaphor();
    void lock();
    void unlock();
};

DWORD WINAPI threadFunction(LPVOID lpParam);
extern Semaphor *s;

#endif // SEMAPHOR_H_INCLUDED
