#ifndef PROCESSOR_CONFIG_H_
#define PROCESSOR_CONFIG_H_


#include "..\Libraries\StackLibrary\Stack.h"

struct ProcessorStructure
{
    char   *code;
    size_t instructionPointer;
    size_t codeLength;
    Stack  stack;
};

void ProcessorConstructor(const char* inputFileName);

#endif