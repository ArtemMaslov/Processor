#ifndef LISTING_CREATOR_H_
#define LISTING_CREATOR_H_


#include <stdio.h>


const char ListingMessageLength = 20;
const char ListingBinCodeLength = 30;


void CreateListingFileHeader(FILE* file, const char* fileName);

void ListingAddLine(FILE* file, size_t ip, const char* msg, const char* bin);


#endif // !LISTING_CREATOR_H_