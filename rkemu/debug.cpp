#include "debug.hpp"
#include "isa.hpp"
#include "utils.hpp"
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

bool IntrPoints::is_intr(uint16_t time) {
  if(use)
    return this->contains(time);
  else
    return false;
}

void DumpPoints::init(std::string fname) {
  this->fname = fname;
  this->use = true;
  std::ifstream fin(fname);
  if(!fin) error("Cant Open File: " + fname);
  for(std::string line; std::getline(fin, line);) {
    auto tokens = split(line, ' ');
    if(tokens.size() == 0) continue;
    DumpOption dump;
    uint16_t pc = std::stoi(tokens.at(0), nullptr, 0);
    for(unsigned int i = 1; i < tokens.size(); ++i) {
      std::string token = tokens.at(i);
      if(token == "-s") {
        dump.stack = true;
      } else if(token == "-") {
        uint16_t begin = std::stoi(tokens.at(i - 1), nullptr, 0);
        uint16_t end = std::stoi(tokens.at(i + 1), nullptr, 0);
        for(uint16_t j = begin; j < end; ++j) dump.address.insert(j);
        ++i;
      } else {
        dump.address.insert(std::stoi(token, nullptr, 0));
      }
    }
    this->insert(std::make_pair(pc, dump));
  }
}

bool DumpPoints::is_dump(uint16_t pc) {
  if(all)
    return true;
  else if(use)
    return this->contains(pc);
  return false;
}

std::string Debug::print_code(uint32_t code) {
  OPDecoder op(code);
  if(op.opc == OPCode::calc) {
    if(op.func == ALUCode::ADD) return cprint("add", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == ALUCode::SUB) return cprint("sub", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == ALUCode::AND) return cprint("and", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == ALUCode::OR) return cprint("or", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == ALUCode::XOR) return cprint("xor", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == ALUCode::NOT) return cprint("not", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    if(op.func == ALUCode::SRS) return cprint("srs", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    if(op.func == ALUCode::SRU) return cprint("sru", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    if(op.func == ALUCode::SL) return cprint("sl", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    if(op.func == ALUCode::EQ) return cprint("eq", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    if(op.func == ALUCode::LTS) return cprint("lts", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    if(op.func == ALUCode::LTU) return cprint("ltu", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    if(op.func == ALUCode::LCAST) return cprint("lcast", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    return "Unknown FuncCode" + hex(true, op.func);
  }
  if(op.opc == OPCode::calci) {
    if(op.func == ALUCode::ADD) return cprint("addi", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == ALUCode::SUB) return cprint("subi", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == ALUCode::AND) return cprint("andi", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == ALUCode::OR) return cprint("ori", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == ALUCode::XOR) return cprint("xori", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == ALUCode::EQ) return cprint("eqi", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == ALUCode::LTS) return cprint("ltsi", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == ALUCode::LTU) return cprint("ltui", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    return "Unknown FuncCode" + hex(true, op.func);
  }
  if(op.opc == OPCode::load) return cprint("load", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  if(op.opc == OPCode::store) return cprint("store", RED, 6) + cprint(hex(false, op.rs2), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  if(op.opc == OPCode::calif) return cprint("calif", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  return "Unknown Opecode" + hex(true, op.opc);
}

std::string Debug::dump(CPU& cpu, DumpOption& opt) {
  std::stringstream ss;
  ss << " +------------+-------------------------+" << std::endl
     << " |  " << cprint("PC: " + hex(false, cpu.ram.get(Reg::pc)), GREEN, 0) << "  |  CSR: " << std::bitset<16>(cpu.ram.get(Reg::csr)) << "  |" << std::endl
     << " |  RA: " << hex(false, cpu.ram.get(Reg::ra)) << "  |  S0: " << hex(false, cpu.ram.get(Reg::s0)) << "  |  T0: " << hex(false, cpu.ram.get(Reg::t0)) << "  |" << std::endl
     << " | IRA: " << hex(false, cpu.ram.get(Reg::ira)) << "  |  S1: " << hex(false, cpu.ram.get(Reg::s1)) << "  |  T1: " << hex(false, cpu.ram.get(Reg::t1)) << "  |" << std::endl
     << " |  SP: " << hex(false, cpu.ram.get(Reg::sp)) << "  |  S2: " << hex(false, cpu.ram.get(Reg::s2)) << "  |  T2: " << hex(false, cpu.ram.get(Reg::t2)) << "  |" << std::endl
     << " |  FP: " << hex(false, cpu.ram.get(Reg::fp)) << "  |  S3: " << hex(false, cpu.ram.get(Reg::s3)) << "  |  T3: " << hex(false, cpu.ram.get(Reg::t3)) << "  |" << std::endl
     << " +------------+------------+------------+" << std::endl;
  for(uint16_t addr : opt.address)
    ss << " | " << hex(true, addr) << " : " << hex(false, cpu.ram.get(addr)) << "                  |" << std::endl;
  if(opt.stack) {
    if(cpu.ram.get(Reg::sp) < cpu.ram.get(Reg::fp)) ss << " +--------------------------------+" << std::endl;
    for(uint16_t sp = cpu.ram.get(Reg::sp); sp < cpu.ram.get(Reg::fp); sp++)
      ss << " | " << hex(true, (uint16_t)(sp + 1)) << " : " << hex(false, cpu.ram.get(sp + 1)) << "                  |" << std::endl;
  }
  return ss.str();
}

std::string Debug::dump(CPU& cpu) {
  std::stringstream ss;
  ss << " +------------+-------------------------+" << std::endl
     << " |  " << cprint("PC: " + hex(false, cpu.ram.get(Reg::pc)), GREEN, 0) << "  |  CSR: " << std::bitset<16>(cpu.ram.get(Reg::csr)) << "  |" << std::endl
     << " |  RA: " << hex(false, cpu.ram.get(Reg::ra)) << "  |  S0: " << hex(false, cpu.ram.get(Reg::s0)) << "  |  T0: " << hex(false, cpu.ram.get(Reg::t0)) << "  |" << std::endl
     << " | IRA: " << hex(false, cpu.ram.get(Reg::ira)) << "  |  S1: " << hex(false, cpu.ram.get(Reg::s1)) << "  |  T1: " << hex(false, cpu.ram.get(Reg::t1)) << "  |" << std::endl
     << " |  SP: " << hex(false, cpu.ram.get(Reg::sp)) << "  |  S2: " << hex(false, cpu.ram.get(Reg::s2)) << "  |  T2: " << hex(false, cpu.ram.get(Reg::t2)) << "  |" << std::endl
     << " |  FP: " << hex(false, cpu.ram.get(Reg::fp)) << "  |  S3: " << hex(false, cpu.ram.get(Reg::s3)) << "  |  T3: " << hex(false, cpu.ram.get(Reg::t3)) << "  |" << std::endl
     << " +------------+------------+------------+" << std::endl;
  return ss.str();
}
