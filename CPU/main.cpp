#include <stdio.h>
#include <Windows.h>

#include "Processor.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    puts("���������:");

    ProcessorConstructor("code.asm",
                         "D:\\���� C\\Processor\\tests\\StackLibraryLogs.html",
                         "D:\\���� C\\Processor\\tests\\CpuLogs.html");
}