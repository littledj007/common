#include <mutex>
#include "gmonitor.h"
#include "windows.h"

mutex g_monitorMutex;      

GMonitor::GMonitor(MONITOR_THREAD_PARA* para):GThread(para)
{
    m_para = para;

    m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    m_program = TEXT("<no name>");
    m_version = TEXT("v1.0.0");
    m_screenWidth = 80;
    m_screenHeight = 6;
    m_idSize = 20;
    m_valueSize = m_screenWidth - 7 - m_idSize;
    m_valueHalfSize = (m_screenWidth - 7) / 2 - m_idSize - 3;
    m_sizeChanged = true;

    // 处
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

void GMonitor::SetProgramName(tstring name, tstring version)
{
    if ((m_welcome.size() + name.size() + version.size() + 6) > m_screenWidth)
    {
        // show error info
        return;
    }

    m_program = name;
    m_version = version;
}

void GMonitor::SetScreenWidth(uint32_t width)
{
    uint32_t w = width < MIN_MONITOR_WIDTH ? MIN_MONITOR_WIDTH : width;
    w = w > MAX_MONITOR_WIDTH ? MAX_MONITOR_WIDTH : w;

    if ((m_welcome.size() + m_program.size() + m_version.size() + 6) > w)
    {
        // show error info
        return;
    }

    g_monitorMutex.lock();
    m_screenWidth = w;
    m_valueSize = m_screenWidth - 7 - m_idSize;
    m_valueHalfSize = (m_screenWidth - 7) / 2 - m_idSize - 3;
    m_sizeChanged = true;
    g_monitorMutex.unlock();
}

void GMonitor::RegisterItem(tstring id, MONITOR_COLOR color, uint32_t refresh)
{
    if (id.size() > (m_screenWidth / 2))
    {
        return;
    }

    g_monitorMutex.lock();
    // 项名称长度改变
    if (id.size() > m_idSize)
    {
        m_idSize = id.size();
        m_valueSize = m_screenWidth - 7 - m_idSize;
        m_valueHalfSize = (m_screenWidth - 7) / 2 - m_idSize - 3;
        m_sizeChanged = true;
    }

    // 注册item 
    m_items.insert_or_assign(id, 
        MONITOR_ITEM({id, TEXT(""), true, ITEM_SIZE::LINE_HALF, color, refresh}));    
    m_sizeChanged = true;
    if (m_items.size() % 2 != 0)
    {
        m_screenHeight += 2;
    }
    g_monitorMutex.unlock();
}

void GMonitor::SetItemValue(tstring id, const tstring format, ...)
{
    auto item = m_items.find(id);
    if (item == m_items.end())
    {
        // 状态栏
        return;
    }
    if (id.size() > (m_screenWidth / 2))
    {
        return;
    }

    // format value
    va_list ap;
    va_start(ap, format);
    tstring value;
    if (format.find('%') != tstring::npos)
    {
        tchar* value_tmp = new tchar[format.length() + 1024 + 2];
        vsntprintf(value_tmp, 1024 + 2, format.c_str(), ap);
        value.assign(value_tmp);
        delete[] value_tmp;
    }
    else
    {
        value = format;
    }
    va_end(ap);        
    
    // calc value size
    ITEM_SIZE vsize = ITEM_SIZE::LINE_HALF;
    if (value.size() > (m_valueSize * 3))
    {
        // cut
        value.erase(value.begin() + m_valueSize * 3 - 3, value.end());
        value.append(TEXT("..."));
        vsize = ITEM_SIZE::LINE_3;
    }
    else if (value.size() > (m_valueSize * 2))
        vsize = ITEM_SIZE::LINE_3;
    else if (value.size() > m_valueSize)
        vsize = item->second.size > ITEM_SIZE::LINE_2 ? item->second.size : ITEM_SIZE::LINE_2;
    else if (value.size() > m_valueHalfSize)
        vsize = item->second.size > ITEM_SIZE::LINE_1 ? item->second.size : ITEM_SIZE::LINE_1;
    else
        vsize = item->second.size > ITEM_SIZE::LINE_HALF ? item->second.size : ITEM_SIZE::LINE_HALF;

    g_monitorMutex.lock();
    // 项值所占行数改变
    if (vsize > item->second.size)
    {
        m_screenHeight += (int)vsize - (int)item->second.size + 1;
        m_sizeChanged = true;
    }    
    item->second.changed = true;
    item->second.size = vsize;
    item->second.value = value;
    g_monitorMutex.unlock();
}

void GMonitor::UpdateContent()
{
    // refresh every second
    static time_t tt = time(NULL);
    if (time(NULL) == tt)
        return;

    // need to refresh all
    if (m_sizeChanged)
    {
        // rebuild UI table
        RefreshTable();

        // need refresh all items
        for (auto item : m_items)
        {
            item.second.changed = true;
        }
    }

    for (auto item : m_items)
    {
        if (item.second.changed)
        {
            RefreshData(item.second, );
        }
    }
}

void GMonitor::RefreshTable()
{
    // 清除所有
    for (size_t y = 0; y < m_screenHeight; y++)
    {
        SetPrintCur(0, y);
        for (size_t  x = 0; x < m_screenWidth; x++)
        {            
            tprintf(TEXT(" "));
        }
    }

    // 边框颜色
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::DARK_WHITE);

    // 外上边框
    size_t x = 0, y = 0;
    SetPrintCur(0, 0);
    for (size_t i = 0; i < m_screenWidth; i++)
        tprintf(TEXT("="));
    tprintf(TEXT("\n"));y++;

    // 标题
    tprintf(TEXT("|"));
    size_t x = (m_screenWidth - (m_welcome.size() + m_program.size() + m_version.size() + 2)) / 2;
    SetPrintCur(x, y);
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::BRIGHT_CYAN);
    tprintf((m_welcome + TEXT(" ")).c_str());
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::BRIGHT_PURPLE);
    tprintf((m_program + TEXT(" ")).c_str());
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::BRIGHT_CYAN);
    tprintf(m_version.c_str());
    
    // 标题下边框
    tprintf(TEXT("|"));
    for (size_t i = 1; i < m_screenWidth-1; i++)
        tprintf(TEXT("="));
    tprintf(TEXT("|"));

    // 外左边框
    SetPrintCur(0, m_screenHeight - 2);
    
    // 状态栏

    // 外下边框
    SetPrintCur(0, m_screenHeight - 2);
    for (size_t i = 0; i < m_screenWidth; i++)
        tprintf(TEXT("="));
}

void GMonitor::RefreshData(const MONITOR_ITEM &item, const COORD pos)
{
    DWORD count;
    FillConsoleOutputCharacter(m_hStdOut, ' ', maxLen, pos, &count);
    SetConsoleCursorPosition(m_hStdOut, pos);
    if (data.size() <= maxLen)
    {
        SetConsoleTextAttribute(m_hStdOut, (WORD)color);
        WriteConsole(m_hStdOut, data.c_str(), data.size(), &count, 0);
    }
    else
    {
        SetConsoleTextAttribute(m_hStdOut, (WORD)PRINT_COLOR::DARK_YELLOW);
        WriteConsole(m_hStdOut, m_defaultValue.c_str(), m_defaultValue.size(), &count, 0);
    }
}

void GMonitor::SetPrintCur(short x, short y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x, y });
}
