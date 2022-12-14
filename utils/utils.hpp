#pragma once
#include <string>
#include <vector>

void error(std::string message);
void warn(std::string message);

enum Collor {
  RED = 31,
  BLUE = 34,
  GREEN = 32,
  YELLOW = 33,
};

std::string cprint(const std::string, Collor, int);

std::string hex(bool, const uint16_t);
std::string hex(bool, const uint32_t);

uint16_t decode_func(uint32_t, uint16_t);
uint16_t decode_imm(uint32_t, uint16_t);

std::vector<std::string> split(const std::string&, const char);
bool is_empty(const std::string&);
std::string trim_comment(std::string);
