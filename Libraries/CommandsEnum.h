#ifndef COMMANDS_ENUM_H_
#define COMMANDS_ENUM_H_


//#define DEF_CMD (name, commandNumber, argumentCount, ...) CMD_##name = commandNumber,

const size_t commandSize = sizeof(char);

const int PushParamNumber = 1;
const int PushParamReg    = 2;

union Command
{
    struct
    {
        unsigned int RAM    :1;
        unsigned int Reg    :1;
        unsigned int Number :1;
        unsigned int type   :5;
    };

    unsigned char cmd_byte;
};

#define CMD_DEF(number, name, argc, code, ...) cmd_##name = number,

enum AssemblerCommand
{
    cmd_unknown = 0,
#include "CommandsDef.h"
};

#define CMD_DEF(number, name, argc, code, ...) #name,

const char CommandsName[][8] = 
{
    "",
#include "CommandsDef.h"
};

const char regNames[][3] = 
{
    "qx",
    "wx",
    "ex",

    "rx",
    "tx",
    "yx",

    "ax",
    "sx",
    "dx",

    "fx",
    "gx",
    "hx",
};

const size_t regsCount = sizeof(regNames) / sizeof(regNames[0]);


#endif