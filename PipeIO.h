#pragma once
#include <cstdint>
#include "tstream.h"

using namespace std;
using namespace gcommon;

/********************************************************************/
/* 类名: CPipeIO
/* 描述: 实现服务端和客户端的管道通信
/* 修改记录:
/*   2013-11-17,littledj: create
/********************************************************************/
class CPipeIO
{
public:
    CPipeIO();
    CPipeIO(const tstring& pipeName);
    ~CPipeIO();
    
public:
    static const uint32_t MAX_PIPENAME_LEN = 128;
    static const uint32_t MAX_PIPEMSG_LEN = 255 * 100;

private:
    bool m_bInit;
    tstring m_PipeName;
    void* m_hPipe;

public:
    // 服务端函数
    bool CreatePipe(const tstring& pipeName);
    void Connect();
    void Disconnect();

    // 客户端函数
    bool OpenPipe(const tstring& pipeName);
    BOOL Wait(const tstring& pipeName, uint32_t dwTimeout);

    // 共用函数
    bool ReadPipe(char* buff, uint32_t len, uint32_t& nRead, uint32_t timeout);//读取数据
    bool WritePipe(char* data, uint32_t len, uint32_t& nWrite, uint32_t timeout);//写入数据
    void Close();    
};