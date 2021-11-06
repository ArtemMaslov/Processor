//#ifndef DSL_H_
//#define DSL_H_

/// Добавить элемент в конец стека. ptr - это указатель на double.
#define PUSH(ptr) StackPush(stk, (ptr))

/// Удалить последний элемент из стека. err_ptr - это указатель на int, нужен для получения кода ошибки, может быть nullptr.
#define POP(err_ptr) StackPop(stk, (err_ptr))

/// Получить номер регистра из кода. ptr - это указатель на выходные данные, тип size_t.
#define GET_REG_INDEX(ptr)                                  \
    GetArg(code, &ip, sizeof(size_t), (ptr));               \
    if (*(ptr) >= regsCount)                                \
    {                                                       \
        printf("IndexOutOfRangeRegister = %d", *(ptr));     \
        return false;                                       \
    }

/// Получить аргумент типа double из кода. ptr - это указатель на выходные данные, тип double.
#define GET_DOUBLE_ARG(ptr) GetArg(code, &ip, sizeof(double), (ptr))

/// Получить аргумент типа size_t из кода. ptr - это указатель на выходные данные, тип size_t.
#define GET_SIZE_T_ARG(ptr) GetArg(code, &ip, sizeof(size_t), (ptr))

/// Получить значение, лежащее в регистре процессора. ptr - это указатель на номер регистра, тип size_t.
#define GET_REG_VALUE(ptr) cpu->regs[ptr]

/// Выполняет code, если команда содержит регистр в аргументе.
#define IF_REG(code)                                        \
    if (cmd.Reg)                                            \
    {    code     }

/// Выполняет code, если команда содержит числовой аргумент.
#define IF_NUMBER(code)                                     \
    if (cmd.Number)                                         \
    {    code     }

/// Выполняет code, если команда содержит оперативную память в аргументе.
#define IF_RAM(code)                                        \
    if (cmd.RAM)                                            \
    {    code     }

/// Выполняет code, если условие if ложно.
#define ELSE(code)                                          \
    else                                                    \
    {    code     }

/// Задержка при обращении к оперативной памяти. Показывает, что процессор работает с регистрами быстро, а с оперативной памятью - медленно.
#define RAM_DELAY Sleep(RamSleep)

/// Извлечь значение из указателя на double. ptr - указатель на область памяти, в которой хранится double число.
#define GET_DOUBLE(ptr) *((double*)(ptr))

/// Если err отличен от 0, то вызывается информация о текущем состоянии стека. Тип err = int. 
#define STACK_DUMP(err)                                     \
    if ((err) > 0)                                          \
        StackDump(stk, stdout);

/// Получить доступ к оперативной памяти процессора.
#define _RAM cpu->RAM

/// Получить доступ к регистрам процессора.
#define _REGS cpu->regs

/// Совершить переход к instruction.
#define JMP_TO(instruction) ip = (int)(instruction)

/// Обработать команду jmp/call. Получить адрес перехода из кода и выполнить переход.
#define JMP                                                 \
    {                                                       \
        size_t jmpLabel = 0;                                \
        GET_SIZE_T_ARG(&jmpLabel);                          \
        JMP_TO(jmpLabel);                                   \
    }

/// Извлечь из стека два числа и выполнить с ними action (+, -, *, /, ...), результат положить обратно в стек.
/// Если number1 - последнее число в стеке, а number2 - предпоследнее, то при action == '-', результат будет number2 - number1.
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

/// Извлечь число из стека, выполнить с ним унарную операцию, результат положить обратно в стек.
#define UNARY_OPERATION(operation)                          \
    {                                                       \
        int error = 0;                                      \
        double number = GET_DOUBLE(POP(&error));            \
        STACK_DUMP(error);                                  \
                                                            \
        number = operation;                                 \
        PUSH(&number);                                      \
    }

/// Выполнить переход, если два последних числа в стеке удовлетворяют соотношению action (>, <, ==, ...).
#define JMP_ACTION(action)                                  \
    {                                                       \
        size_t jmpLabel = 0;                                \
        GET_SIZE_T_ARG(&jmpLabel);                          \
                                                            \
        int error = 0;                                      \
        double number1 = GET_DOUBLE(POP(&error));           \
        STACK_DUMP(error);                                  \
                                                            \
        double number2 =  GET_DOUBLE(POP(&error));          \
        STACK_DUMP(error);                                  \
                                                            \
        if (number2 action number1)                         \
            JMP_TO(jmpLabel);                               \
    }


///***-***\\\--///***----***\\\--///***-***\\\
///***-***\\\--///AsmDefines\\\--///***-***\\\
///***-***\\\--///***----***\\\--///***-***\\\

/// Обработать команду с аргументами.
#define GenerateArgsCommand                                                                     \
    {                                                                                           \
        if (!ParseArgument(&string, cmdType, &ip, outputFile, listingMsg, listingBin))          \
        return false;                                                                           \
    }

/// Обработать команду перехода.
#define GenerateJumpCommand                                                                     \
    {                                                                                           \
        fwrite(&cmd, commandSize, 1, outputFile);                                               \
        ip += commandSize;                                                                      \
                                                                                                \
        if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile))   \
        {                                                                                       \
            printf("Ошибка чтения строки <%s>", string);                                        \
            return false;                                                                       \
        }                                                                                       \
        fwrite(&ip_null, sizeof(size_t), 1, outputFile);                                        \
        ip += sizeof(size_t);                                                                   \
    }

/// Обработать обычную команду.
#define GenerateDefaultCommand                                                                  \
    {                                                                                           \
        fwrite(&cmd, commandSize, 1, outputFile);                                               \
        ip += commandSize;                                                                      \
    }

//#endif // !DSL_H_