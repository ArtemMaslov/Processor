#ifndef  ASSEMBLER_H_
#define ASSEMBLER_H_


#include "..\Libraries\StringLibrary\StringLibrary.h"

struct Assembler
{
    Text   text;
    size_t bufferIndex;
    

};


void AssemblerConstructor(const char* fileName, const char* outputFileName);




#endif // ! ASSEMBLER_H_
