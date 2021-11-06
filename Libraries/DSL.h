//#ifndef DSL_H_
//#define DSL_H_

/// �������� ������� � ����� �����. ptr - ��� ��������� �� double.
#define PUSH(ptr) StackPush(stk, (ptr))

/// ������� ��������� ������� �� �����. err_ptr - ��� ��������� �� int, ����� ��� ��������� ���� ������, ����� ���� nullptr.
#define POP(err_ptr) StackPop(stk, (err_ptr))

/// �������� ����� �������� �� ����. ptr - ��� ��������� �� �������� ������, ��� size_t.
#define GET_REG_INDEX(ptr)                                  \
    GetArg(code, &ip, sizeof(size_t), (ptr));               \
    if (*(ptr) >= regsCount)                                \
    {                                                       \
        printf("IndexOutOfRangeRegister = %d", *(ptr));     \
        return false;                                       \
    }

/// �������� �������� ���� double �� ����. ptr - ��� ��������� �� �������� ������, ��� double.
#define GET_DOUBLE_ARG(ptr) GetArg(code, &ip, sizeof(double), (ptr))

/// �������� �������� ���� size_t �� ����. ptr - ��� ��������� �� �������� ������, ��� size_t.
#define GET_SIZE_T_ARG(ptr) GetArg(code, &ip, sizeof(size_t), (ptr))

/// �������� ��������, ������� � �������� ����������. ptr - ��� ��������� �� ����� ��������, ��� size_t.
#define GET_REG_VALUE(ptr) cpu->regs[ptr]

/// ��������� code, ���� ������� �������� ������� � ���������.
#define IF_REG(code)                                        \
    if (cmd.Reg)                                            \
    {    code     }

/// ��������� code, ���� ������� �������� �������� ��������.
#define IF_NUMBER(code)                                     \
    if (cmd.Number)                                         \
    {    code     }

/// ��������� code, ���� ������� �������� ����������� ������ � ���������.
#define IF_RAM(code)                                        \
    if (cmd.RAM)                                            \
    {    code     }

/// ��������� code, ���� ������� if �����.
#define ELSE(code)                                          \
    else                                                    \
    {    code     }

/// �������� ��� ��������� � ����������� ������. ����������, ��� ��������� �������� � ���������� ������, � � ����������� ������� - ��������.
#define RAM_DELAY Sleep(RamSleep)

/// ������� �������� �� ��������� �� double. ptr - ��������� �� ������� ������, � ������� �������� double �����.
#define GET_DOUBLE(ptr) *((double*)(ptr))

/// ���� err ������� �� 0, �� ���������� ���������� � ������� ��������� �����. ��� err = int. 
#define STACK_DUMP(err)                                     \
    if ((err) > 0)                                          \
        StackDump(stk, stdout);

/// �������� ������ � ����������� ������ ����������.
#define _RAM cpu->RAM

/// �������� ������ � ��������� ����������.
#define _REGS cpu->regs

/// ��������� ������� � instruction.
#define JMP_TO(instruction) ip = (int)(instruction)

/// ���������� ������� jmp/call. �������� ����� �������� �� ���� � ��������� �������.
#define JMP                                                 \
    {                                                       \
        size_t jmpLabel = 0;                                \
        GET_SIZE_T_ARG(&jmpLabel);                          \
        JMP_TO(jmpLabel);                                   \
    }

/// ������� �� ����� ��� ����� � ��������� � ���� action (+, -, *, /, ...), ��������� �������� ������� � ����.
/// ���� number1 - ��������� ����� � �����, � number2 - �������������, �� ��� action == '-', ��������� ����� number2 - number1.
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

/// ������� ����� �� �����, ��������� � ��� ������� ��������, ��������� �������� ������� � ����.
#define UNARY_OPERATION(operation)                          \
    {                                                       \
        int error = 0;                                      \
        double number = GET_DOUBLE(POP(&error));            \
        STACK_DUMP(error);                                  \
                                                            \
        number = operation;                                 \
        PUSH(&number);                                      \
    }

/// ��������� �������, ���� ��� ��������� ����� � ����� ������������� ����������� action (>, <, ==, ...).
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

/// ���������� ������� � �����������.
#define GenerateArgsCommand                                                                     \
    {                                                                                           \
        if (!ParseArgument(&string, cmdType, &ip, outputFile, listingMsg, listingBin))          \
        return false;                                                                           \
    }

/// ���������� ������� ��������.
#define GenerateJumpCommand                                                                     \
    {                                                                                           \
        fwrite(&cmd, commandSize, 1, outputFile);                                               \
        ip += commandSize;                                                                      \
                                                                                                \
        if (!ParseJmpCommand(&string, labels, &labelsIndex, fixUps, &fixUpIndex, outputFile))   \
        {                                                                                       \
            printf("������ ������ ������ <%s>", string);                                        \
            return false;                                                                       \
        }                                                                                       \
        fwrite(&ip_null, sizeof(size_t), 1, outputFile);                                        \
        ip += sizeof(size_t);                                                                   \
    }

/// ���������� ������� �������.
#define GenerateDefaultCommand                                                                  \
    {                                                                                           \
        fwrite(&cmd, commandSize, 1, outputFile);                                               \
        ip += commandSize;                                                                      \
    }

//#endif // !DSL_H_