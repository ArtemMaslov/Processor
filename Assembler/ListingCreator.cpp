#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


#include "ListingCreator.h"

static size_t HeaderOffset = 0;

void CreateListingFileHeader(FILE* file, FileHeader* header, const char* fileName)
{
    assert(file);
    //assert(header);
    assert(fileName);

    fprintf(file, "Листинг компиляции файла <%s>\n", fileName);

    char* bin = (char*)calloc(sizeof(FileHeader) * 3, sizeof(char));
    if (header)
        WriteBytes(bin, header, sizeof(FileHeader));
    fprintf(file, "%-*s  %-*s\n", 18 + ListingMessageLength, "Заголовок:", sizeof(FileHeader) * 3 - 1, bin);

    fprintf(file, "%-18s %-*s %-*s\n", "Адрес", ListingMessageLength, "Текст", ListingBinCodeLength, "Бинарный код");
}

void ListingAddLine(FILE* file, size_t ip, const char* msg, const char* bin)
{
    assert(file);
    assert(msg);
    assert(bin);

    fprintf(file, "0x%016x %-*s %-*s\n", ip, ListingMessageLength, msg, ListingBinCodeLength, bin);
}

void WriteBytes(char* bin, const void* data, size_t dataSize)
{
    assert(bin);
    assert(data);

    for (int st = 0; st < dataSize; st++)
    {
        unsigned char byte = ((char*)data)[st];
        sprintf(bin + st * 3, "%02x ", byte);
    }
    bin[dataSize * 3 - 1] = '\0';
}