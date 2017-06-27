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
                tchar* argv[10] = { NULL };
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
                // argc, argv[], prefix, pos
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
                // ret, out, test_id
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

            const tchar* out = NULL;
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
            //Assert::AreEqual((size_t)0, gcommon::SplitString(NULL,' ').size());
            Assert::AreEqual((size_t)0, gcommon::SplitString("", ' ').size());
            Assert::AreEqual((size_t)1, gcommon::SplitString("abcde", 0).size());
            Assert::AreEqual("abcde", gcommon::SplitString("abcde", 0).at(0).c_str());
            Assert::AreEqual((size_t)1, gcommon::SplitString("abcde", ' ').size());
            Assert::AreEqual("abcde", gcommon::SplitString("abcde", ' ').at(0).c_str());
            Assert::AreEqual((size_t)1, gcommon::SplitString("abcde", 'a').size());
            Assert::AreEqual("bcde", gcommon::SplitString("abcde", 'a').at(0).c_str());
            Assert::AreEqual((size_t)1, gcommon::SplitString("abcde", 'e').size());
            Assert::AreEqual("abcd", gcommon::SplitString("abcde", 'e').at(0).c_str());
            Assert::AreEqual((size_t)2, gcommon::SplitString("abcbe", 'c').size());
            Assert::AreEqual("ab", gcommon::SplitString("abcde", 'c').at(0).c_str());
            Assert::AreEqual("de", gcommon::SplitString("abcde", 'c').at(1).c_str());
            Assert::AreEqual((size_t)3, gcommon::SplitString("abcbe", 'b').size());
            Assert::AreEqual("a", gcommon::SplitString("abcbe", 'b').at(0).c_str());
            Assert::AreEqual("c", gcommon::SplitString("abcbe", 'b').at(1).c_str());
            Assert::AreEqual("e", gcommon::SplitString("abcbe", 'b').at(2).c_str());

            Assert::AreEqual((size_t)0, gcommon::SplitString(L"", ' ').size());
            Assert::AreEqual((size_t)1, gcommon::SplitString(L"abcde", 0).size());
            Assert::AreEqual(L"abcde", gcommon::SplitString(L"abcde", 0).at(0).c_str());
            Assert::AreEqual((size_t)1, gcommon::SplitString(L"abcde", ' ').size());
            Assert::AreEqual(L"abcde", gcommon::SplitString(L"abcde", ' ').at(0).c_str());
            Assert::AreEqual((size_t)1, gcommon::SplitString(L"abcde", 'a').size());
            Assert::AreEqual(L"bcde", gcommon::SplitString(L"abcde", 'a').at(0).c_str());
            Assert::AreEqual((size_t)1, gcommon::SplitString(L"abcde", 'e').size());
            Assert::AreEqual(L"abcd", gcommon::SplitString(L"abcde", 'e').at(0).c_str());
            Assert::AreEqual((size_t)2, gcommon::SplitString(L"abcbe", 'c').size());
            Assert::AreEqual(L"ab", gcommon::SplitString(L"abcde", 'c').at(0).c_str());
            Assert::AreEqual(L"de", gcommon::SplitString(L"abcde", 'c').at(1).c_str());
            Assert::AreEqual((size_t)3, gcommon::SplitString(L"abcbe", 'b').size());
            Assert::AreEqual(L"a", gcommon::SplitString(L"abcbe", 'b').at(0).c_str());
            Assert::AreEqual(L"c", gcommon::SplitString(L"abcbe", 'b').at(1).c_str());
            Assert::AreEqual(L"e", gcommon::SplitString(L"abcbe", 'b').at(2).c_str());
        }
        TEST_METHOD(Test_TrimString)
        {
            string test_set[] = {
                "",
                " ",
                " a b c",
                "a b c ",
                " a b c ",
            };
            string result_set[] = {
                "",
                "",
                "a b c",
                "a b c",
                "a b c",
            };
            for (size_t i = 0; i < 5; i++)
            {
                string& ret = gcommon::TrimString(test_set[i], ' ');
                Assert::AreSame(ret, test_set[i]);
                Assert::AreEqual(result_set[i], ret);
            }
            string test_str = "abc";
            string& ret = gcommon::TrimString(test_str, 'a');
            Assert::AreSame(ret, test_str);
            Assert::AreEqual(string("bc"), ret);

            wstring test_setw[] = {
                L"",
                L" ",
                L" a b c",
                L"a b c ",
                L" a b c ",
            };
            wstring result_setw[] = {
                L"",
                L"",
                L"a b c",
                L"a b c",
                L"a b c",
            };
            for (size_t i = 0; i < test_setw->size(); i++)
            {
                wstring& retw = gcommon::TrimString(test_setw[i], ' ');
                Assert::AreSame(retw, test_setw[i]);
                Assert::AreEqual(result_setw[i], retw);
            }
            wstring test_strw = L"abc";
            wstring& retw = gcommon::TrimString(test_strw, 'a');
            Assert::AreSame(retw, test_strw);
            Assert::AreEqual(wstring(L"bc"), retw);
        }
        TEST_METHOD(Test_RemoveAllChar)
        {
            string test_str = "";
            string& ret = gcommon::RemoveAllChar(test_str, ' ');
            Assert::AreSame(ret, test_str);
            Assert::AreEqual(string(""), ret);
            test_str = " ";
            ret = gcommon::RemoveAllChar(test_str, ' ');
            Assert::AreSame(ret, test_str);
            Assert::AreEqual(string(""), ret);
            test_str = " a b c ";
            ret = gcommon::RemoveAllChar(test_str, ' ');
            Assert::AreSame(ret, test_str);
            Assert::AreEqual(string("abc"), ret);

            wstring test_strw = L"";
            wstring& retw = gcommon::RemoveAllChar(test_strw, ' ');
            Assert::AreSame(retw, test_strw);
            Assert::AreEqual(wstring(L""), retw);
            test_strw = L" ";
            retw = gcommon::RemoveAllChar(test_strw, ' ');
            Assert::AreSame(retw, test_strw);
            Assert::AreEqual(wstring(L""), retw);
            test_strw = L" a b c ";
            retw = gcommon::RemoveAllChar(test_strw, ' ');
            Assert::AreSame(retw, test_strw);
            Assert::AreEqual(wstring(L"abc"), retw);
        }
        TEST_METHOD(Test_ReplaseAllSubString)
        {
            typedef struct _test_set
            {
                string ret;
                string str;
                string subsrc;
                string subdst;
                tstring testid;
            }TEST_SET;
            TEST_SET testset[] =
            {
                { "", "", "abc", "123", TEXT("T1") },
                { "def", "def", "abc", "123", TEXT("T2") },
                { "ppp123qqq", "pppabcqqq", "abc", "123", TEXT("T3") },
                { "pppabcqqq", "pppabcqqq", "abc", "abc", TEXT("T4") },
                { "pppabcqqq", "pppabcqqq", "", "123", TEXT("T5") },
                { "pppqqq", "pppabcqqq", "abc", "", TEXT("T6") },
                { "ppp123ttt123qqq", "pppabctttabcqqq", "abc", "123", TEXT("T7") },
                { "ppptttqqq", "pppabctttabcqqq", "abc", "", TEXT("T8") },
                { "ppp123123qqq", "pppabcabcqqq", "abc", "123", TEXT("T9") },
                { "pppabcqqq", "pppabccqqq", "abc", "ab", TEXT("T10") },
            };

            for (size_t i = 0; i < 10; i++)
            {
                string& ret = gcommon::ReplaseAllSubString(testset[i].str, testset[i].subsrc, testset[i].subdst);
                Assert::AreSame(ret, testset[i].str, testset[i].testid.c_str());
                Assert::AreEqual(testset[i].ret, ret, testset[i].testid.c_str());
            }


            typedef struct _test_setw
            {
                wstring ret;
                wstring str;
                wstring subsrc;
                wstring subdst;
                tstring testid;
            }TEST_SETW;
            TEST_SETW testsetw[] =
            {
                { L"", L"", L"abc", L"123", TEXT("T1") },
                { L"def", L"def", L"abc", L"123", TEXT("T2") },
                { L"ppp123qqq", L"pppabcqqq", L"abc", L"123", TEXT("T3") },
                { L"pppabcqqq", L"pppabcqqq", L"abc", L"abc", TEXT("T4") },
                { L"pppabcqqq", L"pppabcqqq", L"", L"123", TEXT("T5") },
                { L"pppqqq", L"pppabcqqq", L"abc", L"", TEXT("T6") },
                { L"ppp123ttt123qqq", L"pppabctttabcqqq", L"abc", L"123", TEXT("T7") },
                { L"ppptttqqq", L"pppabctttabcqqq", L"abc", L"", TEXT("T8") },
                { L"ppp123123qqq", L"pppabcabcqqq", L"abc", L"123", TEXT("T9") },
                { L"pppabcqqq", L"pppabccqqq", L"abc", L"ab", TEXT("T10") },
            };

            for (size_t i = 0; i < 10; i++)
            {
                wstring& retw = gcommon::ReplaseAllSubString(testsetw[i].str, testsetw[i].subsrc, testsetw[i].subdst);
                Assert::AreSame(retw, testsetw[i].str, testsetw[i].testid.c_str());
                Assert::AreEqual(testsetw[i].ret, retw, testsetw[i].testid.c_str());
            }
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