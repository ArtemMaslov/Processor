#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


#include "Disassembler.h"
#include "..\Libraries\CommandsEnum.h"
#include "..\Libraries\FilesFormat.h"


void GetDoubleArg(char* code, size_t* ip, double* number);

bool ParseText(Disassembler* disasm, FILE* outputFile);


void DisassemblerConstructor(const char* inputFileName, const char* outputFileName)
{
    assert(inputFileName);
    assert(outputFileName);

    Disassembler disasm = {};
    
    if (FILE* inputFile = OpenFile(inputFileName, "rb"))
    {
        FileHeader header = ReadFileHeader(inputFile);

        if (header.Signature == ProcessorFileHeader.Signature)
        {
            if (header.DisasseblerVersion == ProcessorFileHeader.DisasseblerVersion &&
                header.AssemblerVersion == ProcessorFileHeader.AssemblerVersion)
            {
                if (ReadFile(&disasm.text, &header, inputFile))
                {
                    disasm.text.bufferSize--;

                    if (FILE* outputFile = OpenFile(outputFileName, "w"))
                    {
                        ParseText(&disasm, outputFile);

                        fclose(outputFile);
                    }
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
}

bool ParseText(Disassembler* disasm, FILE* outputFile)
{
    assert(disasm);

    char* code = disasm->text.buffer;
    size_t ip = disasm->instructionPointer;
    size_t codeLength = disasm->text.bufferSize;
    
    while (ip < codeLength)
    {
        int error = 0;
        double number1 = 0;
        double number2 = 0;
        switch (code[ip++])
        {
            case CMD_PUSH:
                GetDoubleArg(code, &ip, &number1);
                fprintf(outputFile, "push %lf\n", number1);
                break;
            case CMD_POP:
                fputs("pop\n", outputFile);
                break;
            case CMD_OUT:
                fputs("out\n", outputFile);
                break;
            case CMD_ADD:
                fputs("add\n", outputFile);
                break;
            case CMD_SUB:
                fputs("sub\n", outputFile);
                break;
            case CMD_MUL:
                fputs("mul\n", outputFile);
                break;
            case CMD_DIV:
                fputs("div\n", outputFile);
                break;
            case CMD_HLT:
                fputs("hlt\n", outputFile);
                break;
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