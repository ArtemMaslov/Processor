#include <stdio.h>
#include <time.h>
#include <string>


FILE* LogConstructor(const char *fileName, FILE *file = nullptr)
{
    if (file)
        file = fopen(fileName, "w");

    time_t rawTime = time(nullptr);
    tm* curTime = localtime(&rawTime);
    char date[17] = "";
    strftime(date, 16, "%H:%M:%S %d.%m.%Y", curTime);

    fprintf(file, "Лог файл структуры стек.\nStackLibrary.\nДата создания: %s.\n\n", date);

    return file;
}

void LogDestructor(FILE* file)
{
    if (file)
    {
        fflush(file);
        fclose(file);
    }
}

void LogLine(FILE* file, const char* message)
{
    if (file)
    {
        fputs(message, file);
        fflush(file);
    }
}