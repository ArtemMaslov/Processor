#include <stdio.h>
#include <Windows.h>

#include "Assembler.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    puts("���������:");

    AssemblerConstructor("D:\\���� C\\Processor\\tests\\asm_test1.code", 
                         "D:\\���� C\\Processor\\tests\\asm_out.asm",
                         "D:\\���� C\\Processor\\tests\\asm_listing.txt");
}