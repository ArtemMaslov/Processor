#ifndef PROCESSOR_CONFIG_H_
#define PROCESSOR_CONFIG_H_

struct ProcessorCommand
{
    const char* command;
    int code;
};

const char commentSymvol = ';';

struct ProcessorStructure
{
    char   *code;
    size_t instructionPointer;
    size_t codeLength;
    
};

#endif