#ifndef LOGS_H_
#define LOGS_H_

#include <stdio.h>

FILE* LogConstructor(const char *fileName, FILE *file = nullptr);

void LogDestructor(FILE* file);

void LogLine(FILE* file, const char* message);

#define LogFLine(file, format, ...)                   \
    if (file)                                         \
    {                                                 \
        fprintf(file, format, __VA_ARGS__);           \
        fputs("", file);                              \
        fflush(file);                                 \
    }


#endif // !LOGS_H_
