#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


#include "ListingCreator.h"

void CreateListingFileHeader(FILE* file, const char* fileName)
{
    fprintf(file, "Листинг компиляции файла <%s>\n", fileName);
    fprintf(file, "%-16s %-*s %-*s\n", "Адрес", ListingMessageLength, "Текст", ListingBinCodeLength, "Бинарный код");
}

void ListingAddLine(FILE* file, size_t ip, const char* msg, const char* bin)
{
    fprintf(file, "%#-16x %-*s %-*s\n", ip, ListingMessageLength, msg, ListingBinCodeLength, bin);
}