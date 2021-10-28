#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <assert.h>

#include "Logs.h"

int TextOffset = 0;

void LogConstructor(FILE *file)
{
    assert(file);

    time_t rawTime = time(nullptr);
    tm* curTime = localtime(&rawTime);

    const char* dateFormat = "00:00:00 01.01.2021";
    size_t dateLength = strlen(dateFormat) + 1;
    char* date = (char*)calloc(dateLength, sizeof(char));

    if (date)
    {
        strftime(date, dateLength, "%H:%M:%S %d.%m.%Y", curTime);

        fprintf(file, "<html>\n"
                      "<head><title>Лог бибилотеки StackLibrary.</title><style>font {line-height: 0.8;} body {background-color: #404040;} head {background-color: #404040;}</style></head>\n"
                      "<body>\n"
                      "<h1><font color=\"99B333\">Лог бибилотеки StackLibrary. %s</font></h1>\n", date);

        TextOffset = ftell(file);

        fputs("</body>\n"
              "</html>\n", file);
        TextOffset -= ftell(file);
        free(date);
    }
    else
    {
        fprintf(file, "<html>\n"
                      "<head><title>Лог бибилотеки StackLibrary.</title><style>font {line-height: 0.8;} body {background-color: #404040;} head {background-color: #404040;}</style></head>\n"
                      "<body>\n"
                      "<h1><font color=\"99B333\">Лог бибилотеки StackLibrary.</font></h1>\n");
        
        TextOffset = ftell(file);

        fputs("</body>\n"
              "</html>\n", file);
        TextOffset -= ftell(file);
    }
}

/**
 * @brief      Закрывает файл логов.
 * @param file Указатель на поток вывода.
*/
void LogDestructor(FILE* file)
{
    if (file)
        fclose(file);
}

/**
 * @brief         Добавляет строку в файл логов.
 * @param file    Указатель на поток вывода.
 * @param message Строка, которую необходимо добавить.
*/
void LogLine(FILE* file, const char* message, int logLevel)
{
    assert(message);
    assert(file);

    time_t rawTime = time(nullptr);
    tm* curTime = localtime(&rawTime);

    const char* dateFormat = "00:00:00 > ";
    size_t dateLength = strlen(dateFormat) + 1;
    char* date = (char*)calloc(dateLength, sizeof(char));

    if (date)
    {
        strftime(date, dateLength, "%H:%M:%S > ", curTime);
    
        fseek(file, TextOffset, SEEK_END);
        switch (logLevel)
        {
            case DEBUG:
                fprintf(file, "<pre><font color=\"5C65C0\">%s</font><font color=\"E0E0E0\">%s</font><\pre>\n", date, message);
                break;
            case WARNING:
                fprintf(file, "<pre><font color=\"5C65C0\">%s</font><font color=\"E89827\">%s</font><\pre>\n", date, message);
                break;
            case ERROR:
                fprintf(file, "<pre><font color=\"5C65C0\">%s</font><font color=\"E84127\">%s</font><\pre>\n", date, message);
                break;
        }
        free(date);

        fputs("</body>\n"
                "</html>\n", file);
    }


    fflush(file);
}