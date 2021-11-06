#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#include "FilesFormat.h"

FileHeader ProcessorFileHeader = 
{
    'MAS!',
    AssemblerVesrion,
    ProcessorVersion,
    DisassemblerVersion,
    sizeof(FileHeader)
};


FILE* OpenFile(const char* fileName, const char* mode)
{
    assert(fileName);
    assert(mode);

    FILE* file = fopen(fileName, mode);
    if (!file)
        printf("������ �������� ����� <%s>, ����� �������� <%s>\n", fileName, mode);

    return file;
}

FileHeader ReadFileHeader(FILE* file)
{
    assert(file);

    FileHeader header = {};

    size_t readed = fread((char*)(&header), sizeof(char), sizeof(header), file);

    if (readed != sizeof(FileHeader))
        puts("��������� ����� �� �� ����� ������");

    return header;
}

char* CreateOutFileName(char* src)
{
    size_t fileNameLength = strlen(src);
    char* outputFileName = (char*)calloc(fileNameLength + 1 + 4, sizeof(char));

    if (!outputFileName)
    {
        puts("�� ������� ������, ����� ������� ��� ��������� �����.\n"
                "������� ��� ��������� ����� ������ ���������� � ��������� ������.");
        return 0;
    }

    char* dotPtr = strrchr(src, '.');
    if (dotPtr)
    {
        size_t dotIndex = dotPtr - src;
        strncat(outputFileName, src, dotIndex);
            
        strncat(outputFileName, "_out", 4);

        strncat(outputFileName, dotPtr, fileNameLength - dotIndex);
    }
    else
    {
        strncat(outputFileName, src, fileNameLength);
        strncat(outputFileName, "_out", 4);
    }
}

int CallocString(char** dest, const char* src)
{
    size_t length = strlen(src) + 1;

    *dest = (char*)calloc(length, sizeof(char));
    
    if (!(*dest))
    {
        puts("�� ������� ������, ����� ������� ��� ��������� �����.\n"
                "������� ��� ��������� ����� ������ ���������� � ��������� ������.");
        return 1;
    }

    strncpy(*dest, src, length);
    return 0;
}