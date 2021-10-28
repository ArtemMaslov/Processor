#include <stdio.h>
#include <assert.h>


#include "StringLibrary.h"
#include "Assembler.h"
#include "CommandsEnum.h"


void AssemblerConstructor(const char* fileName)
{
    assert(fileName);

    Assembler assembler = {};
    assembler.text = {};
    assembler.bufferIndex = 0;

    bool res = ReadFile(&assembler.text, fileName);
    
    if (res)
    {
        ParseFileToLines(&assembler.text);
    }

    ParseText(&assembler);
}

void ParseText(Assembler* assembler)
{
    assert(assembler);

    char*   buffer       = assembler->text.buffer;
    String* strings      = assembler->text.strings;
    size_t  bufferSize   = assembler->text.bufferSize;
    size_t  stringsCount = assembler->text.stringsCount;

    bool hlt = false;
    for (int st = 0; st < stringsCount; st++)
    {
        char* string = buffer + strings->startIndex;
        AssemblerCommand cmd = GetNextCommand(string);
        
        switch (st)
        {
            case CMD_PUSH:
                double value = 0;
                if (GetNextDoubleArgument(string, &value))
                {

                }
                else
                    printf("Ошибка чтения аргумента <%s>\n", buffer + strings->startIndex);
                break;
            case CMD_POP:
                break;
            case CMD_IN:
                break;
            case CMD_OUT:
                break;
            case CMD_ADD:
                break;
            case CMD_SUB:
                break;
            case CMD_MUL:
                break;
            case CMD_DIV:
                break;
            case CMD_HLT:
                hlt = true;
                break;
            default:
                printf("Неизвестная команда <%s>\n", buffer + strings->startIndex);
                return;
        }
    }

}

AssemblerCommand GetNextCommand(const char** buffer)
{
    assert(buffer);

    while (**buffer)
    {
        for (int st = 0; st < sizeof(CommandsName) / sizeof(CommandsName[0]); st++)
        {
            bool cmp = true;
            int st1 = 0;
            for (; st1 < sizeof(CommandsName[st]) / sizeof(char); st1++)
            {
                if (buffer[st1] != CommandsName[st][st1])
                {
                    cmp = false;
                    break;
                }
            }
            if (cmp && (buffer[st1] == ' ' || buffer[st1] == '\n'))
            {
                switch (st)
                {
                    case CMD_PUSH:
                        return CMD_PUSH;
                    case CMD_POP:
                        return CMD_POP;
                    case CMD_IN:
                        return CMD_IN;
                    case CMD_OUT:
                        return CMD_OUT;
                    case CMD_ADD:
                        return CMD_ADD;
                    case CMD_SUB:
                        return CMD_SUB;
                    case CMD_MUL:
                        return CMD_MUL;
                    case CMD_DIV:
                        return CMD_DIV;
                    case CMD_HLT:
                        return CMD_HLT;
                    default:
                        return CMD_UNKNOWN;
                }
            }
        }
        buffer++;
    }
    return CMD_UNKNOWN;
}

bool GetNextDoubleArgument(const char* buffer, double* number)
{
    int readed = sscanf(buffer, " %lf", number);

    if (readed == 1)
    {
        return true;
    }
    else
        return false;
}

double GetNextIntArgument(const char* buffer, int* number)
{
    int readed = sscanf(buffer, " %d", number);

    if (readed == 1)
    {
        return true;
    }
    else
        return false;
}