#ifndef  ASSEMBLER_H_
#define ASSEMBLER_H_


#include "StringLibrary.h"

struct Assembler
{
    Text   text;
    size_t bufferIndex;
    

};


void AssemblerConstructor(const char* fileName);

void ParseText(Assembler* assembler);


#endif // ! ASSEMBLER_H_
