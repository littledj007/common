#include <mutex>
#include "gmonitor.h"
#include "windows.h"

using namespace gmonitor;
mutex g_monitorMutex;      

GMonitor::GMonitor(MONITOR_THREAD_PARA* para):GThread(para)
{
    m_para = para;

    m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    m_program = TEXT("<no name>");
    m_version = TEXT("v1.0.0");
    m_screenWidth = 80;
    m_screenHeight = 6;
    m_idSize = 15;
    m_valueSize = m_screenWidth - 7 - m_idSize;
    m_valueHalfSize = (m_screenWidth - 7) / 2 - m_idSize - 3;
    m_sizeChanged = true;

    // 隐藏光标
    CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
    SetConsoleCursorInfo(m_hStdOut, &cursor_info);
}


GMonitor::~GMonitor()
{
}

void GMonitor::ThreadMain()
{
    RefreshItems();
    RefreshDateTime();
    Sleep(100);    
}

void GMonitor::SetProgramName(tstring name, tstring version)
{
    if ((m_welcome.size() + name.size() + version.size() + 6) > (uint32_t)m_screenWidth)
    {
        // show error info
        return;
    }

    g_monitorMutex.lock();
    m_program = name;
    m_version = version;
    m_sizeChanged = true;
    g_monitorMutex.unlock();
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
    if (id.size() > (uint32_t)(m_screenWidth / 2))
    {
        return;
    }

    g_monitorMutex.lock();
    // 项名称长度改变
    if (id.size() > (uint32_t)m_idSize)
    {
        m_idSize = id.size();
        m_valueSize = m_screenWidth - 7 - m_idSize;
        m_valueHalfSize = (m_screenWidth - 7) / 2 - m_idSize - 3;
    }

    // 注册item 
    m_items.insert_or_assign(id, 
        MONITOR_ITEM({ id, TEXT(""), true, ITEM_SIZE::LINE_HALF, color, { 0, 0 }, { 0, 0 }, refresh }));
    m_sizeChanged = true;
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
    if (id.size() > (uint32_t)(m_screenWidth / 2))
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
    if (value.size() > (uint32_t)(m_valueSize * 3))
    {
        // cut
        value.erase(value.begin() + m_valueSize * 3 - 3, value.end());
        value.append(TEXT("..."));
        vsize = ITEM_SIZE::LINE_3;
    }
    else if (value.size() > (uint32_t)(m_valueSize * 2))
        vsize = ITEM_SIZE::LINE_3;
    else if (value.size() > (uint32_t)m_valueSize)
        vsize = item->second.size > ITEM_SIZE::LINE_2 ? item->second.size : ITEM_SIZE::LINE_2;
    else if (value.size() > (uint32_t)m_valueHalfSize)
        vsize = item->second.size > ITEM_SIZE::LINE_1 ? item->second.size : ITEM_SIZE::LINE_1;
    else
        vsize = item->second.size > ITEM_SIZE::LINE_HALF ? item->second.size : ITEM_SIZE::LINE_HALF;

    g_monitorMutex.lock();
    // 项值所占行数改变
    if (vsize > item->second.size)
    {
        m_sizeChanged = true;
    }    
    item->second.changed = true;
    item->second.size = vsize;
    item->second.value = value;
    g_monitorMutex.unlock();
}

void gmonitor::GMonitor::SetStateInfo(const tstring msg)
{
}

void gmonitor::GMonitor::SetStateWarning(const tstring msg)
{
}

void gmonitor::GMonitor::SetStateError(const tstring msg)
{
}

void GMonitor::RefreshItems()
{ 
    g_monitorMutex.lock();

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
            RefreshItem(item.second);
        }
    }
    
    g_monitorMutex.unlock();
}

void GMonitor::RefreshTable()
{
    // 改变界面大小 ？？
    size_t ni = 0;
    m_screenHeight = 6;
    bool newline = true;
    for (auto item : m_items)
    {
        if (item.second.size == ITEM_SIZE::LINE_HALF)
        {
            if (newline)
            {
                m_screenHeight += 2;
                newline = false;
            }
            else
                newline = true;
        }
        else
        {
            newline = true;
            m_screenHeight += (short)item.second.size + 1;
        }
        ni++;
    }
    tstring cmd = TEXT("mode con cols=") + to_tstring(m_screenWidth + 1) + TEXT(" lines=") + to_tstring(m_screenHeight + 1);
    tsystem(cmd.c_str());

    // 获取窗口长宽
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(m_hStdOut, &csbi))
    {
        return;
    }

    // 清空屏幕
    DWORD count;
    uint32_t cellCount = csbi.dwSize.X *csbi.dwSize.Y;
    if (!FillConsoleOutputCharacter(
        m_hStdOut, ' ', cellCount, { 0, 0 }, &count))
    {
        return;
    }   

    // 边框颜色
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::DARK_WHITE);

    // 外上边框
    short x = 0, y = 0;
    SetConsoleCursorPosition(m_hStdOut, { 0, 0 });
    for (size_t i = 0; i < (uint32_t)m_screenWidth; i++)
        tprintf(TEXT("="));
    tprintf(TEXT("\n")); y++;

    // 标题
    tprintf(TEXT("|"));
    x = (m_screenWidth - (m_welcome.size() + m_program.size() + m_version.size() + 2)) / 2;
    SetConsoleCursorPosition(m_hStdOut, { x, y });
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::BRIGHT_CYAN);
    tprintf((m_welcome + TEXT(" ")).c_str());
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::BRIGHT_PURPLE);
    tprintf((m_program + TEXT(" ")).c_str());
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::BRIGHT_CYAN);
    tprintf(m_version.c_str());
    SetConsoleCursorPosition(m_hStdOut, { m_screenWidth - 1, y });
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::DARK_WHITE);
    tprintf(TEXT("|\n")); y++;

    // 标题下边框
    tprintf(TEXT("|"));
    for (size_t i = 1; i < (uint32_t)m_screenWidth - 1; i++)
        tprintf(TEXT("="));
    tprintf(TEXT("|\n")); y++;

    // 外左右边框、内部边框
    short x_midle = m_screenWidth / 2;
    short x_item1 = m_idSize + 3;
    short x_item2 = x_midle + m_idSize + 3;
    newline = false;
    ni = 0;
    for (auto &item : m_items)
    {


        if (ni % 2 != 0 && !newline &&
            item.second.size == ITEM_SIZE::LINE_HALF)
        {
            if(ni % 2 != 0)
                y -= 2;

            item.second.idpos = { x_midle + 2 ,y };
            item.second.vpos = { x_item2 + 2 ,y };
            SetConsoleCursorPosition(m_hStdOut, { x_midle, y });
            tprintf(TEXT("|"));
            SetConsoleCursorPosition(m_hStdOut, { x_item2, y });
            tprintf(TEXT("|"));
            SetConsoleCursorPosition(m_hStdOut, { m_screenWidth - 1, y });
            tprintf(TEXT("|\n")); y++;
            newline = false;            
        }
        else
        {
            short new_y = item.second.size == ITEM_SIZE::LINE_3 ? y + 1 : y;
            item.second.idpos = { 2 ,new_y };
            item.second.vpos = { x_item1 + 2 ,y };
            if (item.second.size == ITEM_SIZE::LINE_HALF)
            {
                newline = false;
                SetConsoleCursorPosition(m_hStdOut, { 0, y });
                tprintf(TEXT("|"));
                SetConsoleCursorPosition(m_hStdOut, { x_item1, y });
                tprintf(TEXT("|"));
                SetConsoleCursorPosition(m_hStdOut, { m_screenWidth - 1, y });
                tprintf(TEXT("|\n")); y++;
            }
            else
            {
                newline = true;
                for (size_t i = 0; i < (uint32_t)item.second.size; i++)
                {
                    SetConsoleCursorPosition(m_hStdOut, { 0, y });
                    tprintf(TEXT("|"));
                    SetConsoleCursorPosition(m_hStdOut, { x_item1, y });
                    tprintf(TEXT("|"));
                    SetConsoleCursorPosition(m_hStdOut, { m_screenWidth - 1, y });
                    tprintf(TEXT("|\n")); y++;
                }
            }
        }        

        // 横向分割线
        tprintf(TEXT("|"));
        for (size_t i = 1; i < (uint32_t)m_screenWidth - 1; i++)
            tprintf(TEXT("-"));
        tprintf(TEXT("|\n")); y++;

        ni++;
    }

    // 打印所有item的id
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::DARK_CYAN);
    for (auto &item : m_items)
    {
        SetConsoleCursorPosition(m_hStdOut, { item.second.idpos.X, item.second.idpos.Y });
        tprintf(item.second.id.c_str());
    }
    
    // 状态栏
    SetConsoleTextAttribute(m_hStdOut, (WORD)MONITOR_COLOR::DARK_WHITE);
    SetConsoleCursorPosition(m_hStdOut, { 0, y });
    tprintf(TEXT("|"));
    SetConsoleCursorPosition(m_hStdOut, { m_screenWidth - 1, y });
    tprintf(TEXT("|\n")); y++;

    // 外下边框
    SetConsoleCursorPosition(m_hStdOut, { 0, y });
    for (size_t i = 0; i < (uint32_t)m_screenWidth; i++)
        tprintf(TEXT("="));
    tprintf(TEXT("\n")); y++;

    // 日期 时间 显示
    SetConsoleCursorPosition(m_hStdOut, { m_screenWidth - 23, y });
    tprintf(TEXT("[                     ]"));
    m_screenHeight = y + 1;

    m_sizeChanged = false;    
}

void GMonitor::RefreshItem(MONITOR_ITEM &item)
{    
    DWORD count;

    // 清除已有数据
    if (item.size == ITEM_SIZE::LINE_HALF)
    {
        FillConsoleOutputCharacter(m_hStdOut, ' ', m_valueHalfSize, item.vpos, &count);       
    }
    else if (item.size == ITEM_SIZE::LINE_1)
    {
        FillConsoleOutputCharacter(m_hStdOut, ' ', m_valueSize, item.vpos, &count);
    }
    else if (item.size == ITEM_SIZE::LINE_2)
    {
        FillConsoleOutputCharacter(m_hStdOut, ' ', m_valueSize, item.vpos, &count);
        FillConsoleOutputCharacter(m_hStdOut, ' ', m_valueSize, { item.vpos.X,  item.vpos.Y + 1 }, &count);
    }
    else if (item.size == ITEM_SIZE::LINE_3)
    {
        FillConsoleOutputCharacter(m_hStdOut, ' ', m_valueSize, item.vpos, &count);
        FillConsoleOutputCharacter(m_hStdOut, ' ', m_valueSize, { item.vpos.X,  item.vpos.Y + 1 }, &count);
        FillConsoleOutputCharacter(m_hStdOut, ' ', m_valueSize, { item.vpos.X,  item.vpos.Y + 2 }, &count);
    }        

    // 写入新数据
    SetConsoleTextAttribute(m_hStdOut, (WORD)item.color);
    if (item.value.size() > (uint32_t)m_valueSize * 2)
    {
        SetConsoleCursorPosition(m_hStdOut, item.vpos);
        WriteConsole(m_hStdOut, item.value.c_str(), m_valueSize, &count, 0);
        SetConsoleCursorPosition(m_hStdOut, { item.vpos.X,  item.vpos.Y + 1 });
        WriteConsole(m_hStdOut, item.value.substr(m_valueSize).c_str(), m_valueSize, &count, 0);
        SetConsoleCursorPosition(m_hStdOut, { item.vpos.X,  item.vpos.Y + 2 });
        WriteConsole(m_hStdOut, item.value.substr(m_valueSize * 2).c_str(), item.value.substr(m_valueSize * 2).size(), &count, 0);
    }
    else if (item.value.size() > (uint32_t)m_valueSize)
    {
        SetConsoleCursorPosition(m_hStdOut, item.vpos);
        WriteConsole(m_hStdOut, item.value.c_str(), m_valueSize, &count, 0);
        SetConsoleCursorPosition(m_hStdOut, { item.vpos.X,  item.vpos.Y + 1 });
        WriteConsole(m_hStdOut, item.value.substr(m_valueSize).c_str(), item.value.substr(m_valueSize).size(), &count, 0);
    }
    else
    {
        SetConsoleCursorPosition(m_hStdOut, item.vpos);
        WriteConsole(m_hStdOut, item.value.c_str(), item.value.size(), &count, 0);
    }

    item.changed = false;
}

void gmonitor::GMonitor::RefreshData(tstring data, const COORD pos, const short maxSize, MONITOR_COLOR color)
{
    DWORD count;
    FillConsoleOutputCharacter(m_hStdOut, ' ', maxSize, pos, &count);
    SetConsoleCursorPosition(m_hStdOut, pos);
    if (data.size() > maxSize)
        data.resize(maxSize);
    SetConsoleTextAttribute(m_hStdOut, (WORD)color);
    WriteConsole(m_hStdOut, data.c_str(), data.size(), &count, 0);
}


void GMonitor::RefreshDateTime()
{
    static time_t last = time(NULL);
    static tchar timeFormat[24 + 1];
    
    time_t current = time(NULL);
    if (current > last)
    {
        last = current;
        tm f;
        localtime_s(&f, &current);
        tcsftime(timeFormat, sizeof(timeFormat),
            TEXT("%Y-%m-%d %H:%M:%S"), &f);

        g_monitorMutex.lock();
        RefreshData(tstring(timeFormat), { m_screenWidth - 21, m_screenHeight - 1 }, 19, MONITOR_COLOR::DARK_CYAN);
        g_monitorMutex.unlock();
    }
}