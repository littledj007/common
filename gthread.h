#pragma once
#ifdef __LINUX__
#include <unistd.h>
#include <pthread.h>
#define Sleep(x) usleep(1000*x)
#else
#include "windows.h"
#endif

#include <string>
#include <cstdint>
#include "glogger.h"

using namespace std;
namespace gcommon
{
    enum class THREAD_CONTROL
    {
        UNKNOWN,
        START,      // 开始执行        
        STOP,       // 停止
        SUSPEND,    // 暂停
        CONTINUE,   // 继续执行
    };

    // 线程或模块的运行状态常量,及其名称
    enum class THREAD_STATE
    {
        UNKNOWN,    // 其他
        CREATE,     // 创建
        INIT,       // 初始化
        RUNNING,    // 正常执行
        SUSPENDING, // 挂起
        STOPPED,    // 已停止
        SLEEPING    // 睡眠中
    };

#pragma pack(push,1)

    // 线程参数结构体
    typedef struct _thread_para
    {
        tstring threadName;         // 线程名称
        uint32_t debugLevel = 0;    // 调试信息显示级别
        PRINT_TARGET printTarget = PRINT_TARGET::SCREEN;    // 线程消息打印输出选择
        tstring logfile;            // 日志文件全路径
        bool enableColor = true;    // 允许彩色输出

        //THREAD_CONTROL control = THREAD_CONTROL::START;   // 由构造函数设置，对线程进行控制
        //THREAD_STATE state = THREAD_STATE::CREATE;        // 由构造函数设置，线程状态
        void* current = NULL;       // 由构造函数设置，当前线程所在类
    }THREAD_PARA, *PTHREAD_PARA;

#pragma pack(pop)

    class GThread
    {
    public:
        GThread();
        GThread(PTHREAD_PARA para);
        virtual ~GThread();

    private:
        PTHREAD_PARA m_para;
#ifdef __LINUX__
        pthread_t m_hThread = 0;
#else
        HANDLE m_hThread = NULL;
#endif

    public:
        GLogger m_glogger;
        THREAD_CONTROL m_control = THREAD_CONTROL::UNKNOWN;
        THREAD_STATE m_state = THREAD_STATE::UNKNOWN;

    public:
        virtual void SetPara(PTHREAD_PARA para);

        bool Run();
        bool Stop();
        bool Suspend();
        bool Continue();

        virtual void ThreadMain() = 0; // 用户事务处理函数

        static tstring THREAD_STATE_NAME(THREAD_STATE state);
        static tstring THREAD_STATE_IMG(THREAD_STATE state);

    private:
#ifdef __LINUX__
        static void *MainLoop(void *para);
#else
        static DWORD WINAPI MainLoop(LPVOID para);
#endif
    };

}