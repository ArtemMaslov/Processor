#ifndef PROCESSOR_CONFIG_H_
#define PROCESSOR_CONFIG_H_


#include "..\Libraries\StackLibrary\Stack.h"
#include "..\Libraries\StringLibrary\StringLibrary.h"
#include "..\Libraries\CommandsEnum.h"

struct ProcessorStructure
{
    Text   text;
    size_t instructionPointer;
    Stack  stack;
    double regs[regsCount];
};

void ProcessorConstructor(const char* inputFileName, const char* stackLogFileName, const char* cpuLogFileName);

#endif