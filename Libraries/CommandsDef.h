/// Добавить элемент в конец стека. ptr - это адрес double числа
#define PUSH(ptr) StackPush(stk, number)

#define GET_REG_INDEX(ptr)                                  \
    GetRegIndex(code, &ip, ptr);                            \
    if (regIndex >= regsCount)                              \
    {                                                       \
        printf("IndexOutOfRangeRegister = %d", regIndex);   \
        return false;                                       \
    }

#define GET_DOUBLE_ARG(ptr) GetDoubleArg(code, &ip, ptr)

#define GET_SIZE_T_ARG(ptr) GetSize_TArg(code, &ip, ptr)

#define GET_REG_VALUE(ptr) cpu->regs[ptr]

#define IF_REG(code)                                        \
    if (cmd.Reg)                                            \
    {   code   }

#define IF_NUMBER(code)                                     \
    if (cmd.Number)                                         \
    {    code     }

#define IF_RAM(code)                                        \
    if (cmd.RAM)                                            \
    {    code     }

#define ELSE(code)                                          \
    else                                                    \
    {    code     }

#define RAM_DELAY Sleep(RamSleep)

#define POP(err_ptr) StackPop(stk, err_ptr)

#define GET_DOUBLE(ptr) *((double*)ptr);

#define STACK_DUMP(err)                                     \
    if (err > 0)                                            \
        StackDump(stk, stdout);

#define _RAM cpu->RAM

#define _REGS cpu->regs

#define JMP_TO(instruction) ip = (int)instruction

#define JMP                                                 \
    {                                                       \
        size_t jmpLabel = 0;                                \
        GET_SIZE_T_ARG(&jmpLabel);                          \
        JMP_TO(jmpLabel);                                   \
    }

#define CALCULATE(action)                                   \
    {                                                       \
        int error = 0;                                      \
        double number1 = GET_DOUBLE(POP(&error));           \
        STACK_DUMP(error);                                  \
                                                            \
        double number2 =  GET_DOUBLE(POP(&error));          \
        STACK_DUMP(error);                                  \
                                                            \
        number2 action= number1;                            \
        PUSH(&number2);                                     \
        STACK_DUMP(error);                                  \
    }

#define UNARY_OPERATION(operation)                          \
    {                                                       \
        int error = 0;                                      \
        double number = GET_DOUBLE(POP(&error));            \
        STACK_DUMP(error);                                  \
                                                            \
        number = operation;                                 \
        PUSH(&number);                                      \
    }

#define JMP_ACTION(action)                                  \
    {                                                       \
        int error = 0;                                      \
        double number1 = GET_DOUBLE(POP(&error));           \
        STACK_DUMP(error);                                  \
                                                            \
        double number2 =  GET_DOUBLE(POP(&error));          \
        STACK_DUMP(error);                                  \
                                                            \
        if (number2 action number1)                         \
            JMP;                                            \
    }

///***-***\\\--///***-***\\\--///***-***\\\
///***-***\\\--///***-***\\\--///***-***\\\
///***-***\\\--///***-***\\\--///***-***\\\

CMD_DEF(0, unknown, 0, 
    { 
        assert("Command is nullptr" == nullptr); 
    })

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
    })

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

                int ramIndex = number1 + _REGS[regIndex];
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
    })

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
    })

CMD_DEF(4, out, 0, 
    {
        int error = 0;
        printf("%lf\n", GET_DOUBLE(POP(&error));
        STACK_DUMP(error);
    })

CMD_DEF(5, add, 0, 
    {
        CALCULATE(+)
    })

CMD_DEF(6, sub, 0, 
    {
        CALCULATE(-)
    })

CMD_DEF(7, mul, 0, 
    {
        CALCULATE(*)
    })

CMD_DEF(8, div, 0, 
    {
        CALCULATE(/)
    })

CMD_DEF(9, hlt, 0,
    {
        return true;
    })

CMD_DEF(10, jmp, -2, 
    {
        JMP;
    })

CMD_DEF(11, ja, -2, 
    {
        JMP_ACTION(>)
    })

CMD_DEF(12, jae, -2, 
    {
        JMP_ACTION(>=)
    })

CMD_DEF(13, jb, -2, 
    {
        JMP_ACTION(<)
    })

CMD_DEF(14, jbe, -2, 
    {
        JMP_ACTION(<=)
    })

CMD_DEF(15, je, -2, 
    {
        JMP_ACTION(==)
    })

CMD_DEF(16, jne, -2, 
    {
        JMP_ACTION(!=)
    })

CMD_DEF(17, call, -2, 
    {
        double retLabel = (double)ip + sizeof (int);
        PUSH(&retLabel);
        
        JMP;
    })

CMD_DEF(18, ret, 0, 
    {
        int error = 0;
        double retLabel = GET_DOUBLE(POP(&error));
        STACK_DUMP(error)

        JMP_TO(retLabel)
    })

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
    })

CMD_DEF(20, cos, 0, 
    {
        UNARY_OPERATION(cos(number * PI / 180))
    })

CMD_DEF(21, sin, 0, 
    {
        UNARY_OPERATION(sin(number * PI / 180))
    })

CMD_DEF(22, sqrt, 0, 
    {
        UNARY_OPERATION(sqrt(number))
    })

CMD_DEF(23, int, 0, 
    {
        UNARY_OPERATION((int)(number))
    })

CMD_DEF(24, meow, 0, 
    {
        double number = 0;
        GET_DOUBLE_ARG(&number);

        for (int st = 0; st < (int)number; st++)
            puts("Мяяяууу!");
    })


#undef PUSH
#undef GET_REG_INDEX
#undef GET_DOUBLE_ARG
#undef GET_SIZE_T_ARG
#undef GET_REG_VALUE
#undef IF_REG
#undef IF_NUMBER
#undef IF_RAM
#undef ELSE
#undef RAM_DELAY
#undef POP
#undef GET_DOUBLE
#undef STACK_DUMP
#undef _RAM
#undef _REGS
#undef JMP_TO
#undef JMP
#undef CALCULATE
#undef UNARY_OPERATION
#undef JMP_ACTION


#undef CMD_DEF