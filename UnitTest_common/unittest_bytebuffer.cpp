#include "stdafx.h"
#include "CppUnitTest.h"
#include "bytebuffer.h"
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
	TEST_CLASS(UnitTest_ByteBuffer)
	{
	public:

		TEST_METHOD(Test_ByteBuffer1)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_ByteBuffer2)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_ByteBuffer3)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_writeBuffer)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_writeByteArray)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_writeInt8)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_writeInt16)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_writeInt32)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_fillBytes)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_readBuffer)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_readByteArray)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_readInt8)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_readInt16)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_readInt32)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_getWritePosition)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_setWritePosition)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_getReadPosition)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_setReadPosition)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_reset)
		{
			Assert::IsTrue(false, L"no test");
		}

		TEST_METHOD(Test_skip)
		{
			Assert::IsTrue(false, L"no test");
		}

	};
}