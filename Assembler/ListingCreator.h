#ifndef LISTING_CREATOR_H_
#define LISTING_CREATOR_H_


#include <stdio.h>

void CreateListingFileHeader(FILE* file, const char* fileName);

void AddLineToListing(FILE* file, size_t ip, const char* msg, const char* bin);


#endif // !LISTING_CREATOR_H_