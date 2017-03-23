#include "stdafx.h"
#include "CppUnitTest.h"
#include "common.h"
#include <tchar.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace gcommon;

#ifndef TEXT

#ifdef UNICODE
#define TEXT(quote) L##quote 
#else
#define TEXT(quote) quote
#endif

#endif

namespace UnitTest_common
{
	TEST_CLASS(UnitTest_common)
	{
	public:

		TEST_METHOD(Test_htons)
		{
			Assert::AreEqual((short)0x1234, (short)g_htons(0x3412));
			Assert::AreEqual((short)0xeeff, (short)g_htons(0xffee));
		}
		TEST_METHOD(Test_ntohs)
		{
			Assert::AreEqual((short)0x1234, (short)g_ntohs(0x3412));
			Assert::AreEqual((short)0xeeff, (short)g_ntohs(0xffee));
		}
		TEST_METHOD(Test_htonl)
		{
			Assert::AreEqual((unsigned int)0x12345678, g_htonl(0x78563412));
			Assert::AreEqual((unsigned int)0xccddeeff, g_htonl(0xffeeddcc));
		}
		TEST_METHOD(Test_ntohl)
		{
			Assert::AreEqual((unsigned int)0x12345678, g_ntohl(0x78563412));
			Assert::AreEqual((unsigned int)0xccddeeff, g_ntohl(0xffeeddcc));
		}

		TEST_METHOD(Test_GetParaFromARG)
		{

		}

	};
}