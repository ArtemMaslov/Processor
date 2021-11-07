#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


#include "Disassembler.h"
#include "..\Libraries\Logs\Logs.h"
#include "..\Libraries\CommandsEnum.h"
#include "..\Libraries\FilesFormat.h"


static void GetArg(char* code, size_t* ip, size_t res_size, void* out_res);

bool ParseText(Disassembler* disasm, FILE* outputFile);


void DisassemblerConstructor(FILE* inputFile, FILE* outputFile)
{
    assert(inputFile);
    assert(outputFile);

    Disassembler disasm = {};
    
    FileHeader header = ReadFileHeader(inputFile);

    if (header.Signature == ProcessorFileHeader.Signature)
    {
        if (header.ProcessorVersion == ProcessorFileHeader.ProcessorVersion &&
            header.AssemblerVersion == ProcessorFileHeader.AssemblerVersion)
        {
            if (ReadFile(&disasm.text, &header, inputFile))
            {
                disasm.text.bufferSize--;

                ParseText(&disasm, outputFile);
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

#define CMD_DEF(cmdNumber, cmdName, argc, procCode, asmGen, disasm, ...)                                \
    case cmd_##cmdName:                                                                                 \
        fputs(#cmdName, outputFile);                                                                    \
        disasm                                                                                          \
        break;

bool ParseText(Disassembler* disasm, FILE* outputFile)
{
    assert(disasm);

    char* code = disasm->text.buffer;
    size_t ip = disasm->instructionPointer;
    size_t codeLength = disasm->text.bufferSize;
    
    while (ip < codeLength)
    {
        Command cmd = {};
        int val = 0;
        val = code[ip++];
        memmove(&cmd, &(val), sizeof(char));

        unsigned char cmdType = cmd.type;
        switch (cmdType)
        {
#include "..\Libraries\CommandsDef.h"
            default:
                printf("Неизвестная команда: <%c>; ip = %zd", code[ip], ip);
                return false;
        }
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