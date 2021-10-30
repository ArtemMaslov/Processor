#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Processor.h"
#include "..\Libraries\Logs\Logs.h"
#include "..\Libraries\StringLibrary\StringLibrary.h"
#include "..\Libraries\CommandsEnum.h"
#include "..\Libraries\FilesFormat.h"


void GetDoubleArg(char* code, size_t* ip, double* number);

void GetRegArg(char* code, size_t* ip, size_t* regIndex);

bool ParseText(ProcessorStructure* cpu);


void ProcessorConstructor(const char* inputFileName, const char* stackLogFileName, const char* cpuLogFileName)
{
    assert(inputFileName);
    assert(stackLogFileName);
    assert(cpuLogFileName);
    
    if (FILE* stackLogs = OpenFile(stackLogFileName, "w"))
    {
        stackLogFile = stackLogs;
        StackLogConstructor(stackLogFile);

        ProcessorStructure cpu = {};

        if (FILE* inputFile = OpenFile(inputFileName, "rb"))
        {
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
                printf("Неопознанная сигнатура файла <%s>", header.Signature);
            fclose(inputFile);
        }
        fclose(stackLogs);
    }
}

bool ParseText(ProcessorStructure* cpu)
{
    assert(cpu);

    char* code = cpu->text.buffer;
    size_t ip = cpu->instructionPointer;
    size_t codeLength = cpu->text.bufferSize;
    Stack* stk = &cpu->stack;
    
    while (ip < codeLength)
    {
        int error       = 0;
        double number1  = 0;
        double number2  = 0;
        size_t regIndex = 0;

        Command cmd = {};
        memmove(&cmd, &(code[ip++]), sizeof(Command));

        switch (cmd.type)
        {
            case CMD_PUSH:
            {
                double regValue = 0;
                if (cmd.params & PushParamReg > 0)
                {
                    GetRegArg(code, &ip, &regIndex);
                    if (regIndex >= regsCount)
                    {
                        printf("IndexOutOfRangeRegister = %d", regIndex);
                        return false;
                    }
                    regValue = cpu->regs[regIndex];
                }
                if (cmd.params & PushParamNumber > 0)
                {
                    GetDoubleArg(code, &ip, &number1);
                }
                number1 += regValue;
                StackPush(stk, &number1);
                break;
            }
            case CMD_POP:
            {
                if (cmd.params & PushParamReg > 0)
                {
                    GetRegArg(code, &ip, &regIndex);
                    if (regIndex >= regsCount)
                    {
                        printf("IndexOutOfRangeRegister = %d", regIndex);
                        return false;
                    }
                    cpu->regs[regIndex] = *((double*)StackPop(stk, &error));
                }
                else
                    StackPop(stk, &error);
                if (error > 0)
                    StackDump(stk, stdout);
                break;
            }
            case CMD_OUT:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                printf("%lf", number1);
                break;
            }
            case CMD_ADD:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 += number1;
                StackPush(stk, &number2);
                break;
            }
            case CMD_SUB:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 -= number1;
                StackPush(stk, &number2);
                break;
            }
            case CMD_MUL:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 *= number1;
                StackPush(stk, &number2);
                break;
            }
            case CMD_DIV:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 /= number1;
                StackPush(stk, &number2);
                break;
            }
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

void GetRegArg(char* code, size_t* ip, size_t* regIndex)
{
    assert(code);
    assert(ip);
    assert(regIndex);

    memmove(regIndex, code + *ip, sizeof(char));
    *ip += sizeof(char);
}
