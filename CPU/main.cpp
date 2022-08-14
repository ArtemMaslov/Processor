#include <stdio.h>
#include <Windows.h>

#include "Processor.h"
#include "..\Libraries\config.h"


int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    puts("Процессор:");

    FILE* inputFile     = nullptr;
    FILE* cpuLogsFile   = nullptr;

    if (argc > 1)
    {
        const char* stackLogFileName = "stack_logs.html";
        const char* cpuLogsFileName  = "cpu_logs.html";

        if ( (inputFile     = OpenFile(argv[1],          "r")) &&
             (stackLogFile  = OpenFile(stackLogFileName, "w"))  &&
             (cpuLogsFile   = OpenFile(cpuLogsFileName,  "w"))   )
        {
            ProcessorConstructor(inputFile, stackLogFile, cpuLogsFile);
        }
    }
#ifdef DEBUG
    else
    {
        const char* dgbInput     = "..\\..\\tests\\asm_src\\comp.asm";
        const char* dgbCpuLogs   = "cpu_logs.html";
        const char* dbgStackLogs = "stack_logs.html";

        if ( (inputFile     = OpenFile(dgbInput,     "r")) &&
             (cpuLogsFile   = OpenFile(dgbCpuLogs,   "w")) &&
             (stackLogFile  = OpenFile(dbgStackLogs, "w"))   )
        {
            ProcessorConstructor(inputFile, stackLogFile, cpuLogsFile);
        }
    }
#else
    else
        puts("Запускайте программу, указывая первым параметром путь входного файла.");
#endif
    
    if (inputFile)
        fclose(inputFile);
    if (stackLogFile)
        fclose(stackLogFile);
    if (cpuLogsFile)
        fclose(cpuLogsFile);

    //getchar();
    //getchar();
}