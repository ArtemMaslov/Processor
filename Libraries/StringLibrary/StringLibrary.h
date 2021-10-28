#ifndef STRING_LIBRARY_H
#define STRING_LIBRARY_H


#include <stdio.h>

struct String
{
    size_t Length;
    size_t startIndex;
};

struct Text
{
    char*   buffer;
    size_t  bufferSize;
    size_t  stringsCount;
    String* strings;
};

bool ReadFile(Text* text, const char* fileName);

void ParseFileToLines(Text* text);

void WriteTextToFile(Text* text, const char* fileName);

size_t GetFileSize(FILE* file);

void TextDestructor(Text* text);


#endif
