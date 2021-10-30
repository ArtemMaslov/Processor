#ifndef COMMANDS_ENUM_H_
#define COMMANDS_ENUM_H_



//#define DEF_CMD (name, commandNumber, argumentCount, ...) CMD_##name = commandNumber,

const size_t commandSize = sizeof(char);

const int PushParamNumber = 1;
const int PushParamReg    = 2;

struct Command
{
    int RAM    :1;
    int params :2;
    int type   :5;
};


enum AssemblerCommand
{
//#include "..\CPU\Commands.h"
    CMD_UNKNOWN  = 0,
    CMD_PUSH     = 1,
    CMD_POP      = 2,
    CMD_IN       = 3,
    CMD_OUT      = 4,
    CMD_ADD      = 5,
    CMD_SUB      = 6,
    CMD_MUL      = 7,
    CMD_DIV      = 8,
    CMD_HLT      = 9,
};

const char CommandsName[][5] = 
{
    "",
    "push",
    "pop",
    "in",
    "out",
    "add",
    "sub",
    "mul",
    "div",
    "hlt",
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