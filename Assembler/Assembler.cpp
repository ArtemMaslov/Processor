#include <stdio.h>
#include <assert.h>
#include <ctype.h>


#include "..\Libraries\StringLibrary\StringLibrary.h"
#include "Assembler.h"
#include "..\Libraries\CommandsEnum.h"


static AssemblerCommand GetNextCommand(char** buffer);

static bool GetNextDoubleArgument(char** buffer, double* number);

static bool GetNextIntArgument(char** buffer, int* number);

static bool ParseText(Assembler* assembler, FILE* outputFile);

static bool IgnoreComment(char** buffer, const char* const src);


void AssemblerConstructor(const char* inputFileName, const char* outputFileName)
{
    assert(inputFileName);
    assert(outputFileName);

    Assembler assembler = {};
    assembler.text = {};
    assembler.bufferIndex = 0;

    bool res = ReadFile(&assembler.text, inputFileName);
    
    if (res)
    {
        ParseFileToLines(&assembler.text);
    }

    FILE* outputFile = fopen(outputFileName, "wb");

    if (outputFile)
    {
        ParseText(&assembler, outputFile);
        fclose(outputFile);
    }
    else
        printf("Ошибка открытия файла <%s>\n", outputFileName);
}

static bool ParseText(Assembler* assembler, FILE* outputFile)
{
    assert(assembler);
    assert(outputFile);

    char*   buffer       = assembler->text.buffer;
    String* strings      = assembler->text.strings;
    size_t  bufferSize   = assembler->text.bufferSize;
    size_t  stringsCount = assembler->text.stringsCount;

    bool hlt = false;
    size_t instructionPointer = 0;
    for (size_t st = 0; st < stringsCount && !hlt; st++)
    {
        char* string = buffer + strings[st].startIndex;
        AssemblerCommand cmd = GetNextCommand(&string);
        
        switch (cmd)
        {
            case CMD_PUSH:
            {
                double doubleVal = 0;
                if (GetNextDoubleArgument(&string, &doubleVal))
                {
                    fprintf(outputFile, "%c", CMD_PUSH);
                    instructionPointer += commandSize;

                    fwrite(&doubleVal, sizeof(double), 1, outputFile);
                    instructionPointer += sizeof(double);
                }
                else
                {
                    printf("Ошибка чтения аргумента <%s>\n", buffer + strings->startIndex);
                    return false;
                }
                break;
            }
            case CMD_POP:
                fprintf(outputFile, "%c", CMD_POP);
                instructionPointer += commandSize;
                break;
            case CMD_IN:
            {
                double doubleVal = 0;
                puts("Введите число:");
                if (scanf("%lf", &doubleVal))
                {
                    fprintf(outputFile, "%c", CMD_PUSH);
                    instructionPointer += commandSize;
                    
                    fwrite(&doubleVal, sizeof(double), 1, outputFile);
                    instructionPointer += sizeof(double);
                }
                else
                {
                    printf("Ошибка чтения аргумента <%s>\n", buffer + strings->startIndex);
                    return false;
                }
                break;
            }
            case CMD_OUT:
                fprintf(outputFile, "%c", CMD_OUT);
                instructionPointer += commandSize;
                break;
            case CMD_ADD:
            case CMD_SUB:
            case CMD_MUL:
            case CMD_DIV:
                fprintf(outputFile, "%c", cmd);
                instructionPointer += commandSize;
                break;
            case CMD_HLT:
                fprintf(outputFile, "%c", cmd);
                instructionPointer += commandSize;
                hlt = true;
                break;
            default:
                printf("Неизвестная команда <%s>\n", buffer + strings->startIndex);
                return false;
        }
        IgnoreComment(&string, buffer + strings->startIndex);
    }

    if (hlt)
    {
        fputc('\0', outputFile);
        return true;
    }
    else
    {
        puts("Не обнаружена комманда hlt в конце программы");
        return false;
    }
}

static AssemblerCommand GetNextCommand(char** buffer)
{
    assert(buffer);
    assert(*buffer);

    while (**buffer)
    {
        for (int st = 0; st < sizeof(CommandsName) / sizeof(CommandsName[0]); st++)
        {
            bool cmp = true;
            int st1 = 0;
            for (const char* cmd = CommandsName[st]; cmd[st1]; st1++)
            {
                if ((*buffer)[st1] != CommandsName[st][st1])
                {
                    cmp = false;
                    break;
                }
            }
            if (cmp && ((*buffer)[st1] == ' ' || (*buffer)[st1] == '\n' || (*buffer)[st1] == '\0'))
            {
                *(buffer) += st1;
                return (AssemblerCommand)st;
            }
        }
        (*buffer)++;
    }
    return CMD_UNKNOWN;
}

static bool GetNextDoubleArgument(char** buffer, double* number)
{
    assert(buffer);
    assert(*buffer);
    assert(number);

    int stringOffset = 0;
    int readed = sscanf(*buffer, " %lf%n", number, &stringOffset);

    if (readed == 1)
    {
        *(buffer) += stringOffset;
        return true;
    }
    else
        return false;
}

static bool GetNextIntArgument(char** buffer, int* number)
{
    assert(buffer);
    assert(*buffer);
    assert(number);

    int stringOffset = 0;
    int readed = sscanf(*buffer, " %d%n", number, &stringOffset);

    if (readed == 1)
    {
        *(buffer) += stringOffset;
        return true;
    }
    else
        return false;
}

static bool IgnoreComment(char** buffer, const char* const src)
{
    assert(buffer);
    assert(*buffer);
    assert(src);

    bool res = true;
    while (**buffer)
    {
        if (!isspace(**buffer))
        {
            if (**buffer == ';')
            {
                return true;
            }
            else
            {
                printf("Ошибка чтения комманды <%s>", src);
                return false;
            }
        }
        *buffer++;
    }
    return true;
}