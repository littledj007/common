// ver 2.0
// modified: 
//   !!请不要对该文件使用预编译头!!
//   2014-12-17,littledj: info、warning、error、debug函数的输出自动增加回车
//   2014-12-19,littledj: ?无法输出中文
//   2015-11-21,littledj: 支持中文
//	 2015-11-21,littledj: 增加彩色输出功能，不同信息类型一目了然
//   2016-07-06,littledj: 在linux上编译成功，g++ -std=c++11 -D__LINUX__


#pragma once
#include <string>
#include "tstream.h"

using namespace std;

namespace gcommon
{
	enum class PRINT_COLOR
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

	static tstring LINUX_COLOR[] =
	{
		TEXT("\033[0m"),
		TEXT("\033[34m"),
		TEXT("\033[32m"),
		TEXT("\033[36m"),
		TEXT("\033[31m"),
		TEXT("\033[35m"),
		TEXT("\033[33m"),
		TEXT("\033[37m"),
		TEXT("\033[30m"),
		TEXT("\033[1m\033[34m"),
		TEXT("\033[1m\033[32m"),
		TEXT("\033[1m\033[36m"),
		TEXT("\033[1m\033[31m"),
		TEXT("\033[1m\033[35m"),
		TEXT("\033[1m\033[33m"),
		TEXT("\033[1m\033[37m"),
		TEXT("\033[1m\033[30m"),
	};
	
	enum class PRINT_TYPE
	{
		RAW = 0,		// 原始信息（不加任何附加信息）
		INFO = 1,		// 普通信息
		WARNING = 2,	// 警告信息
		ERR = 3,		// 错误信息
		DEBUG1 = 4,		// 调试信息（level=1）
		DEBUG2 = 5,		// 调试信息（level=2）
		DEBUG3 = 6,		// 调试信息（level=3）
		UNKNOWN = 7,	// 未知类型
	};
	enum class PRINT_TARGET
	{
		BLANK = 0,
		SCREEN = 1,
		FILE = 2,
		BOTH = 3,
	};

	class GLogger
	{
	public:
		GLogger();
		~GLogger();

	public:
		// 彩色输出开关
		void enableColor(bool para = true);
		// 设置默认颜色
		void setDefaultColor(PRINT_COLOR color = PRINT_COLOR::DARK_WHITE);
		// 设置输出目标:屏幕
		void setOnlyScreen();
		// 设置输出目标：日志
		void setOnlyLogfile();
		// 设置输出目标：屏幕和日志
		void setTargetBoth();
		// 设置输出
		void setTarget(PRINT_TARGET target);
		// 获取当前信息
		tstring getLastMsg();
		// 获取错误信息
		tstring getLastError();
		// 获取警告信息
		tstring getLastWarning();

		// 设置名称
		void setHeader(const tstring header = TEXT("##"));

		// 设置日志文件路径
		void setLogFile(const tstring logFile = TEXT(""));

		// 设置调试信息等级
		void setDebugLevel(const int debugLevel = 0);
		// 设置日志的调试信息等级
		void setLogDebugLevel(const int debugLevel = 0);

		// 输出错误信息
		void error(const tstring format, ...);

		// 输出警告信息
		void warning(const tstring format, ...);

		// 输出普通信息
		void info(const tstring format, ...);

		// 只输出调试等级1的信息
		void debug1(const tstring format, ...);

		// 输出调试等级1/2的信息
		void debug2(const tstring format, ...);

		// 输出调试等级1/2/3的信息
		void debug3(const tstring format, ...);

		// 输出原始信息到屏幕（不添加任何前导字符）
		void screen(const tstring format, ...);

		// 输出原始信息到日志（不添加任何前导字符）
		void logfile(const tstring format, ...);

		// 插入当前时间
		void insertCurrentTime(tstring format = TEXT("** yyyy-MM-dd HH:mm:ss **\n"));

		// 将消息池中的信息写入日志
		void logMessagePool();
		// 清空消息池
		void clearMessagePool();

	private:
		// 线程安全的信息输出函数
		void output(const tstring& msg = TEXT(""), PRINT_COLOR color = PRINT_COLOR::DARK_WHITE);
		// 打印
		void output_screen(const tstring& msg = TEXT(""), PRINT_COLOR color = PRINT_COLOR::DARK_WHITE);
		// 写文件
		void output_file(const tstring& msg = TEXT(""));
		// 格式化字符串保存到m_msg
		tstring formatMsg_v(const PRINT_TYPE type, const tstring& format, va_list ap);
		// 格式化字符串保存到m_msg
		tstring formatMsg(const PRINT_TYPE type, const tstring format, ...);
		// 保存当前消息至消息池
		void saveToMessagePool(const PRINT_TYPE type, const tstring& msg = TEXT(""));
		// 显示类型图标
		tstring PRINT_TYPE_ICON(const PRINT_TYPE type);

	private:
		//short m_wpTarget;		// 输出目标
		const static int MUTEX_TIMEOUT = 2000; // ms
		const static int MAX_HEADER_LEN = 16;  // bytes
		const static int MAX_DEBUG_LEVEL = 3;
		const static int MIN_DEBUG_LEVEL = 0;
		const static int MAX_MESSAGE_LEN = 1000;   // bytes
		const static int MAX_POOL_SIZE = 100;

#ifndef MAX_PATH
		const static int MAX_PATH = 128;
#endif

		PRINT_TARGET m_wpTarget;// 输出目标
		tstring m_msg;			// 当前消息
		tstring m_errormsg;		// 错误消息
		tstring m_warningmsg;	// 警告消息
		tstring m_msgPool[MAX_POOL_SIZE];	// 消息池
		int m_msgPoolCount;		// 消息池中的消息数量
		int m_msgPoolPos;		// 消息池当前位置
		tstring m_logFile;		// 日志文件路径
		tstring m_header;		// 信息头
		int m_debugLevel;		// 调试信息显示等级
		int m_logDebugLevel;	// 调试信息日志等级
		bool m_enableColor;		// 允许彩色输出
		PRINT_COLOR m_defaultColor; // 默认颜色
	};
}