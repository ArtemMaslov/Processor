#include <stdio.h>
#include <Windows.h>

#include "Assembler.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    puts("јссемблер:");

    AssemblerConstructor("D:\\язык C\\Processor\\tests\\asm_test1.code", 
                         "D:\\язык C\\Processor\\tests\\asm_out.asm",
                         "D:\\язык C\\Processor\\tests\\asm_listing.txt");
}