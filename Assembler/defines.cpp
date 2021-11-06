# 0 "..\\Libraries\\CommandsEnum.h"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "..\\Libraries\\CommandsEnum.h"






const size_t commandSize = sizeof(char);

const int PushParamNumber = 1;
const int PushParamReg = 2;

union Command
{
    struct
    {
        unsigned int RAM :1;
        unsigned int Reg :1;
        unsigned int Number :1;
        unsigned int type :5;
    };

    unsigned char cmd_byte;
};



enum AssemblerCommand
{
# 1 "..\\Libraries\\CommandsDef.h" 1
# 98 "..\\Libraries\\CommandsDef.h"
cmd_unknown = 0,




cmd_push = 1,
# 130 "..\\Libraries\\CommandsDef.h"
cmd_pop = 2,
# 182 "..\\Libraries\\CommandsDef.h"
cmd_in = 3,
# 197 "..\\Libraries\\CommandsDef.h"
cmd_out = 4,







cmd_add = 5,




cmd_sub = 6,




cmd_mul = 7,




cmd_div = 8,




cmd_hlt = 9,




cmd_jmp = 10,




cmd_ja = 11,




cmd_jae = 12,




cmd_jb = 13,




cmd_jbe = 14,




cmd_je = 15,




cmd_jne = 16,




cmd_call = 17,







cmd_ret = 18,
# 282 "..\\Libraries\\CommandsDef.h"
cmd_dsp = 19,
# 305 "..\\Libraries\\CommandsDef.h"
cmd_cos = 20,




cmd_sin = 21,




cmd_sqrt = 22,




cmd_int = 23,




cmd_meow = 24,
# 30 "..\\Libraries\\CommandsEnum.h" 2
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
    "jmp",
    "ja",
    "jae",
    "jb",
    "jbe",
    "je",
    "jne",
    "call",
    "ret",
    "dsp",
    "cos",
    "sin",
    "sqrt",
    "int",
    "meow"
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
