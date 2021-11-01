#include <stdio.h>
#include <Windows.h>

#include "Processor.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    puts("ѕроцессор:");

    ProcessorConstructor("code.asm",
                         "D:\\язык C\\Processor\\tests\\StackLibraryLogs.html",
                         "D:\\язык C\\Processor\\tests\\CpuLogs.html");
}