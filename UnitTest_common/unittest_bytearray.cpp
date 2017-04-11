#include "stdafx.h"
#include "CppUnitTest.h"
#include "bytearray.h"
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
    TEST_CLASS(UnitTest_ByteArray)
    {
    public:
        
        TEST_METHOD(Test_ByteArray1)
        {
            ByteArray array((uint32_t)0);
            Assert::AreEqual((uint32_t)1, array.capacity());
            Assert::AreEqual((uint32_t)0, array.size());
            Assert::AreNotEqual((uint8_t*)NULL, array.getRawPointer());

            ByteArray array2(10);
            Assert::AreEqual((uint32_t)10, array2.capacity());
            Assert::AreEqual((uint32_t)0, array2.size());
            Assert::AreNotEqual((uint8_t*)NULL, array2.getRawPointer());

            // 无法测试，运行时错误
            //ByteArray array3(0xffffffff);
            //Assert::AreEqual((uint32_t)0, array3.capacity());
            //Assert::AreEqual((uint32_t)0, array3.size());
            //Assert::AreEqual((uint8_t*)NULL, array3.getRawPointer());
        }

        TEST_METHOD(Test_ByteArray2)
        {
            ByteArray array1(0, (uint8_t)0x41);
            Assert::AreEqual((uint32_t)1, array1.capacity());
            Assert::AreEqual((uint32_t)0, array1.size());
            Assert::AreNotEqual((uint8_t*)NULL, array1.getRawPointer());
            Assert::AreEqual((uint8_t)0, array1.getRawPointer()[0]);

            ByteArray array2(10, (uint8_t)'b');
            Assert::AreEqual((uint32_t)10, array2.capacity());
            Assert::AreEqual((uint32_t)10, array2.size());
            Assert::AreNotEqual((uint8_t*)NULL, array2.getRawPointer());
            Assert::AreEqual(0, memcmp(array2.getRawPointer(),"bbbbbbbbbb",10));
        }

        TEST_METHOD(Test_ByteArray3)
        {
            typedef struct _testset
            {
                const uint8_t* str;
                uint32_t size;
            }TESTSET;
            
            typedef struct _resultset
            {
                const uint8_t* str;
                uint32_t size;
                uint32_t capacity;                
                const wchar_t* err_msg;
            }RESULTSET;

            TESTSET test_set[] =
            {
                { (const uint8_t*)NULL, 0 },
                { (const uint8_t*)NULL, 10 },
                { (const uint8_t*)"abcdef", 0 },
                { (const uint8_t*)"abcdef", 3 }
            };        

            RESULTSET result_set[] =
            {
                { (const uint8_t*)NULL, 0, 1, L"T1: " },
                { (const uint8_t*)NULL, 0, 1, L"T2: " },
                { (const uint8_t*)"", 0, 1, L"T3: " },
                { (const uint8_t*)"abc", 3, 3, L"T4: " },
            };

            size_t test_count = sizeof(test_set) / sizeof(TESTSET);
            ByteArray* testarray = NULL;
            for (size_t i = 0; i < test_count; i++)
            {
                testarray = new ByteArray(test_set[i].str, test_set[i].size);

                Assert::AreEqual(result_set[i].capacity, testarray->capacity(), 
                    result_set[i].err_msg);

                Assert::AreEqual(result_set[i].size,  testarray->size(), 
                    result_set[i].err_msg);

                Assert::AreNotEqual((uint8_t*)NULL,  testarray->getRawPointer(), 
                    result_set[i].err_msg);

                if (test_set[i].str != NULL)
                {
                    Assert::AreEqual(0,
                        memcmp(testarray->getRawPointer(), test_set[i].str, test_set[i].size),
                        result_set[i].err_msg);
                }
                
                if(testarray) delete testarray;
            }
        }

        TEST_METHOD(Test_ByteArray4)
        {
            typedef struct _testset
            {
                const uint8_t* str;
                uint32_t size;
                uint32_t capacity;
            }TESTSET;

            typedef struct _resultset
            {
                const uint8_t* str;
                uint32_t size;
                uint32_t capacity;
                const wchar_t* err_msg;
            }RESULTSET;

            TESTSET test_set[] =
            {
                { (const uint8_t*)NULL, 0, 0 },
                { (const uint8_t*)NULL, 0, 3 },
                { (const uint8_t*)NULL, 3, 0 },
                { (const uint8_t*)NULL, 3, 3 },
                { (const uint8_t*)NULL, 10, 3 },
                { (const uint8_t*)NULL, 3, 10 },
                { (const uint8_t*)"abcdef", 0, 0 },
                { (const uint8_t*)"abcdef", 0, 3 },
                { (const uint8_t*)"abcdef", 3, 0 },
                { (const uint8_t*)"abcdef", 3, 3 },
                { (const uint8_t*)"abcdef", 6, 3 },
                { (const uint8_t*)"abcdef", 3, 6 },
            };            

            RESULTSET result_set[] =
            {
                { (const uint8_t*)NULL, 0, 1, L"T1: " },
                { (const uint8_t*)NULL, 0, 3, L"T2: " },
                { (const uint8_t*)NULL, 0, 1, L"T3: " },
                { (const uint8_t*)NULL, 0, 3, L"T4: " },
                { (const uint8_t*)NULL, 0, 3, L"T5: " },
                { (const uint8_t*)NULL, 0, 10, L"T6: " },
                { (const uint8_t*)"", 0, 1, L"T7: " },
                { (const uint8_t*)"", 0, 3, L"T8: " },
                { (const uint8_t*)"", 0, 1, L"T9: " },
                { (const uint8_t*)"abc", 3, 3, L"T10: " },
                { (const uint8_t*)"abc", 3, 3, L"T11: " },
                { (const uint8_t*)"abc", 3, 6, L"T12: " }
            };

            size_t test_count = sizeof(test_set) / sizeof(TESTSET);
            ByteArray* testarray = NULL;
            for (size_t i = 0; i < test_count; i++)
            {
                testarray = new ByteArray(test_set[i].str, test_set[i].size, test_set[i].capacity);

                Assert::AreEqual(result_set[i].capacity, testarray->capacity(), 
                    result_set[i].err_msg);

                Assert::AreEqual(result_set[i].size, testarray->size(), 
                    result_set[i].err_msg);

                Assert::AreNotEqual((uint8_t*)NULL, testarray->getRawPointer(), 
                    result_set[i].err_msg);

                if (test_set[i].str != NULL)
                {
                    Assert::AreEqual(0,
                        memcmp(testarray->getRawPointer(), result_set[i].str, result_set[i].size),
                        result_set[i].err_msg);
                }

                if(testarray) delete testarray;
            }
        }

        TEST_METHOD(Test_ByteArray5)
        {
            ByteArray test("test", 4, 10);
            ByteArray newtest(test);
            Assert::AreEqual(test.size(), newtest.size());
            Assert::AreEqual(test.capacity(), newtest.capacity());
            Assert::AreEqual(0, memcmp(test.getRawPointer(), newtest.getRawPointer(), test.size()));
            Assert::AreNotSame(test.getRawPointer(), newtest.getRawPointer());
        }

        TEST_METHOD(Test_getRawPointer) 
        {
            uint8_t teststr[] = "test";
            ByteArray test(teststr, 4, 10);
            uint8_t* point = test.getRawPointer();
            Assert::AreNotEqual((uint8_t*)NULL, point);
            Assert::AreNotEqual((uint8_t*)teststr, point);
            Assert::AreEqual(0, memcmp(teststr, point, 4));
        }
        
        TEST_METHOD(Test_operator_eq)
        {
            typedef struct _testset
            {
                ByteArray left;
                ByteArray right;
            }TESTSET;

            typedef struct _resultset
            {
                bool result;
                const wchar_t* err_msg;
            }RESULTSET;

            TESTSET test_set[] =
            {
                { *(new ByteArray((uint32_t)0)), *(new ByteArray((uint32_t)0)) },
                { *(new ByteArray((uint32_t)0)), *(new ByteArray(1)) },
                { *(new ByteArray(4)), *(new ByteArray(4)) },
                { *(new ByteArray(4)), *(new ByteArray(10)) },
                { *(new ByteArray(0, (uint8_t)'a')), *(new ByteArray(0, (uint8_t)'a')) },
                { *(new ByteArray(0, (uint8_t)'a')), *(new ByteArray(0, (uint8_t)'b')) },
                { *(new ByteArray(4, (uint8_t)'a')), *(new ByteArray(4, (uint8_t)'a')) },
                { *(new ByteArray((uint8_t *)NULL, 0)), *(new ByteArray((uint8_t *)NULL, 0)) },
                { *(new ByteArray((uint8_t *)NULL, 0)), *(new ByteArray((uint8_t *)NULL, 4)) },
                { *(new ByteArray((uint8_t *)NULL, 4)), *(new ByteArray((uint8_t *)NULL, 4)) },
                { *(new ByteArray((uint8_t *)"test", 4)), *(new ByteArray((uint8_t *)"test", 4)) },
                { *(new ByteArray((uint8_t *)NULL, 0, 0)), *(new ByteArray((uint8_t *)NULL, 0, 0)) },
                { *(new ByteArray((uint8_t *)NULL, 0, 0)), *(new ByteArray((uint8_t *)NULL, 0, 1)) },
                { *(new ByteArray((uint8_t *)NULL, 4, 4)), *(new ByteArray((uint8_t *)NULL, 4, 4)) },
                { *(new ByteArray((uint8_t *)NULL, 4, 4)), *(new ByteArray((uint8_t *)NULL, 4, 10)) },
                { *(new ByteArray((uint8_t *)"test", 4, 10)), *(new ByteArray((uint8_t *)"test", 4, 10)) },
                { *(new ByteArray((uint8_t *)"test", 4, 10)), *(new ByteArray((uint8_t *)"test", 4, 11)) },
                { *(new ByteArray((uint8_t *)"testa", 4, 10)), *(new ByteArray((uint8_t *)"testb", 4, 10)) },
                
                { *(new ByteArray(0, (uint8_t)'a')), *(new ByteArray(1, (uint8_t)'a')) },
                { *(new ByteArray(4, (uint8_t)'a')), *(new ByteArray(4, (uint8_t)'b')) },
                { *(new ByteArray(4, (uint8_t)'a')), *(new ByteArray(10, (uint8_t)'a')) },
                { *(new ByteArray((uint8_t *)"test", 2)), *(new ByteArray((uint8_t *)"test", 4)) },                
                { *(new ByteArray((uint8_t *)"test", 2, 10)), *(new ByteArray((uint8_t *)"test", 4, 10)) },
                { *(new ByteArray((uint8_t *)"testa", 5, 10)), *(new ByteArray((uint8_t *)"testb", 5, 10)) },
            };

            RESULTSET result_set[] =
            {
                { true, L"T 1" },
                { true, L"T 2" },
                { true, L"T 3" },
                { true, L"T 4" },
                { true, L"T 5" },
                { true, L"T 6" },
                { true, L"T 7" },
                { true, L"T 8" },
                { true, L"T 9" },
                { true, L"T 10" },
                { true, L"T 11" },
                { true, L"T 12" },
                { true, L"T 13" },
                { true, L"T 14" },
                { true, L"T 15" },
                { true, L"T 16" },
                { true, L"T 17" },
                { true, L"T 18" },
                { false, L"T 19" },
                { false, L"T 20" },
                { false, L"T 21" },
                { false, L"T 22" },
                { false, L"T 23" },
                { false, L"T 24" },
            };

            size_t test_count = sizeof(test_set) / sizeof(TESTSET);
            for (size_t i = 0; i < test_count; i++)
            {
                Assert::AreEqual(result_set[i].result, test_set[i].left == test_set[i].right,
                    result_set[i].err_msg);
            }
        }
        
        TEST_METHOD(Test_operator_lt)
        {
            typedef struct _testset
            {
                ByteArray left;
                ByteArray right;
            }TESTSET;

            typedef struct _resultset
            {
                bool result;
                const wchar_t* err_msg;
            }RESULTSET;

            TESTSET test_set[] =
            {
                { *(new ByteArray(0, (uint8_t)'a')), *(new ByteArray(1, (uint8_t)'a')) },
                { *(new ByteArray(4, (uint8_t)'a')), *(new ByteArray(4, (uint8_t)'b')) },
                { *(new ByteArray(4, (uint8_t)'a')), *(new ByteArray(10, (uint8_t)'a')) },
                { *(new ByteArray((uint8_t *)"test", 2)), *(new ByteArray((uint8_t *)"test", 4)) },
                { *(new ByteArray((uint8_t *)"test", 2, 10)), *(new ByteArray((uint8_t *)"test", 4, 10)) },
                { *(new ByteArray((uint8_t *)"testa", 5, 10)), *(new ByteArray((uint8_t *)"testb", 5, 10)) },

                { *(new ByteArray((uint32_t)0)), *(new ByteArray((uint32_t)0)) },
                { *(new ByteArray((uint32_t)0)), *(new ByteArray(1)) },
                { *(new ByteArray(4)), *(new ByteArray(4)) },
                { *(new ByteArray(4)), *(new ByteArray(10)) },
                { *(new ByteArray(0, (uint8_t)'a')), *(new ByteArray(0, (uint8_t)'a')) },
                { *(new ByteArray(0, (uint8_t)'a')), *(new ByteArray(0, (uint8_t)'b')) },
                { *(new ByteArray(4, (uint8_t)'a')), *(new ByteArray(4, (uint8_t)'a')) },
                { *(new ByteArray((uint8_t *)NULL, 0)), *(new ByteArray((uint8_t *)NULL, 0)) },
                { *(new ByteArray((uint8_t *)NULL, 0)), *(new ByteArray((uint8_t *)NULL, 4)) },
                { *(new ByteArray((uint8_t *)NULL, 4)), *(new ByteArray((uint8_t *)NULL, 4)) },
                { *(new ByteArray((uint8_t *)"test", 4)), *(new ByteArray((uint8_t *)"test", 4)) },
                { *(new ByteArray((uint8_t *)NULL, 0, 0)), *(new ByteArray((uint8_t *)NULL, 0, 0)) },
                { *(new ByteArray((uint8_t *)NULL, 0, 0)), *(new ByteArray((uint8_t *)NULL, 0, 1)) },
                { *(new ByteArray((uint8_t *)NULL, 4, 4)), *(new ByteArray((uint8_t *)NULL, 4, 4)) },
                { *(new ByteArray((uint8_t *)NULL, 4, 4)), *(new ByteArray((uint8_t *)NULL, 4, 10)) },
                { *(new ByteArray((uint8_t *)"test", 4, 10)), *(new ByteArray((uint8_t *)"test", 4, 10)) },
                { *(new ByteArray((uint8_t *)"test", 4, 10)), *(new ByteArray((uint8_t *)"test", 4, 11)) },
                { *(new ByteArray((uint8_t *)"testa", 4, 10)), *(new ByteArray((uint8_t *)"testb", 4, 10)) },                                
            };

            RESULTSET result_set[] =
            {
                { true, L"T 19" },
                { true, L"T 20" },
                { true, L"T 21" },
                { true, L"T 22" },
                { true, L"T 23" },
                { true, L"T 24" },

                { false, L"T 1" },
                { false, L"T 2" },
                { false, L"T 3" },
                { false, L"T 4" },
                { false, L"T 5" },
                { false, L"T 6" },
                { false, L"T 7" },
                { false, L"T 8" },
                { false, L"T 9" },
                { false, L"T 10" },
                { false, L"T 11" },
                { false, L"T 12" },
                { false, L"T 13" },
                { false, L"T 14" },
                { false, L"T 15" },
                { false, L"T 16" },
                { false, L"T 17" },
                { false, L"T 18" },                
            };

            size_t test_count = sizeof(test_set) / sizeof(TESTSET);
            for (size_t i = 0; i < test_count; i++)
            {
                Assert::AreEqual(result_set[i].result, test_set[i].left < test_set[i].right,
                    result_set[i].err_msg);
            }
        }
        
        TEST_METHOD(Test_operator_bt)
        {
            typedef struct _testset
            {
                ByteArray left;
                ByteArray right;
            }TESTSET;

            typedef struct _resultset
            {
                bool result;
                const wchar_t* err_msg;
            }RESULTSET;

            TESTSET test_set[] =
            {
                { *(new ByteArray(1, (uint8_t)'a')), *(new ByteArray(0, (uint8_t)'a')) },
                { *(new ByteArray(4, (uint8_t)'b')), *(new ByteArray(4, (uint8_t)'a')) },
                { *(new ByteArray(10, (uint8_t)'a')), *(new ByteArray(4, (uint8_t)'a')) },
                { *(new ByteArray((uint8_t *)"test", 4)), *(new ByteArray((uint8_t *)"test", 2)) },
                { *(new ByteArray((uint8_t *)"test", 4, 10)), *(new ByteArray((uint8_t *)"test", 2, 10)) },
                { *(new ByteArray((uint8_t *)"testb", 5, 10)), *(new ByteArray((uint8_t *)"testa", 5, 10)) },

                { *(new ByteArray((uint32_t)0)), *(new ByteArray((uint32_t)0)) },
                { *(new ByteArray((uint32_t)0)), *(new ByteArray(1)) },
                { *(new ByteArray(4)), *(new ByteArray(4)) },
                { *(new ByteArray(4)), *(new ByteArray(10)) },
                { *(new ByteArray(0, (uint8_t)'a')), *(new ByteArray(0, (uint8_t)'a')) },
                { *(new ByteArray(0, (uint8_t)'a')), *(new ByteArray(0, (uint8_t)'b')) },
                { *(new ByteArray(4, (uint8_t)'a')), *(new ByteArray(4, (uint8_t)'a')) },
                { *(new ByteArray((uint8_t *)NULL, 0)), *(new ByteArray((uint8_t *)NULL, 0)) },
                { *(new ByteArray((uint8_t *)NULL, 0)), *(new ByteArray((uint8_t *)NULL, 4)) },
                { *(new ByteArray((uint8_t *)NULL, 4)), *(new ByteArray((uint8_t *)NULL, 4)) },
                { *(new ByteArray((uint8_t *)"test", 4)), *(new ByteArray((uint8_t *)"test", 4)) },
                { *(new ByteArray((uint8_t *)NULL, 0, 0)), *(new ByteArray((uint8_t *)NULL, 0, 0)) },
                { *(new ByteArray((uint8_t *)NULL, 0, 0)), *(new ByteArray((uint8_t *)NULL, 0, 1)) },
                { *(new ByteArray((uint8_t *)NULL, 4, 4)), *(new ByteArray((uint8_t *)NULL, 4, 4)) },
                { *(new ByteArray((uint8_t *)NULL, 4, 4)), *(new ByteArray((uint8_t *)NULL, 4, 10)) },
                { *(new ByteArray((uint8_t *)"test", 4, 10)), *(new ByteArray((uint8_t *)"test", 4, 10)) },
                { *(new ByteArray((uint8_t *)"test", 4, 10)), *(new ByteArray((uint8_t *)"test", 4, 11)) },
                { *(new ByteArray((uint8_t *)"testa", 4, 10)), *(new ByteArray((uint8_t *)"testb", 4, 10)) },
            };

            RESULTSET result_set[] =
            {
                { true, L"T 19" },
                { true, L"T 20" },
                { true, L"T 21" },
                { true, L"T 22" },
                { true, L"T 23" },
                { true, L"T 24" },

                { false, L"T 1" },
                { false, L"T 2" },
                { false, L"T 3" },
                { false, L"T 4" },
                { false, L"T 5" },
                { false, L"T 6" },
                { false, L"T 7" },
                { false, L"T 8" },
                { false, L"T 9" },
                { false, L"T 10" },
                { false, L"T 11" },
                { false, L"T 12" },
                { false, L"T 13" },
                { false, L"T 14" },
                { false, L"T 15" },
                { false, L"T 16" },
                { false, L"T 17" },
                { false, L"T 18" },
            };

            size_t test_count = sizeof(test_set) / sizeof(TESTSET);
            for (size_t i = 0; i < test_count; i++)
            {
                Assert::AreEqual(result_set[i].result, test_set[i].left > test_set[i].right,
                    result_set[i].err_msg);
            }
        }
        
        TEST_METHOD(Test_operator_id)
        {
            ByteArray test1("abcdefg", 7);
            Assert::AreEqual((uint8_t)'a', test1[0]);
            Assert::AreEqual((uint8_t)'b', test1[1]);
            Assert::AreEqual((uint8_t)'c', test1[2]);
            Assert::AreEqual((uint8_t)'d', test1[3]);
            Assert::AreEqual((uint8_t)'e', test1[4]);
            Assert::AreEqual((uint8_t)'f', test1[5]);
            Assert::AreEqual((uint8_t)'g', test1[6]);
            Assert::AreEqual((uint8_t)0, test1[7]);
            Assert::AreEqual((uint8_t)0, test1[1000]);
        }
        
        TEST_METHOD(Test_clone)
        {
            ByteArray test1("test123", 7);
            auto_ptr<ByteArray> test2 = test1.clone();
            Assert::AreEqual(test1.size(), test2->size());
            Assert::AreEqual(test1.capacity(), test2->capacity());
            Assert::AreNotEqual(test1.getRawPointer(), test2->getRawPointer());
            Assert::AreEqual(0, memcmp(test1.getRawPointer(), test2->getRawPointer(), test1.size()));
        }
        
        TEST_METHOD(Test_size)
        {            
            ByteArray test1("test123", 7);
            Assert::AreEqual((uint32_t)7, test1.size());
            ByteArray test2("test123", 3, 10);
            Assert::AreEqual((uint32_t)3, test2.size());
        }
        
        TEST_METHOD(Test_capacity)
        {
            ByteArray test1("test123", 7);
            Assert::AreEqual((uint32_t)7, test1.capacity());
            ByteArray test2("test123", 3, 10);
            Assert::AreEqual((uint32_t)10, test2.capacity());
        }
        
        TEST_METHOD(Test_setSize)
        {
            ByteArray test1("test123", 7);
            Assert::AreEqual((uint32_t)7, test1.size());
            test1.setSize(3);
            Assert::AreEqual((uint32_t)3, test1.size());
            test1.setSize(100);
            Assert::AreEqual((uint32_t)7, test1.size());
        }
    };
}