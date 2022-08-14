#include "DSL.h"

///***-***\\\--///***-***\\\--///***-***\\\
///***-***\\\--///***-***\\\--///***-***\\\
///***-***\\\--///***-***\\\--///***-***\\\

//CMD_DEF(0, unknown, 0, 
//    { 
//        assert("Command is nullptr" == nullptr); 
//    },
//    ;)

// ???Декодер аргумента в команде???
CMD_DEF(1, push, -1, 
    {
        size_t regIndex = 0;
        double regValue = 0;
        double number  = 0;
        IF_REG
        (
            GET_REG_INDEX(&regIndex);
            regValue = GET_REG_VALUE(regIndex);
        )
        IF_NUMBER
        (
            GET_DOUBLE_ARG(&number);
        )
        
        number += regValue;
        IF_RAM
        (
            PUSH(_RAM + (int)number);
            RAM_DELAY;
        )
        ELSE
        (
            PUSH(&number);
        )
    },
    GenerateArgsCommand,
    DisasmArgsCommand)

CMD_DEF(2, pop, -1, 
    {
        int    error    = 0;
        size_t regIndex = 0;
        double number   = 0;
        if (cmd.Reg == 0 && cmd.Number == 0 && cmd.RAM == 0)
            POP(&error);
        else
        {
            IF_RAM
            (
                IF_REG
                (
                    GET_REG_INDEX(&regIndex);
                )
                IF_NUMBER
                (
                    GET_DOUBLE_ARG(&number);
                )

                int ramIndex = (int)number + (int)(_REGS[regIndex]);
                if (ramIndex < DispOffset) // Оперативная память
                {
                    _RAM[ramIndex] = GET_DOUBLE(POP(&error));
                }
                else // Видео память
                {
                    ramIndex = ramIndex - DispOffset;
                    char ramByte = ((char*)cpu->RAM)[DispOffset * sizeof(double) + ramIndex / 8];
                    int bitValue = (int)(*((double*)StackPop(stk, &error)));

                    unsigned char bitMask = 1 << (7 - ramIndex % 8);
                    if (bitValue)
                        ((char*)_RAM)[DispOffset * sizeof(double) + ramIndex / 8] = (ramByte | bitMask);
                    else
                        ((char*)_RAM)[DispOffset * sizeof(double) + ramIndex / 8] = (ramByte & (~bitMask));

                }
                RAM_DELAY;
            )
            ELSE
            (
                IF_REG
                (
                    GET_REG_INDEX(&regIndex);
                    _REGS[regIndex] = GET_DOUBLE(POP(&error));
                )
            )
        }
        STACK_DUMP(error);
    },
    GenerateArgsCommand,
    DisasmArgsCommand)

CMD_DEF(3, in, 0, 
    {
        double doubleVal = 0;
        puts("Введите число (Не число для завершения программы):");
        if (scanf("%lf", &doubleVal))
        {
            PUSH(&doubleVal);
        }
        else
        {
            printf("Программа завершена\n");
            return false;
        }
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(4, out, 0,
    {
        int error = 0;
        double* number = (double*)POP(&error);
        if (number)
            printf("%lf\n", GET_DOUBLE(number));
        else
            puts("");
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(5, add, 0, 
    {
        CALCULATE(+)
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(6, sub, 0, 
    {
        CALCULATE(-)
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(7, mul, 0, 
    {
        CALCULATE(*)
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(8, div, 0, 
    {
        CALCULATE(/)
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(9, hlt, 0,
    {
        return true;
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(10, jmp, -2, 
    {
        JMP;
    },
    GenerateJumpCommand,
    DisasmJumpCommand)

CMD_DEF(11, ja, -2, 
    {
        JMP_ACTION(>)
    },
    GenerateJumpCommand,
    DisasmJumpCommand)

CMD_DEF(12, jae, -2, 
    {
        JMP_ACTION(>=)
    },
    GenerateJumpCommand,
    DisasmJumpCommand)

CMD_DEF(13, jb, -2, 
    {
        JMP_ACTION(<)
    },
    GenerateJumpCommand,
    DisasmJumpCommand)

CMD_DEF(14, jbe, -2, 
    {
        JMP_ACTION(<=)
    },
    GenerateJumpCommand,
    DisasmJumpCommand)

CMD_DEF(15, je, -2, 
    {
        JMP_ACTION(==)
    },
    GenerateJumpCommand,
    DisasmJumpCommand)

CMD_DEF(16, jne, -2, 
    {
        JMP_ACTION(!=)
    },
    GenerateJumpCommand,
    DisasmJumpCommand)

CMD_DEF(17, call, -2, 
    {
        double retLabel = (double)ip + sizeof (size_t);
        PUSH(&retLabel);
        
        JMP;
    },
    GenerateJumpCommand,
    DisasmJumpCommand)

CMD_DEF(18, ret, 0, 
    {
        int error = 0;
        double retLabel = GET_DOUBLE(POP(&error));
        STACK_DUMP(error)

        JMP_TO(retLabel);
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(19, dsp, 0, 
    {
        size_t dispBytesCount = (DispHeightD * DispWidthD / (8 * sizeof(double)) + 1) * 8;
        putchar('\n');
        size_t pixelsCount = 0;
        for (size_t st = 0; st < dispBytesCount; st++)
        {
            char byte = ((char*)_RAM)[DispOffset * sizeof(double) + st];

            for (unsigned char pixelMask = 1 << 7; pixelMask > 0 && pixelsCount < DispHeightD * DispWidthD; pixelMask>>=1)
            {
                if (byte & pixelMask)
                    putchar('*');
                else
                    putchar('.');
                pixelsCount++;
                if (pixelsCount % DispWidthD == 0)
                    putchar('\n');
            }
        }
        fflush(stdout);
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(20, cos, 0, 
    {
        UNARY_OPERATION(cos(number * PI / 180))
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(21, sin, 0, 
    {
        UNARY_OPERATION(sin(number * PI / 180))
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(22, sqrt, 0, 
    {
        UNARY_OPERATION(sqrt(number))
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

CMD_DEF(23, int, 0, 
    {
        UNARY_OPERATION((int)(number))
    },
    GenerateDefaultCommand,
    DisasmDefaultCommand)

//CMD_DEF(24, meow, -1, 
//    {
//        double number = 0;
//        GET_DOUBLE_ARG(&number);
//
//        for (int st = 0; st < (int)number; st++)
//            puts("Мяяяууу!");
//    },
//    GenerateArgsCommand,
//    DisasmArgsCommand)

#undef CMD_DEF
        
#include "UndefDSL.h"