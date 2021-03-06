//#include "stdafx.h"
#include "semaphor.h"
#include <string>
#include <iostream>
#include<sstream>
#define BUF_SIZE 255

using namespace std;

template <typename T>
std::string to_string(T value)
{
	//create an output string stream
	std::ostringstream os;

	//throw the value into the string stream
	os << value;

	//convert the string stream into a string and return
	return os.str();
}

//ConsoleOutput
ConsoleOutput::ConsoleOutput() { InitializeCriticalSection(&cs); }
ConsoleOutput::~ConsoleOutput() { DeleteCriticalSection(&cs); }

void ConsoleOutput::print(std::string str)
{
	EnterCriticalSection(&cs);
	cout << str << endl << endl;
	LeaveCriticalSection(&cs);
}

//Critical section
CriticalSection::CriticalSection()
{
	InitializeCriticalSection(&cs);
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&cs);
}

void CriticalSection::lock()
{
	EnterCriticalSection(&cs);
}

void CriticalSection::unlock()
{
	LeaveCriticalSection(&cs);
}

//Semaphor
Semaphor::Semaphor(int maxCount) : maxCount(maxCount), count(0)
{
	cs = new CriticalSection();
	cs_unlock_evnt = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		TEXT("cs_unlock_evnt"));
}

Semaphor::~Semaphor()
{
	delete cs;
}

void Semaphor::lock()
{
	cs->lock();

	count++;

	if ((count) <= maxCount)
	{
		CO->print(string("(lock()) CS is unlocked, total number of threads: ") + to_string(count) + to_string("\n"));
		cs->unlock();
		return;
	}
    CO->print(string("(lock()) CS is locked, total number of threads: ") + to_string(count) + string(". Waiting for event\n"));
    cs->unlock();
    WaitForSingleObject(cs_unlock_evnt, INFINITE);
}

void Semaphor::unlock()
{
	cs->lock();

    count--;
    if (count != 0)
        if (((count) <= maxCount) && (count != 0))
        {
            SetEvent(cs_unlock_evnt);
            cs->unlock();
            return;
        }
        else
            CO->print("(unlock()) CS is locked, total number of threads: " + to_string(count));
	cs->unlock();
}

DWORD WINAPI threadFunction(LPVOID lpParam)
{
	s->lock();

	int sleepTime = (int)lpParam * 100; //ms to sec

	CO->print(string("Thread name:") + to_string((int)GetCurrentThreadId()) + string("\nSleep time = ") + to_string(sleepTime) + string("\nPreparing to fall asleep...\n"));
	Sleep(sleepTime);
	CO->print(string("Thread ") + to_string((int)GetCurrentThreadId()) + string(" had a good snap\n"));

	s->unlock();

	return 0;
}
