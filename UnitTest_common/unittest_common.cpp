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
            Assert::AreEqual((short)0x1234, (short)gcommon::g_htons(0x3412));
            Assert::AreEqual((short)0xeeff, (short)gcommon::g_htons(0xffee));
        }
        TEST_METHOD(Test_ntohs)
        {
            Assert::AreEqual((short)0x1234, (short)gcommon::g_ntohs(0x3412));
            Assert::AreEqual((short)0xeeff, (short)gcommon::g_ntohs(0xffee));
        }
        TEST_METHOD(Test_htonl)
        {
            Assert::AreEqual((unsigned int)0x12345678, gcommon::g_htonl(0x78563412));
            Assert::AreEqual((unsigned int)0xccddeeff, gcommon::g_htonl(0xffeeddcc));
        }
        TEST_METHOD(Test_ntohl)
        {
            Assert::AreEqual((unsigned int)0x12345678, gcommon::g_ntohl(0x78563412));
            Assert::AreEqual((unsigned int)0xccddeeff, gcommon::g_ntohl(0xffeeddcc));
        }
        TEST_METHOD(Test_GetParaFromARG)
        {
            typedef struct _testset
            {
                int argc;
                tchar* argv[10] = {NULL};
                tchar* prefix;
                int pos;
            }TESTSET;
            typedef struct _resultset
            {
                int ret;
                tchar* out;
                const tchar* err_msg;
            }RESULTSET;

            TESTSET test_set[] =
            {
                { 1,{ NULL }, TEXT("-a"), 1 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, NULL, 1 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT(""), 1 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, NULL, 2 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT(""), 2 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, NULL, 3 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT(""), 3 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT("-b"), 0 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT("-b"), 3 },
                { 0,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT("-b"), 1 },
                { 5,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT("-b"), 1 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT("-c"), 1 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT("aa"), 1 },
                { 4,{ TEXT("-a"), TEXT("-c"), TEXT("-b"), TEXT("bb") }, TEXT("-a"), 1 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT("-a"), 1 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-a"), TEXT("bb") }, TEXT("-a"), 1 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-a"), TEXT("bb") }, TEXT("-a"), 2 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b"), TEXT("bb") }, TEXT("-b"), 1 },
                { 4,{ TEXT("-a"), TEXT("aa"), TEXT("-b1"), TEXT("bb") }, TEXT("-b1"), 1 }
            };

            RESULTSET result_set[] =
            {
                { 0, NULL, TEXT("T1") },
                { 2, TEXT("aa"), TEXT("T2") },
                { 2, TEXT("aa"), TEXT("T3") },
                { 1, TEXT("bb"), TEXT("T4") },
                { 1, TEXT("bb"), TEXT("T5") },
                { 0, NULL, TEXT("T6") },
                { 0, NULL, TEXT("T7") },
                { 0, NULL, TEXT("T8") },
                { 0, NULL, TEXT("T9") },
                { 0, NULL, TEXT("T10") },
                { 1, TEXT("bb"), TEXT("T11") },
                { 0, NULL, TEXT("T12") },
                { 0, NULL, TEXT("T13") },
                { 0, TEXT(""), TEXT("T14") },
                { 1, TEXT("aa"), TEXT("T15") },
                { 2, TEXT("aa"), TEXT("T16") },
                { 1, TEXT("bb"), TEXT("T17") },
                { 1, TEXT("bb"), TEXT("T18") },
                { 1, TEXT("bb"), TEXT("T19") }
            };

            tchar* out = NULL;
            int ret = -3;
            for (size_t i = 0; i < 18; i++)
            {
                ret = gcommon::GetParaFromARG(test_set[i].argc, test_set[i].argv, test_set[i].prefix, out, test_set[i].pos);
                Assert::AreEqual(result_set[i].ret, ret, result_set[i].err_msg);
                Assert::AreEqual(result_set[i].out, out, result_set[i].err_msg);
            }
        }
        TEST_METHOD(Test_inet_ltot)
        {
            tstring ip1 = gcommon::inet_ltot(0x01020304);
            tstring ip2 = gcommon::inet_ltot(0xf1f2f3f4);
            Assert::AreEqual(TEXT("4.3.2.1"), ip1.c_str());
            Assert::AreEqual(TEXT("244.243.242.241"), ip2.c_str());
        }
        TEST_METHOD(Test_inet_ttol)
        {
            Assert::AreEqual((uint32_t)0, gcommon::inet_ttol((const char*)0));
            Assert::AreEqual((uint32_t)0, gcommon::inet_ttol(""));
            Assert::AreEqual((uint32_t)0, gcommon::inet_ttol("aaa"));
            Assert::AreEqual((uint32_t)0, gcommon::inet_ttol("4.3.2.1.0"));
            Assert::AreEqual((uint32_t)0x01020304, gcommon::inet_ttol("4.3.2.1"));
            Assert::AreEqual((uint32_t)0xf1f2f3f4, gcommon::inet_ttol("244.243.242.241"));
            Assert::AreEqual((uint32_t)0, gcommon::inet_ttol((const wchar_t*)0));
            Assert::AreEqual((uint32_t)0, gcommon::inet_ttol(L""));
            Assert::AreEqual((uint32_t)0, gcommon::inet_ttol(L"aaa"));
            Assert::AreEqual((uint32_t)0, gcommon::inet_ttol(L"4.3.2.1.0"));
            Assert::AreEqual((uint32_t)0x01020304, gcommon::inet_ttol(L"4.3.2.1"));
            Assert::AreEqual((uint32_t)0xf1f2f3f4, gcommon::inet_ttol(L"244.243.242.241"));
        }
        TEST_METHOD(Test_random)
        {
            Assert::AreEqual((uint32_t)0, gcommon::random(0, 0));
            Assert::AreEqual((uint32_t)100, gcommon::random(100, 100));
            Assert::AreEqual((uint32_t)10, gcommon::random(10, 9));
            
            vector<uint32_t> rands;        
            for (size_t i = 0; i < 1000; i++)
            {
                rands.push_back(gcommon::random(0, 100));
            }
            Assert::AreEqual((uint32_t)0, *min_element(rands.begin(), rands.end()));
            Assert::AreEqual((uint32_t)100, *max_element(rands.begin(), rands.end()));
            for (uint32_t i = 0; i < 101; i++)
            {
                Assert::IsTrue(rands.end() != find(rands.begin(), rands.end(), i));
            }
            
        }
        TEST_METHOD(Test_wtoa)
        {
            Assert::AreEqual(NULL, gcommon::wtoa(NULL));
            Assert::AreEqual("", gcommon::wtoa(L""));
            Assert::AreEqual("test1", gcommon::wtoa(L"test1"));
            Assert::AreEqual("te", gcommon::wtoa(L"test1", 2));
            Assert::AreEqual("test1", gcommon::wtoa(L"test1"), 10);
        }
        TEST_METHOD(Test_atow)
        {
            Assert::AreEqual(NULL, gcommon::atow(NULL));
            Assert::AreEqual(L"", gcommon::atow(""));
            Assert::AreEqual(L"test1", gcommon::atow("test1"));
            Assert::AreEqual(L"te", gcommon::atow("test1", 2));
            Assert::AreEqual(L"test1", gcommon::atow("test1"), 10);
        }
        TEST_METHOD(Test_SplitString)
        {
            Assert::Fail(TEXT("no test"));
        }
        TEST_METHOD(Test_TrimString)
        {
            Assert::Fail(TEXT("no test"));
        }
        TEST_METHOD(Test_RemoveAllChar)
        {
            Assert::Fail(TEXT("no test"));
        }
        TEST_METHOD(Test_ReplaseAllSubString)
        {
            Assert::Fail(TEXT("no test"));
        }
        TEST_METHOD(Test_GetConfigString)
        {
            Assert::Fail(TEXT("no test"));
        }
        TEST_METHOD(Test_GetConfigInt)
        {
            Assert::Fail(TEXT("no test"));
        }
        TEST_METHOD(Test_SetConfigString)
        {
            Assert::Fail(TEXT("no test"));
        }
        TEST_METHOD(Test_SetConfigInt)
        {
            Assert::Fail(TEXT("no test"));
        }
        TEST_METHOD(Test_DeleteEmptyItems)
        {
            Assert::Fail(TEXT("no test"));
        }
    };
}