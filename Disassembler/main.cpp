#include <stdio.h>
#include <Windows.h>

#include "Disassembler.h"

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    puts("������������:");

    DisassemblerConstructor("D:\\���� C\\Processor\\tests\\asm_out.asm", 
                            "D:\\���� C\\Processor\\tests\\disasm.code");
}