#include "gmonitor.h"
#include "windows.h"

GMonitor::GMonitor(MONITOR_THREAD_PARA* para):GThread(para)
{
	m_para = para;
}


GMonitor::~GMonitor()
{
}

void GMonitor::ThreadMain()
{

	Sleep(1);
	gotoxy(0, 0);
	//_tprintf(TEXT("*********** test ***********\n"));
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)PRINT_COLOR::BRIGHT_GREEN);
	//_tprintf(TEXT("  test: %02d%% \n"),m_para->test);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)PRINT_COLOR::DARK_WHITE);
	//_tprintf(TEXT("****************************\n"));

}

void GMonitor::UpdateContent(tstring name, int value)
{
}

void GMonitor::gotoxy(short x, short y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
}
