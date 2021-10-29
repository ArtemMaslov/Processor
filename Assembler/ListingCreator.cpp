#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


#include "ListingCreator.h"

void CreateListingFileHeader(FILE* file, const char* fileName)
{
    fprintf(file, "������� ���������� ����� <%s>\n", fileName);
    fprintf(file, "%16s %20s %30s", "�����", "�����", "�������� ���");
}

void AddLineToListing(FILE* file, size_t ip, const char* msg, const char* bin)
{
    fprintf(file, "%-16xd %20s %30s", ip, msg, bin);
}