#ifndef PROCESSOR_CONFIG_H_
#define PROCESSOR_CONFIG_H_


#include "..\Libraries\StackLibrary\Stack.h"
#include "..\Libraries\StringLibrary\StringLibrary.h"
#include "..\Libraries\CommandsEnum.h"

const size_t DispOffset = 128;
const size_t DispWidthD = 60;
const size_t DispHeightD = 60;
const size_t RamSize = DispOffset + DispWidthD * DispHeightD / (8 * sizeof(double)) + 1;
const long   RamSleep = 0;

struct ProcessorStructure
{
    Text   text;
    size_t instructionPointer;
    Stack  stack;
    double regs[regsCount];
    double RAM[RamSize];
};

void ProcessorConstructor(const char* inputFileName, const char* stackLogFileName, const char* cpuLogFileName);

#endif