#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "StringLibrary.h"
#include "..\FilesFormat.h"

bool ReadFile(Text* text, FileHeader* header, FILE* file)
{
    assert(text);
    assert(header);
    assert(file);

    text->buffer       = nullptr;
    text->bufferSize   = 0;
    text->strings      = nullptr;
    text->stringsCount = 0;

    size_t fileSize = header->BodySize;

    text->buffer = (char*)calloc(fileSize + 1, sizeof(char));// Прибавляем 1, чтобы в конце файла был нуль-терминатор

    if (text->buffer)
    {
        size_t readed = fread(text->buffer, sizeof(char), fileSize, file);
        text->bufferSize = readed + 1;

        if (readed == 0)
        {
            puts("Ошибка чтения файла.");
            return false;
        }
        text->buffer[readed] = '\0';
    }
    else
    {
        puts("Ошибка выделения памяти.");
        return false;
    }

    fclose(file);
    return true;
}

void ParseFileToLines(Text* text)
{
    assert(text);

    char* ptr = text->buffer;
    size_t stringsCount = 0;
    bool stringStarted = false;
    for (size_t st = 0; st < text->bufferSize; st++)
    {
        if (ptr[st] != '\n' && ptr[st] != ' ' && ptr[st] != '\t' && !stringStarted)
        {
            stringStarted = true;
        }
        else if ((ptr[st] == '\n' || ptr[st] == '\0') && stringStarted)
        {
            stringsCount++;
            stringStarted = false;
        }
    }
    text->stringsCount = stringsCount;
    
    text->strings = (String*)calloc(stringsCount, sizeof(String));

    if (text->strings)
    {
        ptr = text->buffer;
        size_t bufferSize = text->bufferSize;
        size_t startIndex = 0;
        size_t stringIndex = 0;
        stringStarted = false;

        for (size_t st = 0; st < bufferSize; st++)
        {
            if (st == bufferSize - 2)
                puts("");
            char c = ptr[st];
            if (ptr[st] != '\n' && ptr[st] != ' ' && ptr[st] != '\t' && !stringStarted)
            {
                startIndex = st;
                stringStarted = true;
            }
            else if ((ptr[st] == '\n' || ptr[st] == '\0') && stringStarted)
            {
                text->strings[stringIndex].startIndex = startIndex;
                text->strings[stringIndex].Length = st - startIndex;

                ptr[st] = '\0';

                stringStarted = false;
                stringIndex++;
            }
        }
    }
    else
        puts("Нехватка памяти StringLibrary.ParseFileToLines(Text*)");
}

void WriteTextToFile(Text* text, const char* fileName)
{
    assert(text);
    assert(fileName);
    assert(text->buffer);
    assert(text->strings);

    String* strings = text->strings;
    char* buffer = text->buffer;

    FILE* file = fopen(fileName, "w");

    if (file)
    {
        for (size_t st = 0; st < text->stringsCount; st++)
        {
            char c = buffer[strings[st].startIndex];
            fputs(buffer + strings[st].startIndex, file);
            fputs("\n",file);
        }
        fclose(file);
    }

}

size_t GetFileSize(FILE* file)
{
    assert(file);

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    return fileSize;
}

void TextDestructor(Text* text)
{
    if (text)
    {
        free(text->buffer);
        free(text->strings);
    }
}
