#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ProcessorConfig.h"


void ProcessorConstructor(ProcessorStructure *proc, char *fileName)
{
    FILE *file = nullptr;
    errno_t fileError = fopen_s(&file, fileName, "rb");

    
    fclose(file);
}

void ParseProcessor(ProcessorStructure *proc)
{
    char *ptr = proc->code;
    char *end = proc->code + proc->codeLength;
    while (proc->code)
    {

    }
}

void GetNextCommand(char* code)
{

}
