#include <stdio.h>
#include <Windows.h>

#include "Assembler.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    puts("���������:");

    AssemblerConstructor("D:\\���� C\\Processor\\tests\\asm_src\\task_fact.code", 
                         "D:\\���� C\\Processor\\tests\\task_fact.asm",
                         "D:\\���� C\\Processor\\tests\\asm_listing.txt",
                         "D:\\���� C\\Processor\\tests\\AsmLogs.html");
}