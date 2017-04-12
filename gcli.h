#pragma once
#include <map>
#include <list>
#include <cstdint>
#include <algorithm>
#include <vector>
#include "tstream.h"

using namespace std;
using namespace gcommon;

namespace gcli
{

    enum class CMD_TYPE
    {
        FULL = 0x00,            // 命令全名
        SHORT_FORM = 0x01,      // 命令缩写
        SAME = 0x02,            // 同名命令
        EXPIRED = 0x03,         // 过期命令
        BAD = 0xff,             // 错误命令
    };


    // 命令回调函数声明
    typedef uint32_t(*CMD_CALLBACK)(const tstring&);

    // 命令信息结构体
    typedef struct _shell_handler
    {
        tstring strCMD;              // 命令名称
        CMD_TYPE type;               // 命令类型
        CMD_CALLBACK handler;        // 命令回调函数
        tstring strToolTip;          // 命令简介
        tstring strUsage;            // 命令使用帮助
    }SHELL_HANDLER, *PSHELL_HANDLER;


    /********************************************************************/
    /* 类名: GCLI
    /* 描述: 通用的shell命令类,通过它用户能够轻松创建自己的shell程序，
    /*       主要是实现了按键处理循环，并支持用户注册自己的命令回调函数。
    /* 修改记录:
    /*   2013-12-14,littledj: create
    /********************************************************************/
    class GCLI
    {
    public:
        GCLI(void);
        ~GCLI(void);

    public:
        static const uint32_t MAX_PROMPT_LEN = 16;          // 前导字符串最大长度
        static const uint32_t MAX_CMD_LEN = 63;             // 命令最大长度
        static const uint32_t MAX_TOOLTIP_LEN = 255;        // 命令描述最大长度
        static const uint32_t MAX_CMD_PARA_COUNT = 10;      // 命令最多参数个数

    private:
        const tstring DEFAULT_PROMPT = TEXT("#shell> ");            // 默认前导字符串
        const tstring DEFAULT_CMD_HELP = TEXT("help");              // 默认帮助命令
        const tstring DEFAULT_CMD_EXIT = TEXT("exit");              // 默认退出命令

        map<tstring, SHELL_HANDLER> m_handlerMap;
        tstring m_prompt;
        bool m_shouldExit; // 用于判断是否退出

    public:
        bool Init(const tstring& prompt = TEXT(""));
        bool RegisterHandler(SHELL_HANDLER handler);
        bool RegisterHandlers(SHELL_HANDLER handler[], int count);
        void MainLoop();

    private:
        void DispatchCommand(const tstring& cmd);
        PSHELL_HANDLER GetNextAssociat(const tstring& cmd);
        void ListAssociat(const tstring& cmd);
        void Exit();
        void ShowHelp(const tstring& cmd = TEXT("help"));
    };

}