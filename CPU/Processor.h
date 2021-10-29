#ifndef PROCESSOR_CONFIG_H_
#define PROCESSOR_CONFIG_H_


#include "..\Libraries\StackLibrary\Stack.h"
#include "..\Libraries\StringLibrary\StringLibrary.h"

struct ProcessorStructure
{
    Text   text;
    size_t instructionPointer;
    Stack  stack;
};

void ProcessorConstructor(const char* inputFileName);

#endif