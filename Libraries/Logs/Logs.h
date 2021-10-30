#ifndef LOGS_H_
#define LOGS_H_

#include <stdio.h>

extern int TextOffset;

enum LogLevel
{
    DEBUG = 0,
    WARNING = 1,
    ERROR = 2
};

/**
 * @brief         Конструктор файла логов.
 * @param file    Указатель на поток вывода.
 * @param caption Заголовок файла логов
*/
void LogConstructor(FILE *file, const char* caption);

/**
 * @brief      Закрывает файл логов.
 * @param file Указатель на поток вывода.
*/
void LogDestructor(FILE* file);

/**
 * @brief         Добавляет строку в файл логов.
 * @param file    Указатель на поток вывода.
 * @param message Строка, которую необходимо добавить.
*/
void LogLine(FILE* file, const char* message, int error = DEBUG);


#endif // !LOGS_H_
