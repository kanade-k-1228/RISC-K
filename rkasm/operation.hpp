#pragma once
#include "imm.hpp"
#include "label_def.hpp"
struct Operation {
  uint16_t address;
  std::string mnemonic;
  std::vector<std::string> operand;
  std::string func, rs1, rs2, rd;
  Imm imm;
  Operation(const uint16_t address, const std::vector<std::string> str);
  uint32_t get_bin();
  std::string print();
};