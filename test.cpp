#include <Windows.h>
#include "common.h"
#include "glogger.h"
#include "gcli.h"
using namespace gcommon;

uint32_t ff(const tstring &ss)
{
    printf("aaaa\n");
    return 111;
}

gcli::SHELL_HANDLER handler = {
    TEXT("tttt"), gcli::CMD_TYPE::FULL, ff, TEXT("test cmd"), TEXT("usa 1: aaa\nuse2: bbb")
};

int main()
{
    gcli::GCLI mycli;
    mycli.RegisterHandler(handler);
    mycli.MainLoop();

    getchar();
}