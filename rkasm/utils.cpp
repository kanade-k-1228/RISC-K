#include "utils.hpp"
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

std::string cprint(const std::string str, Collor collor, int width) {
  std::ostringstream ss;
  ss << "\033[" << collor << "m" << std::setw(width) << str << "\033[m";
  return ss.str();
}

std::string hex(bool prefix, const uint16_t n) {
  std::ostringstream ss;
  ss.setf(std::ios::hex, std::ios::basefield);
  const char fill_save = ss.fill('0');
  ss << (prefix ? "0x" : "") << std::setw(4) << n;
  ss.fill(fill_save);
  return ss.str();
}

std::string hex(bool prefix, const uint32_t n) {
  std::ostringstream ss;
  ss.setf(std::ios::hex, std::ios::basefield);
  const char fill_save = ss.fill('0');
  ss << (prefix ? "0x" : "")
     << std::setw(4) << ((n >> 16) & 0xffff)
     << "_" << std::setw(4) << (n & 0xffff);
  return ss.str();
}

// 区切り文字 sep で分割
std::vector<std::string> split(const std::string& str, const char sep) {
  std::vector<std::string> v;
  std::stringstream ss(str);
  std::string buffer;
  while(std::getline(ss, buffer, sep)) {
    if(buffer == "") continue;  // 区切り文字が連続した場合、削除
    v.push_back(buffer);
  }
  return v;
}

bool include(std::vector<std::string> list, std::string find) {
  return std::find(list.begin(), list.end(), find) != list.end();
}

template <typename T>
T* find(std::vector<T> vec, std::function<bool(T)> cond) {
  for(auto& item : vec) {
    if(cond(item)) {
      return &item;
    }
  }
  return nullptr;
}

std::string print_error(std::string fname, int line_cnt, std::string line, std::string msg) {
  std::stringstream ss;
  std::string place = fname + ":" + std::to_string(line_cnt);
  ss << std::string(place.size(), ' ') << " | " << std::endl
     << place << " | " << line << std::endl
     << std::string(place.size(), ' ') << " | \033[31m ERROR! \033[m " << msg << std::endl;
  return ss.str();
}
