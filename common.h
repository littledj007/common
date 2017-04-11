// ver 1.0
// modified: 
//   2014-12-17,littledj: 去除非common内容
//   2015-01-23,littledj: 修改SYS_LOGO

#pragma once
#pragma warning(disable:4996 4200)

#include "tstream.h"

#ifdef __LINUX__
#else
#ifdef _WIN64
typedef __int64 ssize_t;
#else
typedef __int32 ssize_t;
#endif
#endif

using namespace std;

namespace gcommon
{

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define IDLE_TIME				100
#define THREAD_START_TIMEOUT	2000			// 启动线程超时时间（毫秒）
#define THREAD_STOP_TIMEOUT		2000			// 停止线程超时时间

	uint16_t g_htons(const uint16_t data);
	uint16_t g_ntohs(const uint16_t data);
	uint32_t g_htonl(const uint32_t data);
	uint32_t g_ntohl(const uint32_t data);
	int GetParaFromARG(int argc, tchar* argv[], tchar* prefix, tchar* &out, int pos = 1);
	tstring GetCurrentDirPath();
	string inet_ltoi(uint32_t ip);
	wstring inet_ltow(uint32_t ip);
	tstring inet_ltot(uint32_t ip);
	uint32_t inet_ttol(const char* strIP);
	uint32_t inet_ttol(const wchar_t* strIP);
	uint32_t random(uint32_t start, uint32_t end);
	char* wtoa(const wchar_t* data, int len = 0);
	wchar_t* atow(const char* data, int len = 0);
	vector<string> SplitString(const string& str, const char ch);
	vector<wstring> SplitString(const wstring& str, const wchar_t ch);
	string& TrimString(string& str, const char ch = ' ');
	wstring& TrimString(wstring& str, const wchar_t ch = ' ');
	string& RemoveAllChar(string& str, const char ch);
	wstring& RemoveAllChar(wstring& str, const wchar_t ch);
	string& ReplaseAllSubString(string& str, const string& src, const string& dst);
	wstring& ReplaseAllSubString(wstring& str, const wstring& src, const wstring& dst);
	void DeleteEmptyItems(vector<string>& strs);
	wstring StringToWString(const string& str);
	string WStringToString(const wstring& str);
	tstring StringToTString(const string& str);
	tstring WStringToTString(const wstring& str);
	string TStringToString(const tstring& str);
	wstring TStringToWString(const tstring& str);	
	tstring GetConfigString(const tstring& filename, const tstring& key,
		const tstring& dft = TEXT(""), const tstring& title = TEXT("config"));
	int GetConfigInt(const tstring& filename, const tstring& key,
		const int dft = 0, const tstring& title = TEXT("config"));
	void SetConfigString(const tstring& filename, const tstring& key,
		const tstring& value = TEXT(""), const tstring& title = TEXT("config"));
	void SetConfigInt(const tstring& filename, const tstring& key,
		const int value = 0, const tstring& title = TEXT("config"));
}