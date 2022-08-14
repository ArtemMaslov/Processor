#ifndef  ASSEMBLER_H_
#define ASSEMBLER_H_


#include <stdio.h>
#include "..\Libraries\StringLibrary\StringLibrary.h"

const size_t FixUpsCount = 512;
const size_t LabelsCount = 128;
const size_t LabelNameLength = 32;

struct Label
{
    char   msg[30];
    size_t ip     = 0;
    bool   inited = false;
};

struct FixUp
{
    size_t LabelIndex = 0;
    long   fileIndex  = 0;
};

struct Assembler
{
    Text   text;
    size_t bufferIndex;
    
    FileHeader header;
    
    Label* labels;
    FixUp* fixUps;

    size_t fixUpIndex;

    FILE* outputFile;
    FILE* listingFile;
};

extern char* listingFileName;
extern char* asmLogsFileName;
extern char* outputFileName;

void AssemblerConstructor(FILE* inputFile, FILE* outputFile, FILE* listingFile, FILE* asmLogFile);


#endif // ! ASSEMBLER_H_
