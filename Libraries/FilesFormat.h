#ifndef FILES_FORMAT_H_
#define FILES_FORMAT_H_


struct FileHeader
{
    int    Signature;
    int    AssemblerVersion;
    int    ProcessorVersion;
    int    DisasseblerVersion;
    size_t HeaderSize;
    size_t BodySize;
    size_t CommandsCount;
    int    _reserved         :32;
};

const size_t HeaderFileSizeOffset = sizeof(int) * 4 + sizeof(size_t);

const int AssemblerVesrion    = 1;
const int ProcessorVersion    = 1;
const int DisassemblerVersion = 1;


extern FileHeader ProcessorFileHeader;

FILE* OpenFile(const char* fileName, const char* mode);

FileHeader ReadFileHeader(FILE* file);

char* CreateOutFileName(char* src);

int CallocString(char** dest, const char* src);


#endif
