#include "stdafx.h"
#include "CppUnitTest.h"
#include "glogger.h"
#include <ctime>
#include "tstream.h"
#include <io.h>
#include <tchar.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace gcommon;

namespace UnitTest_common
{        
    TEST_CLASS(UnitTest_GLogger)
    {
        GLogger glogger;

    public:        

        TEST_METHOD(Test_debug1)
        {
            glogger.setHeader(TEXT("##"));

            glogger.setDebugLevel(0);
            glogger.debug1(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());

            glogger.setDebugLevel(1);
            glogger.debug1(TEXT("test"));
            Assert::AreEqual(TEXT("+1 [##] test\n"), glogger.getLastMsg().data());

            glogger.setDebugLevel(2);
            glogger.debug1(TEXT("test"));
            Assert::AreEqual(TEXT("+1 [##] test\n"), glogger.getLastMsg().data());

            glogger.setDebugLevel(3);
            glogger.debug1(TEXT("test"));
            Assert::AreEqual(TEXT("+1 [##] test\n"), glogger.getLastMsg().data());
        }

        TEST_METHOD(Test_debug2)
        {
            glogger.setHeader(TEXT("##"));
            
            glogger.setDebugLevel(0);
            glogger.debug2(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());

            glogger.setDebugLevel(1);
            glogger.debug2(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());

            glogger.setDebugLevel(2);
            glogger.debug2(TEXT("test"));
            Assert::AreEqual(TEXT("+2 [##] test\n"), glogger.getLastMsg().data());

            glogger.setDebugLevel(3);
            glogger.debug2(TEXT("test"));
            Assert::AreEqual(TEXT("+2 [##] test\n"), glogger.getLastMsg().data());
        }

        TEST_METHOD(Test_debug3)
        {
            glogger.setHeader(TEXT("##"));
            
            glogger.setDebugLevel(0);
            glogger.debug3(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());

            glogger.setDebugLevel(1);
            glogger.debug3(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());

            glogger.setDebugLevel(2);
            glogger.debug3(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());

            glogger.setDebugLevel(3);
            glogger.debug3(TEXT("test"));
            Assert::AreEqual(TEXT("+3 [##] test\n"), glogger.getLastMsg().data());
        }

        TEST_METHOD(Test_error)
        {
            glogger.setHeader(TEXT("##"));

            glogger.error(TEXT("test"));
            Assert::AreEqual(TEXT(" x [##] test\n"), glogger.getLastMsg().data());

            glogger.error(TEXT("test %s %d"), TEXT("arg1"), 1);
            Assert::AreEqual(TEXT(" x [##] test arg1 1\n"), glogger.getLastMsg().data());
        }

        TEST_METHOD(Test_getLastMsg)
        {
            glogger.setHeader(TEXT("##"));

            glogger.info(TEXT("1111"));
            Assert::AreEqual(TEXT("   [##] 1111\n"), glogger.getLastMsg().data());

            //glogger.info(NULL);
            glogger.screen(TEXT(""));
            Assert::IsTrue(glogger.getLastMsg().empty());

            glogger.info(TEXT("2222"));
            Assert::AreEqual(TEXT("   [##] 2222\n"), glogger.getLastMsg().data());
        }

        TEST_METHOD(Test_getLastError)
        {
            glogger.setHeader(TEXT("##"));

            glogger.info(TEXT("1111"));
            Assert::IsTrue(glogger.getLastError().empty());

            glogger.warning(TEXT("2222"));
            Assert::IsTrue(glogger.getLastError().empty());

            glogger.error(TEXT("3333"));
            Assert::AreEqual(TEXT(" x [##] 3333\n"), glogger.getLastError().data());
        }

        TEST_METHOD(Test_getLastWarning)
        {
            glogger.setHeader(TEXT("##"));

            glogger.info(TEXT("1111"));
            Assert::IsTrue(glogger.getLastWarning().empty());

            glogger.warning(TEXT("2222"));
            Assert::AreEqual(TEXT(" ! [##] 2222\n"), glogger.getLastWarning().data());

            glogger.error(TEXT("3333"));
            Assert::AreEqual(TEXT(" x [##] 3333\n"), glogger.getLastWarning().data());
        }

        TEST_METHOD(Test_info)
        {
            glogger.setHeader(TEXT("##"));

            glogger.info(TEXT("test"));
            Assert::AreEqual(TEXT("   [##] test\n"), glogger.getLastMsg().data());

            glogger.info(TEXT("test %s %d"), TEXT("arg1"), 1);
            Assert::AreEqual(TEXT("   [##] test arg1 1\n"), glogger.getLastMsg().data());
        }

        TEST_METHOD(Test_insertCurrentTime)
        {    
            struct tm newtime;
            __time64_t long_time;
            _time64(&long_time);
            _localtime64_s(&newtime, &long_time);

            glogger.insertCurrentTime();
            tstring datetime1 = glogger.getLastMsg();
            glogger.insertCurrentTime(TEXT("yyyy MM dd HH mm ss"));
            tstring datetime2 = glogger.getLastMsg();

            int year1, month1, day1, hour1, min1, second1;
            int year2, month2, day2, hour2, min2, second2;
            _stscanf_s(datetime1.data(), TEXT("** %04d-%02d-%02d %02d:%02d:%02d **\n"),
                &year1, &month1, &day1, &hour1, &min1, &second1);
            _stscanf_s(datetime2.data(), TEXT("%04d %02d %02d %02d %02d %02d\n"),
                &year2, &month2, &day2, &hour2, &min2, &second2);

            Assert::AreEqual(newtime.tm_year+1900, year1);
            Assert::AreEqual(newtime.tm_mon+1, month1);
            Assert::AreEqual(newtime.tm_mday, day1);
            Assert::AreEqual(newtime.tm_hour, hour1);
            Assert::AreEqual(newtime.tm_min, min1);
            Assert::IsTrue(second1 - newtime.tm_sec < 2);

            Assert::AreEqual(newtime.tm_year+1900, year2);
            Assert::AreEqual(newtime.tm_mon+1, month2);
            Assert::AreEqual(newtime.tm_mday, day2);
            Assert::AreEqual(newtime.tm_hour, hour2);
            Assert::AreEqual(newtime.tm_min, min2);
            Assert::IsTrue(second2 - newtime.tm_sec < 2);
        }

        TEST_METHOD(Test_logfile)
        {
            tstring test_max_str;
            tstring test_max_argstr = TEXT("test ");
            tstring test_long_str_ret;

            size_t max_str_len = 1000;
            for (size_t i = 0; i < max_str_len/10-1; i++)
            {
                test_max_str += TEXT("123456789 ");
                test_max_argstr += TEXT("123456789 ");
                test_long_str_ret += TEXT("123456789 ");
            }

            test_max_str += TEXT("123456...");
            test_max_argstr += TEXT("1...");
            test_long_str_ret += TEXT("123456789 123");

            typedef struct _testset
            {
                tstring format;
                const tchar* arg;
            }TESTSET;
            typedef struct _resultset
            {
                tstring str;
                const tchar* err_msg;
            }RESULTSET;

            TESTSET test_set[] = 
            {
                { TEXT("\n"), NULL },
                { TEXT("test\n"), NULL },
                { TEXT("test %s\n"), NULL },
                { TEXT("test %s\n"), TEXT("") },
                { TEXT("test %s\n"), TEXT("arg") },
                { TEXT("test %s\n"), test_long_str_ret.data() },
                { test_long_str_ret, NULL }
            };

            RESULTSET result_set[] =
            {
                { TEXT(""), TEXT("T1") },
                { TEXT("test"), TEXT("T2") },
                { TEXT("test (null)"), TEXT("T3") },
                { TEXT("test "), TEXT("T4") },
                { TEXT("test arg"), TEXT("T5") },
                { test_max_argstr, TEXT("T6") },
                { test_max_str, TEXT("T7") }
            };

            tstring testlogfile = TEXT("testlog.log");
            glogger.setTargetBoth();
            glogger.setLogFile(testlogfile);
            glogger.setHeader(TEXT("##"));
            _tremove(testlogfile.data());

            for each (TESTSET ts in test_set)
            {
                glogger.logfile(ts.format, ts.arg);
            }

            tifstream infile(testlogfile);
            if (infile.bad())
            {
                _tremove(testlogfile.data());
                Assert::IsTrue(false);
                return;
            }

            tchar* line = new tchar[max_str_len+1];
            for each (RESULTSET rs in result_set)
            {
                infile.getline(line, max_str_len, TEXT('\n'));
                Assert::AreEqual(rs.str.data(), line, rs.err_msg);
            }

            infile.close();
            _tremove(testlogfile.data());
        }

        TEST_METHOD(Test_logMessagePool)
        {
            typedef struct _testset
            {
                tstring format;
                const tchar* arg;
            }TESTSET;
            typedef struct _resultset
            {
                tstring str;
                const tchar* err_msg;
            }RESULTSET;

            TESTSET test_set[] =
            {
                { TEXT("aaaa\n"), NULL },
                { TEXT("bbbb\n"), NULL },
                { TEXT("cccc %s\n"), NULL },
                { TEXT("dddd %s\n"), TEXT("") },
                { TEXT("eeee %s\n"), TEXT("arg1") },
                { TEXT("ffff %s\n"), TEXT("arg2") }
            };

            RESULTSET result_set[] =
            {
                { TEXT("aaaa"), TEXT("T1") },
                { TEXT("bbbb"), TEXT("T2") },
                { TEXT("cccc (null)"), TEXT("T3") },
                { TEXT("dddd "), TEXT("T4") },
                { TEXT("eeee arg1"), TEXT("T5") },
                { TEXT("ffff arg2"), TEXT("T6") }
            };

            // 删除日志文件
            tstring testlogfile = TEXT("testlog.log");
            _tremove(testlogfile.data());
            glogger.setLogFile(testlogfile);            

            glogger.clear();
            for each (TESTSET ts in test_set)
            {
                glogger.screen(ts.format, ts.arg);
            }
            glogger.logMessagePool();

            tifstream infile(testlogfile);
            if (infile.bad())
            {
                _tremove(testlogfile.data());
                Assert::IsTrue(false);
                return;
            }

            tchar line[81];
            for each (RESULTSET rs in result_set)
            {
                infile.getline(line, 80, TEXT('\n'));
                Assert::AreEqual(rs.str.data(), line, rs.err_msg);
            }

            infile.close();
            _tremove(testlogfile.data());
        }

        TEST_METHOD(Test_screen)
        {
            tstring test_max_str;
            tstring test_max_str_ret;
            tstring test_max_argstr = TEXT("test ");
            tstring test_long_str;
            tstring test_long_str_ret;

            size_t max_str_len = 1000;
            for (size_t i = 0; i < max_str_len/10-1; i++)
            {
                test_max_str += TEXT("123456789 ");
                test_max_str_ret += TEXT("123456789 ");
                test_max_argstr += TEXT("123456789 ");
                test_long_str += TEXT("123456789 ");
                test_long_str_ret += TEXT("123456789 ");
            }

            test_max_str += TEXT("123456...\n");
            test_max_str_ret += TEXT("123456...\n");
            test_max_argstr += TEXT("1...\n");
            test_long_str += TEXT("123456789 123");
            test_long_str_ret += TEXT("123456789 123\n");

            //glogger.screen(NULL);
            //Assert::IsTrue(glogger.getLastMsg().empty());
            glogger.setTargetBoth();

            glogger.screen(TEXT(""));
            Assert::IsTrue(glogger.getLastMsg().empty());

            glogger.screen(TEXT("test"));
            Assert::AreEqual(TEXT("test"), glogger.getLastMsg().data());

            glogger.screen(TEXT("test %s"), NULL);
            Assert::AreEqual(TEXT("test (null)"), glogger.getLastMsg().data());

            glogger.screen(TEXT("test %s"), TEXT(""));
            Assert::AreEqual(TEXT("test "), glogger.getLastMsg().data());

            glogger.screen(TEXT("test %s"), TEXT("arg"));
            Assert::AreEqual(TEXT("test arg"), glogger.getLastMsg().data());

            glogger.screen(TEXT("test %s"), test_long_str.data());
            Assert::AreEqual(test_max_argstr.data(), glogger.getLastMsg().data());
            
            glogger.screen(test_long_str);
            Assert::AreEqual(test_max_str.data(), glogger.getLastMsg().data());

            glogger.screen(test_long_str_ret);
            Assert::AreEqual(test_max_str_ret.data(), glogger.getLastMsg().data());
        }        

        TEST_METHOD(Test_setDebugLevel)
        {
            glogger.setHeader(TEXT("##"));

            glogger.setDebugLevel(0);            
            glogger.debug1(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());
            glogger.debug2(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());
            glogger.debug3(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());
            glogger.info(TEXT("test"));
            Assert::AreEqual(TEXT("   [##] test\n"), glogger.getLastMsg().data());

            glogger.clear();
            glogger.setDebugLevel(1);            
            glogger.debug2(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());
            glogger.debug3(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());
            glogger.info(TEXT("test"));
            Assert::AreEqual(TEXT("   [##] test\n"), glogger.getLastMsg().data());
            glogger.debug1(TEXT("test"));
            Assert::AreEqual(TEXT("+1 [##] test\n"), glogger.getLastMsg().data());

            glogger.clear();
            glogger.setDebugLevel(2);            
            glogger.debug3(TEXT("test"));
            Assert::IsTrue(glogger.getLastMsg().empty());
            glogger.info(TEXT("test"));
            Assert::AreEqual(TEXT("   [##] test\n"), glogger.getLastMsg().data());
            glogger.debug1(TEXT("test"));
            Assert::AreEqual(TEXT("+1 [##] test\n"), glogger.getLastMsg().data());
            glogger.debug2(TEXT("test"));
            Assert::AreEqual(TEXT("+2 [##] test\n"), glogger.getLastMsg().data());

            glogger.clear();
            glogger.setDebugLevel(3);
            glogger.info(TEXT("test"));
            Assert::AreEqual(TEXT("   [##] test\n"), glogger.getLastMsg().data());
            glogger.debug1(TEXT("test"));
            Assert::AreEqual(TEXT("+1 [##] test\n"), glogger.getLastMsg().data());
            glogger.debug2(TEXT("test"));
            Assert::AreEqual(TEXT("+2 [##] test\n"), glogger.getLastMsg().data());
            glogger.debug3(TEXT("test"));
            Assert::AreEqual(TEXT("+3 [##] test\n"), glogger.getLastMsg().data());
        }

        TEST_METHOD(Test_setHeader)
        {
            size_t max_head_len = 16;
            tstring test_max_head;
            tstring test_long_head;
            tstring test_verylong_head;
            for (size_t i = 0; i < max_head_len; i++)
            {
                test_max_head += TEXT("h"); 
                test_long_head += TEXT("h");
                test_verylong_head += TEXT("h");
            }
            test_long_head += TEXT("h");
            test_verylong_head += test_long_head;

            tstring test_maxhead_result = TEXT("   [");
            test_maxhead_result += test_max_head;
            test_maxhead_result += TEXT("] test\n");

            //glogger.setHeader(NULL);
            //glogger.info(TEXT("test"));
            //Assert::AreEqual(TEXT("   [] test\n"), glogger.getLastMsg().data());

            glogger.setHeader(TEXT(""));
            glogger.info(TEXT("test"));
            Assert::AreEqual(TEXT("   [] test\n"), glogger.getLastMsg().data());

            glogger.setHeader(TEXT("hh"));
            glogger.info(TEXT("test"));
            Assert::AreEqual(TEXT("   [hh] test\n"), glogger.getLastMsg().data());

            glogger.setHeader(test_max_head);
            glogger.info(TEXT("test"));
            Assert::AreEqual(test_maxhead_result.data(), glogger.getLastMsg().data());

            glogger.setHeader(test_long_head);
            glogger.info(TEXT("test"));
            Assert::AreEqual(TEXT("   [##] test\n"), glogger.getLastMsg().data());
            
            glogger.setHeader(test_verylong_head);
            glogger.info(TEXT("test"));
            Assert::AreEqual(TEXT("   [##] test\n"), glogger.getLastMsg().data());

        }

        TEST_METHOD(Test_setLogDebugLevel)
        {
            tstring testlogfile = TEXT("testlog.log");
            glogger.setTargetBoth();
            glogger.setLogFile(testlogfile);
            glogger.setHeader(TEXT("##"));
            _tremove(testlogfile.data());                                

            for (size_t i = 0; i < 4; i++)
            {
                glogger.setDebugLevel(i);
                glogger.info(TEXT("test"));
                glogger.debug1(TEXT("test"));
                glogger.debug2(TEXT("test"));
                glogger.debug3(TEXT("test"));
            }

            tifstream infile(testlogfile);
            if (infile.bad())
            {
                _tremove(testlogfile.data());
                Assert::IsTrue(false);
                return;
            }

            tchar line[81];
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("   [##] test"), line);
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("   [##] test"), line);
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("+1 [##] test"), line);
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("   [##] test"), line);
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("+1 [##] test"), line);
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("+2 [##] test"), line);
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("   [##] test"), line);
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("+1 [##] test"), line);
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("+2 [##] test"), line);
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("+3 [##] test"), line);

            infile.close();
            _tremove(testlogfile.data());
        }

        TEST_METHOD(Test_setLogFile)
        {
            //Assert::IsTrue(false);
            tstring testlogfile = TEXT("testlog.log");
            glogger.setTargetBoth();
            glogger.setLogFile(testlogfile);
            glogger.setHeader(TEXT("##"));

            _tremove(testlogfile.data());

            glogger.info(TEXT("logtest"));
            tifstream infile(testlogfile);
            if (infile.bad())
            {                
                _tremove(testlogfile.data());
                Assert::IsTrue(false);
                return;
            }

            tchar line[81];
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("   [##] logtest"), line);

            infile.close();
            _tremove(testlogfile.data());
        }

        TEST_METHOD(Test_setOnlyScreen)
        {
            tstring testlogfile = TEXT("testlog.log");            
            glogger.setLogFile(testlogfile);
            glogger.setHeader(TEXT("##"));
            _tremove(testlogfile.data());

            glogger.setOnlyScreen();
            glogger.info(TEXT("logtest"));            
            Assert::AreEqual(-1, _taccess(testlogfile.data(), 0));

            _tremove(testlogfile.data());
        }

        TEST_METHOD(Test_setOnlyLogfile)
        {
            tstring testlogfile = TEXT("testlog.log");
            glogger.setLogFile(testlogfile);
            glogger.setHeader(TEXT("##"));

            _tremove(testlogfile.data());
            glogger.setOnlyLogfile();
            glogger.info(TEXT("logtest"));
            tifstream infile(testlogfile);
            if (infile.bad())
            {
                _tremove(testlogfile.data());
                Assert::IsTrue(false);
                return;
            }

            tchar line[81];
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("   [##] logtest"), line);

            infile.close();
            _tremove(testlogfile.data());
        }

        TEST_METHOD(Test_setTargetBoth)
        {
            tstring testlogfile = TEXT("testlog.log");
            glogger.setLogFile(testlogfile);
            glogger.setHeader(TEXT("##"));
            _tremove(testlogfile.data());

            glogger.setTargetBoth();
            glogger.info(TEXT("logtest"));
            tifstream infile(testlogfile);
            if (infile.bad())
            {
                _tremove(testlogfile.data());
                Assert::IsTrue(false);
                return;
            }

            tchar line[81];
            infile.getline(line, 80, TEXT('\n'));
            Assert::AreEqual(TEXT("   [##] logtest"), line);

            infile.close();
            _tremove(testlogfile.data());
        }

        TEST_METHOD(Test_warning)
        {
            glogger.setHeader(TEXT("##"));

            glogger.warning(TEXT("test"));
            Assert::AreEqual(TEXT(" ! [##] test\n"), glogger.getLastMsg().data());

            glogger.warning(TEXT("test %s %d"), TEXT("arg1"), 1);
            Assert::AreEqual(TEXT(" ! [##] test arg1 1\n"), glogger.getLastMsg().data());
        }
    };
}