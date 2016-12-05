//#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "semaphor.h"
#define MAX_THREADS 10
#define BUF_SIZE 255

using namespace std;

ConsoleOutput* CO = new ConsoleOutput;
Semaphor *s = new Semaphor(3);
/*HANDLE all_threads_created_evnt = CreateEvent(
	NULL,
	TRUE,
	FALSE,
	TEXT("all_threads_created_evnt"));;*/

int main()
{
	HANDLE hThreadArray[MAX_THREADS];
	DWORD dwThreadIdArray[MAX_THREADS];

	for (int i = 0; i < MAX_THREADS; i++)
	{
		CO->print(string("number of step: ") + to_string(i));
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

	//SetEvent(all_threads_created_evnt);

	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

	for (int i = 0; i<MAX_THREADS; i++)
		CloseHandle(hThreadArray[i]);

	return 0;
}
