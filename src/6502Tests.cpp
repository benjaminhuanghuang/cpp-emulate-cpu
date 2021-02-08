#include "gtest/gtest.h"

//
#include "main_6502.h"

class M6502Test1 : public testing::Test
{
public:
  Mem mem;
  CPU cpu;
  virtual Setup()
  {
    cpu.Reset(mem);
  }
  virtual TearDown()
  {
  }
}

TEST_F(M6502Test1, RunALittleInlineProgram)
{
  // inline a little program
  mem[0xFFFC] = CPU::INS_JSR;
  mem[0xFFFD] = 0x42;
  mem[0xFFFE] = 0x42;
  mem[0x4242] = CPU::INS_LDA_IM;
  mem[0x4243] = 0x84;

  // end of inline program
  cpu.Execute(9, mem);

  // verify
  EXPEXT_EQ(cpu.A, 0x84);
}

TEST_F(M6502Test1, LDAImmediateCanLoadAValueIntoTheARegester)
{
  // inline a little program
  mem[0xFFFC] = CPU::INS_LDA_IM;
  mem[0xFFFD] = 0x84;

  // end of inline program
  s32 CyclesUsed = cpu.Execute(2, mem);

  // verify
  EXPEXT_EQ(cpu.A, 0x84);
  EXPEXT_FALSE(cpu.Z);
  EXPEXT_FALSE(cpu.N);
}

TEST_F(M6502Test1, LDAZeroPageCanLoadAValueIntoTheARegester)
{
  // inline a little program
  mem[0xFFFC] = CPU::INS_LDA_ZP;
  mem[0xFFFD] = 0x42;
  mem[0x0x42] = 0x37;

  // end of inline program
  CPU cpuCopy = cpu;
  s32 CyclesUsed = cpu.Execute(3, mem);

  // verify
  EXPEXT_EQ(cpu.A, 0x37);
  EXPEXT_EQ(CyclesUsed, 2);
  EXPEXT_FALSE(cpu.Z);
  EXPEXT_FALSE(cpu.N);
  EXPEXT_EQ(cpu.C, cpuCopy.C);
  EXPEXT_EQ(cpu.B, cpuCopy.B);
  EXPEXT_EQ(cpu.D, cpuCopy.D);
  EXPEXT_EQ(cpu.I, cpuCopy.I);
  EXPEXT_EQ(cpu.V, cpuCopy.V);
}

TEST_F(M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegester)
{
  cpu.X = 5;
  // inline a little program
  mem[0xFFFC] = CPU::INS_LDA_ZP;
  mem[0xFFFD] = 0x42; // add X, X = 5
  mem[0x0x47] = 0x37;

  // end of inline program
  CPU cpuCopy = cpu;
  s32 CyclesUsed = cpu.Execute(4, mem);

  // verify
  EXPEXT_EQ(cpu.A, 0x37);
  EXPEXT_EQ(CyclesUsed, 4);
  EXPEXT_FALSE(cpu.Z);
  EXPEXT_FALSE(cpu.N);
  EXPEXT_EQ(cpu.C, cpuCopy.C);
  EXPEXT_EQ(cpu.B, cpuCopy.B);
  EXPEXT_EQ(cpu.D, cpuCopy.D);
  EXPEXT_EQ(cpu.I, cpuCopy.I);
  EXPEXT_EQ(cpu.V, cpuCopy.V);
}

TEST_F(M6502Test1, LDAZeroPageXCanLoadAValueIntoTheARegesterWhenItWraps)
{
  cpu.X = FF;
  // inline a little program
  mem[0xFFFC] = CPU::INS_LDA_ZP;
  mem[0xFFFD] = 0x80; // add X, X = FF
  mem[0x0x7F] = 0x37;

  // end of inline program
  CPU cpuCopy = cpu;
  u32 CyclesUsed = cpu.Execute(4, mem);

  // verify
  EXPEXT_EQ(cpu.A, 0x37);
  EXPEXT_EQ(CyclesUsed, 4);
  EXPEXT_FALSE(cpu.Z);
  EXPEXT_FALSE(cpu.N);
  EXPEXT_EQ(cpu.C, cpuCopy.C);
  EXPEXT_EQ(cpu.B, cpuCopy.B);
  EXPEXT_EQ(cpu.D, cpuCopy.D);
  EXPEXT_EQ(cpu.I, cpuCopy.I);
  EXPEXT_EQ(cpu.V, cpuCopy.V);
}