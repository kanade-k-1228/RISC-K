#include "operation.hpp"
#include "isa.hpp"
#include "utils.hpp"
#include <regex>
#include <sstream>

bool is_calc(std::string s) { return include(mnemonics_calc, s); }
bool is_calci(std::string s) { return include(mnemonics_calci, s); }
bool is_ctrl(std::string s) { return include(mnemonics_ctrl, s); }

uint16_t reg_stoi(std::string name) {
  if(name == "zero") return Reg::zero;
  if(name == "pc") return Reg::pc;
  if(name == "ira") return Reg::ira;
  if(name == "sp") return Reg::sp;
  if(name == "ra") return Reg::ra;
  if(name == "fp") return Reg::fp;
  if(name == "a0") return Reg::a0;
  if(name == "a1") return Reg::a1;
  if(name == "t0") return Reg::t0;
  if(name == "t1") return Reg::t1;
  if(name == "t2") return Reg::t2;
  if(name == "t3") return Reg::t3;
  if(name == "s0") return Reg::s0;
  if(name == "s1") return Reg::s1;
  if(name == "s2") return Reg::s2;
  if(name == "s3") return Reg::s3;
  throw new std::string("Invalid Registor Name: " + name);
  return 0;
}

uint16_t alu_stoi(std::string s) {
  if(s == "add" || s == "addi") return ALUCode::ADD;
  if(s == "sub" || s == "subi") return ALUCode::SUB;
  if(s == "and" || s == "andi") return ALUCode::AND;
  if(s == "or" || s == "ori") return ALUCode::OR;
  if(s == "xor" || s == "xori") return ALUCode::XOR;
  if(s == "not") return ALUCode::NOT;
  if(s == "srs") return ALUCode::SRS;
  if(s == "sru") return ALUCode::SRU;
  if(s == "sl") return ALUCode::SL;
  if(s == "eq" || s == "eqi") return ALUCode::EQ;
  if(s == "lts" || s == "ltsi") return ALUCode::LTS;
  if(s == "ltu" || s == "ltui") return ALUCode::LTU;
  // Pseudo Operation
  if(s == "nop") return ALUCode::ADD;
  if(s == "mov") return ALUCode::ADD;
  if(s == "loadi") return ALUCode::ADD;
  throw new std::string("This operation doesn't have ALUCode: " + s);
  return 0;
}

const Mnemonic* getMnemonic(std::string mnemonic) {
  for(auto& format : isa) {
    if(format.mnemonic == mnemonic) {
      return &format;
    }
  }
  return nullptr;
}

Operation::Operation(const uint16_t address, const std::vector<std::string> str)
    : address(address), mnemonic(str.at(0)), rd("zero"), rs1("zero"), rs2("zero") {

  const Mnemonic* info = getMnemonic(mnemonic);
  if(info == nullptr)
    throw new std::string("Invalid mnemonic" + mnemonic);

  const std::vector<std::string> arg;
  if(str.size() - 1 != info->arg.size())
    throw new std::string("Required " + std::to_string(info->arg.size()) + " Operand");

  // Initiate with default value
  // TODO

  // Read arguments value
  for(size_t i = 0; i < info->arg.size(); ++i) {
    const std::string arg_type = info->arg.at(i);
    if(arg_type == "rd") rd = str.at(i + 1);
    if(arg_type == "rs1") rs1 = str.at(i + 1);
    if(arg_type == "rs2") rs2 = str.at(i + 1);
    if(arg_type == "imm") imm = Imm(str.at(i + 1));
  }
}

uint32_t pack(uint8_t u4_0, uint8_t u4_1, uint8_t u4_2, uint8_t u4_3, uint16_t u16) {
  return (u4_0 & 0x0F) | ((u4_1 & 0x0F) << 4) | ((u4_2 & 0x0F) << 8) | ((u4_3 & 0x0F) << 12) | ((u16 & 0xFFFF) << 16);
}

uint32_t Operation::getBin() {
  uint32_t ret = 0;
  if(is_calc(mnemonic))
    return pack(OPCode::calc, reg_stoi(rs1), reg_stoi(rs2), reg_stoi(rd), alu_stoi(mnemonic));
  if(is_calci(mnemonic))
    return pack(OPCode::calci, reg_stoi(rs1), alu_stoi(mnemonic), reg_stoi(rd), imm.value);
  if(mnemonic == "load")
    return pack(OPCode::load, reg_stoi(rs1), 0, reg_stoi(rd), imm.value);
  if(mnemonic == "store")
    return pack(OPCode::store, reg_stoi(rs1), reg_stoi(rs2), 0, imm.value);
  if(is_ctrl(mnemonic))
    return pack(OPCode::ctrl, reg_stoi(rs1), reg_stoi(rs2), reg_stoi(rd), imm.value);
  return ret;
}

std::string Operation::print() {
  std::stringstream ss;
  ss << cprint(mnemonic, RED, 6);
  if(is_calc(mnemonic))
    ss << cprint(rd, BLUE, 6) << cprint(rs1, BLUE, 8) << cprint(rs2, BLUE, 8);
  else if(is_calci(mnemonic))
    ss << cprint(rd, BLUE, 6) << cprint(rs1, BLUE, 8) << imm.print();
  else if(mnemonic == "load")
    ss << cprint(rd, BLUE, 6) << cprint(rs1, BLUE, 8) << imm.print();
  else if(mnemonic == "store")
    ss << cprint(rs2, BLUE, 6) << cprint(rs1, BLUE, 8) << imm.print();
  else if(mnemonic == "mov")
    ss << cprint(rd, BLUE, 6) << "        " << cprint(rs1, BLUE, 8);
  else if(mnemonic == "loadi" || mnemonic == "if")
    ss << cprint(rd, BLUE, 6) << "        " << imm.print();
  else if(mnemonic == "jump" || mnemonic == "call")
    ss << "              " << imm.print();
  else if(mnemonic == "nop")
    ss << "";
  return ss.str();
}
