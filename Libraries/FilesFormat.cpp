#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


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
        printf("Ошибка открытия файла <%s>\n", fileName);

    return file;
}

FileHeader ReadFileHeader(FILE* file)
{
    assert(file);

    FileHeader header = {};

    size_t readed = fread((char*)(&header), sizeof(char), sizeof(header), file);

    if (readed != sizeof(FileHeader))
        puts("Заголовок файла не до конца считан");

    return header;
}