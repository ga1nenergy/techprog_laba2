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
	CO->print("critical section initialized\n");
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&cs);
	CO->print("critical section deleted\n");
}

void CriticalSection::lock()
{
	EnterCriticalSection(&cs);
	CO->print("locked critical section\n");
}

void CriticalSection::unlock()
{
	LeaveCriticalSection(&cs);
	CO->print("unlocked critical section\n");
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

	WaitForSingleObject(cs_unlock_evnt, INFINITE);
}

void Semaphor::unlock()
{
	cs->lock();
	count--;
	if ((count) <= maxCount)
	{
		cs->unlock();
		SetEvent(cs_unlock_evnt);
		return;
	}

	CO->print("(unlock()) CS is locked, total number of threads: " + to_string(count));
}

DWORD WINAPI threadFunction(LPVOID lpParam)
{
	//WaitForSingleObject(all_threads_created_evnt, INFINITE);
	s->lock();

	int sleepTime = (int)lpParam /** 1000*/; //ms to sec

	CO->print(string("Thread name:") + to_string((int)GetCurrentThreadId()) + string("\nSleep time = ") + to_string(sleepTime) + string("\nPreparing to fall asleep...\n"));
	Sleep(sleepTime);
	CO->print(string("Thread ") + to_string((int)GetCurrentThreadId()) + string(" had a good snap\n"));

	s->unlock();

	return 0;
}
