#include <stdio.h>
#include <Windows.h>

#include "Assembler.h"
#include "..\Libraries\config.h"
#include "..\Libraries\FilesFormat.h"


int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    puts("Ассемблер:");
    
    FILE* inputFile   = nullptr;
    FILE* outputFile  = nullptr;
    FILE* listingFile = nullptr;
    FILE* logsFile    = nullptr;

    if (argc > 1)
    {
        CallocString(&listingFileName, "asm_listing.txt");
        CallocString(&asmLogsFileName, "asm_logs.html");
        outputFileName  = nullptr;
        
        if (argc > 2)
        {
            outputFileName = CreateOutFileName(argv[2]);
            outputFile = OpenFile(argv[2], "wb");
        }
        else
        {
            outputFileName = CreateOutFileName(argv[1]);
            outputFile = OpenFile(outputFileName, "wb");
        }

        if ( (inputFile   = OpenFile(argv[1], "r")        ) &&
             (outputFile                                  ) &&
             (listingFile = OpenFile(listingFileName, "w")) &&
             (logsFile    = OpenFile(asmLogsFileName, "w"))    )
        {
            AssemblerConstructor(inputFile, outputFile, listingFile, logsFile);
        }
    }
#ifdef DEBUG
    else
    {
        const char* dgbInput   = "..\\..\\tests\\asm_src\\task_circle.code";
        //const char* dgbInput   = "compiled0.code";

        CallocString(&listingFileName, "asm_listing.txt");
        CallocString(&asmLogsFileName, "AsmLogs.html");
        CallocString(&outputFileName,  "..\\..\\tests\\asm_src\\task_circle.asm");
        
        inputFile   = OpenFile(dgbInput, "r");
        outputFile  = OpenFile(outputFileName, "wb");
        listingFile = OpenFile(listingFileName, "w");
        logsFile    = OpenFile(asmLogsFileName, "w");

        if ( inputFile && outputFile && listingFile && logsFile)
        {
            AssemblerConstructor(inputFile, outputFile, listingFile, logsFile);
        }
    }
#else
    else
        puts("Запускайте программу, указывая первым параметром путь входного файла.");
#endif
    
    free(outputFileName);
    free(listingFileName);
    free(asmLogsFileName);

    if (inputFile)
        fclose(inputFile);
    if (outputFile)
        fclose(outputFile);
    if (listingFile)
        fclose(listingFile);
    if (logsFile)
        fclose(logsFile);

    //getchar();
}