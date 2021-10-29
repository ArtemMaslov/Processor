#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Processor.h"
#include "..\Libraries\StringLibrary\StringLibrary.h"
#include "..\Libraries\CommandsEnum.h"


bool ReadFile(FILE* file, ProcessorStructure* cpu);

void GetDoubleArg(char* code, size_t* ip, double* number);

bool ParseProcessor(ProcessorStructure* cpu);


void ProcessorConstructor(const char* inputFileName)
{
    assert(inputFileName);
    
    FILE* stackLogs = fopen("StackLibraryLogs.html", "w");
    FILE* file = fopen(inputFileName, "rb");

    if (file && stackLogs)
    {
        stackLogFile = stackLogs;
        StackLogConstructor(stackLogFile);

        ProcessorStructure cpu = {};

        if (ReadFile(file, &cpu))
        {
            StackConstructor(&cpu.stack, sizeof(double));

            ParseProcessor(&cpu);
        }
        fclose(file);
        fclose(stackLogs);
    }
}

bool ReadFile(FILE* file, ProcessorStructure* cpu)
{
    assert(file);
    assert(cpu);

    size_t fileSize = GetFileSize(file) + 1;

    cpu->code = (char*)calloc(fileSize, sizeof(char));
    cpu->codeLength = fileSize;

    size_t readed = fread(cpu->code, sizeof(char), fileSize, file);

    if (readed == 0)
    {
        puts("Ошибка чтения файла.");
        return false;
    }
    cpu->code[readed] = '\0';

    return true;
}

bool ParseProcessor(ProcessorStructure* cpu)
{
    assert(cpu);

    char* code = cpu->code;
    size_t ip = cpu->instructionPointer;
    size_t codeLength = cpu->codeLength;
    Stack* stk = &cpu->stack;
    
    while (ip < codeLength)
    {
        int error = 0;
        double number1 = 0;
        double number2 = 0;
        switch (code[ip++])
        {
            case CMD_PUSH:
                GetDoubleArg(code, &ip, &number1);
                StackPush(stk, &number1);
                break;
            case CMD_POP:
                StackPop(stk, &error);
                if (error > 0)
                    StackDump(stk, stdout);
                break;
            case CMD_OUT:
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                printf("%lf", number1);
                break;
            case CMD_ADD:
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 += number1;
                StackPush(stk, &number2);
                break;
            case CMD_SUB:
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 -= number1;
                StackPush(stk, &number2);
                break;
            case CMD_MUL:
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 *= number1;
                StackPush(stk, &number2);
                break;
            case CMD_DIV:
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 /= number1;
                StackPush(stk, &number2);
                break;
            case CMD_HLT:
                return true;
            default:
                printf("Неизвестная команда: <%c>; ip = %zd", code[ip], ip);
                return false;
        }
    }

    return true;
}

void GetDoubleArg(char* code, size_t* ip, double* number)
{
    assert(code);
    assert(number);
    assert(ip);

    /*char* ptr1 = (char*)number;
    char* ptr2 = code + (*ip);

    for (size_t st = 0; st < sizeof(double); st++)
    {
        ptr1[st] = ptr2[st];
    }*/
    memmove(number, code + *ip, sizeof(double));
    *ip += sizeof(double);
}

