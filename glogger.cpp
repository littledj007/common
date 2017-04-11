#define _CRT_SECURE_NO_WARNINGS // for printf_s ...
#define _CRT_NON_CONFORMING_SWPRINTFS

#ifdef __LINUX__
// add .h in linux 
#else
#include <Windows.h>
#endif

#include <mutex>
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <fstream>

#include "glogger.h"

namespace gcommon
{
    mutex g_logMutex;       // ��־�ź���
    mutex g_printMutex;     // ��ӡ�ź���
    mutex g_poolMutex;      // ��Ϣ�ز����ź���

    GLogger::GLogger()
    {
        locale::global(locale(""));
        tcout.imbue(locale(""));        

        m_wpTarget = PRINT_TARGET::BOTH;        // ��ӡ���Ŀ��
        m_msg = TEXT("");
        m_msgPoolCount = 0;
        m_msgPoolPos = 0;
        m_logFile = TEXT("default.log");
        m_header = TEXT("##");
        m_debugLevel = 0;
        m_logDebugLevel = 0;
        m_enableColor = true;
        m_defaultColor = PRINT_COLOR::DARK_WHITE;
    }

    GLogger::~GLogger()
    {
    }

    /********************************************************************
    * [������]: enableColor
    * [����]: �����Ƿ������ɫ���
    * [�޸ļ�¼]:
    *   2015-11-21,littledj: create
    ********************************************************************/
    void GLogger::enableColor(bool para)
    {
        m_enableColor = para;
    }

    /********************************************************************
    * [������]: setDefaultColor
    * [����]: ����Ĭ�ϲ�ɫ
    * [�޸ļ�¼]:
    *   2015-11-21,littledj: create
    ********************************************************************/
    void GLogger::setDefaultColor(PRINT_COLOR color)
    {
        m_defaultColor = color;
    }

    /********************************************************************
    * [������]: setOnlyScreen
    * [����]: ����ֻ����Ļ���
    * [�޸ļ�¼]:
    *   2015-05-26,littledj: create
    ********************************************************************/
    void GLogger::setOnlyScreen()
    {
        m_wpTarget = PRINT_TARGET::SCREEN;
    }

    /********************************************************************
    * [������]: setOnlyLogfile
    * [����]: ����ֻ����־���
    * [�޸ļ�¼]:
    *   2015-05-26,littledj: create
    ********************************************************************/
    void GLogger::setOnlyLogfile()
    {
        m_wpTarget = PRINT_TARGET::FILE;
    }

    /********************************************************************
    * [������]: setTargetBoth
    * [����]: ������Ļ����־�������
    * [�޸ļ�¼]:
    *   2015-05-26,littledj: create
    ********************************************************************/
    void GLogger::setTargetBoth()
    {
        m_wpTarget = PRINT_TARGET::BOTH;
    }

    void GLogger::setTarget(PRINT_TARGET target)
    {
        m_wpTarget = target;
    }

    /********************************************************************
    * [������]: setHeader
    * [����]: ���ô�ӡʱ��Ҫ��ʾ������
    * [����]:
    *   header: ����
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::setHeader(const tstring header)
    {
        if (header.length() > MAX_HEADER_LEN)
        {
            m_header = TEXT("##");
            return;
        }

        if (header.empty())
        {
            m_header = TEXT("");
            return;
        }

        m_header = header;
    }

    /********************************************************************
    * [������]: setLogFile
    * [����]: ������־�ļ�·��
    * [����]:
    *   logFile: ��־�ļ�����·��
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::setLogFile(const tstring logFile)
    {
        if (logFile.empty() || logFile.length() > MAX_PATH)
        {
            m_logFile = TEXT("");
            return;
        }

        m_logFile = logFile;
    }

    /********************************************************************
    * [������]: setDebugLevel
    * [����]: ���õ�����Ϣ�ȼ�
    * [����]:
    *   debugLevel: ���Եȼ�
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::setDebugLevel(const int debugLevel)
    {
        m_debugLevel = 
            (debugLevel > MAX_DEBUG_LEVEL) ? MAX_DEBUG_LEVEL : debugLevel;
        m_debugLevel = 
            (debugLevel < MIN_DEBUG_LEVEL) ? MIN_DEBUG_LEVEL : debugLevel;
    }

    /********************************************************************
    * [������]: setLogDebugLevel
    * [����]: ������־������Ϣ�ȼ�
    * [����]:
    *   logDebugLevel: ���Եȼ�
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::setLogDebugLevel(const int logDebugLevel)
    {
        m_logDebugLevel = 
            (logDebugLevel > MAX_DEBUG_LEVEL) ? MAX_DEBUG_LEVEL : logDebugLevel;
        m_logDebugLevel = 
            (logDebugLevel < MIN_DEBUG_LEVEL) ? MIN_DEBUG_LEVEL : logDebugLevel;
    }

    /********************************************************************
    * [������]: getLastMsg
    * [����]: ��ȡ��ǰ��Ϣm_msg
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    tstring GLogger::getLastMsg()
    {
        return m_msg;
    }
    tstring GLogger::getLastError()
    {
        return m_errormsg;
    }
    tstring GLogger::getLastWarning()
    {
        if (!m_errormsg.empty())
            return m_errormsg;
        return m_warningmsg;
    }

    /********************************************************************
    * [������]: output
    * [����]: ��ӡ��Ϣ
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::output(const tstring& msg, PRINT_COLOR color)
    {
        if (msg.empty())
            output_screen(TEXT("<empty message>"), color);

        // �ж��Ƿ��������Ļ
        if (m_wpTarget == PRINT_TARGET::SCREEN)
            output_screen(msg, color);

        // �ж��Ƿ��������־�ļ�
        else if (m_wpTarget == PRINT_TARGET::FILE)
            output_file(msg);

        // �����
        else if (m_wpTarget == PRINT_TARGET::BOTH)
        {
            output_screen(msg, color);
            output_file(msg);
        }
    }

    /********************************************************************
    * [������]: output_screen
    * [����]: ����Ϣ��ӡ����Ļ
    * [�޸ļ�¼]:
    *   2015-05-27,littledj: create
    ********************************************************************/
    void GLogger::output_screen(const tstring& msg, PRINT_COLOR color)
    {
        if (msg.empty())
            return;

        // �ȴ���ӡ�ź�������ֹ���߳�ͬʱ��ӡʱ���໥����Ϣ��λ��
        g_printMutex.lock();
        if (m_enableColor)
        {
#ifdef __LINUX__
            tcout << LINUX_COLOR[(uint16_t)color] << msg;
            tcout << LINUX_COLOR[0]; // reset color
#else // Ĭ��Ϊ WINDOWS
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (uint16_t)color);
            tcout << msg;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (uint16_t)m_defaultColor);
#endif // __LINUX__
        }
        else
            tcout << msg;
        g_printMutex.unlock();// �ͷŴ�ӡ�ź���
    }

    /********************************************************************
    * [������]: output_file
    * [����]: ����Ϣд����־�ļ�
    * [�޸ļ�¼]:
    *   2015-05-27,littledj: create
    ********************************************************************/
    void GLogger::output_file(const tstring& msg)
    {
        if (msg.empty())
            return;

        g_logMutex.lock();
        tofstream logfile(m_logFile, ios_base::out | ios_base::app);
        if (!logfile.bad())
        {
            logfile << msg;
            logfile.close();
        }
        g_logMutex.unlock();
    }

    /********************************************************************
    * [������]: formatMsg
    * [����]: ��ʽ����ǰ��Ϣm_msg
    * [����]:
    *   type����Ϣ���ͣ�INFO��WARNING��...��
    *   format: �����Ϣ��ʽ���ַ���
    *   ap: �����б�
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    tstring GLogger::formatMsg_v(const PRINT_TYPE type, const tstring& format, va_list ap)
    {
        tstring msg;
        if (format.find('%') != tstring::npos)
        {
            tchar* msg_tmp = new tchar[format.length() + MAX_MESSAGE_LEN + 2];
            vsntprintf(msg_tmp, MAX_MESSAGE_LEN + 2, format.c_str(), ap);
            msg.assign(msg_tmp);
            delete[] msg_tmp;
        }
        else
        {
            msg = format;
        }

        if (type != PRINT_TYPE::RAW)
        {
            msg = PRINT_TYPE_ICON(type) + TEXT("[") + m_header + TEXT("] ") + msg + TEXT("\n");
        }

        // Ϊ�˷�ֹ���������Ϣ�ض�ΪMAX_MSG_LEN
        if (msg.length() > MAX_MESSAGE_LEN)
        {            
            msg.erase(msg.begin() + MAX_MESSAGE_LEN - 4, msg.end());
            msg.append(TEXT("...\n"));
        }            
        return msg;
    }

    /********************************************************************
    * [������]: formatMsg
    * [����]: ��ʽ����ǰ��Ϣm_msg���ɱ����
    * [����]:
    *   type����Ϣ���ͣ�INFO��WARNING��...��
    *   format: �����Ϣ��ʽ���ַ���
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    tstring GLogger::formatMsg(const PRINT_TYPE type, const tstring format, ...)
    {
        va_list ap;
        va_start(ap, format);
        tstring msg = formatMsg_v(type, format, ap);
        va_end(ap);
        return msg;
    }

    /********************************************************************
    * [������]: saveToMessagePool
    * [����]: ���浱ǰ��Ϣ����Ϣ��
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::saveToMessagePool(const PRINT_TYPE type, const tstring& msg)
    {
        // ��������Ϣ������
        g_poolMutex.lock();
        m_msg = msg;
        if (type == PRINT_TYPE::WARNING)
            m_warningmsg = msg;
        else if (type == PRINT_TYPE::ERR)
        {
            m_warningmsg = msg;
            m_errormsg = msg;
        }
        m_msgPool[m_msgPoolPos] = msg;
        m_msgPoolPos = (m_msgPoolPos + 1) % MAX_POOL_SIZE;
        if (m_msgPoolCount < MAX_POOL_SIZE)        
            m_msgPoolCount++;
        g_poolMutex.unlock();
    }

    /********************************************************************
    * [������]: error
    * [����]: ��ӡ������Ϣ
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::error(const tstring format, ...)
    {
        va_list ap;
        va_start(ap, format);
        tstring msg = formatMsg_v(PRINT_TYPE::ERR, format, ap);
        va_end(ap);

        output(msg, PRINT_COLOR::BRIGHT_RED);
        saveToMessagePool(PRINT_TYPE::ERR, msg);
    }    

    /********************************************************************
    * [������]: warning
    * [����]: ��ӡ������Ϣ
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::warning(const tstring format, ...)
    {
        va_list ap;
        va_start(ap, format);
        tstring msg = formatMsg_v(PRINT_TYPE::WARNING, format, ap);
        va_end(ap);

        output(msg, PRINT_COLOR::DARK_YELLOW);
        saveToMessagePool(PRINT_TYPE::WARNING, msg);
    }

    /********************************************************************
    * [������]: info
    * [����]: ��ӡ��ͨ��Ϣ
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::info(const tstring format, ...)
    {
        va_list ap;
        va_start(ap, format);
        tstring msg = formatMsg_v(PRINT_TYPE::INFO, format, ap);
        va_end(ap);    

        output(msg, m_defaultColor);
        saveToMessagePool(PRINT_TYPE::INFO, msg);
    }

    /********************************************************************
    * [������]: debug
    * [����]: ��ӡ������Ϣ
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::debug1(const tstring format, ...)
    {
        if (m_debugLevel>0)
        {
            va_list ap;
            va_start(ap, format);
            tstring msg = formatMsg_v(PRINT_TYPE::DEBUG1, format, ap);
            va_end(ap);
            
            output(msg, PRINT_COLOR::DARK_GREEN);
            saveToMessagePool(PRINT_TYPE::DEBUG1, msg); //������������
        }
    }
    void GLogger::debug2(const tstring format, ...)
    {
        if (m_debugLevel>1)
        {
            va_list ap;
            va_start(ap, format);
            tstring msg = formatMsg_v(PRINT_TYPE::DEBUG2, format, ap);
            va_end(ap);

            output(msg, PRINT_COLOR::DARK_GREEN);
            saveToMessagePool(PRINT_TYPE::DEBUG2, msg);
        }
    }
    void GLogger::debug3(const tstring format, ...)
    {
        if (m_debugLevel>2)
        {
            va_list ap;
            va_start(ap, format);
            tstring msg = formatMsg_v(PRINT_TYPE::DEBUG3, format, ap);
            va_end(ap);

            output(msg, PRINT_COLOR::DARK_GREEN);
            saveToMessagePool(PRINT_TYPE::DEBUG3, msg);
        }
    }

    /********************************************************************
    * [������]: screen
    * [����]: �����Ϣ����Ļ
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::screen(const tstring format, ...)
    {
        va_list ap;
        va_start(ap, format);
        tstring msg = formatMsg_v(PRINT_TYPE::RAW, format, ap);
        va_end(ap);
        
        output_screen(msg, m_defaultColor);
        saveToMessagePool(PRINT_TYPE::RAW, msg);
    }

    /********************************************************************
    * [������]: logfile
    * [����]: ԭʼ��־��Ϣ
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::logfile(const tstring format, ...)
    {
        va_list ap;
        va_start(ap, format);
        tstring msg = formatMsg_v(PRINT_TYPE::RAW, format, ap);
        va_end(ap);

        output_file(msg);
        saveToMessagePool(PRINT_TYPE::RAW, msg);
    }

    /********************************************************************
    * [������]: insertCurrentTime
    * [����]: ��־��ǰʱ�� (eg: yyy-MM-dd hh:mm:ss)
    *         y: year    M: month    d: day
    *         h: hour    m: minute   s: second
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    *   2015-05-20,littledj: �����Զ����ʽ
    ********************************************************************/
    void GLogger::insertCurrentTime(tstring format, PRINT_COLOR color)
    {
        if (format.empty())
            format = TEXT("** yyyy-MM-dd hh:mm:ss **\n");

        time_t tt = time(0);
        struct tm *lt = localtime(&tt);

        tchar* ct = new tchar[format.length() + 4];        
        tchar tmp[5];
        tcscpy(ct, format.c_str());
        for (size_t i = 0; i < format.size(); i++)
        {
            if (tcsncmp(ct + i, TEXT("yyyy"), 4 ) == 0)
            {
                stprintf(tmp, TEXT("%04d"), lt->tm_year + 1900);
                tcsncpy(ct + i, tmp, 4);
                i += 3;
                continue;
            }
            if (tcsncmp(ct + i, TEXT("MM"), 2) == 0)
            {
                stprintf(tmp, TEXT("%02d"), lt->tm_mon + 1);
                tcsncpy(ct + i, tmp, 2);
                i++;
                continue;
            }
            if (tcsncmp(ct + i, TEXT("dd"), 2) == 0)
            {
                stprintf(tmp, TEXT("%02d"), lt->tm_mday);
                tcsncpy(ct + i, tmp, 2);
                i++;
                continue;
            }
            if (tcsncmp(ct + i, TEXT("hh"), 2) == 0 || tcsncmp(ct + i, TEXT("HH"), 2) == 0)
            {
                stprintf(tmp, TEXT("%02d"), lt->tm_hour);
                tcsncpy(ct + i, tmp, 2);
                i++;
                continue;
            }
            if (tcsncmp(ct + i, TEXT("mm"), 2) == 0)
            {
                stprintf(tmp, TEXT("%02d"), lt->tm_min);
                tcsncpy(ct + i, tmp, 2);
                i++;
                continue;
            }
            if (tcsncmp(ct + i, TEXT("ss"), 2) == 0)
            {
                stprintf(tmp, TEXT("%02d"), lt->tm_sec);
                tcsncpy(ct + i, tmp, 2);
                i++;
                continue;
            }
        }

        tstring msg = formatMsg(PRINT_TYPE::RAW, TEXT("%s"), ct);
        delete[] ct;

        output(msg, color);
        saveToMessagePool(PRINT_TYPE::RAW, msg);
    }

    /********************************************************************
    * [������]: logMessagePool
    * [����]: ����Ϣ���е���Ϣд����־
    * [�޸ļ�¼]:
    *   2015-05-20,littledj: create
    ********************************************************************/
    void GLogger::logMessagePool()
    {
        g_poolMutex.lock();
        int msgStart = (m_msgPoolPos + MAX_POOL_SIZE - m_msgPoolCount) % MAX_POOL_SIZE;
        for (int i = 0; i < m_msgPoolCount; i++)
        {
            int msgReadPos = (msgStart + i) % MAX_POOL_SIZE;
            output_file(m_msgPool[msgReadPos]);
        }
        g_poolMutex.unlock();
    }

    /********************************************************************
    * [������]: clearMessagePool
    * [����]: ����Ϣ�����
    * [�޸ļ�¼]:
    *   2015-05-27,littledj: create
    ********************************************************************/
    void GLogger::clear()
    {
        g_poolMutex.lock();
        this->m_msgPoolCount = 0;
        this->m_msgPoolPos = 0;
        m_msg.clear();
        m_warningmsg.clear();
        m_errormsg.clear();
        g_poolMutex.unlock();
    }

    /********************************************************************
    * [������]: PRINT_TYPE_ICON
    * [����]: �������͵�ͼ��
    * [�޸ļ�¼]:
    *   2015-05-25,littledj: create
    ********************************************************************/    
    tstring GLogger::PRINT_TYPE_ICON(const PRINT_TYPE type)
    {
        switch (type)
        {
        case PRINT_TYPE::RAW:
            return TEXT("");        // raw
        case PRINT_TYPE::INFO:
            return TEXT("   ");     // info
        case PRINT_TYPE::WARNING:
            return TEXT(" ! ");     // warning
        case PRINT_TYPE::ERR:
            return TEXT(" x ");     // error
        case PRINT_TYPE::DEBUG1:
            return TEXT("+1 ");     // debug1
        case PRINT_TYPE::DEBUG2:
            return TEXT("+2 ");     // debug2
        case PRINT_TYPE::DEBUG3:
            return TEXT("+3 ");     // debug3
        case PRINT_TYPE::UNKNOWN:
            return TEXT(" ? ");     // unknown
        default:
            return TEXT(" ? ");
        }
    }
}