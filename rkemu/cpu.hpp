#pragma once
#include <array>
#include <fstream>
#include <string>

class CPU {
public:
  std::array<uint16_t, 0x10000> mem;
  uint16_t pc = 0;
  std::array<uint32_t, 0x10000> rom;
  std::string fname;

  bool intr_latch[4] = {false};

  CPU();

  void load_rom(std::string);

  bool cstop();
  int serial();

  void external_interrupt(int);
  void catch_interrupt();
  void jump_interrupt();

  void execute(uint32_t);
  void add(uint16_t, uint16_t, uint16_t);
  void addi(uint16_t, uint16_t, uint16_t);
  void sub(uint16_t, uint16_t, uint16_t);
  void subi(uint16_t, uint16_t, uint16_t);
  void land(uint16_t, uint16_t, uint16_t);
  void landi(uint16_t, uint16_t, uint16_t);
  void lor(uint16_t, uint16_t, uint16_t);
  void lori(uint16_t, uint16_t, uint16_t);
  void lxor(uint16_t, uint16_t, uint16_t);
  void lxori(uint16_t, uint16_t, uint16_t);
  void lnot(uint16_t, uint16_t);
  void lrot(uint16_t, uint16_t);
  void rrot(uint16_t, uint16_t);
  void load(uint16_t, uint16_t, uint16_t);
  void loadi(uint16_t, uint16_t);
  void store(uint16_t, uint16_t, uint16_t);
  void jump(uint16_t, uint16_t, uint16_t);
  void breq(uint16_t, uint16_t, uint16_t);
  void brlt(uint16_t, uint16_t, uint16_t);
};
