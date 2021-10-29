#ifndef  ASSEMBLER_H_
#define ASSEMBLER_H_


#include "..\Libraries\StringLibrary\StringLibrary.h"

struct Assembler
{
    Text   text;
    size_t bufferIndex;
};

void AssemblerConstructor(const char* inputFileName, const char* outputFileName, const char* listingFileName);


#endif // ! ASSEMBLER_H_
