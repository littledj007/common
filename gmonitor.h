#pragma once
#include "tstream.h"
#include "gthread.h"
#include "glogger.h"

using namespace gcommon;

#pragma pack(push,1)

// 线程参数结构体
typedef struct _monitor_para : _thread_para
{
	int test;
}MONITOR_THREAD_PARA, *PMONITOR_THREAD_PARA;

#pragma pack(pop)


class GMonitor: public GThread
{
public:
	GMonitor(MONITOR_THREAD_PARA* para);
	~GMonitor();

private:
	MONITOR_THREAD_PARA* m_para;
	bool bupdate;

public:
	void ThreadMain();
	void SetTitle(tstring title);
	void SetContent(tstring group, tstring name);

	void UpdateContent(tstring name, int value);

private:

	void gotoxy(short x, short y);
};

