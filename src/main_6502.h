#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;  // 8  byte
using Word = unsigned short; // 16 byte

using u32 = unsigned int;
using s32 = signed int;


struct Mem
{
  static constexpr u32 MAX_MEM = 1024 * 64;
  Byte Data[MAX_MEM];

  void Initialize()
  {
    for (u32 i = 0; i < MAX_MEM; i++)
    {
      Data[i] = 0;
    }
  }
  // Read 1 byte
  Byte operator[](u32 Address) const
  {
    return Data[Address];
  }
  // Write 1 byte
  Byte &operator[](u32 Address)
  {
    return Data[Address];
  }

  void WriteWord( Word value, u32 Address, u32 &Cycles){
    Data[Address] = value & 0xFF; // lower byte
    Data[Address + 1] = (value >> 8);
    Cycles -= 2;
  }
};

struct CPU
{
  Word PC; // program counter
  Word SP; // Stack counter

  Byte A, X, Y; // register

  //Byte PS;         // processor status regisger
  Byte C : 1;
  Byte Z : 1;
  Byte I : 1;
  Byte D : 1;
  Byte B : 1;
  Byte V : 1;
  Byte N : 1;

  void Reset(Mem &memory)
  {
    PC = 0xFFFC;
    SP = 0x0100;
    C = Z = I = D = B = V = N = 0;

    A = X = Y = 0;
    memory.Initialize();
  }
  // Fetch instruction
  Byte FetchByte(u32 &Cycles, Mem &memory)
  {
    Byte Data = memory[PC];
    PC++;
    Cycles--;
    return Data;
  }
  
  Byte FetchWord(u32 &Cycles, Mem &memory)
  {
    // CPU-6502 is little endian
    Word Data = memory[PC]; // lower byte
    PC++;

    Data |= (memory[PC] << 8);
    PC++;

    Cycles -= 2;

    return Data;
  }
  // Read data
  Byte ReadByte(u32 &Cycles, Byte Address, Mem &memory)
  {
    Byte Data = memory[Address];
    Cycles--;
    return Data;
  }

  // opcodes
  static constexpr Byte
      INS_LDA_IM = 0xA9,  //  addressing mode Immediate
      INS_LDA_ZP = 0xA5,  //  addressing mode zero page
      INS_LDA_ZPX = 0xB5, //  addressing mode zero page X
      INS_JSR = 0x20; 

  void LDASetStatus()
  {
    Z = (A == 0);
    N = (A & 0b10000000) > 0;
  }

  /* Return the number of cycles that were used*/
  s32 Execute(u32 Cycles, Mem &memory)
  {
    const u32 CyclesRequested = Cycles;

    while (Cycles > 0)
    {
      Byte instruction = FetchByte(Cycles, memory);

      switch (instruction)
      {
      case INS_LDA_IM:
      {
        Byte Value = FetchByte(Cycles, memory);
        A = Value;
        LDASetStatus();
        break;
      }
      case INS_LDA_ZP:
      {
        Byte zeroPageeAddress = FetchByte(Cycles, memory);
        A = ReadByte(Cycles, zeroPageeAddress, memory);
        LDASetStatus();
        break;
      }
      case INS_LDA_ZPX:
      {
        Byte zeroPageeAddress = FetchByte(Cycles, memory);
        zeroPageeAddress += X;
        Cycles--;
        A = ReadByte(Cycles, zeroPageeAddress, memory);
        LDASetStatus();
        break;
      }
      case INS_JSR:  // jump to subroutine
      {
        Word SubAddr = FetchWord(Cycles, memory);
        memory.WriteWord(PC -1, SP, Cycles);
        SP +=2;
        PC = SubAddr;
        Cycles --;
      }
      default:
        printf("Instruction not handled %d \n", instruction);
        break;
      }
    }
    const s32 NumCyclesUsed =  CyclesRequested - Cycles;
    return NumCyclesUsed;
  }
};