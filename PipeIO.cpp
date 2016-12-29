// pipeio.cpp
// 命名管道类，负责创建命名管道，并能够从管道中读取数据	
// 2013-11-28,littledj: CPipeIO类不再依赖于pipeInterface
// 需要包含：Advapi32.lib

#include "windows.h"
#include "PipeIO.h"
#include "common.h"

CPipeIO::CPipeIO()
{
	m_PipeName.clear();
	m_hPipe = INVALID_HANDLE_VALUE;
	m_bInit = false;
}

/********************************************************************/
/* 构造函数
/* 初始化成员变量
/* 修改记录: 
/*   2013-11-17,littledj: create 
/*   2013-11-28,littledj: 增加一个参数pipeName
/********************************************************************/
CPipeIO::CPipeIO(const tstring& pipeName)
{
	m_PipeName = pipeName;
	m_hPipe = INVALID_HANDLE_VALUE;
	m_bInit = false;
}

/********************************************************************/
/* 析构函数
/********************************************************************/
CPipeIO::~CPipeIO()
{
	if (m_hPipe)
	{
		CloseHandle(m_hPipe);
		m_hPipe = NULL;
	}
		
}

/********************************************************************/
/* 函数名: CreatePipe
/* 描述: 创建命名管道（读写message，由服务端初始化调用）
/* 输入: 
/*   pipeName: 管道名称
/* 输出: 
/*   true: 创建成功
/*   false: 创建失败
/* 修改记录: 
/*   2013-12-03,littledj: create 
/********************************************************************/
bool CPipeIO::CreatePipe(const tstring& pipeName)
{
	// 参数检查
	if(pipeName.empty() || 
		pipeName.length() > MAX_PIPENAME_LEN)
		return false;
	m_PipeName = pipeName;

	// 创建命令管道
	SECURITY_ATTRIBUTES secAttrib;
	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION );  // Advapi32.lib
	SetSecurityDescriptorDacl( &sd, true, NULL, false );                // Advapi32.lib
	secAttrib.bInheritHandle= true;
	secAttrib.nLength= sizeof(SECURITY_ATTRIBUTES);
	secAttrib.lpSecurityDescriptor= &sd;	
	m_hPipe = CreateNamedPipe( m_PipeName.c_str(), 
		PIPE_ACCESS_DUPLEX,
		PIPE_READMODE_MESSAGE|PIPE_TYPE_MESSAGE|PIPE_WAIT,
		1,
		0, 
		0,
		5000,
		&secAttrib);
	if( INVALID_HANDLE_VALUE == m_hPipe )
		return false;

	m_bInit = true;
	return true;
}

/********************************************************************/
/* 函数名: OpenPipe
/* 描述: 打开命名管道（读写message，由客户端调用）
/* 输入: 
/*   pipeName: 管道名称
/* 输出: 
/*   true: 打开成功
/*   false: 打开失败
/* 修改记录: 
/*   2013-12-03,littledj: create 
/********************************************************************/
bool CPipeIO::OpenPipe(const tstring& pipeName)
{
	// 参数检查
	if (pipeName.empty() ||
		pipeName.length() > MAX_PIPENAME_LEN)
		return false;
	m_PipeName = pipeName;

	// 打开命名管道		
	m_hPipe = CreateFile( m_PipeName.c_str(),
		GENERIC_READ|GENERIC_WRITE,0,NULL,
		OPEN_EXISTING,0,NULL);
	if(m_hPipe == INVALID_HANDLE_VALUE)
		return false;

	m_bInit = true;
	return true;
}

/********************************************************************/
/* 函数名: ReadPipe
/* 描述: 读取管道信息
/* 输入: 
/*   buff: 调用者传进来的缓冲区，存放读取的信息
/*   len: 缓冲区的大小（字节）
/*   nRead: 读取信息的大小（字节）
/*   dwTimeout: 超时时间
/* 输出: 
/*   true: 读取成功
/*   false: 读取失败
/* 修改记录: 
/*   2013-11-17,littledj: create 
/*   2013-12-01,littledj: 增加参数检查
/*   2013-12-04,littledj: 删除ConnectNamedPipe和DisconnectNamedPipe相关操作
/********************************************************************/
bool CPipeIO::ReadPipe(char* buff,uint32_t len,uint32_t& nRead,uint32_t dwTimeout)
{	
	// 参数检查
	if (!m_bInit)
		return false;
	if(m_hPipe == NULL || buff == NULL || len == 0)
		return false;

	uint32_t t=0;
	while( !ReadFile( m_hPipe, buff, len, (LPDWORD)&nRead, NULL ) )
	{
		Sleep(IDLE_TIME);
		t += IDLE_TIME;
		if (t>dwTimeout)	// 读取管道信息超时
		{
			return false;
		}
	}
	return true;
}

/********************************************************************/
/* 函数名: WritePipe
/* 描述: 向管道写入信息
/* 输入: 
/*   buff: 数据指针，存放要写入的信息
/*   len: 数据的长度（字节）
/*   nWrite: 写入长度（字节）
/*   dwTimeout: 超时时间
/* 输出: 
/*   true: 写入成功
/*   false: 写入失败
/* 修改记录: 
/*   2013-11-28,littledj: create 
/********************************************************************/
bool CPipeIO::WritePipe( char* data, uint32_t len, uint32_t& nWrite, uint32_t dwTimeOut )
{
	// 参数检查
	if (!m_bInit)
		return false;
	if(m_hPipe == NULL || data == NULL || len == 0)
		return false;

	uint32_t t=0;
	while (!WriteFile(m_hPipe, data, len, (LPDWORD)&nWrite, NULL))
	{
		Sleep(IDLE_TIME);
		t += IDLE_TIME;
		if (t>dwTimeOut)	// 读取管道信息超时
		{
			return false;
		}
	}	
	return true;
}

/********************************************************************/
/* 函数名: Close
/* 描述: 关闭管道句柄
/* 修改记录: 
/*   2013-11-17,littledj: create 
/********************************************************************/
void CPipeIO::Close()
{
	if (m_bInit)
	{
		CloseHandle(m_hPipe);
		m_hPipe = INVALID_HANDLE_VALUE;
	}
}

/********************************************************************/
/* 函数名: Disconnect
/* 描述: 服务端使用，断开客户端
/* 修改记录: 
/*   2013-12-04,littledj: create 
/********************************************************************/
void CPipeIO::Disconnect()
{
	if (m_bInit)
	{
		DisconnectNamedPipe(m_hPipe);
	}
}

/********************************************************************/
/* 函数名: Connect
/* 描述: 服务端使用，等待客户端连接
/* 修改记录: 
/*   2013-12-04,littledj: create 
/********************************************************************/
void CPipeIO::Connect()
{
	if (m_bInit)
	{
		ConnectNamedPipe(m_hPipe,NULL);
	}
}

/********************************************************************/
/* 函数名: Wait
/* 描述: 客户端使用，等待连接管道
/* 修改记录: 
/*   2013-12-04,littledj: create 
/********************************************************************/
BOOL CPipeIO::Wait(const tstring& pipeName, uint32_t dwTimeout)
{	
	return WaitNamedPipe(pipeName.c_str(),dwTimeout);
}

