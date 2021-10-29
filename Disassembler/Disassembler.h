#ifndef DISASSEMBLER_H_
#define DISASSEMBLER_H_


#include "..\Libraries\StringLibrary\StringLibrary.h"

struct Disassembler
{
    Text   text;
    size_t instructionPointer;
};


void DisassemblerConstructor(const char* fileName, const char* outputFileName);




#endif // ! DISASSEMBLER_H_

