#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


#include "..\Libraries\StringLibrary\StringLibrary.h"
#include "..\Libraries\Logs\Logs.h"
#include "Assembler.h"
#include "..\Libraries\CommandsEnum.h"
#include "..\Libraries\FilesFormat.h"
#include "ListingCreator.h"


static AssemblerCommand GetNextCommand(char** buffer);

static bool GetNextDoubleArgument(char** buffer, double* number);

static bool GetNextIntArgument(char** buffer, int* number);

static bool ParseText(Assembler* assembler, FILE* outputFile, FILE* listingFile);

static bool GetNextRegister(char** buffer, int* regIndex);

static bool IgnoreComment(char** buffer);

static bool ParseArgument(char** buffer, AssemblerCommand cmdType, size_t* instructionPointer, FILE* outputFile);


static FILE* logFile = nullptr;


void AssemblerConstructor(const char* inputFileName, const char* outputFileName, const char* listingFileName,
                          const char* asmLogFileName)
{
    assert(inputFileName);
    assert(outputFileName);

    Assembler assembler = {};
    assembler.text = {};
    assembler.bufferIndex = 0;

    if (logFile = OpenFile(asmLogFileName, "w"))
    {
        if (FILE* inputFile = OpenFile(inputFileName, "r"))
        {
            LogLine(logFile, "inputFile успешно открыт");

            FileHeader header = {};
            header.BodySize = GetFileSize(inputFile);
            bool res = ReadFile(&assembler.text, &header, inputFile);
            
            if (res)
            {
                LogLine(logFile, "Файл успешно прочитан");
                if (FILE* outputFile = OpenFile(outputFileName, "wb"))
                {
                    LogLine(logFile, "inputFile успешно открыт");
                    if (FILE* listingFile = OpenFile(listingFileName, "w"))
                    {
                        ParseFileToLines(&assembler.text);
                    
                        CreateListingFileHeader(listingFile, outputFileName);

                        ParseText(&assembler, outputFile, listingFile);
                        fclose(outputFile);
                    }
                }
            }
            fclose(inputFile);
        }
        fclose(logFile);
    }
}

static bool ParseText(Assembler* assembler, FILE* outputFile, FILE* listingFile)
{
    assert(assembler);
    assert(outputFile);

    LogLine(logFile, "ParseText");

    FileHeader header = ProcessorFileHeader;
    fwrite(&header, sizeof(FileHeader), 1, outputFile);

    char*   buffer       = assembler->text.buffer;
    String* strings      = assembler->text.strings;
    size_t  bufferSize   = assembler->text.bufferSize;
    size_t  stringsCount = assembler->text.stringsCount;

    bool    hlt = false;
    size_t  instructionPointer = 0;
    size_t  commandsCount = 0;

    for (size_t st = 0; st < stringsCount && !hlt; st++)
    {
        char* string = buffer + strings[st].startIndex;
        
        if (IgnoreComment(&string))
            continue;

        AssemblerCommand cmd = GetNextCommand(&string);
        
        size_t oldInstructionPointer = instructionPointer;
        
        char listingMsg[ListingMessageLength] = {};
        char listingBin[ListingBinCodeLength] = {};

        switch (cmd)
        {
            case CMD_PUSH:
            {
                if (!ParseArgument(&string, cmd, &instructionPointer, outputFile))
                    return false;
                break;
            }
            case CMD_POP:
            {
                if (!ParseArgument(&string, cmd, &instructionPointer, outputFile))
                    return false;
                break;
            }
            case CMD_IN:
            {
                LogLine(logFile, "in");
                double doubleVal = 0;
                puts("Введите число:");
                if (scanf("%lf", &doubleVal))
                {
                    LogLine(logFile, "in argument readed");
                    sprintf(listingMsg, "in %lf", doubleVal);

                    fprintf(outputFile, "%c", CMD_PUSH);
                    instructionPointer += commandSize;
                    
                    fwrite(&doubleVal, sizeof(double), 1, outputFile);
                    instructionPointer += sizeof(double);
                }
                else
                {
                    LogLine(logFile, "in bad argument");
                    printf("Ошибка чтения аргумента <%s>\n", buffer + strings[st].startIndex);
                    return false;
                }
                break;
            }
            case CMD_OUT:
            {
                LogLine(logFile, "out");
                sprintf(listingMsg, "out");

                fprintf(outputFile, "%c", CMD_OUT);
                instructionPointer += commandSize;
                break;
            }
            case CMD_ADD:
            {
                LogLine(logFile, "add");
                sprintf(listingMsg, "add");

                fprintf(outputFile, "%c", cmd);
                instructionPointer += commandSize;
                break;
            }
            case CMD_SUB:
            {
                LogLine(logFile, "sub");
                sprintf(listingMsg, "sub");

                fprintf(outputFile, "%c", cmd);
                instructionPointer += commandSize;
                break;
            }
            case CMD_MUL:
            {
                LogLine(logFile, "mul");
                sprintf(listingMsg, "mul");

                fprintf(outputFile, "%c", cmd);
                instructionPointer += commandSize;
                break;
            }
            case CMD_DIV:
            {
                LogLine(logFile, "mul");
                sprintf(listingMsg, "div");

                fprintf(outputFile, "%c", cmd);
                instructionPointer += commandSize;
                break;
            }
            case CMD_HLT:
            {
                LogLine(logFile, "hlt");
                sprintf(listingMsg, "hlt");

                fprintf(outputFile, "%c", cmd);
                instructionPointer += commandSize;
                hlt = true;
                break;
            }
            default:
                LogLine(logFile, "Неизвестная команда");
                printf("Неизвестная команда <%s>\n", buffer + strings[st].startIndex);
                return false;
        }

        ListingAddLine(listingFile, oldInstructionPointer, listingMsg, listingBin);

        commandsCount++;
        if (!IgnoreComment(&string))
            printf("Ошибка чтения комманды <%s>", buffer + strings[st].startIndex);
    }

    if (hlt)
    {
        fseek(outputFile, HeaderFileSizeOffset, SEEK_SET);
        fwrite(&instructionPointer, sizeof(instructionPointer), 1, outputFile);
        fwrite(&commandsCount, sizeof(commandsCount), 1, outputFile);
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
    
    LogLine(logFile, "GetNextCommand");

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
            if (cmp && ((*buffer)[st1] == ' ' || (*buffer)[st1] == '\n' ||
                (*buffer)[st1] == '\0' || (*buffer)[st1] == ';'))
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
    
    LogLine(logFile, "GetNextDoubleArgument");

    int stringOffset = 0;
    int readed = sscanf(*buffer, " %lf%n", number, &stringOffset);

    if (readed == 1)
    {
        (*buffer) += stringOffset;
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
    
    LogLine(logFile, "GetNextIntArgument");

    int stringOffset = 0;
    int readed = sscanf(*buffer, " %d%n", number, &stringOffset);

    if (readed == 1)
    {
        (*buffer) += stringOffset;
        return true;
    }
    else
        return false;
}

static bool IgnoreComment(char** buffer)
{
    assert(buffer);
    assert(*buffer);
    
    LogLine(logFile, "IgnoreComment");

    bool res = true;
    while (**buffer)
    {
        if (!isspace(**buffer))
        {
            if (**buffer == ';')
                return true;
            else
                return false;
        }
        (*buffer)++;
    }
    return true;
}

static bool GetNextRegister(char** buffer, int* regIndex)
{
    assert(buffer);
    assert(*buffer);
    assert(regIndex);
    
    LogLine(logFile, "GetNextRegister");

    char* buf = *buffer;
    while (*buf)
    {
        for (int st = 0; st < sizeof(regNames) / sizeof(regNames[0]); st++)
        {
            bool cmp = true;
            int st1 = 0;
            for (const char* cmd = regNames[st]; cmd[st1]; st1++)
            {
                if (buf[st1] != CommandsName[st][st1])
                {
                    cmp = false;
                    break;
                }
            }
            if (cmp && (buf[st1] == ' ' || buf[st1] == '\n' ||
                buf[st1] == '\0' || buf[st1] == ';' || buf[st1] == '+' || buf[st1] == '-'))
            {
                buf += st1;
                (*buffer) = buf;
                *regIndex = st;
                return true;
            }
        }
        buf++;
    }

    return false;
}

static bool ParseArgument(char** buffer, AssemblerCommand cmdType, size_t* instructionPointer, FILE* outputFile)
{
    double doubleVal = 0;
    int    regIndex  = 0;
    bool   number    = false;
    bool   reg       = false;

    while (**buffer)
    {
        if (GetNextDoubleArgument(buffer, &doubleVal))
        {
            if (!number)
            {
                if (cmdType == CMD_PUSH)
                {
                    LogLine(logFile, "push number");
                    number = true;
                }
                else
                {
                    printf("Ошибка чтения строки. Числовой аргумент может быть только у команды push. <%s>", *buffer);
                    return false;
                }
            }
            else
            {
                printf("Ошибка чтения строки. Числовой аргумент встретился два раза. <%s>", *buffer);
                return false;
            }
        }
        else if (GetNextRegister(buffer, &regIndex))
        {
            if (!reg)
            {
                LogLine(logFile, "push number");
                reg = true;
            }
            else
            {
                printf("Ошибка чтения строки. Регистр встретился два раза в аргументе. <%s>", *buffer);
                return false;
            }
        }
        else if (**buffer == '+' || isspace(**buffer))
            (*buffer)++;
        else
        {
            printf("Ошибка чтения строки. <%s>", *buffer);
            return false;
        }
    }

    Command cmd = {};
    cmd.type = cmdType;

    if (number)
        cmd.params |= PushParamNumber;
    if (reg)
        cmd.params |= PushParamReg;

    if (reg && number)
        LogLine(logFile, "push reg and number");
    else if (reg)
        LogLine(logFile, "push reg");
    else if (number)
        LogLine(logFile, "push number");

    fprintf(outputFile, "%c", cmd);
    *instructionPointer += commandSize;

    if (reg)
    {
        fprintf(outputFile, "%c", regIndex);
        instructionPointer += sizeof(char);
    }
    if (number)
    {
        fwrite(&doubleVal, sizeof(double), 1, outputFile);
        instructionPointer += sizeof(double);
    }
    return true;
}