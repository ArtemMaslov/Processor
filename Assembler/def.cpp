# 0 "Assembler.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "Assembler.cpp"
# 16 "Assembler.cpp"
static FILE* logFile;
char* listingFileName = nullptr;
char* asmLogsFileName = nullptr;
char* outputFileName = nullptr;


static AssemblerCommand GetNextCommand(char** buffer);

static bool GetNextDoubleArgument(char** buffer, double* number);

static bool GetNextIntArgument(char** buffer, int* number);

static bool GenerateCode(Assembler* assembler);

static bool GenerateHeader(Assembler* assembler);

static bool GenerateEnding(Assembler* assembler);

static bool GetNextRegister(char** buffer, int* regIndex);

static bool IgnoreComment(char** buffer);

static bool GetNextArgument(char** buffer, const char* format, void* out_number);

static bool ParseArgument(char** buffer, AssemblerCommand cmdType, size_t* instructionPointer, FILE* outputFile, char* msg, char* bin);

static bool ParseLabel(char** buffer, size_t ip, Label* labels, size_t* labelIndex);

static bool GetLabelArg(char** buffer, char** start, size_t* length, bool* IsLabelInit);

static bool ParseJmpCommand(char** buffer, Label* labels, size_t* labelIndex, FixUp* fixUps, size_t* fixUpIndex, FILE* outputFile);


void AssemblerConstructor(FILE* inputFile, FILE* outputFile, FILE* listingFile, FILE* asmLogFile)
{
    assert(inputFile);
    assert(outputFile);
    assert(listingFile);
    assert(asmLogFile);

    assert(listingFileName);
    assert(asmLogsFileName);
    assert(outputFileName);

    Assembler assembler = {};
    assembler.text = {};
    assembler.bufferIndex = 0;
    assembler.listingFile = listingFile;
    assembler.outputFile = outputFile;

    FileHeader header = {};
    header.BodySize = GetFileSize(inputFile);
    bool res = ReadFile(&assembler.text, &header, inputFile);

    logFile = asmLogFile;

    if (res)
    {
        LogLine(logFile, "Файл успешно прочитан");
        ParseFileToLines(&assembler.text);

        CreateListingFileHeader(listingFile, nullptr, outputFileName);

        assembler.labels = (Label*)calloc(LabelsCount, sizeof(Label));
        assembler.fixUps = (FixUp*)calloc(FixUpsCount, sizeof(FixUp));

        bool result = false;

        GenerateHeader(&assembler);

        GenerateCode(&assembler);

        GenerateEnding(&assembler);

        free(assembler.labels);
        free(assembler.fixUps);
    }
}

static bool GenerateHeader(Assembler* assembler)
{
    assembler->header = ProcessorFileHeader;
    fwrite(&assembler->header, sizeof(FileHeader), 1, assembler->outputFile);

    return true;
}
# 115 "Assembler.cpp"
static bool GenerateCode(Assembler* assembler)
{
    assert(assembler);

    LogLine(logFile, "ParseText");

    FILE* outputFile = assembler->outputFile;

    char* buffer = assembler->text.buffer;
    String* strings = assembler->text.strings;
    size_t bufferSize = assembler->text.bufferSize;
    size_t stringsCount = assembler->text.stringsCount;

    bool hlt = false;
    size_t ip = 0;
    size_t commandsCount = 0;
    size_t labelsIndex = 0;
    size_t fixUpIndex = 0;

    Label* labels = assembler->labels;
    FixUp* fixUps = assembler->fixUps;

    for (size_t st = 0; st < stringsCount; st++)
    {
        char* string = buffer + strings[st].startIndex;

        if (IgnoreComment(&string))
            continue;

        AssemblerCommand cmdType = GetNextCommand(&string);
        Command cmd = {};
        cmd.type = cmdType;

        size_t oldInstructionPointer = ip;

        char listingMsg[ListingMessageLength] = {};
        char listingBin[ListingBinCodeLength] = {};

        if (cmdType == cmd_unknown)
        {
            string = buffer + strings[st].startIndex;
            if (!ParseLabel(&string, ip, labels, &labelsIndex))
                return false;
        }
        else
        {
            if (cmdType == cmd_hlt)
                hlt = true;

            switch (cmdType)
            {
# 1 "..\\Libraries\\CommandsDef.h" 1
# 1 "..\\Libraries\\DSL.h" 1
# 2 "..\\Libraries\\CommandsDef.h" 2





case cmd_unknown: { LogLine(logFile, "unknown"); sprintf(listingMsg, "unknown"); sprintf(listingBin, "%02x ", cmd.cmd_byte);
size_t ip_null = 0; ; break; }





case cmd_push: { 
    LogLine(logFile, "push"); 
    sprintf(listingMsg, "push"); 
    sprintf(listingBin, "%02x ", cmd.cmd_byte); 
    size_t ip_null = 0; 
    { if (!ParseArgument(&string, cmdType, &ip, outputFile, listingMsg, listingBin)) return false; } break; }
# 41 "..\\Libraries\\CommandsDef.h"
case cmd_pop: { LogLine(logFile, "pop"); sprintf(listingMsg, "pop"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { if (!ParseArgument(&string, cmdType, &ip, outputFile, listingMsg, listingBin)) return false; } break; }
# 94 "..\\Libraries\\CommandsDef.h"
case cmd_in: { LogLine(logFile, "in"); sprintf(listingMsg, "in"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }
# 110 "..\\Libraries\\CommandsDef.h"
case cmd_out: { LogLine(logFile, "out"); sprintf(listingMsg, "out"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }
# 119 "..\\Libraries\\CommandsDef.h"
case cmd_add: { LogLine(logFile, "add"); sprintf(listingMsg, "add"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }





case cmd_sub: { LogLine(logFile, "sub"); sprintf(listingMsg, "sub"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }





case cmd_mul: { LogLine(logFile, "mul"); sprintf(listingMsg, "mul"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }





case cmd_div: { LogLine(logFile, "div"); sprintf(listingMsg, "div"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }





case cmd_hlt: { LogLine(logFile, "hlt"); sprintf(listingMsg, "hlt"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }





case cmd_jmp: { LogLine(logFile, "jmp"); sprintf(listingMsg, "jmp"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile)) { printf("Ошибка чтения строки <%s>", string); return false; } fwrite(&ip_null, sizeof(size_t), 1, outputFile); ip += sizeof(size_t); } break; }





case cmd_ja: { LogLine(logFile, "ja"); sprintf(listingMsg, "ja"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile)) { printf("Ошибка чтения строки <%s>", string); return false; } fwrite(&ip_null, sizeof(size_t), 1, outputFile); ip += sizeof(size_t); } break; }





case cmd_jae: { LogLine(logFile, "jae"); sprintf(listingMsg, "jae"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile)) { printf("Ошибка чтения строки <%s>", string); return false; } fwrite(&ip_null, sizeof(size_t), 1, outputFile); ip += sizeof(size_t); } break; }





case cmd_jb: { LogLine(logFile, "jb"); sprintf(listingMsg, "jb"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile)) { printf("Ошибка чтения строки <%s>", string); return false; } fwrite(&ip_null, sizeof(size_t), 1, outputFile); ip += sizeof(size_t); } break; }





case cmd_jbe: { LogLine(logFile, "jbe"); sprintf(listingMsg, "jbe"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile)) { printf("Ошибка чтения строки <%s>", string); return false; } fwrite(&ip_null, sizeof(size_t), 1, outputFile); ip += sizeof(size_t); } break; }





case cmd_je: { LogLine(logFile, "je"); sprintf(listingMsg, "je"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile)) { printf("Ошибка чтения строки <%s>", string); return false; } fwrite(&ip_null, sizeof(size_t), 1, outputFile); ip += sizeof(size_t); } break; }





case cmd_jne: { LogLine(logFile, "jne"); sprintf(listingMsg, "jne"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile)) { printf("Ошибка чтения строки <%s>", string); return false; } fwrite(&ip_null, sizeof(size_t), 1, outputFile); ip += sizeof(size_t); } break; }





case cmd_call: { LogLine(logFile, "call"); sprintf(listingMsg, "call"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile)) { printf("Ошибка чтения строки <%s>", string); return false; } fwrite(&ip_null, sizeof(size_t), 1, outputFile); ip += sizeof(size_t); } break; }
# 200 "..\\Libraries\\CommandsDef.h"
case cmd_ret: { LogLine(logFile, "ret"); sprintf(listingMsg, "ret"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }
# 210 "..\\Libraries\\CommandsDef.h"
case cmd_dsp: { LogLine(logFile, "dsp"); sprintf(listingMsg, "dsp"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }
# 234 "..\\Libraries\\CommandsDef.h"
case cmd_cos: { LogLine(logFile, "cos"); sprintf(listingMsg, "cos"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }





case cmd_sin: { LogLine(logFile, "sin"); sprintf(listingMsg, "sin"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }





case cmd_sqrt: { LogLine(logFile, "sqrt"); sprintf(listingMsg, "sqrt"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }





case cmd_int: { LogLine(logFile, "int"); sprintf(listingMsg, "int"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { fwrite(&cmd, commandSize, 1, outputFile); ip += commandSize; } break; }





case cmd_meow: { LogLine(logFile, "meow"); sprintf(listingMsg, "meow"); sprintf(listingBin, "%02x ", cmd.cmd_byte); size_t ip_null = 0; { if (!ParseArgument(&string, cmdType, &ip, outputFile, listingMsg, listingBin)) return false; } break; }
# 270 "..\\Libraries\\CommandsDef.h"
# 1 "..\\Libraries\\UndefDSL.h" 1
# 271 "..\\Libraries\\CommandsDef.h" 2
# 167 "Assembler.cpp" 2
                default:
                    LogLine(logFile, "Неизвестная команда");
                    printf("Неизвестная команда <%s>\n", buffer + strings[st].startIndex);
                    return false;
            }

            ListingAddLine(assembler->listingFile, oldInstructionPointer, listingMsg, listingBin);
        }

        commandsCount++;
        if (!IgnoreComment(&string))
        {
            printf("Ошибка чтения комманды <%s>", buffer + strings[st].startIndex);
            return false;
        }
    }

    assembler->header.CommandsCount = commandsCount;
    assembler->header.BodySize = ip;
    assembler->fixUpIndex = fixUpIndex;

    if (hlt)
    {
        return true;
    }
    else
    {
        puts("Не обнаружена комманда hlt в конце программы");
        return false;
    }
}

static bool GenerateEnding(Assembler* assembler)
{
    FILE* outputFile = assembler->outputFile;

    fseek(outputFile, 0, SEEK_SET);
    fwrite(&assembler->header, sizeof(FileHeader), 1, assembler->outputFile);

    Label* labels = assembler->labels;
    FixUp* fixUps = assembler->fixUps;
    size_t fixUpIndex = assembler->fixUpIndex;

    for (size_t st = 0; st < fixUpIndex; st++)
    {
        Label label = labels[fixUps[st].LabelIndex];

        if (label.inited)
        {
            fseek(outputFile, fixUps[st].fileIndex, SEEK_SET);
            fwrite(&label.ip, sizeof(size_t), 1, outputFile);
        }
        else
        {
            printf("Метка <%s> не проинициализирована", label.msg);
            return false;
        }
    }

    fseek(assembler->listingFile, 0, SEEK_SET);
    CreateListingFileHeader(assembler->listingFile, &assembler->header, outputFileName);
    fseek(assembler->listingFile, 0, SEEK_END);

    return true;
}

static AssemblerCommand GetNextCommand(char** buffer)
{
    assert(buffer);
    assert(*buffer);

    LogLine(logFile, "GetNextCommand");

    while (**buffer)
    {
        for (int st = 0; st < sizeof(CommandsName) / sizeof(CommandsName[0]); st++)
        {
            bool cmp = true;
            int st1 = 0;
            for (const char* cmd = CommandsName[st]; cmd[st1]; st1++)
            {
                if ((*buffer)[st1] != CommandsName[st][st1])
                {
                    cmp = false;
                    break;
                }
            }
            if (cmp && ((*buffer)[st1] == ' ' || (*buffer)[st1] == '\n' ||
                (*buffer)[st1] == '\0' || (*buffer)[st1] == ';' || (*buffer)[st1] == '['))
            {
                *(buffer) += st1;
                return (AssemblerCommand)st;
            }
        }
        (*buffer)++;
    }
    return cmd_unknown;
}

static bool GetNextDoubleArgument(char** buffer, double* number)
{
    assert(buffer);
    assert(*buffer);
    assert(number);

    LogLine(logFile, "GetNextDoubleArgument");

    int stringOffset = 0;
    int readed = sscanf(*buffer, " %lf%n", number, &stringOffset);

    if (readed == 1)
    {
        (*buffer) += stringOffset;
        return true;
    }
    else
        return false;
}

static bool GetNextArgument(char** buffer, const char* format, void* out_number)
{
    assert(buffer);
    assert(*buffer);
    assert(format);
    assert(out_number);

    LogLine(logFile, "GetNextDoubleArgument");

    int stringOffset = 0;

    size_t formatLength = strlen(format) + 1 + 3;
    char* strFormat = (char*)calloc(formatLength, sizeof(char));
    if (!format)
    {
        puts("Не хватает памяти");
        return false;
    }
    sprintf(strFormat, " %s%%n", format);

    int readed = sscanf(*buffer, strFormat, out_number, &stringOffset);

    free(strFormat);

    if (readed == 1)
    {
        (*buffer) += stringOffset;
        return true;
    }
    else
        return false;
}

static bool GetNextIntArgument(char** buffer, int* number)
{
    assert(buffer);
    assert(*buffer);
    assert(number);

    LogLine(logFile, "GetNextIntArgument");

    int stringOffset = 0;
    int readed = sscanf(*buffer, " %d%n", number, &stringOffset);

    if (readed == 1)
    {
        (*buffer) += stringOffset;
        return true;
    }
    else
        return false;
}

static bool IgnoreComment(char** buffer)
{
    assert(buffer);
    assert(*buffer);

    LogLine(logFile, "IgnoreComment");

    bool res = true;
    while (**buffer)
    {
        unsigned char c = **buffer;
        if (!isspace(c))
        {
            if (**buffer == ';')
                return true;
            else
                return false;
        }
        (*buffer)++;
    }
    return true;
}

static bool GetNextRegister(char** buffer, int* regIndex)
{
    assert(buffer);
    assert(*buffer);
    assert(regIndex);

    LogLine(logFile, "GetNextRegister");

    char* buf = *buffer;
    while (*buf)
    {
        while (isspace((unsigned char)(*buf)))
            buf++;

        for (int st = 0; st < sizeof(regNames) / sizeof(regNames[0]); st++)
        {
            bool cmp = true;
            int st1 = 0;
            for (const char* cmd = regNames[st]; cmd[st1]; st1++)
            {
                if (buf[st1] != regNames[st][st1])
                {
                    cmp = false;
                    break;
                }
            }
            if (cmp && (buf[st1] == ' ' || buf[st1] == '\n' ||
                buf[st1] == '\0' || buf[st1] == ';' || buf[st1] == '+' || buf[st1] == '-' || buf[st1] == ']'))
            {
                buf += st1;
                (*buffer) = buf;
                *regIndex = st;
                return true;
            }
        }
        buf++;
    }

    return false;
}

static bool ParseArgument(char** buffer, AssemblerCommand cmdType, size_t* instructionPointer, FILE* outputFile, char* msg, char* bin)
{
    double doubleVal = 0;
    int regIndex = 0;
    bool number = false;
    bool reg = false;
    int RAM = 0;
    char* bufPtr = *buffer;

    while (**buffer)
    {
        if (**buffer == '+' || isspace(**buffer))
        {
            (*buffer)++;
        }
        else if (**buffer == '[')
        {
            RAM = 1;
            (*buffer)++;
        }
        else if (**buffer == ']' && RAM == 1)
        {
            RAM = 2;
            (*buffer)++;
            if (!IgnoreComment(buffer))
            {
                printf("Ошибка чтения строки <%s>", bufPtr);
                return false;
            }
        }
        else if (GetNextDoubleArgument(buffer, &doubleVal))
        {
            if (!number)
                number = true;
            else
            {
                printf("Ошибка чтения строки. Числовой аргумент встретился два раза. <%s>", *buffer);
                return false;
            }
        }
        else if (GetNextRegister(buffer, &regIndex))
        {
            if (!reg)
            {
                LogLine(logFile, "push number");
                reg = true;
            }
            else
            {
                printf("Ошибка чтения строки. Регистр встретился два раза в аргументе. <%s>", *buffer);
                return false;
            }
        }
        else if (IgnoreComment(buffer))
            break;
        else
        {
            printf("Ошибка чтения строки <%s>", bufPtr);
            return false;
        }
    }

    if (RAM != 0 && RAM != 2)
    {
        printf("Ошибка чтения строки. RAM. <%s>", bufPtr);
        return false;
    }

    Command cmd = {};
    cmd.type = cmdType;

    if (RAM == 2)
        cmd.RAM = 1;

    if (number)
        cmd.Number = 1;
    if (reg)
        cmd.Reg = 1;

    if (reg && number)
    {
        LogLine(logFile, "push reg and number");
        sprintf(msg, "%s %s + %d", CommandsName[cmdType], regNames[regIndex], (int)doubleVal);
    }
    else if (reg)
    {
        LogLine(logFile, "push reg");
        sprintf(msg, "%s %s", CommandsName[cmdType], regNames[regIndex]);
    }
    else if (number)
    {
        LogLine(logFile, "push number");
        sprintf(msg, "%s %lf", CommandsName[cmdType], doubleVal);
    }

    int val1 = ((char*)&cmd)[0];
    fprintf(outputFile, "%c", val1);
    sprintf(bin, "%02x ", val1);
    bin += 3;

    *instructionPointer += commandSize;

    if (reg)
    {
        fprintf(outputFile, "%c", regIndex);
        sprintf(bin, "%02x ", regIndex);
        bin += 3;
        *instructionPointer += sizeof(char);
    }
    if (number)
    {
        fwrite(&doubleVal, sizeof(double), 1, outputFile);
        WriteBytes(bin, &doubleVal, sizeof(double));
        *instructionPointer += sizeof(double);
    }
    return true;
}

static bool ParseLabel(char** buffer, size_t ip, Label* labels, size_t* labelIndex)
{
    assert(buffer);
    assert(*buffer);
    assert(labels);
    assert(labelIndex);

    assert(*labelIndex < LabelsCount);

    char* start = nullptr;
    size_t length = 0;
    size_t _li = *labelIndex;
    bool IsLabelInit = false;

    if (GetLabelArg(buffer, &start, &length, &IsLabelInit) && IsLabelInit)
    {
        assert(length < LabelNameLength);

        char* colon = strchr(start, ':');
        if (colon)
            *colon = '\0';

        for (size_t st = 0; st < _li; st++)
        {
            if (strcmp(start, labels[st].msg) == 0)
            {
                if (!labels[st].inited)
                {
                    labels[st].ip = ip;
                    labels[st].inited = true;
                    return true;
                }
                else
                {
                    printf("Метка <%.*s> встретилась в коде дважды", length, start);
                    return false;
                }
            }
        }

        strncpy(labels[_li].msg, start, length);
        labels[_li].inited = true;
        labels[_li].ip = ip;
        (*labelIndex)++;

        return true;
    }
    printf("Строка <%s> меткой не является", *buffer);
    return false;
}

static bool ParseJmpCommand(char** buffer, Label* labels, size_t* labelIndex, FixUp* fixUps, size_t* fixUpIndex, FILE* outputFile)
{
    assert(buffer);
    assert(*buffer);
    assert(labels);
    assert(fixUps);
    assert(labelIndex);
    assert(fixUpIndex);
    assert(outputFile);

    assert(*labelIndex < LabelsCount);
    assert(*fixUpIndex < FixUpsCount);

    char* start = nullptr;
    size_t length = 0;
    size_t _li = *labelIndex;
    size_t _fi = *fixUpIndex;

    if (GetLabelArg(buffer, &start, &length, nullptr))
    {
        assert(length < LabelNameLength);

        char* colon = strchr(start, ':');
        if (colon)
            *colon = '\0';

        for (size_t st = 0; st < _li; st++)
        {
            if (strcmp(start, labels[st].msg) == 0)
            {
                fixUps[_fi].LabelIndex = st;
                fixUps[_fi].fileIndex = ftell(outputFile);
                (*fixUpIndex)++;
                return true;
            }
        }

        strncpy(labels[_li].msg, start, length);
        labels[_li].ip = 0;
        (*labelIndex)++;

        fixUps[_fi].LabelIndex = _li;
        fixUps[_fi].fileIndex = ftell(outputFile);
        (*fixUpIndex)++;

        return true;
    }
    printf("Строка <%s> меткой не является", *buffer);
    return false;
}

static bool GetLabelArg(char** buffer, char** start, size_t* length, bool* IsLabelInit)
{
    assert(buffer);
    assert(*buffer);
    assert(start);

    assert(length);


    bool stringInited = false;
    size_t strLength = 0;

    while (**buffer)
    {
        if (**buffer == ':')
        {
            if (IsLabelInit)
                *IsLabelInit = true;
            (*buffer)++;
            break;
        }
        if (!stringInited && !isspace(**buffer))
        {
            *start = *buffer;
            stringInited = true;
        }
        if (stringInited)
                strLength++;
        (*buffer)++;
    }

    if (stringInited)
    {
        *length = strLength;
        return true;
    }
    else
        return false;
}
