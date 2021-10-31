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

static void GetDoubleArg(char* code, size_t* ip, double* number);

static void GetRegArg(char* code, size_t* ip, size_t* regIndex);

static bool ParseText(ProcessorStructure* cpu);

static void GetSize_TArg(char* code, size_t* ip, size_t* number);


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
            case CMD_PUSH:
            {
                double regValue = 0;
                if (cmd.Reg)
                {
                    GetRegArg(code, &ip, &regIndex);
                    if (regIndex >= regsCount)
                    {
                        printf("IndexOutOfRangeRegister = %d", regIndex);
                        return false;
                    }
                    regValue = cpu->regs[regIndex];
                }
                if (cmd.Number)
                {
                    GetDoubleArg(code, &ip, &number1);
                }
                number1 += regValue;
                if (cmd.RAM)
                {
                    StackPush(stk, cpu->RAM + (int)number1);
                    Sleep(RamSleep);
                }
                else
                    StackPush(stk, &number1);
                break;
            }
            case CMD_POP:
            {
                if (cmd.Reg == 0 && cmd.Number == 0 && cmd.RAM == 0)
                    StackPop(stk, &error);
                else
                {
                    if (cmd.RAM)
                    {
                        if (cmd.Reg)
                        {
                            GetRegArg(code, &ip, &regIndex);
                            if (regIndex >= regsCount)
                            {
                                printf("IndexOutOfRangeRegister = %d", regIndex);
                                return false;
                            }
                        }
                        if (cmd.Number)
                            GetDoubleArg(code, &ip, &number1);

                        int ramIndex = number1 + cpu->regs[regIndex];
                        if (ramIndex < DispOffset) // Оперативная память
                        {
                            cpu->RAM[ramIndex] = *((double*)StackPop(stk, &error));
                            Sleep(RamSleep);
                        }
                        else // Видео память
                        {
                            ramIndex = ramIndex - DispOffset;
                            char ramByte = ((char*)cpu->RAM)[DispOffset * sizeof(double) + ramIndex / 8];
                            int bitValue = (int)(*((double*)StackPop(stk, &error)));

                            unsigned char bitMask = 1 << (7 - ramIndex % 8);
                            if (bitValue)
                                ((char*)cpu->RAM)[DispOffset * sizeof(double) + ramIndex / 8] = (ramByte | bitMask);
                            else
                                ((char*)cpu->RAM)[DispOffset * sizeof(double) + ramIndex / 8] = (ramByte & (~bitMask));

                            Sleep(RamSleep);
                        }
                    }
                    else if (cmd.Reg)
                    {
                        GetRegArg(code, &ip, &regIndex);
                        if (regIndex >= regsCount)
                        {
                            printf("IndexOutOfRangeRegister = %d", regIndex);
                            return false;
                        }
                        cpu->regs[regIndex] = *((double*)StackPop(stk, &error));
                    }
                }
                if (error > 0)
                    StackDump(stk, stdout);
                break;
            }
            case CMD_IN:
            {
                double doubleVal = 0;
                puts("Введите число (Не число для завершения программы):");
                if (scanf("%lf", &doubleVal))
                {
                    StackPush(stk, &doubleVal);
                }
                else
                {
                    printf("Программа завершена\n");
                    return false;
                }
                break;
            }
            case CMD_OUT:
            {
                double* ptr = (double*)StackPop(stk, &error);
                if (ptr)
                    printf("%lf\n", *ptr);
                else
                    puts("");
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
            case CMD_DSP:
            {
                size_t dispBytesCount = (DispHeightD * DispWidthD / (8 * sizeof(double)) + 1) * 8;
                putchar('\n');
                size_t pixelsCount = 0;
                for (size_t st = 0; st < dispBytesCount; st++)
                {
                    char byte = ((char*)cpu->RAM)[DispOffset * sizeof(double) + st];

                    for (unsigned char pixelMask = 1 << 7; pixelMask > 0 && pixelsCount < DispHeightD * DispWidthD; pixelMask>>=1)
                    {
                        if (byte & pixelMask)
                            putchar('*');
                        else
                            putchar('.');
                        pixelsCount++;
                        if (pixelsCount % DispWidthD == 0)
                        {
                            putchar('\n');
                        }
                    }
                }
                fflush(stdout);
                break;
            }
            case CMD_JMP:
            {
                size_t jmpLabel = 0;
                GetSize_TArg(code, &ip, &jmpLabel);
                ip = jmpLabel;
                break;
            }
            case CMD_JA:
            {
                size_t jmpLabel = 0;
                GetSize_TArg(code, &ip, &jmpLabel);

                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                if (number2 > number1)
                    ip = jmpLabel;
                break;
            }
            case CMD_JAE:
            {
                size_t jmpLabel = 0;
                GetSize_TArg(code, &ip, &jmpLabel);

                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                if (number2 >= number1)
                    ip = jmpLabel;
                break;
            }
            case CMD_JB:
            {
                size_t jmpLabel = 0;
                GetSize_TArg(code, &ip, &jmpLabel);

                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                if (number2 < number1)
                    ip = jmpLabel;
                break;
            }
            case CMD_JBE:
            {
                size_t jmpLabel = 0;
                GetSize_TArg(code, &ip, &jmpLabel);

                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                if (number2 <= number1)
                    ip = jmpLabel;
                break;
            }
            case CMD_JE:
            {
                size_t jmpLabel = 0;
                GetSize_TArg(code, &ip, &jmpLabel);

                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                if (number2 == number1)
                    ip = jmpLabel;
                break;
            }
            case CMD_JNE:
            {
                size_t jmpLabel = 0;
                GetSize_TArg(code, &ip, &jmpLabel);

                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number2 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                if (number2 != number1)
                    ip = jmpLabel;
                break;
            }
            case CMD_CALL:
            {
                number1 = ip + sizeof (int);
                StackPush(stk, &number1);

                size_t jmpLabel = 0;
                GetSize_TArg(code, &ip, &jmpLabel);
                ip = jmpLabel;
                break;
            }
            case CMD_RET:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                ip = (int)number1;
                break;
            }
            case CMD_COS:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number1 = cos(number1 * PI / 180);
                StackPush(stk, &number1);
                break;
            }
            case CMD_SIN:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number1 = sin(number1 * PI / 180);
                StackPush(stk, &number1);
                break;
            }
            case CMD_SQRT:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number1 = sqrt(number1);
                StackPush(stk, &number1);
                break;
            }
            case CMD_INT:
            {
                number1 = *((double*)StackPop(stk, &error));
                if (error > 0)
                    StackDump(stk, stdout);

                number1 = (int)number1;
                StackPush(stk, &number1);
                break;
            }
            case CMD_HLT:
                return true;
            default:
                printf("Неизвестная команда: <%c>; ip = %zd", code[ip], ip);
                return false;
        }
        commandCounter++;
    }

    return true;
}

static void GetDoubleArg(char* code, size_t* ip, double* number)
{
    assert(code);
    assert(number);
    assert(ip);

    memmove(number, code + *ip, sizeof(double));
    *ip += sizeof(double);
}

static void GetSize_TArg(char* code, size_t* ip, size_t* number)
{
    assert(code);
    assert(number);
    assert(ip);

    memmove(number, code + *ip, sizeof(size_t));
    *ip += sizeof(size_t);
}

static void GetRegArg(char* code, size_t* ip, size_t* regIndex)
{
    assert(code);
    assert(ip);
    assert(regIndex);

    memmove(regIndex, code + *ip, sizeof(char));
    *ip += sizeof(char);
}
