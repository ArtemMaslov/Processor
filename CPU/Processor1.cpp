#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cmath>


#include "Processor.h"
#include "..\Libraries\Logs\Logs.h"
#include "..\Libraries\StringLibrary\StringLibrary.h"
#include "..\Libraries\CommandsEnum.h"
#include "..\Libraries\FilesFormat.h"
#include <Windows.h>


static bool ParseText(ProcessorStructure* cpu);

static void GetArg(char* code, size_t* ip, size_t res_size, void* out_res);


void ProcessorConstructor(FILE* inputFile, FILE* stackLogFile, FILE* cpuLogFile)
{
    assert(inputFile);
    assert(stackLogFile);
    assert(cpuLogFile);

    StackLogConstructor(stackLogFile);

    ProcessorStructure cpu = {};

    FileHeader header = ReadFileHeader(inputFile);

    if (header.Signature == ProcessorFileHeader.Signature)
    {
        if (header.ProcessorVersion == ProcessorFileHeader.ProcessorVersion &&
            header.AssemblerVersion == ProcessorFileHeader.AssemblerVersion)
        {
            if (ReadFile(&cpu.text, &header, inputFile))
            {
                cpu.text.bufferSize--;

                StackConstructor(&cpu.stack, sizeof(double));

                ParseText(&cpu);
            }
        }
        else
            printf("Текущая версия процессора %d\n"
                    "Версия процессора в файле %d\n"
                    "Текущая версия ассемблера %d\n"
                    "Версия ассемблера в файле %d\n",
                ProcessorFileHeader.ProcessorVersion, header.ProcessorVersion,
                ProcessorFileHeader.AssemblerVersion, header.AssemblerVersion);
    }
    else
    {
        printf("Неопознанная сигнатура файла <%c%c%c%c>",
            (header.Signature % (0xFF)),
            (header.Signature & (0xFF << 8)) >> 8,
            (header.Signature & (0xFF << 16)) >> 16,
            (header.Signature & (0xFF << 24)) >> 24);
    }
}

static bool ParseText(ProcessorStructure* cpu)
{
    assert(cpu);

    char* code = cpu->text.buffer;
    size_t ip = cpu->instructionPointer;
    size_t codeLength = cpu->text.bufferSize;
    size_t commandCounter = 0;
    Stack* stk = &cpu->stack;

    while (ip < codeLength)
    {
        int error = 0;
        double number1 = 0;
        double number2 = 0;
        size_t regIndex = 0;

		Command cmd = {};
		int val = 0;
		val = code[ip++];
		memmove(&cmd, &(val), sizeof(char));

		unsigned char cmdType = cmd.type;
		switch (cmdType)
		{
			case cmd_push:
			{
				size_t regIndex = 0;
				double regValue = 0; 
				double number = 0;
				if (cmd.Reg)
				{
					GetArg(code, &ip, sizeof(char), (&regIndex)); 
					if (*(&regIndex) >= regsCount)
					{
						printf("IndexOutOfRangeRegister = %d", *(&regIndex)); 
						return false;
					};
					regValue = cpu->regs[regIndex];
				} 
				if (cmd.Number)
				{
					GetArg(code, &ip, sizeof(double), (&number));
				}

				number += regValue;

				if (cmd.RAM)
				{
					StackPush(stk, (cpu->RAM + (int)number));
					Sleep(RamSleep);
				}
				else
				{
					StackPush(stk, (&number));
				}
				break;
			}
			case cmd_pop:
			{
				int error = 0; 
				size_t regIndex = 0; 
				double number = 0; 

				if (cmd.Reg == 0 && cmd.Number == 0 && cmd.RAM == 0)
					StackPop(stk, (&error)); 
				else
				{
					if (cmd.RAM)
					{
						int ramIndex = 0;
						if (cmd.Reg)
						{
							GetArg(code, &ip, sizeof(char), (&regIndex)); 
							if (*(&regIndex) >= regsCount)
							{
								printf("IndexOutOfRangeRegister = %d", *(&regIndex)); 
								return false;
							};
							ramIndex += (int)(cpu->regs[regIndex]);
						} 

						if (cmd.Number)
						{
							GetArg(code, &ip, sizeof(double), (&number));
							ramIndex += (int)number;
						} 
						
						if (ramIndex < DispOffset)
						{
							cpu->RAM[ramIndex] = *((double*)(StackPop(stk, (&error))));
						}
						else
						{
							ramIndex = ramIndex - DispOffset;
							int value = (int)(*((double*)StackPop(stk, &error)));
							((char*)cpu->RAM)[DispOffset * sizeof(double) + ramIndex] = value;
						}
						Sleep(RamSleep);
					}
					else
					{
						if (cmd.Reg)
						{
							GetArg(code, &ip, sizeof(char), (&regIndex));
							
							if (*(&regIndex) >= regsCount)
							{
								printf("IndexOutOfRangeRegister = %d", *(&regIndex));
								return false;
							}; 
							cpu->regs[regIndex] = *((double*)(StackPop(stk, (&error))));
						}
					}
				}
				break;
			}
			case cmd_in:
			{
				double doubleVal = 0;
				puts("Введите число (Не число для завершения программы):");

				if (scanf("%lf", &doubleVal))
				{
					StackPush(stk, (&doubleVal));
				}
				else
				{
					printf("Программа завершена\n");
					return false;
				}
				break;
			}
			case cmd_out:
			{
				int error = 0;
				double* number = (double*)StackPop(stk, (&error));
				if (number)
					printf("%lf\n", *((double*)(number)));
				else
					puts("");

				break;
			}
			case cmd_add:
			{
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));

				double number2 = *((double*)(StackPop(stk, (&error))));

				number2 += number1;
				StackPush(stk, (&number2));
				break;
			}
			case cmd_sub:
			{
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));
				double number2 = *((double*)(StackPop(stk, (&error))));
				number2 -= number1;
				StackPush(stk, (&number2));
				break;
			}
			case cmd_mul:
			{
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));
				double number2 = *((double*)(StackPop(stk, (&error))));
				number2 *= number1;
				StackPush(stk, (&number2));
				break;
			}
			case cmd_div:
			{
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));
				double number2 = *((double*)(StackPop(stk, (&error))));
				number2 /= number1;
				StackPush(stk, (&number2));
				break;
			}
			case cmd_hlt:
			{
				return true;
			}
			case cmd_jmp:
			{
				size_t jmpLabel = 0;
				GetArg(code, &ip, sizeof(size_t), (&jmpLabel));
				ip = (int)(jmpLabel);
				break;
			}
			case cmd_ja:
			{
				size_t jmpLabel = 0;
				GetArg(code, &ip, sizeof(size_t), (&jmpLabel));
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));
				double number2 = *((double*)(StackPop(stk, (&error))));
				if (number2 > number1)
					ip = (int)(jmpLabel);
				break;
			}
			case cmd_jae:
			{
				size_t jmpLabel = 0;
				GetArg(code, &ip, sizeof(size_t), (&jmpLabel));
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));
				double number2 = *((double*)(StackPop(stk, (&error))));
				if (number2 >= number1)
					ip = (int)(jmpLabel);
				break;
			}
			case cmd_jb:
			{
				size_t jmpLabel = 0;
				GetArg(code, &ip, sizeof(size_t), (&jmpLabel));
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));
				double number2 = *((double*)(StackPop(stk, (&error))));
				if (number2 < number1)
					ip = (int)(jmpLabel);
				break;
			}
			case cmd_jbe:
			{
				size_t jmpLabel = 0;
				GetArg(code, &ip, sizeof(size_t), (&jmpLabel));
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));
				double number2 = *((double*)(StackPop(stk, (&error))));
				if (number2 <= number1)
					ip = (int)(jmpLabel);
				break;
			}
			case cmd_je:
			{
				size_t jmpLabel = 0;
				GetArg(code, &ip, sizeof(size_t), (&jmpLabel));
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));
				double number2 = *((double*)(StackPop(stk, (&error))));
				if (number2 == number1)
					ip = (int)(jmpLabel);
				break;
			}
			case cmd_jne:
			{
				size_t jmpLabel = 0;
				GetArg(code, &ip, sizeof(size_t), (&jmpLabel));
				int error = 0;
				double number1 = *((double*)(StackPop(stk, (&error))));
				double number2 = *((double*)(StackPop(stk, (&error))));
				if (number2 != number1)
					ip = (int)(jmpLabel);
				break;
			}
			case cmd_call:
			{
				double retLabel = (double)ip + sizeof (size_t); 
				StackPush(stk, (&retLabel));
				size_t jmpLabel = 0;
				GetArg(code, &ip, sizeof(size_t), (&jmpLabel));
				ip = (int)(jmpLabel);
				break;
			}
			case cmd_ret:
			{
				int error = 0;
				double retLabel = *((double*)(StackPop(stk, (&error))));
				ip = (int)(retLabel);
				break;
			}
			case cmd_dsp:
			{
				size_t dispBytesCount = DispHeightD * DispWidthD;
				putchar('\n');
				size_t pixelsCount = 0; 

				const char* videoMem = (char*)(cpu->RAM + DispOffset);
				
				for (size_t st = 0; st < dispBytesCount; st++)
				{
					char byte = videoMem[st];

					if (byte)
						putchar('*');
					else
						putchar('.');
					pixelsCount++;

					if (pixelsCount % DispWidthD == 0)
						putchar('\n');
				}
				fflush(stdout);
				break;
			}
			case cmd_cos:
			{
				int error = 0;
				double number = *((double*)(StackPop(stk, (&error))));
				number = cos(number * 3.14159265 / 180);
				StackPush(stk, (&number));
				break;
			}
			case cmd_sin:
			{
				int error = 0;
				double number = *((double*)(StackPop(stk, (&error))));
				number = sin(number * 3.14159265 / 180);
				StackPush(stk, (&number));
				break;
			}
			case cmd_sqrt:
			{
				int error = 0;
				double number = *((double*)(StackPop(stk, (&error))));
				number = sqrt(number);
				StackPush(stk, (&number));
				break;
			}
			case cmd_int:
			{
				int error = 0;
				double number = *((double*)(StackPop(stk, (&error))));
				number = (int)(number);
				StackPush(stk, (&number));
				break;
			}
			default:
				printf("Неизвестная команда <%c>. ip = %zd", code[ip], ip);
				return false;
		}
		commandCounter++;
	}

	return true;
}

static void GetArg(char* code, size_t* ip, size_t res_size, void* out_res)
{
    assert(code);
    assert(out_res);
    assert(ip);

    memmove(out_res, code + *ip, res_size);
    *ip += res_size;
}
