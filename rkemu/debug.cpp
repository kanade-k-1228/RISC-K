#include "debug.hpp"
#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include <fstream>
#include <regex>
#include <string>

void IntrPoints::init(std::string fname) {
  this->fname = fname;
  this->use = true;
  std::ifstream fin(fname);
  if(!fin) error("Cant Open File: " + fname);
  for(std::string line; std::getline(fin, line);) {
    auto tokens = split(line, ' ');
    IntrOption intr_opt;
    int t = std::stoi(tokens.at(0), nullptr, 0);
    intr_opt.ino = std::stoi(tokens.at(1));
    this->insert(std::make_pair(t, intr_opt));
  }
}

bool IntrPoints::contain(int t) {
  return this->find(t) != this->end();
}

void DumpPoints::init(std::string fname) {
  this->fname = fname;
  this->use = true;
  std::ifstream fin(fname);
  if(!fin) error("Cant Open File: " + fname);
  for(std::string line; std::getline(fin, line);) {
    auto tokens = split(line, ' ');
    DumpOption dump;
    uint16_t pc = std::stoi(tokens.at(0), nullptr, 0);
    tokens.erase(tokens.begin());
    for(auto token : tokens) dump.address.push_back(std::stoi(token, nullptr, 0));
    this->insert(std::make_pair(pc, dump));
  }
}

bool DumpPoints::contain(uint16_t pc) {
  return this->find(pc) != this->end();
}

std::string Debug::print_code(uint32_t code) {
  uint16_t opc = (code >> 6) & 0x000f;
  uint16_t func = decode_func(code, opc);
  uint16_t rs1 = (code >> 0) & 0x003f;
  uint16_t rs2 = (code >> 20) & 0x003f;
  uint16_t rd = (code >> 26) & 0x003f;
  uint16_t imm = decode_imm(code, opc);

  if(opc == CALC) {
    if(func == ADD) return cprint("add", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, rs2), BLUE, 6);
    if(func == SUB) return cprint("sub", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, rs2), BLUE, 6);
    if(func == AND) return cprint("and", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, rs2), BLUE, 6);
    if(func == OR) return cprint("or", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, rs2), BLUE, 6);
    if(func == XOR) return cprint("xor", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, rs2), BLUE, 6);
    if(func == NOT) return cprint("not", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6);
    if(func == LROT) return cprint("lrot", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6);
    if(func == RROT) return cprint("rrot", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6);
  }
  if(opc == CALCI) {
    if(func == ADD) return cprint("addi", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
    if(func == SUB) return cprint("subi", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
    if(func == AND) return cprint("andi", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
    if(func == OR) return cprint("ori", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
    if(func == XOR) return cprint("xori", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
  }
  if(opc == LOAD) return cprint("load", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
  if(opc == LOADI) return cprint("loadi", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
  if(opc == STORE) return cprint("store", RED, 6) + cprint(hex(false, rs2), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
  if(opc == JUMP) return cprint("jump", RED, 6) + cprint(hex(false, rd), BLUE, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
  if(opc == BREQ) return cprint("breq", RED, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, rs2), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
  if(opc == BRLT) return cprint("brlt", RED, 6) + cprint(hex(false, rs1), BLUE, 6) + cprint(hex(false, rs2), BLUE, 6) + cprint(hex(false, imm), YELLOW, 6);
  return "Unknown Opecode" + hex(true, opc);
}

std::string Debug::dump(CPU& cpu) {
  std::stringstream ss;
  ss << "------------------------------------" << std::endl
     << "         |  " << cprint("Save", BLUE, 0) << "  |  " << cprint("Temp", BLUE, 0) << "  |  " << cprint("Arg", BLUE, 0) << "  " << std::endl
     << cprint("PC: " + hex(false, cpu.pc), GREEN, 0) << " |0: " << hex(false, cpu.mem.at(S0)) << " |0: " << hex(false, cpu.mem.at(T0)) << " |0: " << hex(false, cpu.mem.at(A0)) << std::endl
     << "RA: " << hex(false, cpu.mem.at(RA)) << " |1: " << hex(false, cpu.mem.at(S1)) << " |1: " << hex(false, cpu.mem.at(T1)) << " |1: " << hex(false, cpu.mem.at(A1)) << std::endl
     << "SP: " << hex(false, cpu.mem.at(SP)) << " |2: " << hex(false, cpu.mem.at(S2)) << " |2: " << hex(false, cpu.mem.at(T2)) << " |2: " << hex(false, cpu.mem.at(A2)) << std::endl
     << "FP: " << hex(false, cpu.mem.at(FP)) << " |3: " << hex(false, cpu.mem.at(S3)) << " |3: " << hex(false, cpu.mem.at(T3)) << " |3: " << hex(false, cpu.mem.at(A3)) << std::endl
     << "CSR: " << std::bitset<16>(cpu.mem.at(CSR)) << std::endl
     << "------------------------------------" << std::endl;
  for(uint16_t sp = cpu.mem.at(SP); sp < cpu.mem.at(FP); sp++)
    ss << hex(false, (uint16_t)(sp + 1)) << " : " << hex(false, cpu.mem.at(sp + 1)) << std::endl;
  if(cpu.mem.at(SP) < cpu.mem.at(FP)) ss << "------------------------------------" << std::endl;
  return ss.str();
}

std::string Debug::dump(CPU& cpu, DumpOption& opt) {
  std::stringstream ss;
  ss << "------------------------------------" << std::endl
     << "         |  " << cprint("Save", BLUE, 0) << "  |  " << cprint("Temp", BLUE, 0) << "  |  " << cprint("Arg", BLUE, 0) << "  " << std::endl
     << cprint("PC: " + hex(false, cpu.pc), GREEN, 0) << " |0: " << hex(false, cpu.mem.at(S0)) << " |0: " << hex(false, cpu.mem.at(T0)) << " |0: " << hex(false, cpu.mem.at(A0)) << std::endl
     << "RA: " << hex(false, cpu.mem.at(RA)) << " |1: " << hex(false, cpu.mem.at(S1)) << " |1: " << hex(false, cpu.mem.at(T1)) << " |1: " << hex(false, cpu.mem.at(A1)) << std::endl
     << "SP: " << hex(false, cpu.mem.at(SP)) << " |2: " << hex(false, cpu.mem.at(S2)) << " |2: " << hex(false, cpu.mem.at(T2)) << " |2: " << hex(false, cpu.mem.at(A2)) << std::endl
     << "FP: " << hex(false, cpu.mem.at(FP)) << " |3: " << hex(false, cpu.mem.at(S3)) << " |3: " << hex(false, cpu.mem.at(T3)) << " |3: " << hex(false, cpu.mem.at(A3)) << std::endl
     << "CSR: " << std::bitset<16>(cpu.mem.at(CSR)) << std::endl
     << "------------------------------------" << std::endl;
  for(uint16_t sp = cpu.mem.at(SP); sp < cpu.mem.at(FP); sp++)
    ss << hex(false, (uint16_t)(sp + 1)) << " : " << hex(false, cpu.mem.at(sp + 1)) << std::endl;
  if(cpu.mem.at(SP) < cpu.mem.at(FP)) ss << "------------------------------------" << std::endl;
  for(uint16_t addr : opt.address)
    ss << hex(false, addr) << " : " << hex(false, cpu.mem.at(addr)) << std::endl;
  if(opt.address.size() != 0) ss << "------------------------------------" << std::endl;
  return ss.str();
}
