#include <stdio.h>
#include <Windows.h>

#include "Processor.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    puts("���������:");

    ProcessorConstructor("D:\\���� C\\Processor\\tests\\task_fact.asm",
                         "D:\\���� C\\Processor\\tests\\StackLibraryLogs.html",
                         "D:\\���� C\\Processor\\tests\\CpuLogs.html");
}