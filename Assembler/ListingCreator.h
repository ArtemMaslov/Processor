#ifndef LISTING_CREATOR_H_
#define LISTING_CREATOR_H_


#include <stdio.h>
#include "..\Libraries\FilesFormat.h"


const char ListingMessageLength = 20;
const char ListingBinCodeLength = 50;


void CreateListingFileHeader(FILE* file, FileHeader* header, const char* fileName);

void ListingAddLine(FILE* file, size_t ip, const char* msg, const char* bin);

void WriteBytes(char* bin, const void* data, size_t dataSize);


#endif // !LISTING_CREATOR_H_