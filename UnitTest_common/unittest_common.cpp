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
	TEST_CLASS(UnitTest_Common)
	{
	public:

		TEST_METHOD(Test_ByteBuffer1)
		{
			Assert::IsTrue(false, L"no test");
		}
	};
}