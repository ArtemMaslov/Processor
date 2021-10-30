#include <stdio.h>

#include "Processor.h"


int main()
{
    puts("ѕроцессор:");

    ProcessorConstructor("D:\\язык C\\Processor\\tests\\asm_out.asm",
                         "D:\\язык C\\Processor\\tests\\StackLibraryLogs.html",
                         "D:\\язык C\\Processor\\tests\\CpuLogs.html");
}