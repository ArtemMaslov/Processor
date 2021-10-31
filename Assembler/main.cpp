#include <stdio.h>
#include <Windows.h>

#include "Assembler.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    puts("јссемблер:");

    AssemblerConstructor("D:\\язык C\\Processor\\tests\\asm_src\\task_fact.code", 
                         "D:\\язык C\\Processor\\tests\\task_fact.asm",
                         "D:\\язык C\\Processor\\tests\\asm_listing.txt",
                         "D:\\язык C\\Processor\\tests\\AsmLogs.html");
}