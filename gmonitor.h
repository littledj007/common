//

#pragma once
#include <map>
#include "tstream.h"
#include "gthread.h"

#define MIN_MONITOR_WIDTH 40
#define MIN_MONITOR_HEIGHT 8
#define MAX_MONITOR_WIDTH 150
#define MAX_MONITOR_HEIGHT 40

namespace gmonitor
{
    enum class MONITOR_COLOR
    {
        DARK_BLUE = 1,
        DARK_GREEN = 2,
        DARK_CYAN = 3,
        DARK_RED = 4,
        DARK_PURPLE = 5,
        DARK_YELLOW = 6,
        DARK_WHITE = 7,
        BRIGHT_BLUE = 9,
        BRIGHT_GREEN = 10,
        BRIGHT_CYAN = 11,
        BRIGHT_RED = 12,
        BRIGHT_PURPLE = 13,
        BRIGHT_YELLOW = 14,
        BRIGHT_WHITE = 15,
    };

    enum class ITEM_SIZE
    {
        LINE_HALF = 0,
        LINE_1 = 1,
        LINE_2 = 2,
        LINE_3 = 3
    };

    using namespace gcommon;

#pragma pack(push,1)

    // 线程参数结构体
    typedef struct _monitor_para : _thread_para
    {
        int test;
    }MONITOR_THREAD_PARA, *PMONITOR_THREAD_PARA;

    typedef struct _monitor_item
    {
        tstring id;
        tstring value;
        bool changed;
        ITEM_SIZE size;
        MONITOR_COLOR color;
        COORD idpos; // id pos
        COORD vpos;  // value pos
        uint32_t refresh_interval; // 毫秒
    }MONITOR_ITEM;

#pragma pack(pop)


    class GMonitor : public GThread
    {
    public:
        GMonitor(MONITOR_THREAD_PARA* para);
        ~GMonitor();

    private:
        const tstring m_welcome = TEXT("Welcome to");
        const tstring ITEM_INFO = TEXT("[[info]]");
        const tstring ITEM_WARNING = TEXT("[[warning]]");
        const tstring ITEM_ERROR = TEXT("[[error]]");

        HANDLE m_hStdOut;
        tstring m_program;
        tstring m_version;
        short m_screenWidth;
        short m_screenHeight;
        short m_idSize;
        short m_valueHalfSize; // half line value max size
        short m_valueSize; // one line value max size
        bool m_sizeChanged;
        MONITOR_THREAD_PARA* m_para;
        map<tstring, MONITOR_ITEM> m_items;

    public:
        void ThreadMain();

        // setup gmonitor
        void SetProgramName(tstring name, tstring version = TEXT("v1.0.0"));
        void SetScreenWidth(uint32_t width = 80);

        // handle items
        void RegisterItem(tstring id, MONITOR_COLOR color = MONITOR_COLOR::DARK_WHITE, uint32_t refresh = 1000);

        // update values
        void SetItemValue(tstring id, const tstring format, ...);
        void SetStateInfo(const tstring msg);
        void SetStateWarning(const tstring msg);
        void SetStateError(const tstring msg);

    private:        
        void RefreshItems();
        void RefreshTable();
        void RefreshItem(MONITOR_ITEM &item);
        void RefreshData(tstring data, const COORD pos, const short maxSize, MONITOR_COLOR color);
        void RefreshDateTime();
    };

}