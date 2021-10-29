#ifndef COMMANDS_ENUM_H_
#define COMMANDS_ENUM_H_


//#define DEF_CMD (name, commandNumber, argumentCount, ...) CMD_##name = commandNumber,

const size_t commandSize = sizeof(char);

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

char CommandsName[][5] = 
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

//#undef DEF_CMD

#endif