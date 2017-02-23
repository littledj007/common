#pragma once
#ifdef __LINUX__
#include <unistd.h>
#else
#include <io.h>  // for access()
#endif

#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
#include <cstdio>
#include <time.h>
#include "string.h"

using namespace std;

namespace gcommon
{
#ifdef UNICODE	

#ifndef TEXT
#define TEXT(quote) L##quote
#endif

#define tcin wcin
#define tcout wcout
#define tcerr wcerr
#define tclog wclog
#define to_tstring to_wstring

#define taccess _waccess
#define tfopen _wfopen
#define tsystem _wsystem

#define ttoi _wtoi
#define itot _itow

#define tprintf wprintf
#define vstprintf vswprintf
#define stprintf swprintf
#define tcslen wcslen 
#define tcscpy wcscpy 
#define tcsstr wcsstr
#define tcschr wcschr
#define tcscat wcscat
#define tcscmp wcscmp
#define tcsicmp wcsicmp
#define tcsrchr wcsrchr
#define tcsncpy wcsncpy 
#define tcsncmp wcsncmp

// time.h
#define tcsftime wcsftime


	typedef wchar_t tchar;
	typedef wstring tstring;
	typedef basic_ios<wchar_t, char_traits<wchar_t> > tios;
	typedef basic_streambuf<wchar_t, char_traits<wchar_t> > tstreambuf;
	typedef basic_istream<wchar_t, char_traits<wchar_t> > tistream;
	typedef basic_ostream<wchar_t, char_traits<wchar_t> > tostream;
	typedef basic_iostream<wchar_t, char_traits<wchar_t> > tiostream;
	typedef basic_stringbuf<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > tstringbuf;
	typedef basic_istringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > tistringstream;
	typedef basic_ostringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > tostringstream;
	typedef basic_stringstream<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > tstringstream;
	typedef basic_filebuf<wchar_t, char_traits<wchar_t> > tfilebuf;
	typedef basic_ifstream<wchar_t, char_traits<wchar_t> > tifstream;
	typedef basic_ofstream<wchar_t, char_traits<wchar_t> > tofstream;
	typedef basic_fstream<wchar_t, char_traits<wchar_t> > tfstream;
	typedef basic_regex<wchar_t> tregex;
	

#else

#ifndef TEXT
#define TEXT(quote) quote
#endif

#define tcin cin
#define tcout cout
#define tcerr cerr
#define tclog clog
#define to_tstring to_string

#define taccess access
#define tfopen fopen
#define tsystem system

#define ttoi atoi
#define itot itoa

#define tprintf printf
#define vstprintf vsprintf
#define stprintf sprintf
#define tcslen strlen
#define tcscpy strcpy 
#define tcsstr strstr
#define tcschr strchr
#define tcscat strcat
#define tcscmp strcmp
#define tcsicmp stricmp
#define tcsrchr strrchr
#define tcsncpy strncpy 
#define tcsncmp strncmp

	// time.h
#define tcsftime strftime

	typedef char tchar;
	typedef string tstring;
	typedef basic_ios<char, char_traits<char> > tios;
	typedef basic_streambuf<char, char_traits<char> > tstreambuf;
	typedef basic_istream<char, char_traits<char> > tistream;
	typedef basic_ostream<char, char_traits<char> > tostream;
	typedef basic_iostream<char, char_traits<char> > tiostream;
	typedef basic_stringbuf<char, char_traits<char>, allocator<char> > tstringbuf;
	typedef basic_istringstream<char, char_traits<char>, allocator<char> > tistringstream;
	typedef basic_ostringstream<char, char_traits<char>, allocator<char> > tostringstream;
	typedef basic_stringstream<char, char_traits<char>, allocator<char> > tstringstream;
	typedef basic_filebuf<char, char_traits<char> > tfilebuf;
	typedef basic_ifstream<char, char_traits<char> > tifstream;
	typedef basic_ofstream<char, char_traits<char> > tofstream;
	typedef basic_fstream<char, char_traits<char> > tfstream;
	typedef basic_regex<char> tregex;

#endif
}