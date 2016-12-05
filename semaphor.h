#ifndef SEMAPHOR_H_INCLUDED
#define SEMAPHOR_H_INCLUDED
#include <string>
#include <windows.h>
//#include "stdafx.h"

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

class ConsoleOutput
{
	CRITICAL_SECTION cs;
public:
	ConsoleOutput();
	~ConsoleOutput();
	void print(std::string str);
};


class Semaphor
{
	int count;
	int maxCount;
	CriticalSection* cs;
	HANDLE cs_unlock_evnt;
	bool locked;

public:
	Semaphor(int maxCount);
	~Semaphor();
	void lock();
	void unlock();
};

DWORD WINAPI threadFunction(LPVOID lpParam);
template <typename T>
    std::string to_string(T value);
extern Semaphor *s;
extern ConsoleOutput* CO;
//extern HANDLE all_threads_created_evnt;

#endif // SEMAPHOR_H_INCLUDED
