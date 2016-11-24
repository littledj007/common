#include <Windows.h>
#include "common.h"
#include "glogger.h"
using namespace gcommon;

GLogger glogger;

DWORD WINAPI TestThread(PVOID para)
{
	tchar* str = (tchar*)para;
	for (size_t i = 0; i < 100; i++)
	{
		glogger.info(TEXT("%s"),str);
		Sleep(10);
	}
	return 0;
}

int main()
{
	const int THREAD_COUNT = 2;
	DWORD id;
	HANDLE* hthread = new HANDLE[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		tstring str = to_tstring(i);
		for (int j = 0; j < 5; j++)
		{
			str += str;
		}
		str = TEXT("[") + str + TEXT("]");
		tchar* arr = new tchar[str.length()+1];
		tcscpy(arr, str.c_str());
		hthread[i] = CreateThread(NULL, 0, TestThread, (PVOID)arr, 0, &id);
	}

	if (WAIT_OBJECT_0 != WaitForMultipleObjects(THREAD_COUNT, hthread, TRUE, INFINITE))
		printf("test error !!");
	else
		printf("test end");
	getchar();
}