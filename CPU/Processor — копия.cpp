#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cmath>


#include "Processor.h"
#include "..\Libraries\Logs\Logs.h"
#include "..\Libraries\StringLibrary\StringLibrary.h"
#include "..\Libraries\CommandsEnum.h"
#include "..\Libraries\FilesFormat.h"
#include <Windows.h>


#define PI 3.14159265

static bool ParseText(ProcessorStructure* cpu);

static void GetArg(char* code, size_t* ip, size_t res_size, void* out_res);


void ProcessorConstructor(FILE* inputFile, FILE* stackLogFile, FILE* cpuLogFile)
{
    assert(inputFile);
    assert(stackLogFile);
    assert(cpuLogFile);
    
    StackLogConstructor(stackLogFile);

    ProcessorStructure cpu = {};

    FileHeader header = ReadFileHeader(inputFile);

    if (header.Signature == ProcessorFileHeader.Signature)
    {
        if (header.ProcessorVersion == ProcessorFileHeader.ProcessorVersion &&
            header.AssemblerVersion == ProcessorFileHeader.AssemblerVersion)
        {
            if (ReadFile(&cpu.text, &header, inputFile))
            {
                cpu.text.bufferSize--;

                StackConstructor(&cpu.stack, sizeof(double));

                ParseText(&cpu);
            }
        }
        else
            printf("Текущая версия процессора %d\n"
                    "Версия процессора в файле %d\n"
                    "Текущая версия ассемблера %d\n"
                    "Версия ассемблера в файле %d\n",
                ProcessorFileHeader.ProcessorVersion, header.ProcessorVersion,
                ProcessorFileHeader.AssemblerVersion, header.AssemblerVersion);
    }
    else
    {
        printf("Неопознанная сигнатура файла <%c%c%c%c>", 
            (header.Signature % (0xFF)), 
            (header.Signature & (0xFF << 8)) >> 8,
            (header.Signature & (0xFF << 16)) >> 16,
            (header.Signature & (0xFF << 24)) >> 24);
    }
}

#define CMD_DEF(number, name, argc, code, ...)      \
    case cmd_##name:                                \
    {                                               \
        code                                        \
        break;                                      \
    }

static bool ParseText(ProcessorStructure* cpu)
{
    assert(cpu);

    char* code = cpu->text.buffer;
    size_t ip = cpu->instructionPointer;
    size_t codeLength = cpu->text.bufferSize;
    size_t commandCounter = 0;
    Stack* stk = &cpu->stack;
    
    while (ip < codeLength)
    {
        int error       = 0;
        double number1  = 0;
        double number2  = 0;
        size_t regIndex = 0;

        Command cmd = {};
        int val = 0;
        val = code[ip++];
        memmove(&cmd, &(val), sizeof(char));

        unsigned char cmdType = cmd.type;
        switch (cmdType)
        {
#include "..\Libraries\CommandsDef.h"
            default:
                printf("Неизвестная команда <%c>. ip = %zd", code[ip], ip);
                return false;
        }
        commandCounter++;
    }

    return true;
}

static void GetArg(char* code, size_t* ip, size_t res_size, void* out_res)
{
    assert(code);
    assert(out_res);
    assert(ip);

    memmove(out_res, code + *ip, res_size);
    *ip += res_size;
}