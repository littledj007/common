#include <Windows.h>
#include "common.h"
#include "glogger.h"
#include "gmonitor.h"
using namespace gcommon;


int main()
{
    gmonitor::MONITOR_THREAD_PARA para;
    
    gmonitor::GMonitor test(&para);
    test.Run();


    test.RegisterItem(TEXT("111"),gmonitor::MONITOR_COLOR::BRIGHT_RED);

    test.RegisterItem(TEXT("ha222h"), gmonitor::MONITOR_COLOR::BRIGHT_RED);
    Sleep(1000);
    test.SetScreenWidth(100);
    for (size_t i = 0; i < 1007770000; i++)
    {
        test.SetItemValue(TEXT("111"), TEXT("12345aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbvvvvvvvvvvvvvvvvvvvvvvvvvv")+to_tstring(i));
        test.SetItemValue(TEXT("ha222h"), TEXT("12345aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbvvvvvvvvvvvvvvvvvvvvvvvvvv") + to_tstring(i));

        Sleep(10);
    }

    
    
    getchar(); 
}