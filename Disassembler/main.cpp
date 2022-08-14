#include <stdio.h>
#include <Windows.h>

#include "..\Libraries\config.h"
#include "Disassembler.h"

int main(int argc, char* argv[])
{
    
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    puts("ƒизассемблер:");

    FILE* inputFile     = nullptr;
    FILE* outputFile    = nullptr;

    if (argc > 1)
    {
        char * outputFileName  = nullptr;
        
        if (argc > 2)
        {
            outputFile = OpenFile(argv[2], "w");
        }
        else
        {
            outputFileName = CreateOutFileName(argv[1]);
            outputFile     = OpenFile(outputFileName, "w");
        }

        if ( (inputFile      = OpenFile(argv[1], "rb")) &&
              outputFile)
        {
            DisassemblerConstructor(inputFile, outputFile);
        }

        free(outputFileName);
    }
#ifdef DEBUG
    else
    {
        const char* dgbInput     = "D:\\язык C\\Processor\\tests\\asm_src\\task_circle_out.code";
        const char* dbgOutput    = "D:\\язык C\\Processor\\tests\\asm_src\\disasm_out.txt";

        if ( (inputFile     = OpenFile(dgbInput, "rb")) &&
             (outputFile    = OpenFile(dbgOutput, "w"))   )
        {
            DisassemblerConstructor(inputFile, outputFile);
        }
    }
#else
    else
        puts("«апускайте программу, указыва€ первым параметром путь входного файла.");
#endif
    
    if (inputFile)
        fclose(inputFile);
    if (outputFile)
        fclose(outputFile);

    getchar();
}